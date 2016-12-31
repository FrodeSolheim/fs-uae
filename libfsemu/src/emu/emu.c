#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FS_EMU_INTERNAL
#include <fs/emu.h>
#include <fs/emu/benchmark.h>
#include <fs/emu/input.h>
#include <fs/emu/options.h>
#include <fs/emu/video.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef USE_SDL
#include <SDL.h>
#endif

#include <fs/base.h>
#include <fs/i18n.h>
#include <fs/ml.h>
#include <fs/thread.h>
#include <fs/time.h>

#ifdef USE_GLIB
#include <glib.h>
#endif

#include "audio.h"
#include "dialog.h"
#include "emu_lua.h"
#include "hud.h"
#include "input.h"
#include "libfsemu.h"
#include "netplay.h"
#include "theme.h"
#include "video.h"

char *g_fs_emu_title = NULL;
char *g_fs_emu_sub_title = NULL;

//GKeyFile *g_fs_emu_config = NULL;
static fs_emu_pause_function g_pause_function = NULL;
static fs_mutex *g_gui_mutex = NULL;
static int g_gui_mutex_locked = 0;

int g_fs_emu_throttling = 1;
int g_fs_emu_full_sync_allowed = 1;
int g_fs_emu_emulation_thread_running = 0;
int g_fs_emu_emulation_thread_stopped = 0;
static fs_thread *g_emulation_thread = NULL;

void fs_emu_disable_throttling() {
    g_fs_emu_throttling = 0;
}

void fs_emu_disallow_full_sync() {
    // FIXME: can rename to client_inhibit_full_sync, and let it be turn
    // on and off
    g_fs_emu_full_sync_allowed = 0;
}

int64_t g_fs_emu_quit_time = 0;
static fs_emu_simple_function g_quit_function = NULL;

void fs_emu_set_quit_function(fs_ml_void_function function) {
    g_quit_function = function;
}

static void *force_quit_thread(void *data)
{
    for (int i = 0; i < 10; i++) {
        fs_ml_usleep(1000 * 1000);
        fs_log("force_quit_thread: %d seconds passed\n", i + 1);
    }
    fs_log("force_quit_thread: should force quit (FIXME: not implemented)\n");
    printf("force_quit_thread: should force quit (FIXME: not implemented)\n");
    return NULL;
}

static void on_quit()
{
    g_fs_emu_quit_time = fs_emu_monotonic_time();
    if (g_quit_function) {
        fs_log("libfsemu on_quit: executing quit function\n");
        g_quit_function();
    } else {
        fs_log("libfsemu on_quit: no quit function\n");
    }
    // FIXME: detached?
    fs_thread_create("force-quit", force_quit_thread, NULL);
}

void fs_emu_quit()
{
    fs_ml_quit();
}

int fs_emu_is_quitting()
{
    // FIXME: move to header file for possible inlining...
    return fs_ml_is_quitting();
}

void fs_emu_warning(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    int len = strlen(buffer);
    // strip trailing newline, of any
    if (len > 0 && buffer[len] == '\n') {
        buffer[len] = '\0';
    }
    fs_log("WARNING: %s\n", buffer);
    printf("WARNING: %s\n", buffer);
    fs_emu_hud_add_console_line(buffer, 0);
    g_free(buffer);
}

void fs_emu_deprecated(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    int len = strlen(buffer);
    // strip trailing newline, if any
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    fs_log("DEPRECATED: %s\n", buffer);
    fs_emu_hud_add_console_line(buffer, 0);
    g_free(buffer);
}

const char *fs_emu_get_title(void)
{
    return g_fs_emu_title;
}

void fs_emu_set_title(const char *title)
{
    fs_emu_video_render_mutex_lock();
    if (g_fs_emu_title) {
        free(g_fs_emu_title);
    }
    g_fs_emu_title = g_strdup(title);
    fs_emu_video_render_mutex_unlock();
}

const char *fs_emu_get_sub_title(void)
{
    return g_fs_emu_sub_title;
}

void fs_emu_set_sub_title(const char *title)
{
    fs_emu_video_render_mutex_lock();
    if (g_fs_emu_sub_title) {
        free(g_fs_emu_sub_title);
    }
    g_fs_emu_sub_title = g_strdup(title);
    fs_emu_video_render_mutex_unlock();
}

void fs_emu_msleep(int msec)
{
    fs_ml_usleep(msec * 1000);
}

