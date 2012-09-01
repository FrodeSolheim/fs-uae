#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL.h>
#include "fs/base.h"
#include "fs/ml.h"
#include "fs/emu.h"

#include "video.h"
#include "input.h"
#include "audio.h"
#include "libfsemu.h"

//char *g_fs_emu_application_title = NULL;
// FIXME: REMOVE
char *g_fs_emu_window_title = NULL;

char *g_fs_emu_title = NULL;
char *g_fs_emu_sub_title = NULL;

GKeyFile *g_fs_emu_config = NULL;
static fs_emu_pause_function g_pause_function = NULL;
GQueue *g_event_queue = NULL;
SDL_mutex *g_event_queue_mutex = NULL;
static GMutex *g_gui_mutex = NULL;
static int g_gui_mutex_locked = 0;

int g_fs_emu_throttling = 1;
int g_fs_emu_full_sync_allowed = 1;
int g_fs_emu_emulation_thread_running = 0;
int g_fs_emu_emulation_thread_stopped = 0;

void fs_emu_disable_throttling() {
    g_fs_emu_throttling = 0;
}

void fs_emu_disallow_full_sync() {
    g_fs_emu_full_sync_allowed = 0;
}

#ifdef EMULATION_THREAD

#if 0
#ifdef WINDOWS
#include <Windows.h>
HGLRC originalContext = 0;
HDC originalDC = 0;
HGLRC clientContext = 0;
#endif

static int emulation_thread(void* data) {
    fs_emu_log("emulation thread running\n");
#ifdef _WIN32
    if (clientContext == 0) {
        clientContext = wglCreateContext(originalDC);
        if (clientContext == NULL) {
            fs_emu_log("wglCreateContext failed with error %ld\n", GetLastError());
            return 0;
        }
    }
    if (wglMakeCurrent(originalDC, clientContext) == FALSE) {
        fs_emu_log("wglmakeCurrent failed with error %ld\n", GetLastError());
        return 0;
    }
    //return 1;
#else
    //return 0;
#endif
    void (*emulation_thread_func)(void) = data;
    emulation_thread_func();
    return 0;
}
#endif

#if 0
void fs_emu_start_emulation_thread(void (*func)(void)) {
    fs_emu_log("start_emulation_thread\n");

#ifdef WINDOWS
    originalContext = wglGetCurrentContext();
    originalDC = wglGetCurrentDC();
#endif
    SDL_CreateThread(emulation_thread, func);
}
#endif

#if 0
void fs_emu_main() {
    while (1) {
        SDL_Event *event = g_malloc(sizeof(SDL_Event));
        SDL_WaitEvent(event);
        SDL_mutexP(g_event_queue_mutex);
        g_queue_push_head(g_event_queue, event);
        SDL_mutexV(g_event_queue_mutex);
    }
}
#endif

int fs_emu_event_get(fs_emu_event* event) {
    int result = 0;
    SDL_mutexP(g_event_queue_mutex);
    if (g_queue_get_length(g_event_queue) > 0) {
        fs_emu_event* in_event = g_queue_pop_head(g_event_queue);
        *event = *in_event;
        result = 1;
    }
    SDL_mutexV(g_event_queue_mutex);
    return result;
}

#else

#if 0
void (*g_emulation_thread_func)(void) = NULL;

void fs_emu_start_emulation_thread(void (*func)(void)) {
    fs_emu_log("start_emulation_thread\n");
    g_emulation_thread_func = func;
}

void fs_emu_main() {
    fs_emu_log("(currently running emulation thread in main thread)\n");
    g_emulation_thread_func();
}
#endif

int fs_emu_event_get(fs_emu_event* event) {
#ifdef USE_SDL
    return SDL_PollEvent(event);
#else
    return 0;
#endif
}

#endif

/*
void fs_emu_set_application_title(const char *title) {
    if (g_fs_emu_application_title) {
        g_free(g_fs_emu_application_title);
    }
    g_fs_emu_application_title = g_strdup(title);
}
*/

int64_t g_fs_emu_quit_time = 0;
static fs_emu_simple_function g_quit_function = NULL;

void fs_emu_set_quit_function(fs_ml_simple_function function) {
    g_quit_function = function;
}

gpointer force_quit_thread(gpointer data) {
    for (int i = i; i < 5; i++) {
        fs_ml_usleep(1000 * 1000);
    }
    return NULL;
}

