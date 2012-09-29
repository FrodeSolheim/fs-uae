#ifdef WITH_SDL_VIDEO

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <glib.h>
#include <SDL.h>

#include <fs/config.h>
#include <fs/glee.h>
#include <fs/ml.h>
//#include "fs/emu.h"
#include "ml_internal.h"

// FIXME: make libfsml independent of libfsmeu
#include "../emu/video.h"

static GQueue *g_video_event_queue = NULL;
static GMutex *g_video_event_mutex = NULL;

static GCond *g_video_cond = NULL;
static GMutex *g_video_mutex = NULL;

static SDL_Surface *g_sdl_screen = NULL;
//static int g_sync_vblank = 1;

// current size of window or fullscreen view
//static int g_video_width = 0;
//static int g_video_height = 0;
static int g_has_input_grab = 0;
static int g_fs_ml_automatic_input_grab = 1;
static int g_fsaa = 0;

static int g_debug_keys = 0;
static int g_f12_state = 0;
static int g_f11_state = 0;

static char *g_window_title;
static int g_window_width;
static int g_window_height;
static int g_window_resizable;
static int g_fullscreen_width;
static int g_fullscreen_height;

int g_fs_ml_had_input_grab = 0;
int g_fs_ml_was_fullscreen = 0;

#define FS_ML_VIDEO_EVENT_GRAB_INPUT 1
#define FS_ML_VIDEO_EVENT_UNGRAB_INPUT 2
#define FS_ML_VIDEO_EVENT_SHOW_CURSOR 3
#define FS_ML_VIDEO_EVENT_HIDE_CURSOR 4

int fs_ml_get_fullscreen_width() {
    return g_fullscreen_width;
}

int fs_ml_get_fullscreen_height() {
    return g_fullscreen_height;
}

int fs_ml_get_windowed_width() {
    return g_window_width;
}

int fs_ml_get_windowed_height() {
    return g_window_height;
}

static void post_video_event(int event) {
    g_mutex_lock(g_video_event_mutex);
    g_queue_push_head(g_video_event_queue, GINT_TO_POINTER(event));
    g_mutex_unlock(g_video_event_mutex);
}

static void process_video_events() {
    g_mutex_lock(g_video_event_mutex);
    int count = g_queue_get_length(g_video_event_queue);
    for (int i = 0; i < count; i++) {
        int event = GPOINTER_TO_INT(g_queue_pop_tail(g_video_event_queue));
        if (event == FS_ML_VIDEO_EVENT_GRAB_INPUT) {
            fs_ml_grab_input(1, 1);
        }
        else if (event == FS_ML_VIDEO_EVENT_UNGRAB_INPUT) {
            fs_ml_grab_input(0, 1);
        }
        else if (event == FS_ML_VIDEO_EVENT_SHOW_CURSOR) {
            fs_ml_show_cursor(1, 1);
        }
        else if (event == FS_ML_VIDEO_EVENT_HIDE_CURSOR) {
            fs_ml_show_cursor(0, 1);
        }
    }
    g_mutex_unlock(g_video_event_mutex);
}

int fs_ml_has_input_grab() {
    //printf("has input grab? %d\n", g_grab_input);
    return g_has_input_grab;
}

int fs_ml_has_automatic_input_grab() {
    return g_fs_ml_automatic_input_grab;
}

void fs_ml_grab_input(int grab, int immediate) {
    //printf("fs_ml_grab_input %d %d\n", grab, immediate);
    if (immediate) {
        SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
        fs_ml_show_cursor(!grab, 1);
    }
    else {
        post_video_event(grab ? FS_ML_VIDEO_EVENT_GRAB_INPUT :
                FS_ML_VIDEO_EVENT_UNGRAB_INPUT);
    }
    g_has_input_grab = grab ? 1 : 0;
}

void fs_ml_set_video_fsaa(int fsaa) {
    g_fsaa = fsaa;
}

void fs_ml_show_cursor(int show, int immediate) {
    if (immediate) {
        SDL_ShowCursor(show);
    }
    else {
        post_video_event(show ? FS_ML_VIDEO_EVENT_SHOW_CURSOR :
                FS_ML_VIDEO_EVENT_HIDE_CURSOR);
    }
}

