#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_SDL_VIDEO

// FIXME: make libfsml independent of libfsemu
#include "../emu/video.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifdef USE_SDL2
#define USE_SDL
#endif

#ifdef USE_SDL
#include <SDL.h>
#endif

//#ifdef USE_GLIB
//#include <glib.h>
//#endif

#include <fs/conf.h>
#include <fs/lazyness.h>
// #include <GLee.h>
#include <GL/glew.h>
#include <fs/glib.h>
#include <fs/ml.h>
#include <fs/thread.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#define FS_EMU_INTERNAL
#include <fs/emu/input.h>
#include <fs/emu/monitor.h>
#include "ml_internal.h"

SDL_Window *g_fs_ml_window = NULL;
SDL_GLContext g_fs_ml_context = 0;
int g_fs_ml_had_input_grab = 0;
int g_fs_ml_was_fullscreen = 0;

static GQueue *g_video_event_queue;
static fs_mutex *g_video_event_mutex;
static fs_thread_id_t g_video_thread_id;
static fs_condition *g_video_cond;
static fs_mutex *g_video_mutex;
static int g_display;
static int g_has_input_grab = 0;
static int g_initial_input_grab = 0;
static bool g_grab_input_on_activate;
static int g_fs_ml_automatic_input_grab = 1;
static int g_fs_ml_keyboard_input_grab = 1;
static int g_fsaa = 0;
static int g_f12_state, g_f11_state;
static char *g_window_title;
static int g_window_width, g_window_height;
static int g_window_x, g_window_y;
static int g_window_resizable;
static int g_fullscreen_width, g_fullscreen_height;
static GLint g_max_texture_size;

#define FS_ML_VIDEO_EVENT_GRAB_INPUT 1
#define FS_ML_VIDEO_EVENT_UNGRAB_INPUT 2
#define FS_ML_VIDEO_EVENT_SHOW_CURSOR 3
#define FS_ML_VIDEO_EVENT_HIDE_CURSOR 4
#define FS_ML_VIDEO_EVENT_TOGGLE_FULLSCREEN 5
#define FS_ML_VIDEO_EVENT_ENABLE_FULLSCREEN 6
#define FS_ML_VIDEO_EVENT_DISABLE_FULLSCREEN 7
#define FS_ML_VIDEO_EVENT_ACTIVATE_WINDOW_SWITCHER 8

#define FULLSCREEN_FULLSCREEN 0
#define FULLSCREEN_WINDOW 1
#define FULLSCREEN_DESKTOP 2

static inline bool is_video_thread(void)
{
    return fs_thread_id() == g_video_thread_id;
}

int fs_ml_get_max_texture_size()
{
    return g_max_texture_size;
}

int fs_ml_get_fullscreen_width()
{
    return g_fullscreen_width;
}

int fs_ml_get_fullscreen_height()
{
    return g_fullscreen_height;
}

int fs_ml_get_windowed_width()
{
    return g_window_width;
}

int fs_ml_get_windowed_height()
{
    return g_window_height;
}

static void post_video_event(int event)
{
#ifdef FS_EMU_DRIVERS
    // printf("FS_EMU_DRIVERS: ignoring post_video_event\n");
#else
    fs_mutex_lock(g_video_event_mutex);
    g_queue_push_head(g_video_event_queue, FS_INT_TO_POINTER(event));
    fs_mutex_unlock(g_video_event_mutex);
#endif
}

static void process_video_events()
{
    fs_mutex_lock(g_video_event_mutex);
    int count = g_queue_get_length(g_video_event_queue);
    for (int i = 0; i < count; i++) {
        int event = FS_POINTER_TO_INT(g_queue_pop_tail(g_video_event_queue));
        if (event == FS_ML_VIDEO_EVENT_GRAB_INPUT) {
            fs_ml_set_input_grab(true);
        } else if (event == FS_ML_VIDEO_EVENT_UNGRAB_INPUT) {
            fs_ml_set_input_grab(false);
        } else if (event == FS_ML_VIDEO_EVENT_SHOW_CURSOR) {
            fs_ml_show_cursor(1, 1);
        } else if (event == FS_ML_VIDEO_EVENT_HIDE_CURSOR) {
            fs_ml_show_cursor(0, 1);
        } else if (event == FS_ML_VIDEO_EVENT_TOGGLE_FULLSCREEN) {
            fs_ml_toggle_fullscreen();
        } else if (event == FS_ML_VIDEO_EVENT_ENABLE_FULLSCREEN) {
            fs_ml_set_fullscreen(true);
        } else if (event == FS_ML_VIDEO_EVENT_DISABLE_FULLSCREEN) {
            fs_ml_set_fullscreen(false);
        }
    }
    fs_mutex_unlock(g_video_event_mutex);
}

bool fs_ml_input_grab(void)
{
    return g_has_input_grab;
}

bool fs_ml_automatic_input_grab(void)
{
    return g_fs_ml_automatic_input_grab;
}

