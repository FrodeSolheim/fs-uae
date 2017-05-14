#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu.h>
#include <fs/emu/audio.h>
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
    fse_log("[FSE] Read config\n");
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
    fse_log("FATAL: %s\n", msg);
    printf("FATAL: %s\n", msg);
    exit(1);
}

static int g_pause_mode = 0;

void fs_emu_pause(int pause)
{
    if (pause == g_pause_mode) {
        return;
    }
    if (g_pause_function == NULL) {
        fse_log("no pause function set");
        return;
    }
    fse_log("fs_menu_pause %d\n", pause);
    //fs_emu_grab_input(g_pause_mode == 1);
    g_pause_mode = pause;
    g_pause_function(pause);
}

int fs_emu_is_paused()
{
    return g_pause_mode == 1;
}

void fs_emu_acquire_gui_lock()
{
    fs_mutex_lock(g_gui_mutex);
    g_gui_mutex_locked = 1;
}

void fs_emu_assert_gui_lock()
{
    if (!g_gui_mutex_locked) {
        fs_log("ERROR: gui mutex is not locked\n");
        exit(1);
    }
}

void fs_emu_release_gui_lock()
{
    g_gui_mutex_locked = 0;
    fs_mutex_unlock(g_gui_mutex);
}

void fs_emu_volume_control(int volume)
{
    if (volume == -1) {
        if (fse_audio_muted(FS_EMU_AUDIO_MASTER)) {
            fse_set_audio_muted(FS_EMU_AUDIO_MASTER, 0);
            if (fse_audio_volume(FS_EMU_AUDIO_MASTER) == 0) {
                fse_set_set_audio_volume(FS_EMU_AUDIO_MASTER, 10);
            }
        } else {
            fse_set_audio_muted(FS_EMU_AUDIO_MASTER, true);
        }
    } else if (volume == -2) {
        int volume = MAX(0, fse_audio_volume(FS_EMU_AUDIO_MASTER) - 10);
        fse_set_set_audio_volume(FS_EMU_AUDIO_MASTER, volume);
        if (fse_audio_muted(FS_EMU_AUDIO_MASTER)) {
            fse_set_audio_muted(FS_EMU_AUDIO_MASTER, false);
        }
    } else if (volume == -3) {
        int volume = MIN(100, fse_audio_volume(FS_EMU_AUDIO_MASTER) + 10);
        fse_set_set_audio_volume(FS_EMU_AUDIO_MASTER, volume);
        if (fse_audio_muted(FS_EMU_AUDIO_MASTER)) {
            fse_set_audio_muted(FS_EMU_AUDIO_MASTER, false);
        }
    }
    if (fse_audio_muted(FS_EMU_AUDIO_MASTER)) {
        fse_notify(1418909137, _("Volume: Muted"));
    } else {
        fse_notify(1418909137, _("Volume: %d%%"),
                fse_audio_volume(FS_EMU_AUDIO_MASTER));
    }
}

void fse_init_early(void)
{
    FSE_INIT_ONCE();
    fs_log("[FSE] Init (early)\n");

    fs_init_time();

    if (fs_config_true(OPTION_STDOUT)) {
        fs_log_enable_stdout();
    }

    fs_log("[FSE] Calling fs_ml_init\n");
    fs_ml_init();

#ifdef WITH_LUA
    fs_emu_lua_init();
#endif

    g_gui_mutex = fs_mutex_create();
    fse_init_hud_early();
    // fs_emu_dialog_init();
}

void fse_init(int options)
{
    FSE_INIT_ONCE();
    fse_init_early();
    fs_log("[FSE] Init\n");
    read_config();

#ifdef USE_SDL
    fse_log("[FSE] Initializing SDL\n");
    SDL_Init(SDL_INIT_EVERYTHING);
#endif

    fse_init_hud_after_config();

    fse_init_theme();

#ifdef WITH_NETPLAY
    //g_random_set_seed(time(NULL));
    fse_init_netplay();
#endif

#ifdef FSE_DRIVERS
    fse_init_video();
#else
    if (options & FS_EMU_INIT_VIDEO) {
        fse_init_video();
    }

    fse_init_renderer();
#endif

    /* These must (currently) be called after renderer has been initialized,
     * due to a mutex that must be initialized first. */
    fs_emu_set_overlay_state(FS_EMU_TOP_LEFT_OVERLAY, 1);
    fs_emu_set_overlay_state(FS_EMU_TOP_RIGHT_OVERLAY, 1);
    fs_emu_set_overlay_state(FS_EMU_BOTTOM_RIGHT_OVERLAY, 1);
    fs_emu_set_overlay_state(FS_EMU_BOTTOM_LEFT_OVERLAY, 1);

    fse_log("[FSE] Calling fs_ml_init_2\n");
    fs_ml_init_2();
    fs_ml_set_quit_function(on_quit);

    if (options & FS_EMU_INIT_INPUT) {
        fse_init_input();
    }
    if (options & FS_EMU_INIT_AUDIO) {
        fse_init_audio();
    }
}