void on_quit() {
    fs_log("libfsemu:on_quit\n");
    g_fs_emu_quit_time = fs_emu_monotonic_time();
    if (g_quit_function) {
        g_quit_function();
    }
    if (g_thread_create(force_quit_thread, NULL, FALSE, NULL) == NULL) {
        fs_log("WARNING: could not create force quit thread\n");
    }
}

void fs_emu_quit() {
    fs_ml_quit();
}

int fs_emu_is_quitting() {
    // FIXME: move to header file for possible inlining...
    return fs_ml_is_quitting();
}

void fs_emu_warning(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    gchar *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    int len = strlen(buffer);
    // strip trailing newline, of any
    if (len > 0 && buffer[len] == '\n') {
        buffer[len] = '\0';
    }
    fs_log("WARNING: %s\n", buffer);
    //fs_log("(TODO: present this warning to the user in the UI)\n");
    fs_emu_add_console_line(buffer, 0);
    g_free(buffer);
}

/*
void fs_emu_warning(const gchar* warning) {
    fs_log("WARNING: %s\n", warning);
    fs_log("(TODO: present this warning to the user in the UI\n");
}
*/

const char *fs_emu_get_title() {
    return g_fs_emu_title;
}

void fs_emu_set_title(const char *title) {
    fs_emu_video_render_mutex_lock();
    if (g_fs_emu_title) {
        g_free(g_fs_emu_title);
    }
    g_fs_emu_title = g_strdup(title);
    fs_emu_video_render_mutex_unlock();
}

const char *fs_emu_get_sub_title() {
    return g_fs_emu_sub_title;
}

void fs_emu_set_sub_title(const char *title) {
    fs_emu_video_render_mutex_lock();
    if (g_fs_emu_sub_title) {
        g_free(g_fs_emu_sub_title);
    }
    g_fs_emu_sub_title = g_strdup(title);
    fs_emu_video_render_mutex_unlock();
}

void fs_emu_set_window_title(const char *title) {
    fs_emu_video_render_mutex_lock();
    if (g_fs_emu_window_title) {
        g_free(g_fs_emu_window_title);
    }
    g_fs_emu_window_title = g_strdup(title);
    fs_emu_video_render_mutex_unlock();
}

void fs_emu_msleep(int msec) {
    fs_ml_usleep(msec * 1000);
}
/*
#ifdef WINDOWS
#include <Windows.h>
#elif defined(MACOSX)
#include <mach/mach_time.h>
#else
#define CLOCK_FREQ 10000000
#endif

static int64_t syncbase = 0;

static int64_t read_clock (void) {
#ifdef WINDOWS
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result.QuadPart;
#elif defined(MACOSX)
    return mach_absolute_time();
#else
    int clock_gettime(clockid_t clk_id, struct timespec *tp);
    struct timespec tp;
    static time_t base_secs = 0;
    if (base_secs == 0) {
        clock_gettime(CLOCK_REALTIME, &tp);
        base_secs = tp.tv_sec;
    }
    clock_gettime(CLOCK_REALTIME, &tp);
    tp.tv_sec -= base_secs;
    return tp.tv_sec * CLOCK_FREQ + tp.tv_nsec / 100;
#endif
}

static void calculate_clock_freq(void) {
#ifdef WINDOWS
    LARGE_INTEGER result;
    QueryPerformanceFrequency(&result);
    syncbase = result.QuadPart;
#elif defined(MACOSX)
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    syncbase = 1000000000 * info.denom / info.numer;
#else
    syncbase = CLOCK_FREQ;
#endif
    // divide by 10^6 to get microsecond units when we divide read_clock
    // by syncbase
    //syncbase = syncbase / 1000000;
}
*/

#if 0
int64_t fs_emu_monotonic_time() {
    return fs_ml_monotonic_time();
}
#endif

void fs_emu_set_pause_function(fs_emu_pause_function function) {
    g_pause_function = function;
}

static void read_config() {
    //GError *error;
    //gboolean boolean_result;
    gchar *string_result;

    int fullscreen = fs_config_get_boolean("fullscreen");
    if (fullscreen != FS_CONFIG_NONE) {
        g_fs_emu_video_fullscreen = fullscreen;
    }

    g_fs_emu_video_fullscreen_mode = fs_config_get_string("fullscreen_mode");

    string_result = fs_config_get_string("title");
    if (string_result) {
        g_fs_emu_title = string_result;
    }
    string_result = fs_config_get_string("sub_title");
    if (string_result) {
        g_fs_emu_sub_title = string_result;
    }
}