void fs_ml_set_input_grab(bool grab)
{
    if (!is_video_thread()) {
        post_video_event(grab ? FS_ML_VIDEO_EVENT_GRAB_INPUT :
                                FS_ML_VIDEO_EVENT_UNGRAB_INPUT);
        /* FIXME: Not really, yet */
        g_has_input_grab = grab ? 1 : 0;
        return;
    }

    SDL_SetWindowGrab(g_fs_ml_window, grab ? SDL_TRUE : SDL_FALSE);
    SDL_SetRelativeMouseMode(grab ? SDL_TRUE : SDL_FALSE);
    if (fs_ml_cursor_allowed())
        fs_ml_show_cursor(!grab, 1);
    g_has_input_grab = grab ? 1 : 0;
}

void fs_ml_activate_window_switcher(void)
{
    if (!is_video_thread()) {
        post_video_event(FS_ML_VIDEO_EVENT_ACTIVATE_WINDOW_SWITCHER);
        return;
    }

    fs_ml_activate_window_switcher_impl();
}

void fs_ml_set_input_grab_on_activate(bool grab)
{
    g_grab_input_on_activate = grab;
}

void fs_ml_set_video_fsaa(int fsaa)
{
    g_fsaa = fsaa;
}

void fs_ml_show_cursor(int show, int immediate)
{
    if (immediate) {
        SDL_ShowCursor(show);
    }
    else {
        post_video_event(show ? FS_ML_VIDEO_EVENT_SHOW_CURSOR :
                FS_ML_VIDEO_EVENT_HIDE_CURSOR);
    }
}

static void log_opengl_information()
{
    static int written = 0;
    if (written) {
        return;
    }
    written = 1;
    char *software_renderer = NULL;
    const char *str;
    str = (const char*) glGetString(GL_VENDOR);
    if (str) {
        fs_log("opengl vendor: %s\n", str);
    }
    str = (const char*) glGetString(GL_RENDERER);
    if (str) {
        fs_log("opengl renderer: %s\n", str);
        if (strstr(str, "GDI Generic") != NULL) {
            software_renderer = g_strdup(str);
        }
    }
    str = (const char*) glGetString(GL_VERSION);
    if (str) {
        fs_log("opengl version: %s\n", str);
    }
    str = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (str) {
        fs_log("opengl shading language version: %s\n", str);
    }
    str = (const char*) glGetString(GL_EXTENSIONS);
    if (str) {
        fs_log("opengl extensions: %s\n", str);
    }
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &g_max_texture_size);
    fs_log("opengl max texture size (estimate): %dx%d\n", g_max_texture_size,
            g_max_texture_size);

    if (software_renderer) {
        fs_emu_warning("No HW OpenGL driver (\"%s\")", software_renderer);
        g_free(software_renderer);
    }
}