static void log_opengl_information() {
    static int written = 0;
    if (written) {
        return;
    }
    written = 1;
    const GLubyte *s;
    s = glGetString(GL_VENDOR);
    if (s) {
        fs_log("opengl vendor: %s\n", s);
    }
    s = glGetString(GL_RENDERER);
    if (s) {
        fs_log("opengl renderer: %s\n", s);
    }
    s = glGetString(GL_VERSION);
    if (s) {
        fs_log("opengl version: %s\n", s);
    }
    s = glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (s) {
        fs_log("opengl shading language version: %s\n", s);
    }
    s = glGetString(GL_EXTENSIONS);
    if (s) {
        fs_log("opengl extensions: %s\n", s);
    }
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    fs_log("opengl max texture size (estimate): %dx%d\n", max_texture_size,
            max_texture_size);
}

static void set_video_mode() {
    int flags = 0;
    flags |= SDL_DOUBLEBUF;
    flags |= SDL_OPENGL;
    if (g_fs_emu_video_fullscreen == 1) {
        g_fs_ml_video_width = g_fullscreen_width;
        g_fs_ml_video_height = g_fullscreen_height;
        if (g_fs_emu_video_fullscreen_window) {
            fs_log("using fullscreen window mode\n");
            SDL_putenv("SDL_VIDEO_WINDOW_POS=0,0");
            flags |= SDL_NOFRAME;
            fs_ml_set_fullscreen_extra();
        }
        else {
            fs_log("using SDL_FULLSCREEN mode\n");
            flags |= SDL_FULLSCREEN;
        }
        fs_log("setting (fullscreen) video mode %d %d\n", g_fs_ml_video_width,
                g_fs_ml_video_height);
        g_sdl_screen = SDL_SetVideoMode(g_fs_ml_video_width,
                g_fs_ml_video_height, 0, flags);
        //update_viewport();
    }
    else {
        fs_log("using windowed mode\n");
        //SDL_putenv("SDL_VIDEO_WINDOW_POS=");
        g_fs_ml_video_width = g_window_width;
        g_fs_ml_video_height = g_window_height;
        if (g_window_resizable) {
            flags |= SDL_RESIZABLE;
        }
        fs_log("setting (windowed) video mode %d %d\n", g_fs_ml_video_width,
                g_fs_ml_video_height);
        g_sdl_screen = SDL_SetVideoMode(g_fs_ml_video_width,
                g_fs_ml_video_height, 0, flags);
        //update_viewport();
    }

    fs_ml_configure_window();

    // FIXME: this can be removed
    g_fs_ml_opengl_context_stamp++;

    log_opengl_information();

    //Uint8* keystate;
    //int numkeys;
    //keystate = SDL_GetKeyState(&numkeys);
    //printf("%d\n", keystate[SDLK_F11]);
    //printf("%d\n", keystate[SDLK_F12]);
    //g_f11_state = keystate[SDLK_F11] ? FS_ML_KEY_MOD_F11 : 0;
    //g_f12_state = keystate[SDLK_F12] ? FS_ML_KEY_MOD_F12 : 0;
    //printf("g_f12_state is %d\n", g_f12_state);
}

static void destroy_opengl_state() {
    fs_log("destroy_opengl_state\n");
    fs_gl_send_context_notification(FS_GL_CONTEXT_DESTROY);
}

static void recreate_opengl_state() {
    fs_log("recreate_opengl_state\n");
    fs_gl_reset_client_state();
    fs_gl_send_context_notification(FS_GL_CONTEXT_CREATE);
}

void fs_ml_toggle_fullscreen() {
    fs_log("fs_ml_toggle_fullscreen\n");
    g_fs_emu_video_fullscreen = !g_fs_emu_video_fullscreen;

    destroy_opengl_state();
    set_video_mode();
    recreate_opengl_state();
}