void fs_emu_set_pause_function(fs_emu_pause_function function)
{
    g_pause_function = function;
}

static void read_config(void)
{
    char *string_result;

    int fullscreen = fs_config_get_boolean(OPTION_FULLSCREEN);
    if (fullscreen != FS_CONFIG_NONE) {
        g_fs_emu_video_fullscreen = fullscreen;
    }
    g_fs_emu_video_fullscreen_mode_string = fs_config_get_string(
                OPTION_FULLSCREEN_MODE);

    string_result = fs_config_get_string(OPTION_TITLE);
    if (string_result) {
        g_fs_emu_title = string_result;
    }
    string_result = fs_config_get_string(OPTION_SUB_TITLE);
    if (string_result) {
        if (strcmp(string_result, "0") == 0) {
            g_fs_emu_sub_title = g_strdup("");
        } else {
            g_fs_emu_sub_title = string_result;
        }
    }
}

void fs_emu_fatal(const char *msg)
{
    fs_emu_log("FATAL: %s\n", msg);
    printf("FATAL: %s\n", msg);
    exit(1);
}

static int g_pause_mode = 0;

void fs_emu_pause(int pause) {
    if (pause == g_pause_mode) {
        return;
    }
    if (g_pause_function == NULL) {
        fs_emu_log("no pause function set");
        return;
    }
    fs_emu_log("fs_menu_pause %d\n", pause);
    //fs_emu_grab_input(g_pause_mode == 1);
    g_pause_mode = pause;
    g_pause_function(pause);
}

int fs_emu_is_paused() {
    return g_pause_mode == 1;
}

/*
void fs_emu_set_config(GKeyFile* config) {
    g_fs_emu_config = config;
}
*/

void fs_emu_acquire_gui_lock() {
    fs_mutex_lock(g_gui_mutex);
    g_gui_mutex_locked = 1;
}

void fs_emu_assert_gui_lock() {
    if (!g_gui_mutex_locked) {
        fs_log("ERROR: gui mutex is not locked\n");
        exit(1);
    }
}

void fs_emu_release_gui_lock() {
    g_gui_mutex_locked = 0;
    fs_mutex_unlock(g_gui_mutex);
}

void fs_emu_volume_control(int volume)
{
    if (volume == -1) {
        if (fs_emu_audio_muted(FS_EMU_AUDIO_MASTER)) {
            fs_emu_audio_set_muted(FS_EMU_AUDIO_MASTER, 0);
            if (fs_emu_audio_volume(FS_EMU_AUDIO_MASTER) == 0) {
                fs_emu_audio_set_volume(FS_EMU_AUDIO_MASTER, 10);
            }
        }
        else {
            fs_emu_audio_set_muted(FS_EMU_AUDIO_MASTER, true);
        }
    }
    else if (volume == -2) {
        int volume = MAX(0, fs_emu_audio_volume(FS_EMU_AUDIO_MASTER) - 10);
        fs_emu_audio_set_volume(FS_EMU_AUDIO_MASTER, volume);
        if (fs_emu_audio_muted(FS_EMU_AUDIO_MASTER)) {
            fs_emu_audio_set_muted(FS_EMU_AUDIO_MASTER, false);
        }
    }
    else if (volume == -3) {
        int volume = MIN(100, fs_emu_audio_volume(FS_EMU_AUDIO_MASTER) + 10);
        fs_emu_audio_set_volume(FS_EMU_AUDIO_MASTER, volume);
        if (fs_emu_audio_muted(FS_EMU_AUDIO_MASTER)) {
            fs_emu_audio_set_muted(FS_EMU_AUDIO_MASTER, false);
        }
    }

    if (fs_emu_audio_muted(FS_EMU_AUDIO_MASTER)) {
        fs_emu_notification(1418909137, _("Volume: Muted"));
    } else {
        fs_emu_notification(1418909137, _("Volume: %d%%"),
                fs_emu_audio_volume(FS_EMU_AUDIO_MASTER));
    }
}

void fs_emu_init(void)
{
    fs_log("fs_emu_init\n");
    //if (!g_fs_emu_config) {
    //    g_fs_emu_config = g_key_file_new();
    //}
    fs_time_init();

    if (fs_config_get_boolean("stdout") == 1) {
        fs_log_enable_stdout();
    }

    fs_emu_log("calling fs_ml_init\n");
    fs_ml_init();

#ifdef WITH_LUA
    fs_emu_lua_init();
#endif

    g_gui_mutex = fs_mutex_create();
    fs_emu_hud_init();
    fs_emu_dialog_init();

}