static void set_video_mode()
{
    int flags = SDL_WINDOW_OPENGL;
    if (g_fs_emu_video_fullscreen_mode != FULLSCREEN_WINDOW &&
            g_window_resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    int x = g_window_x, y = g_window_y;
    int w = -1, h = -1;

//    if (g_initial_input_grab) {
//        flags |= SDL_WINDOW_INPUT_GRABBED;
//        g_has_input_grab = 1;
//    }

    if (g_fs_emu_video_fullscreen == 1) {
        w = g_fullscreen_width;
        h = g_fullscreen_height;
        //w = g_window_width;
        //h = g_window_height;

        if (g_fs_emu_video_fullscreen_mode == FULLSCREEN_WINDOW) {
            fs_log("using fullscreen window mode\n");
            //x = 0;
            //y = 0;
            //w = g_fullscreen_width;
            //h = g_fullscreen_height;
            flags |= SDL_WINDOW_BORDERLESS;

            FSEmuMonitor monitor;
            fs_emu_monitor_get_by_index(g_display, &monitor);
            x = monitor.rect.x;
            y = monitor.rect.y;
            w = monitor.rect.w;
            h = monitor.rect.h;
        }
        else if (g_fs_emu_video_fullscreen_mode == FULLSCREEN_DESKTOP) {
            fs_log("using fullscreen desktop mode\n");
            // the width and height will not be used for the fullscreen
            // desktop mode, only for the window when toggling fullscreen
            // state
#if 0
            w = g_window_width;
            h = g_window_height;
#else
            FSEmuMonitor monitor;
            fs_emu_monitor_get_by_index(g_display, &monitor);
            x = monitor.rect.x;
            y = monitor.rect.y;
            w = monitor.rect.w;
            h = monitor.rect.h;
#endif
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else {
            fs_log("using SDL_FULLSCREEN mode\n");
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        fs_log("setting (fullscreen) video mode %d %d\n", w, h);
    }
    else {
        w = g_window_width;
        h = g_window_height;

        fs_log("using windowed mode\n");
        //SDL_putenv("SDL_VIDEO_WINDOW_POS=");
        fs_log("setting (windowed) video mode %d %d\n", w, h);
    }

    if (fs_config_get_boolean("window_border") == 0) {
        fs_log("borderless window requested\n");
        flags |= SDL_WINDOW_BORDERLESS;
    }

#if 0
    Uint8 data[] = "\0";
    SDL_Cursor *cursor = SDL_CreateCursor(data, data, 8, 1, 0, 0);
    SDL_SetCursor(cursor);
#endif

    g_fs_ml_video_width = w;
    g_fs_ml_video_height = h;
    fs_log("SDL_CreateWindow(x=%d, y=%d, w=%d, h=%d, flags=%d)\n",
           x, y, w, h, flags);
    g_fs_ml_window = SDL_CreateWindow(g_window_title, x, y, w, h, flags);

    int assume_refresh_rate = fs_config_get_int("assume_refresh_rate");
    if (assume_refresh_rate != FS_CONFIG_NONE) {
        fs_log("[DISPLAY] Assuming host refresh rate: %d Hz (from config)\n",
                assume_refresh_rate);
        g_fs_emu_video_frame_rate_host = assume_refresh_rate;
    } else {
        SDL_DisplayMode mode;
        if (SDL_GetWindowDisplayMode(g_fs_ml_window, &mode) == 0) {
            g_fs_emu_video_frame_rate_host = mode.refresh_rate;
        } else {
            g_fs_emu_video_frame_rate_host = 0;
        }
        fs_log("[DISPLAY] Host refresh rate: %d Hz\n",
               g_fs_emu_video_frame_rate_host);
    }

    if (g_fs_emu_video_frame_rate_host) {
        g_fs_ml_target_frame_time = 1000000 / g_fs_emu_video_frame_rate_host;
    }

    g_fs_ml_context = SDL_GL_CreateContext(g_fs_ml_window);
    static int glew_initialized = 0;
    if (!glew_initialized) {
        GLenum err = glewInit();
        if (GLEW_OK != err) {
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
          fs_emu_fatal("Error initializing glew");
        }
        fs_log("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    }

    fs_ml_configure_window();

    // FIXME: this can be removed
    g_fs_ml_opengl_context_stamp++;

    log_opengl_information();
}

bool fs_ml_fullscreen(void)
{
    /* FIXME: This can return (kind of) false answer if a fullscreen
     * event is unprocessed in the event queue. */
    return g_fs_emu_video_fullscreen;
}

void fs_ml_set_fullscreen(bool fullscreen)
{
    if (!is_video_thread()) {
        if (fullscreen) {
            fs_log("Posting enable fullscreen event\n");
            post_video_event(FS_ML_VIDEO_EVENT_ENABLE_FULLSCREEN);
        } else {
            fs_log("Posting disable fullscreen event\n");
            post_video_event(FS_ML_VIDEO_EVENT_DISABLE_FULLSCREEN);
        }
        return;
    }

    if (fullscreen == g_fs_emu_video_fullscreen)
        return;

    if (g_fs_emu_video_fullscreen_mode == FULLSCREEN_WINDOW) {
        fs_emu_warning("Cannot toggle fullscreen with fullscreen-mode=window");
        return;
    }

    int display_index = 0;
    SDL_DisplayMode mode;
    memset(&mode, 0, sizeof(SDL_DisplayMode));
    if (SDL_GetDesktopDisplayMode(display_index, &mode) == 0) {
        SDL_SetWindowDisplayMode(g_fs_ml_window, &mode);
    }

    int flags = 0;
    if (fullscreen) {
        if (g_fs_emu_video_fullscreen_mode == FULLSCREEN_DESKTOP)
            flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
        else
            flags = SDL_WINDOW_FULLSCREEN;
    }
    SDL_SetWindowFullscreen(g_fs_ml_window, flags);
    g_fs_emu_video_fullscreen = fullscreen;
}

void fs_ml_toggle_fullscreen(void)
{
    if (!is_video_thread()) {
        fs_log("Posting toggle video event\n");
        post_video_event(FS_ML_VIDEO_EVENT_TOGGLE_FULLSCREEN);
        return;
    }
    fs_ml_set_fullscreen(!fs_ml_fullscreen());
}

static int g_fs_emu_monitor_count;
// static FSEmuMonitor g_fs_emu_monitors[FS_EMU_MONITOR_MAX_COUNT];
static GArray *g_fs_emu_monitors;

static gint fs_emu_monitor_compare(gconstpointer a, gconstpointer b)
{
    FSEmuMonitor *am = (FSEmuMonitor *) a;
    FSEmuMonitor *bm = (FSEmuMonitor *) b;

    return am->rect.x - bm->rect.x;
}

int fs_ml_video_mode_get_current(fs_ml_video_mode *mode)
{
    mode->width = 0;
    mode->height = 0;
    mode->fps = 0;
    mode->bpp = 0;
    mode->flags = 0;

    FSEmuMonitor monitor;
    fs_emu_monitor_get_by_index(g_display, &monitor);
    mode->width = monitor.rect.w;
    mode->height = monitor.rect.h;
    mode->fps = monitor.refresh_rate;

    if (mode->fps == 0) {
        fs_log("WARNING: refresh rate was not detected\n");
        fs_log("full video sync will not be enabled automatically, but can "
                "be forced\n");
    }
}

static void fs_emu_monitor_init()
{
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;

    g_fs_emu_monitors = g_array_new(false, true, sizeof(FSEmuMonitor));

    int display_index = 0;
    while (true) {
        SDL_DisplayMode mode;
        int error = SDL_GetDesktopDisplayMode(display_index, &mode);
        if (error) {
            break;
        }

        FSEmuMonitor monitor;
        monitor.index = display_index;
        SDL_Rect rect;
        error = SDL_GetDisplayBounds(display_index, &rect);
        if (error) {
            fs_log("Error retrieving display bounds for display %d: %s\n",
                   display_index, SDL_GetError());
            monitor.rect.x = 0;
            monitor.rect.y = 0;
            monitor.rect.w = 1024;
            monitor.rect.h = 768;
            monitor.refresh_rate = 1;
        } else {
            monitor.rect.x = rect.x;
            monitor.rect.y = rect.y;
            monitor.rect.w = rect.w;
            monitor.rect.h = rect.h;
            monitor.refresh_rate = mode.refresh_rate;
        }
        fs_log("[DISPLAY] %d: %dx%d+%d+%d @%d\n", display_index,
               monitor.rect.w, monitor.rect.h, monitor.rect.x, monitor.rect.y,
               monitor.refresh_rate);
        g_array_append_val(g_fs_emu_monitors, monitor);
        display_index += 1;
    }
    g_fs_emu_monitor_count = display_index;

#if 0
    SDL_DisplayMode mode;
    int error = SDL_GetCurrentDisplayMode(display_index, &mode);
    if (error) {
        fs_log("SDL_GetCurrentDisplayMode failed\n");
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR, "Display Error",
            "SDL_GetCurrentDisplayMode failed.", NULL);
        exit(1);
    }
    g_fs_emu_monitor_count = SDL_GetNumVideoDisplays();

    if (g_fs_emu_monitor_count < 1) {
        fs_log("Error %d retrieving number of displays/monitors\n",
               g_fs_emu_monitor_count);
        g_fs_emu_monitor_count = 1;
    }
    if (g_fs_emu_monitor_count >  FS_EMU_MONITOR_MAX_COUNT) {
        fs_log("Limiting number of displays to %d\n",
                FS_EMU_MONITOR_MAX_COUNT);
        g_fs_emu_monitor_count =  FS_EMU_MONITOR_MAX_COUNT;
    }

    for (int i = 0; i < g_fs_emu_monitor_count; i++) {
        SDL_Rect rect;
        FSEmuMonitor monitor;
        int error = SDL_GetDisplayBounds(i, &rect);
        if (error) {
            fs_log("Error retrieving display bounds for display %d: %s\n",
                   i, SDL_GetError());
            /* Setting dummy values on error*/
            rect.x = 0;
            rect.y = 0;
            rect.w = 1024;
            rect.h = 768;
        }

        monitor.rect.x = rect.x;
        monitor.rect.y = rect.y;
        monitor.rect.w = rect.w;
        monitor.rect.h = rect.h;
        monitor.index = i;

        g_array_append_val(g_fs_emu_monitors, monitor);
    }
#endif

    g_array_sort(g_fs_emu_monitors, fs_emu_monitor_compare);
    for (int i = 0; i < g_fs_emu_monitor_count; i++) {
        g_array_index(g_fs_emu_monitors, FSEmuMonitor, i).index = i;
        /* Set physical position flags (left, m-left, m-right, right) */
        int flags = 0;
        for (int j = 0; j < 4; j++) {
            int pos = (g_fs_emu_monitor_count - 1.0) * j / 3.0 + 0.5;
            fs_log("Monitor - j %d pos %d\n", j, pos);
            if (pos == i) {
                flags |= (1 << j);
            }
        }
        fs_log("Monitor index %d flags %d\n", i, flags);
        g_array_index(g_fs_emu_monitors, FSEmuMonitor, i).flags = flags;
    }
}

int fs_emu_monitor_count()
{
    return g_fs_emu_monitor_count;
}

bool fs_emu_monitor_get_by_index(int index, FSEmuMonitor* monitor)
{
    if (index < 0 || index >= g_fs_emu_monitor_count) {
        monitor->index = -1;
        monitor->flags = 0;
        monitor->rect.x = 0;
        monitor->rect.y = 0;
        monitor->rect.w = 1024;
        monitor->rect.h = 768;
        monitor->refresh_rate = 1;
        return false;
    }
    SDL_assert(monitor != NULL);
    memcpy(monitor, &g_array_index(g_fs_emu_monitors, FSEmuMonitor, index),
           sizeof(FSEmuMonitor));
    return true;
}

bool fs_emu_monitor_get_by_flag(int flag, FSEmuMonitor* monitor)
{
    for (int i = 0; i < g_fs_emu_monitor_count; i++) {
        if ((g_array_index(g_fs_emu_monitors,
                          FSEmuMonitor, i).flags & flag) == flag) {
            fs_log("Monitor: found index %d for flag %d\n", i, flag);
            return fs_emu_monitor_get_by_index(i, monitor);
        }
    }
    fs_emu_monitor_get_by_index(0, monitor);
    return false;
}

int fs_ml_video_create_window(const char *title)
{
    fs_log("fs_ml_video_create_window\n");
    g_window_title = g_strdup(title);

    g_fs_ml_keyboard_input_grab = fs_config_get_boolean(
            "keyboard_input_grab");
    if (g_fs_ml_automatic_input_grab == FS_CONFIG_NONE) {
        g_fs_ml_keyboard_input_grab = 1;
    }
    fs_log("keyboard input grab: %d\n", g_fs_ml_keyboard_input_grab);

    static int initialized = 0;

    SDL_SetHint(SDL_HINT_GRAB_KEYBOARD,
                g_fs_ml_keyboard_input_grab ? "1" : "0");

    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version);
    fs_log("FS-UAE was compiled for SDL %d.%d.%d\n",
           version.major, version.minor, version.patch);

    if (!initialized) {
        int display_index = 0;
        SDL_DisplayMode mode;
        int error = SDL_GetCurrentDisplayMode(display_index, &mode);
        if (error) {
            fs_log("SDL_GetCurrentDisplayMode failed\n");
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR, "Display Error",
                "SDL_GetCurrentDisplayMode failed.", NULL);
            exit(1);
        }

        fs_emu_monitor_init();

        const char *mon = fs_config_get_const_string("monitor");
        int mon_flag = -1;
        if (mon == NULL) {
            mon = "middle-left";
        }
        if (strcmp(mon, "left") == 0) {
            mon_flag = FS_EMU_MONITOR_FLAG_LEFT;
        } else if (strcmp(mon, "middle-left") == 0) {
            mon_flag = FS_EMU_MONITOR_FLAG_MIDDLE_LEFT;
        } else if (strcmp(mon, "middle-right") == 0) {
            mon_flag = FS_EMU_MONITOR_FLAG_MIDDLE_RIGHT;
        } else if (strcmp(mon, "right") == 0) {
            mon_flag = FS_EMU_MONITOR_FLAG_RIGHT;
        }
        else {
            mon_flag = FS_EMU_MONITOR_FLAG_MIDDLE_LEFT;
        }
        FSEmuMonitor monitor;
        fs_emu_monitor_get_by_flag(mon_flag, &monitor);
        fs_log("Monitor \"%s\" (flag %d) => index %d\n",
               mon, mon_flag, monitor.index);
        g_display = monitor.index;

        g_fullscreen_width = fs_config_get_int("fullscreen_width");
        if (g_fullscreen_width == FS_CONFIG_NONE) {
            g_fullscreen_width = mode.w;
        }
        g_fullscreen_height = fs_config_get_int("fullscreen_height");
        if (g_fullscreen_height == FS_CONFIG_NONE) {
            g_fullscreen_height = mode.h;
        }

        if (g_fs_emu_video_fullscreen_mode_string == NULL) {
            g_fs_emu_video_fullscreen_mode = -1;
        }
        else if (g_ascii_strcasecmp(g_fs_emu_video_fullscreen_mode_string,
                "window") == 0) {
            g_fs_emu_video_fullscreen_mode = FULLSCREEN_WINDOW;
        }
        else if (g_ascii_strcasecmp(g_fs_emu_video_fullscreen_mode_string,
                "fullscreen") == 0) {
            g_fs_emu_video_fullscreen_mode = FULLSCREEN_FULLSCREEN;
        }
        else if (g_ascii_strcasecmp(g_fs_emu_video_fullscreen_mode_string,
                "desktop") == 0) {
            g_fs_emu_video_fullscreen_mode = FULLSCREEN_DESKTOP;
        }
        if (g_fs_emu_video_fullscreen_mode == -1) {
#ifdef MACOSX
            g_fs_emu_video_fullscreen_mode = FULLSCREEN_FULLSCREEN;
#else
            g_fs_emu_video_fullscreen_mode = FULLSCREEN_FULLSCREEN;
#endif
            fs_log("defaulting to fullscreen_mode = desktop for SDL2\n");
            g_fs_emu_video_fullscreen_mode = FULLSCREEN_DESKTOP;
        }

        initialized = 1;
    }

    if (g_fs_ml_video_sync) {
        g_fs_ml_vblank_sync = 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (g_fsaa) {
        fs_log("setting FSAA samples to %d\n", g_fsaa);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, g_fsaa);
    }

    g_window_width = fs_config_get_int("window_width");
    if (g_window_width == FS_CONFIG_NONE) {
        g_window_width = 1920 / 2;
    }
    g_window_height = fs_config_get_int("window_height");
    if (g_window_height == FS_CONFIG_NONE) {
        g_window_height = 1080/ 2;
    }
    g_window_x = fs_config_get_int("window_x");
    if (g_window_x == FS_CONFIG_NONE) {
        g_window_x = SDL_WINDOWPOS_CENTERED;
    }
    g_window_y = fs_config_get_int("window_y");
    if (g_window_y == FS_CONFIG_NONE) {
        g_window_y = SDL_WINDOWPOS_CENTERED;
    }
    g_window_resizable = fs_config_get_boolean("window_resizable");
    if (g_window_resizable == FS_CONFIG_NONE) {
        g_window_resizable = 1;
    }

    g_fs_ml_automatic_input_grab = fs_config_get_boolean(
            "automatic_input_grab");
    if (g_fs_ml_automatic_input_grab == FS_CONFIG_NONE) {
        if (fs_ml_mouse_integration()) {
            g_fs_ml_automatic_input_grab = 0;
        } else {
            g_fs_ml_automatic_input_grab = 1;
        }
    }
    fs_log("automatic input grab: %d\n", g_fs_ml_automatic_input_grab);

    g_initial_input_grab = g_fs_ml_automatic_input_grab;
    if (fs_config_get_boolean("initial_input_grab") == 1) {
        g_initial_input_grab = 1;
    }
    else if (fs_config_get_boolean("initial_input_grab") == 0 ||
            // deprecated names:
            fs_config_get_boolean("input_grab") == 0 ||
            fs_config_get_boolean("grab_input") == 0) {
        g_initial_input_grab = 0;
    }

    set_video_mode();

    if (g_fs_ml_vblank_sync) {
        fs_emu_log("*** Setting swap interval to 1 ***\n");
        if (SDL_GL_SetSwapInterval(1) != 0) {
            fs_emu_warning("SDL_GL_SetSwapInterval(1) failed");
        }
    }
    else {
        fs_emu_log("*** Setting swap interval to 0 ***\n");
        SDL_GL_SetSwapInterval(0);
    }

    // we display a black frame as soon as possible (to reduce flickering on
    // startup)
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(g_fs_ml_window);
    fs_gl_finish();

    fs_log("initial input grab: %d\n", g_initial_input_grab);
    if (g_initial_input_grab && !g_has_input_grab) {
        fs_ml_set_input_grab(true);
    }
    fs_ml_show_cursor(0, 1);

    // this function must be called from the video thread
    fs_log("init_opengl\n");
    fs_emu_video_init_opengl();

    SDL_StartTextInput();

    fs_log("create windows is done\n");
    return 1;
}

int g_fs_ml_running = 1;

#ifndef WINDOWS
// using separate implementation on Windows with raw input
void fs_ml_clear_keyboard_modifier_state()
{

}
#endif

#include "sdl2_keys.h"
// modifiers have values in SDL and SDL2, except META is renamed to GUI
#define KMOD_LMETA KMOD_LGUI
#define KMOD_RMETA KMOD_RGUI
#define KMOD_META (KMOD_LMETA|KMOD_RMETA)

static void on_resize(int width, int height)
{
    if (width == g_fs_ml_video_width && height == g_fs_ml_video_height) {
        fs_log("got resize event, but size was unchanged\n");
        return;
    }
    if (g_fs_emu_video_fullscreen) {
        fs_log("not updating window size in fullscreen\n");
    }
    else if (width == g_fullscreen_width &&
        height == g_fullscreen_height) {
        fs_log("not setting window size to fullscreen size\n");
    }
    else {
        g_window_width = width;
        g_window_height = height;
        fs_log("resize event %d %d\n", width, height);
    }
    g_fs_ml_video_width = width;
    g_fs_ml_video_height = height;
}

int fs_ml_event_loop(void)
{
    // printf("fs_ml_event_loop\n");
    int result = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            fs_log("Received SDL_QUIT\n");
            fs_ml_quit();
#ifdef FS_EMU_DRIVERS
            printf("returning 1 from fs_ml_event_loop\n");
            result = 1;
#endif
            continue;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                on_resize(event.window.data1, event.window.data2);
            } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                event.type = SDL_QUIT;
                SDL_PushEvent(&event);
            } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                if (g_grab_input_on_activate) {
                    fs_log("Window focus gained - grabbing input\n");
                    g_grab_input_on_activate = false;
                    fs_ml_set_input_grab(true);
#ifdef MACOSX
                } else if (fs_ml_input_grab()) {
                    /* Input grab could be "lost" due to Cmd+Tab */
                    fs_log("Forcing re-grab of input on OS X\n");
                    fs_ml_set_input_grab(false);
                    fs_ml_set_input_grab(true);
#endif
                }
            }
            continue;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (g_fs_log_input) {
                fs_log("SDL key sym %d mod %d scancode %d state %d repeat %d\n",
                        event.key.keysym.sym, event.key.keysym.mod,
                        event.key.keysym.scancode, event.key.state,
                       event.key.repeat);
            }
            if (event.key.repeat) {
                continue;
            }
            if (event.key.keysym.sym == 0 && event.key.keysym.scancode == 0) {
                /* ignore "ghost key" seen on OS X which without this
                 * specific check will cause the A key to be mysteriously
                 * pressed. */
                if (g_fs_log_input) {
                    fs_log("- ignored key with keysym 0 and scancode 0\n");
                }
                continue;
            }
            /*
            if (event.key.keysym.sym == SDLK_F12) {
                g_f12_state = event.key.state ? FS_ML_KEY_MOD_F12 : 0;
                printf("-- g_f12_state is %d\n", g_f12_state);
            }
            else if (event.key.keysym.sym == SDLK_F11) {
                g_f11_state = event.key.state ? FS_ML_KEY_MOD_F11 : 0;
            }
            */

            const Uint8* key_state;
            int num_keys;
            key_state = SDL_GetKeyboardState(&num_keys);
            g_f11_state = key_state[SDL_SCANCODE_F11] ? FS_ML_KEY_MOD_F11 : 0;
            g_f12_state = key_state[SDL_SCANCODE_F12] ? FS_ML_KEY_MOD_F12 : 0;

            int key = -1;
            if (event.key.keysym.scancode <= LAST_SDL2_SCANCODE) {
                key = g_sdl2_keys[event.key.keysym.scancode];
            }