int fs_ml_video_create_window(const char *title) {
    fs_log("fs_ml_video_create_window\n");
    g_window_title = g_strdup(title);

    static int initialized = 0;
    SDL_Init(SDL_INIT_VIDEO);
    if (!initialized) {
        const SDL_VideoInfo* info = SDL_GetVideoInfo();
        g_fullscreen_width = fs_config_get_int("fullscreen_width");
        if (g_fullscreen_width == FS_CONFIG_NONE) {
            g_fullscreen_width = info->current_w;
        }
        g_fullscreen_height = fs_config_get_int("fullscreen_height");
        if (g_fullscreen_height == FS_CONFIG_NONE) {
            g_fullscreen_height = info->current_h;
        }

        if (g_fs_emu_video_fullscreen_mode == NULL) {
            g_fs_emu_video_fullscreen_window = -1;
        }
        else if (g_ascii_strcasecmp(g_fs_emu_video_fullscreen_mode,
                "window") == 0) {
            g_fs_emu_video_fullscreen_window = 1;
        }
        else if (g_ascii_strcasecmp(g_fs_emu_video_fullscreen_mode,
                "fullscreen") == 0) {
            g_fs_emu_video_fullscreen_window = 0;
        }
        if (g_fs_emu_video_fullscreen_window == -1) {
#ifdef MACOSX
            g_fs_emu_video_fullscreen_window = 0;
#else
            g_fs_emu_video_fullscreen_window = 0;
#endif
        }
        initialized = 1;
    }

    if (g_fs_ml_video_sync) {
        g_fs_ml_vblank_sync = 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if (g_fs_ml_vblank_sync) {
        fs_emu_log("*** SDL_GL_SWAP_CONTROL is enabled ***\n");
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    }
    else {
        fs_emu_log("*** SDL_GL_SWAP_CONTROL is disabled ***\n");
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
    }

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
    g_window_resizable = fs_config_get_boolean("window_resizable");
    if (g_window_resizable == FS_CONFIG_NONE) {
        g_window_resizable = 1;
    }

    set_video_mode();

    // we display a black frame as soon as possible (to reduce flickering on
    // startup)
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    fs_gl_finish();

    SDL_WM_SetCaption(g_window_title, g_get_application_name());

    g_fs_ml_automatic_input_grab = fs_config_get_boolean(
            "automatic_input_grab");
    if (g_fs_ml_automatic_input_grab == FS_CONFIG_NONE) {
        g_fs_ml_automatic_input_grab = 1;
    }
    fs_log("automatic input grab: %d\n", g_fs_ml_automatic_input_grab);

	int initial_input_grab = g_fs_ml_automatic_input_grab;
    if (fs_config_get_boolean("initial_input_grab") == 1) {
        initial_input_grab = 1;
    }
    else if (fs_config_get_boolean("initial_input_grab") == 0 ||
            // deprecated names:
            fs_config_get_boolean("input_grab") == 0 ||
            fs_config_get_boolean("grab_input") == 0) {
        initial_input_grab = 0;
    }
    fs_log("initial input grab: %d\n", initial_input_grab);
    if (initial_input_grab) {
        fs_ml_grab_input(1, 1);
    }
    fs_ml_show_cursor(0, 1);

    // this function must be called from the video thread
    fs_log("init_opengl\n");
    fs_emu_video_init_opengl();

#ifdef WINDOWS
    fs_ml_init_raw_input();
#else
    // enable keysym to unicode char translation
    SDL_EnableUNICODE(1);
#endif
    fs_log("create windows is done\n");
    return 1;
}

int g_fs_ml_running = 1;

#ifndef WINDOWS
// using separate implementation on Windows with raw input
void fs_ml_clear_keyboard_modifier_state() {

}
#endif

static void on_resize(int width, int height) {
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
#ifdef MACOSX
        destroy_opengl_state();
#endif
        set_video_mode();
#ifdef MACOSX
        recreate_opengl_state();
#endif
}

static int event_loop() {
    int result = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            fs_log("intercepted SDL_QUIT\n");
            fs_ml_quit();
            continue;
        case SDL_VIDEORESIZE:
            on_resize(event.resize.w, event.resize.h);
            continue;
        case SDL_ACTIVEEVENT:
            //fs_log("got active event %d %d %d %d\n", event.active.state,
            //      SDL_APPMOUSEFOCUS, SDL_APPINPUTFOCUS, SDL_APPACTIVE);
            if ((event.active.state & SDL_APPINPUTFOCUS)) {
                if (event.active.gain) {
                    fs_log("got keyboard focus\n");
                    // just got keyboard focus -- clearing modifier states
                    fs_ml_clear_keyboard_modifier_state();
                    if (g_fs_ml_had_input_grab) {
                        fs_log("- had input grab, re-acquiring\n");
                        fs_ml_grab_input(1, 1);
                        g_fs_ml_had_input_grab = 0;
                    }
                    if (g_fs_ml_was_fullscreen) {
                        if (!g_fs_emu_video_fullscreen) {
                            fs_log("- was in fullsreen mode before (switching)\n");
                            fs_ml_toggle_fullscreen();
                        }
                        g_fs_ml_was_fullscreen = 0;
                    }
                }
                else {
                    fs_log("lost keyboard focus\n");
                    if (fs_ml_has_input_grab()) {
                        fs_log("- releasing input grab\n");
                        fs_ml_grab_input(0, 1);
                        g_fs_ml_had_input_grab = 1;
                    }
                    else {
                        fs_log("- did not have input grab\n");
                        //g_fs_ml_had_input_grab = 0;
                    }
                }
            }
            continue;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (g_debug_keys) {
                fs_log("SDL key sym %d mod %d scancode %d state %d\n",
                        event.key.keysym.sym, event.key.keysym.mod,
                        event.key.keysym.scancode, event.key.state);
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

            Uint8* key_state;
            int num_keys;
            key_state = SDL_GetKeyState(&num_keys);
            g_f11_state = key_state[SDLK_F11] ? FS_ML_KEY_MOD_F11 : 0;
            g_f12_state = key_state[SDLK_F12] ? FS_ML_KEY_MOD_F12 : 0;

            int key = -1;
            if (0) {
            }
#ifdef MACOSX
            else if (event.key.keysym.sym == SDLK_LSHIFT) {
                key = SDLK_LSHIFT;
            }
            else if (event.key.keysym.sym == SDLK_LCTRL) {
                key = SDLK_LCTRL;
            }
            else if (event.key.keysym.sym == SDLK_LALT) {
                key = SDLK_LALT;
            }
            else if (event.key.keysym.sym == SDLK_LMETA) {
                key = SDLK_LSUPER;
            }
            else if (event.key.keysym.sym == SDLK_RMETA) {
                key = SDLK_RSUPER;
            }
            else if (event.key.keysym.sym == SDLK_RALT) {
                key = SDLK_RALT;
            }
            else if (event.key.keysym.sym == SDLK_RCTRL) {
                key = SDLK_RCTRL;
            }
            else if (event.key.keysym.sym == SDLK_RSHIFT) {
                key = SDLK_RSHIFT;
            }
            else if (event.key.keysym.sym == SDLK_CAPSLOCK) {
                key = SDLK_CAPSLOCK;
            }
#endif
            else {
                key = fs_ml_scancode_to_key(event.key.keysym.scancode);
            }

            if (g_f12_state || g_f11_state) {
                // leave translated key code in keysym
            }
            else if (key >= 0) {
                if (g_debug_keys) {
                    fs_log("- key code set to %d (was %d) based on "
                           "scancode %d\n", key, event.key.keysym.sym,
                           event.key.keysym.scancode);
                }
                event.key.keysym.sym = key;
            }

            int mod = event.key.keysym.mod;
            if (mod & KMOD_LSHIFT || mod & KMOD_RSHIFT) {
                event.key.keysym.mod |= KMOD_SHIFT;
            }
            if (mod & KMOD_LALT || mod & KMOD_RALT) {
                //mod & ~(KMOD_LALT | KMOD_RALT);
                event.key.keysym.mod |= KMOD_ALT;
            }
            if (mod & KMOD_LCTRL || mod & KMOD_RCTRL) {
                event.key.keysym.mod |= KMOD_CTRL;
            }
            if (mod & KMOD_LMETA || mod & KMOD_RMETA) {
                event.key.keysym.mod |= KMOD_META;
            }
            // filter out other modidifers
            event.key.keysym.mod &= (KMOD_SHIFT | KMOD_ALT | KMOD_CTRL |
                    KMOD_META);
            // add F11/F12 state
            event.key.keysym.mod |= g_f11_state | g_f12_state;

            //printf("%d %d %d %d\n", event.key.keysym.mod,
            //        KMOD_ALT, KMOD_LALT, KMOD_RALT);
            break;
        //case SDL_MOUSEBUTTONDOWN:
        //    printf("--- mousebutton down ---\n");
        }
        fs_ml_event *new_event = fs_ml_alloc_event();
        *new_event = event;
        fs_ml_post_event(new_event);
    }
    return result;
}