#if 0
const gchar *fs_emu_get_share_dir() {
    static gchar *share_dir = NULL;
    if (share_dir == NULL) {
        char buffer[FS_PATH_MAX];
        fs_get_application_exe_dir(buffer, FS_PATH_MAX);
        gchar *test;
#ifdef MACOSX
        test = g_build_filename(buffer, "..", "Resources",
                g_get_prgname(), "fs-emu-share-dir", NULL);
        fs_log("checking share dir %s\n", test);
        if (g_file_test(test,  G_FILE_TEST_EXISTS)) {
            share_dir = g_build_filename(buffer, "..", "Resources",
                    g_get_prgname(), NULL);
            g_free(test);
            return share_dir;
        }
        g_free(test);
#endif
        test = g_build_filename(buffer, "..", "share", g_get_prgname(),
                "fs-emu-share-dir", NULL);
        fs_log("checking share dir %s\n", test);
        if (g_file_test(test,  G_FILE_TEST_EXISTS)) {
            share_dir = g_build_filename(buffer, "..", "share",
                    g_get_prgname(), NULL);
            g_free(test);
            return share_dir;
        }
        g_free(test);
        test = g_build_filename("share", g_get_prgname(), "fs-emu-share-dir",
                NULL);
        fs_log("checking share dir %s\n", test);
        if (g_file_test(test,  G_FILE_TEST_EXISTS)) {
            share_dir = g_build_filename("share", g_get_prgname(), NULL);
            g_free(test);
            return share_dir;
        }
        g_free(test);
        const gchar * const *dir = g_get_system_data_dirs();
        while (*dir) {
            test = g_build_filename(*dir, "fs-emu-share-dir", NULL);
            fs_log("checking share dir %s\n", test);
            if (g_file_test(test,  G_FILE_TEST_EXISTS)) {
                share_dir = g_strdup(*dir);
                g_free(test);
                return share_dir;
            }
            g_free(test);
            test = g_build_filename(*dir, g_get_prgname(), "fs-emu-share-dir",
                    NULL);
            fs_log("checking share dir %s\n", test);
            if (g_file_test(test,  G_FILE_TEST_EXISTS)) {
                share_dir = g_build_filename(*dir, g_get_prgname(), NULL);
                g_free(test);
                return share_dir;
            }
            g_free(test);
            test = g_build_filename(*dir, "..", "..", "share", g_get_prgname(),
                    "fs-emu-share-dir", NULL);
            fs_log("checking share dir %s\n", test);
            if (g_file_test(test,  G_FILE_TEST_EXISTS)) {
                //share_dir = g_strdup(*dir);
                share_dir = g_build_filename(*dir, "..", "..", "share",
                        g_get_prgname(), NULL);
                g_free(test);
                return share_dir;
            }
            g_free(test);
            dir++;
        }
        if (share_dir == NULL) {
            share_dir = g_strdup("");
        }
        //fs_emu_log("share dir is \"%s\"\n", share_dir);
    }
    return share_dir;
}
#endif

/*
void fs_emu_log(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    vfs_log(format, ap);
    va_end(ap);
}
*/

extern int g_fs_log_stdout;

static GOptionEntry entries[] = {
/*
    { "fullscreen", 'f', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE,
            &g_fs_emu_video_fullscreen,
            "Open in fullscreen mode instead of window", NULL },
*/
/*
    { "fullscreen-mode", 0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
            &g_fs_emu_video_fullscreen_mode, "Open in fullscreen window", NULL },
*/
    //{ "vsync", 0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE,
    //        &g_fs_emu_video_vsync, "Sync to screen refresh", NULL },
/*
    { "video-sync", 0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
            &g_fs_emu_video_vsync_mode_arg, "Sync to screen refresh", NULL },
*/
/*
    { "netplay-server", 0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
            &g_fs_emu_netplay_server_arg, "Connect to netplay server host", NULL },
    { "netplay-tag", 0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING,
            &g_fs_emu_netplay_tag_arg, "Player tag name", NULL },
    { "stdout", 0, G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE,
            &g_fs_log_stdout, "Log to stdout", NULL },
*/
    { NULL }
};

GOptionGroup* fs_emu_get_option_group() {

    GOptionGroup *group = g_option_group_new("libfsemu",
            "Common options for libfsemu",
            "Common options for libfsemu",
            NULL, NULL);
    g_option_group_add_entries(group, entries);
    return group;
}