void fs_emu_init_2(int options) {
    fs_log("fs_emu_init_2\n");

#ifdef USE_SDL
    fs_emu_log("initializing SDL\n");
    SDL_Init(SDL_INIT_EVERYTHING);
#endif

    fs_emu_hud_init_after_config();

    fs_emu_theme_init();

#ifdef WITH_NETPLAY
    //g_random_set_seed(time(NULL));
    fs_emu_netplay_init();
#endif

#ifdef FS_EMU_DRIVERS
    fs_emu_video_init();
#else
    if (options & FS_EMU_INIT_VIDEO) {
        fs_emu_video_init();
    }

    fs_emu_init_render();
#endif

    // these must (currently) be called after renderer has been initialized,
    // due to a mutex that must be initialized first
    fs_emu_set_overlay_state(FS_EMU_TOP_LEFT_OVERLAY, 1);
    fs_emu_set_overlay_state(FS_EMU_TOP_RIGHT_OVERLAY, 1);
    fs_emu_set_overlay_state(FS_EMU_BOTTOM_RIGHT_OVERLAY, 1);
    fs_emu_set_overlay_state(FS_EMU_BOTTOM_LEFT_OVERLAY, 1);

// #ifdef FS_EMU_DRIVERS_XXX
// #else
    fs_emu_log("calling fs_ml_init_2\n");
    fs_ml_init_2();
    fs_ml_set_quit_function(on_quit);

    fs_emu_log("read config\n");
    read_config();
// #endif

    if (options & FS_EMU_INIT_INPUT) {
        fs_emu_input_init();
#ifdef FS_EMU_DRIVERS
        fs_emu_input_init_2();
#endif
    }
    if (options & FS_EMU_INIT_AUDIO) {
        fs_emu_audio_init();
    }

#ifdef FS_EMU_DRIVERS

#else
    fs_ml_video_set_update_function(fs_emu_video_update_function);
    fs_ml_video_set_render_function(fs_emu_video_render_function);
    fs_ml_video_set_post_render_function(fs_emu_video_after_update);

    if (options & FS_EMU_INIT_VIDEO) {
        char *title;
        if (fs_emu_get_title()) {
            title = g_strdup(fs_emu_get_title());
        }
        else {
            title = g_strdup("Emulator");
        }
        if (fs_emu_get_sub_title() && fs_emu_get_sub_title()[0] != '\0') {
            char *temp = title;
            // using 'MIDDLE DOT' (U+00B7) in UTF-8 format as separator
            title = g_strdup_printf("%s %c%c %s", temp, 0xC2, 0xB7,
                    fs_emu_get_sub_title());
            free(temp);
        }
        fs_ml_video_create_window(title);
        free(title);
    }
#endif
}

int fs_emu_thread_running() {
    return g_fs_emu_emulation_thread_running;
}

static void *emulation_thread_entry(void *data) {
    fs_emu_log("emulation thread started\n");
    g_fs_emu_emulation_thread_running = 1;
#ifdef WINDOWS
    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL)) {
        fs_emu_log("thread priority set to THREAD_PRIORITY_ABOVE_NORMAL\n");
    }
    else {
        int dwError = GetLastError();
        fs_emu_log("Failed to set thread priority (%d)\n", dwError);
    }
#endif

#ifdef WITH_NETPLAY
    if (fs_emu_netplay_enabled()) {
        fs_emu_log("netplay is enabled - waiting for connection\n");
        while (!fs_emu_netplay_connected()) {
            // waiting for connection
            fs_emu_msleep(10);
            if (!fs_emu_netplay_enabled()) {
                // net play mode was aborted
                fs_emu_log("netplay aborted\n");
                break;
            }
        }
    }
#endif

    void (*main_function)() = data;
    if (main_function) {
        fs_emu_log("main function at %p\n", data);
        main_function();
    }
    else {
        fs_emu_fatal("main function is NULL pointer\n");
    }

    // call fs_ml_quit in case the quit was not explicitly requested already
    fs_ml_quit();

    g_fs_emu_emulation_thread_running = 0;

    // with this set, and fs_ml_quit being called, the frame render
    // function will call fs_ml_stop when the fadeout effect is done
    g_fs_emu_emulation_thread_stopped = 1;

    //fs_emu_log("calling fs_ml_stop because emulation thread has ended\n");
    //fs_ml_stop();
    return NULL;
}