int fs_emu_thread_running(void)
{
    return g_fs_emu_emulation_thread_running;
}

#ifdef WINDOWS
static void set_windows_thread_priority(void)
{
    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL)) {
        fse_log("FSE] Thread priority set to THREAD_PRIORITY_ABOVE_NORMAL\n");
    } else {
        int dwError = GetLastError();
        fse_log("[FSE] Failed to set thread priority (%d)\n", dwError);
    }
}
#endif

static void *emulation_thread(void *data)
{
    fse_log("[FSE] Emulation thread started\n");
#ifdef WINDOWS
    set_windows_thread_priority();
#endif
#ifdef WITH_NETPLAY
    if (fs_emu_netplay_enabled()) {
        fse_log("[NETPLAY] Enabled - waiting for connection...\n");
        while (!fs_emu_netplay_connected()) {
            /* Waiting for connection... */
            fs_emu_msleep(10);
            if (!fs_emu_netplay_enabled()) {
                /* Net play mode was aborted. */
                fse_log("netplay aborted\n");
                break;
            }
        }
    }
#endif
    g_fs_emu_emulation_thread_running = 1;
    void (*main_function)() = data;
    if (main_function) {
        fse_log("[FSE] Run main function at %p\n", data);
        main_function();
    } else {
        fs_emu_fatal("[FSE] NULL pointer main function\n");
    }
    /* Call fs_ml_quit in case quit was not explicitly requested already. */
    fs_ml_quit();
    g_fs_emu_emulation_thread_running = 0;
    /* With this set, and fs_ml_quit being called, the frame render
     * function will call fs_ml_stop when the fadeout effect is done. */
    g_fs_emu_emulation_thread_stopped = 1;
    return NULL;
}

int fs_emu_run(fs_emu_main_function function)
{
    fse_log("[FSE] fs_emu_run, main_function at %p\n", function);

    /* FIXME: should wait until we are certain that the video thread is
     * running (i.e. wait for a status / flag). */

#ifdef WITH_NETPLAY
    // FIXME: MOVE
    if (fs_emu_netplay_enabled()) {
        fs_log("[FSE] Netplay is enabled\n");
        fs_emu_netplay_start();
    }
#endif

    g_emulation_thread = fs_thread_create(
                "emulation", emulation_thread, function);
    if (g_emulation_thread == NULL) {
        fse_log("[FSE] Error starting emulation thread\n");
        // FIXME: ERROR MESSAGE HERE
        // FIXME: FATAL
    }

    int result = fse_main_loop();
    fse_log("[FSE] fs_emu_run: main loop is done\n");

    if (g_fs_emu_benchmark_start_time) {
        int64_t t2 = fs_emu_monotonic_time();
        double ttime = ((t2 - g_fs_emu_benchmark_start_time) / 1000000.0);
        double sys_fps = g_fs_emu_total_sys_frames / ttime;
        double emu_fps = g_fs_emu_total_emu_frames / ttime;
        fs_log("[FSE] Average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
    }

    fse_log("[FSE] Waiting for emulation thread to stop...\n");
    while (g_fs_emu_emulation_thread_running) {
        fs_emu_msleep(1);
    }
    fse_log("[FSE] Emulation thread stopped\n");

#ifdef USE_SDL_AUDIO
    fse_log("[FSE] fs_emu_run: calling SDL_CloseAudio\n");
    SDL_CloseAudio();
#endif

    fs_emu_audio_shutdown();
    fse_log("[FSE] Returning from fs_emu_run\n");
    return result;
}

static int wait_for_frame_no_netplay(void)
{
#if 0
    while (1) {
        fs_ml_usleep(100 * 1000);
    }
#endif

    if (g_fs_emu_benchmarking) {
        return 1;
    }
#ifdef FSE_DRIVERS
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

int fs_emu_wait_for_frame(int frame)
{
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