#if defined(MACOSX)
#elif defined(WINDOWS)
#else
            else if (event.key.keysym.sym == SDLK_MODE) {
                key = SDLK_RALT;
            }
#endif
            else {
                key = fs_ml_scancode_to_key(event.key.keysym.scancode);
            }

#ifdef USE_SDL2
            if (0) {
                // the below trick does not currently work for SDL2, as
                // there is no mapping yet for translated keys
            }
#else
            if (g_f12_state || g_f11_state) {
                // leave translated key code in keysym
            }
#endif
            else if (key >= 0) {
                if (g_fs_log_input) {
                    fs_log("- key code set to %d (was %d) based on "
                           "scancode %d\n", key, event.key.keysym.sym,
                           event.key.keysym.scancode);
                }
                event.key.keysym.sym = key;
            }

            int mod = event.key.keysym.mod;
            if (mod & KMOD_LSHIFT || mod & KMOD_RSHIFT)
                event.key.keysym.mod |= KMOD_SHIFT;
#if 0
            if (mod & KMOD_LALT || mod & KMOD_RALT)
                event.key.keysym.mod |= KMOD_ALT;
#endif
            if (mod & KMOD_LCTRL || mod & KMOD_RCTRL)
                event.key.keysym.mod |= KMOD_CTRL;