void fs_emu_fatal(const char *msg) {
    fs_emu_log("%s\n", msg);
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

void fs_emu_set_config(GKeyFile* config) {
    g_fs_emu_config = config;
}

void fs_emu_acquire_gui_lock() {
    g_mutex_lock(g_gui_mutex);
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
    g_mutex_unlock(g_gui_mutex);
}

void fs_emu_init() {
    fs_log("fs_emu_init\n");
    if (!g_fs_emu_config) {
        g_fs_emu_config = g_key_file_new();
    }

    if (fs_config_get_boolean("stdout") == 1) {
        fs_log_enable_stdout();
    }

    fs_emu_log("calling fs_ml_init\n");
    fs_ml_init();

    g_gui_mutex = g_mutex_new();
    fs_emu_initialize_hud_module();
    fs_emu_initialize_dialog_module();

}

void fs_emu_init_2(int options) {
    fs_log("fs_emu_init_2\n");

    fs_emu_log("initializing SDL\n");
    SDL_Init(SDL_INIT_EVERYTHING);
    fs_emu_log("fs_emu_init\n");

    fs_emu_init_theme();

    //g_random_set_seed(time(NULL));
    fs_emu_netplay_init();

    if (options & FS_EMU_INIT_VIDEO) {
        fs_emu_video_init();
    }

    fs_emu_log("calling fs_ml_init_2\n");
    fs_ml_init_2();
    fs_ml_set_quit_function(on_quit);

    g_event_queue = g_queue_new();
    g_event_queue_mutex = SDL_CreateMutex();

    fs_emu_log("read config\n");
    read_config();

    if (options & FS_EMU_INIT_INPUT) {
        fs_emu_init_input();
    }
    if (options & FS_EMU_INIT_AUDIO) {
        fs_emu_audio_init();
    }

    fs_ml_video_set_update_function(fs_emu_video_update_function);
    fs_ml_video_set_render_function(fs_emu_video_render_function);
    fs_ml_video_set_post_render_function(fs_emu_video_after_update);

    //printf("----- %s\n", fs_emu_get_title());
    //printf("----- %s\n", fs_emu_get_sub_title());

    if (options & FS_EMU_INIT_VIDEO) {
        gchar *title;
        if (fs_emu_get_title()) {
            title = g_strdup(fs_emu_get_title());
        }
        else {
            title = g_strdup("Emulator");
        }
        if (fs_emu_get_sub_title()) {
            gchar *temp = title;
            // using 'MIDDLE DOT' (U+00B7) in UTF-8 format as separator
            title = g_strdup_printf("%s %c%c %s", temp, 0xC2, 0xB7,
                    fs_emu_get_sub_title());
            g_free(temp);
        }
        fs_ml_video_create_window(title);
        g_free(title);
    }
}

int fs_emu_thread_running() {
    return g_fs_emu_emulation_thread_running;
}

static gpointer emulation_thread_entry(gpointer data) {
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

    void (*main_function)() = data;
    main_function();

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

    //fs_ml_init();
    //function();

    // FIXME: should wait until we are certain that the video thread is
    // running (i.e. wait for a status / flag)


    // FIXME: MOVE
    if (fs_emu_netplay_enabled()) {
        fs_log("netplay is enabled\n");
        fs_emu_netplay_start();
        //int result = fs_emu_netplay_connect();
        //fs_log("netplay connection result: %d\n", result);
    }

    GError *error = NULL;
    //GThread *emulation_thread =
    g_thread_create(emulation_thread_entry, function, FALSE, &error);
    if (error != NULL) {
        fs_emu_log("error starting video thread\n");
        // FIXME: ERROR MESSAGE HERE
        // FIXME: FATAL
    }

    //int64_t t1 = fs_emu_monotonic_time();
    int result = fs_ml_main_loop();
    fs_emu_log("fs_emu_run: main loop is done\n");

    if (g_fs_emu_benchmark_start_time) {
        int64_t t2 = fs_emu_monotonic_time();
        double ttime = ((t2 - g_fs_emu_benchmark_start_time) / 1000000);
        double sys_fps = g_fs_emu_total_sys_frames / ttime;
        double emu_fps = g_fs_emu_total_emu_frames / ttime;
        fs_log("average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
    }

    fs_emu_log("fs_emu_run: waiting for emulation thread to stop\n");
    while (g_fs_emu_emulation_thread_running) {
        fs_emu_msleep(1);
    }
    fs_emu_log("fs_emu_run: emulation thread stopped\n");

#ifdef WITH_SDL_AUDIO
    fs_emu_log("fs_emu_run: calling SDL_CloseAudio\n");
    SDL_CloseAudio();
#endif

    fs_emu_audio_shutdown();
    fs_emu_log("fs_emu_run: returning\n");
    return result;
}