void fs_ml_video_swap_buffers() {
    SDL_GL_SwapBuffers();
}

int fs_ml_main_loop() {
    while (g_fs_ml_running) {
        event_loop();
        process_video_events();
        fs_ml_render_iteration();
    }
    return 0;
}

void fs_ml_video_init() {
    fs_log("creating condition\n");
    g_video_cond = g_cond_new();
    fs_log("creating mutex\n");
    g_video_mutex = g_mutex_new();

    g_video_event_queue = g_queue_new();
    g_video_event_mutex = g_mutex_new();

    g_debug_keys = getenv("FS_DEBUG_INPUT") && \
            getenv("FS_DEBUG_INPUT")[0] == '1';

    fs_ml_render_init();
}

void fs_ml_input_init() {
    fs_emu_log("fs_ml_input_init\n");
    fs_ml_initialize_keymap();
    /*
    // reset all input mappings
    for (int i = 0; i < (FS_ML_MAX_DEVICES * FS_ML_SLOTS); i++) {
        g_input_action_table[i] = 0;
        g_menu_action_table[i] = 0;
    }
    */
    //g_fs_ml_input_devices[FS_ML_KEYBOARD].name = g_strdup("KEYBOARD");
    //g_fs_ml_input_devices[FS_ML_KEYBOARD].alias = g_strdup("");

    GHashTable *device_counts = g_hash_table_new_full(g_str_hash, g_str_equal,
            g_free, NULL);

    int num_joysticks = SDL_NumJoysticks();
    for (int i = 0; i < num_joysticks; i++) {
        if (i == FS_ML_MAX_JOYSTICKS) {
            fs_emu_log("WARNING: reached max num joysticks");
            break;
        }
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
        gchar* name = g_ascii_strup(SDL_JoystickName(i), -1);
        name = g_strstrip(name);
        int count = GPOINTER_TO_INT(g_hash_table_lookup(device_counts, name));
        count++;
        // hash table now owns name pointer
        g_hash_table_replace(device_counts, g_strdup(name),
                GINT_TO_POINTER(count));
        if (count > 1) {
            gchar *result = g_strdup_printf("%s #%d", name, count);
            g_free(name);
            name = result;
        }
        g_fs_ml_input_devices[i].index = i;
        g_fs_ml_input_devices[i].name = name;
        //g_input_devices[i].alias = g_strdup_printf("JOYSTICK #%d", i);
        if (i == 0) {
            g_fs_ml_input_devices[i].alias = g_strdup("JOYSTICK");
        }
        else {
            g_fs_ml_input_devices[i].alias = g_strdup_printf("JOYSTICK #%d",
                    i + 1);
        }
        g_fs_ml_input_devices[i].hats = SDL_JoystickNumHats(joystick);
        g_fs_ml_input_devices[i].buttons = SDL_JoystickNumButtons(joystick);
        g_fs_ml_input_devices[i].axes = SDL_JoystickNumAxes(joystick);
        g_fs_ml_input_devices[i].balls = SDL_JoystickNumBalls(joystick);
        /*
        for (int j = 0; j < MAX_STATES; j++) {
            g_input_devices[i].actions[j] = 0;
        }
        */
        fs_emu_log("device #%02d found: %s\n", i + 1, name);
        fs_emu_log("- %d buttons %d hats %d axes %d balls\n",
                g_fs_ml_input_devices[i].buttons,
                g_fs_ml_input_devices[i].hats,
                g_fs_ml_input_devices[i].axes,
                g_fs_ml_input_devices[i].balls);
    }

    g_fs_ml_input_devices[num_joysticks].name = g_strdup("KEYBOARD");
    g_fs_ml_input_devices[num_joysticks].alias = g_strdup("");

    g_fs_ml_input_device_count = num_joysticks + 1;
    g_hash_table_destroy(device_counts);

    fs_ml_initialize_keymap();
}

#endif