#if 0
            if (mod & KMOD_LMETA || mod & KMOD_RMETA)
                event.key.keysym.mod |= KMOD_META;
#endif

            /* Filter out other modidifers */
            event.key.keysym.mod &=
                        KMOD_SHIFT | KMOD_ALT | KMOD_CTRL | KMOD_META;
            /* Add F11/F12 modifier state */
            event.key.keysym.mod |= g_f11_state | g_f12_state;

            //printf("%d %d %d %d\n", event.key.keysym.mod,
            //        KMOD_ALT, KMOD_LALT, KMOD_RALT);
            break;
        //case SDL_MOUSEBUTTONDOWN:
        //    printf("--- mousebutton down ---\n");
        }
        fs_ml_event *new_event = NULL;

        if (event.type == SDL_KEYDOWN) {
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_KEYDOWN;
            new_event->key.keysym.sym = event.key.keysym.sym;
            new_event->key.keysym.mod = event.key.keysym.mod;
            new_event->key.state = event.key.state;
        }
        else if (event.type == SDL_KEYUP) {
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_KEYUP;
            new_event->key.keysym.sym = event.key.keysym.sym;
            new_event->key.keysym.mod = event.key.keysym.mod;
            new_event->key.state = event.key.state;
        }
        else if (event.type == SDL_JOYBUTTONDOWN) {
            if (g_fs_log_input) {
                fs_log("SDL_JOYBUTTONDOWN which %d button %d state %d\n",
                       event.jbutton.which, event.jbutton.button,
                       event.jbutton.state);
            }
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_JOYBUTTONDOWN;
            new_event->jbutton.which = \
                    g_fs_ml_sdl_joystick_index_map[event.jbutton.which];
            new_event->jbutton.button = event.jbutton.button;
            new_event->jbutton.state = event.jbutton.state;
        }
        else if (event.type == SDL_JOYBUTTONUP) {
            if (g_fs_log_input) {
                fs_log("SDL_JOYBUTTONUP which %d button %d state %d\n",
                       event.jbutton.which, event.jbutton.button,
                       event.jbutton.state);
            }
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_JOYBUTTONUP;
            new_event->jbutton.which = \
                    g_fs_ml_sdl_joystick_index_map[event.jbutton.which];
            new_event->jbutton.button = event.jbutton.button;
            new_event->jbutton.state = event.jbutton.state;
        }
        else if (event.type == SDL_JOYAXISMOTION) {
            /* Not logging axis motion, too much noise */
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_JOYAXISMOTION;
            new_event->jaxis.which = \
                    g_fs_ml_sdl_joystick_index_map[event.jaxis.which];
            new_event->jaxis.axis = event.jaxis.axis;
            new_event->jaxis.value = event.jaxis.value;
        }
        else if (event.type == SDL_JOYHATMOTION) {
            if (g_fs_log_input) {
                fs_log("SDL_JOYHATMOTION which %d hat %d value %d\n",
                       event.jhat.which, event.jhat.hat, event.jhat.value);
            }
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_JOYHATMOTION;
            new_event->jhat.which = \
                    g_fs_ml_sdl_joystick_index_map[event.jhat.which];
            new_event->jhat.hat = event.jhat.hat;
            new_event->jhat.value = event.jhat.value;
        }
        else if (event.type == SDL_MOUSEMOTION) {
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_MOUSEMOTION;
            new_event->motion.device = g_fs_ml_first_mouse_index;
            new_event->motion.xrel = event.motion.xrel;
            new_event->motion.yrel = event.motion.yrel;
            /* Absolute window coordinates */
            new_event->motion.x = event.motion.x;
            new_event->motion.y = event.motion.y;
            //printf("ISREL %d\n", SDL_GetRelativeMouseMode());

            if (g_fs_log_input) {
                fs_log("SDL mouse event x: %4d y: %4d xrel: %4d yrel: %4d\n", 
                    event.motion.x, event.motion.y,
                    event.motion.xrel, event.motion.yrel);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_MOUSEBUTTONDOWN;
            new_event->button.device = g_fs_ml_first_mouse_index;
            new_event->button.button = event.button.button;
#ifdef MACOSX
            if (new_event->button.button == 1) {
                int mod = SDL_GetModState();
                if (mod & KMOD_ALT) {
                    new_event->button.button = 2;
                }
                else if (mod & KMOD_CTRL) {
                    new_event->button.button = 3;
                }
            }
#endif
            new_event->button.state = event.button.state;
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_MOUSEBUTTONUP;
            new_event->button.device = g_fs_ml_first_mouse_index;
            new_event->button.button = event.button.button;
#ifdef MACOSX
            if (new_event->button.button == 1) {
                int mod = SDL_GetModState();
                if (mod & KMOD_ALT) {
                    new_event->button.button = 2;
                }
                else if (mod & KMOD_CTRL) {
                    new_event->button.button = 3;
                }
            }
#endif
            new_event->button.state = event.button.state;
        }
        else if (event.type == SDL_MOUSEWHEEL) {
            /*
            if (event.wheel.which == SDL_TOUCH_MOUSEID) {

            }
            */
            if (event.wheel.y) {
                if (g_fs_log_input) {
                    fs_log("SDL mouse event y-scroll: %4d\n",
                        event.wheel.y);
                }
                new_event = fs_ml_alloc_event();
                new_event->type = FS_ML_MOUSEBUTTONDOWN;
                if (event.wheel.y > 0) {
                    new_event->button.button = FS_ML_BUTTON_WHEELUP;
                }
                else {
                    new_event->button.button = FS_ML_BUTTON_WHEELDOWN;
                }
                new_event->button.device = g_fs_ml_first_mouse_index;
                new_event->button.state = 1;
            }
        }
        else if (event.type == SDL_TEXTINPUT) {
            new_event = fs_ml_alloc_event();
            new_event->type = FS_ML_TEXTINPUT;
            memcpy(&(new_event->text.text), &(event.text.text),
                   MIN(TEXTINPUTEVENT_TEXT_SIZE, SDL_TEXTINPUTEVENT_TEXT_SIZE));
            new_event->text.text[TEXTINPUTEVENT_TEXT_SIZE - 1] = 0;
        }

        if (new_event) {
            fs_ml_post_event(new_event);
        }
    }
    return result;
}