int fs_emu_run(fs_emu_main_function function) {
    fs_emu_log("fs_emu_run, main_function at %p\n", function);

    // FIXME: should wait until we are certain that the video thread is
    // running (i.e. wait for a status / flag)

#ifdef WITH_NETPLAY
    // FIXME: MOVE
    if (fs_emu_netplay_enabled()) {
        fs_log("netplay is enabled\n");
        fs_emu_netplay_start();
    }
#endif

    g_emulation_thread = fs_thread_create(
                "emulation", emulation_thread_entry, function);
    if (g_emulation_thread == NULL) {
        fs_emu_log("error starting video thread\n");
        // FIXME: ERROR MESSAGE HERE
        // FIXME: FATAL
    }

#ifdef FS_EMU_DRIVERS
    int result = fs_emu_main_loop();
#else
    int result = fs_ml_main_loop();
#endif
    fs_emu_log("fs_emu_run: main loop is done\n");

    if (g_fs_emu_benchmark_start_time) {
        int64_t t2 = fs_emu_monotonic_time();
        double ttime = ((t2 - g_fs_emu_benchmark_start_time) / 1000000.0);
        double sys_fps = g_fs_emu_total_sys_frames / ttime;
        double emu_fps = g_fs_emu_total_emu_frames / ttime;
        fs_log("average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
    }

    fs_emu_log("fs_emu_run: waiting for emulation thread to stop\n");
    while (g_fs_emu_emulation_thread_running) {
        fs_emu_msleep(1);
    }
    fs_emu_log("fs_emu_run: emulation thread stopped\n");

#ifdef USE_SDL_AUDIO
    fs_emu_log("fs_emu_run: calling SDL_CloseAudio\n");
    SDL_CloseAudio();
#endif

    fs_emu_audio_shutdown();
    fs_emu_log("fs_emu_run: returning\n");
    return result;
}

static int wait_for_frame_no_netplay() {
#if 0
    while (1) {
        fs_ml_usleep(100 * 1000);
    }
#endif

    if (g_fs_emu_benchmarking) {
        return 1;
    }
#ifdef FS_EMU_DRIVERS
    if (g_fs_emu_benchmark_mode) {
        return 1;
    }
#endif
    if (!g_fs_emu_throttling) {
        return 1;
    }
    //fs_log("wait_for_frame_no_netplay\n");
    static int64_t last_time = 0;
    static int64_t frame_time = 0;
    if (last_time == 0) {
        last_time = fs_emu_monotonic_time();
    }

    static double last_frame_rate = 0.0;
    double frame_rate = fs_emu_get_video_frame_rate();
    if (frame_rate != last_frame_rate) {
        frame_time = ((int64_t) 1000000) / frame_rate;
        fs_log("wait_for_frame_no_netplay: new frame rate %0.2f (time: %d)\n",
               frame_rate, (int) frame_time);
        last_frame_rate = frame_rate;
    }

    int64_t wait_until = last_time + frame_time;
    //int64_t sleep_until = wait_until;
    int64_t sleep_until = wait_until - 100;
    int64_t t = fs_emu_monotonic_time();
    //fs_log("%lld %lld\n", sleep_until, t);
    while (t < sleep_until) {
        int64_t sleep_time = sleep_until - t;
        //fs_log("%lld %lld %lld\n", sleep_until, t, sleep_time);
        fs_ml_usleep(sleep_time);
        t = fs_emu_monotonic_time();
    }
    while (t < wait_until) {
        t = fs_emu_monotonic_time();
    }
    last_time = last_time + frame_time;
    if (fs_emu_monotonic_time() > last_time + frame_time) {
        // time has elapsed too far, probably due to pause function having
        // been used
        last_time = fs_emu_monotonic_time();
    }
    return 1;
}

int fs_emu_wait_for_frame(int frame) {
#ifdef WITH_NETPLAY
    if (!fs_emu_netplay_enabled()) {
#endif
        if (fs_emu_get_video_sync()) {
            return 1;
        }
        return wait_for_frame_no_netplay();
#ifdef WITH_NETPLAY
    }
    return fs_emu_netplay_wait_for_frame(frame);
#endif
}