void fs_ml_video_swap_buffers()
{
    SDL_GL_SwapWindow(g_fs_ml_window);
}

static void post_main_loop(void)
{
    /* We want to improve the transitioning from FS-UAE back to e.g.
     * FS-UAE Game Center - avoid blinking cursor - so we try to move it (to
     * the bottom right of the screen). This probably requires that the
     * cursor is not grabbed (SDL often keeps the cursor in the center of the
     * screen then). */
    if (g_fs_emu_video_fullscreen) {
        if (SDL_getenv("FSGS_RETURN_CURSOR_TO") &&
                SDL_getenv("FSGS_RETURN_CURSOR_TO")[0]) {
            int x = -1; int y = -1;
            sscanf(SDL_getenv("FSGS_RETURN_CURSOR_TO"), "%d,%d", &x, &y);
            if (x != -1 && y != -1) {
#if 0
                fs_log("trying to move mouse cursor to x=%d y=%d\n", x, y);
#endif
                Uint8 data[] = "\0";
                SDL_SetWindowGrab(g_fs_ml_window, SDL_FALSE);
                /* Setting invisible cursor so we won't see it when we
                 * enable the cursor in order to move it. */
                SDL_Cursor *cursor = SDL_CreateCursor(data, data, 8, 1, 0, 0);
                SDL_SetCursor(cursor);
                SDL_ShowCursor(SDL_ENABLE);
                SDL_WarpMouseInWindow(g_fs_ml_window, x, y);
            }
        }
    }
}

int fs_ml_main_loop()
{
    while (g_fs_ml_running) {
        fs_ml_event_loop();
        process_video_events();
        fs_ml_prevent_power_saving();
        fs_ml_render_iteration();
    }
    post_main_loop();
    return 0;
}

void fs_ml_video_init()
{
    FS_ML_INIT_ONCE;

    g_video_thread_id = fs_thread_id();
    g_video_cond = fs_condition_create();
    g_video_mutex = fs_mutex_create();
    g_video_event_queue = g_queue_new();
    g_video_event_mutex = fs_mutex_create();

    fs_ml_render_init();
}

#endif
