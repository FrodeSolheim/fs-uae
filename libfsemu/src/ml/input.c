#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SDL.h>
#include <stdlib.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <fs/glib.h>
#include <fs/ml/options.h>
#include "ml_internal.h"

// For fs_emu_mouse_integration
#include <fs/emu.h>

static GQueue *g_input_queue = NULL;
static fs_mutex *g_input_mutex = NULL;
static fs_ml_input_function g_input_function = NULL;

int g_fs_ml_first_keyboard_index = 0;
int g_fs_ml_first_mouse_index = 0;
int g_fs_ml_first_joystick_index = 0;

static int g_cursor_mode = 1;
static int g_mouse_integration = 0;

bool fs_ml_mouse_integration(void)
{
    return g_mouse_integration;
}

bool fs_ml_cursor_allowed(void)
{
    return g_cursor_mode != 0;
}

fs_ml_event* fs_ml_alloc_event()
{
    return g_malloc(sizeof(fs_ml_event));
}

static void fs_ml_input_event_free(fs_ml_event *event)
{
    g_free(event);
}

void fs_ml_set_input_function(fs_ml_input_function function)
{
    g_input_function = function;
}

int fs_ml_post_event(fs_ml_event* event)
{
    if (event->type == FS_ML_KEYDOWN || event->type == FS_ML_KEYUP) {
        if (fs_ml_handle_keyboard_shortcut(event)) {
            return 1;
        }
    }
    if (g_input_function) {
        g_input_function(event);
    }
#if 0
    fs_mutex_lock(g_input_mutex);
    fs_queue_push_tail(g_input_queue, event);
    fs_mutex_unlock(g_input_mutex);
#endif
    return 1;
}

char *fs_ml_input_fix_joystick_name(const char *name, int upper)
{
    char *n, *temp;
    n = g_strdup(name);
    g_strstrip(n);

    if (n[0] == '\0') {
        g_free(n);
        n = g_strdup("Unnamed");
    }

#if 0
#warning Joystick device test hack enabled
    if (strcasecmp(n, "Microsoft X-Box 360 pad") == 0) {
        /* To test the above hacks. */
        temp = n;
        //n = g_strdup("Hack Test");
        n = g_strdup("XInput Controller #1");
        g_free(temp);
    }
#endif

    if (strncasecmp(n, "XInput Controller #", 19) == 0) {
        /* Hack because parts of the code don't like #x in the original
         * joystick names. SDL 2 has begun naming XInput devices like this. */
        temp = n;
        n = g_strdup("XInput Controller");
        g_free(temp);
    }

    if (strcmp(n, "\xd0\x89") == 0) {
        /* Hack for a specific joystick adapter having a single non-ASCII
         * letter as device name. */
        temp = n;
        n = g_strdup("Hexagons Joystick Adapter");
        g_free(temp);
    }

    if (upper) {
        temp = n;
        n = g_ascii_strup(n, -1);
        g_free(temp);
    }
    return n;
}

void fs_ml_input_init()
{
    FS_ML_INIT_ONCE;

    SDL_Init(SDL_INIT_JOYSTICK);

    fs_log("fs_ml_input_init\n");

    if (fs_config_get_boolean(OPTION_MOUSE_INTEGRATION) == 1) {
        g_mouse_integration = 1;
    }

    g_cursor_mode = fs_config_get_boolean(OPTION_CURSOR);
    if (fs_config_check_auto(OPTION_CURSOR, FS_CONFIG_AUTO)) {
        if (fs_emu_mouse_integration()) {
            g_cursor_mode = 0;
        } else {
            g_cursor_mode = -1;
        }
    }

    g_input_queue = g_queue_new();
    g_input_mutex = fs_mutex_create();

    fs_log("calling fs_ml_video_init\n");
    fs_ml_video_init();

    int size = sizeof(fs_ml_input_device) * FS_ML_INPUT_DEVICES_MAX;
    // allocate zeroed memory
    g_fs_ml_input_devices = g_malloc0(size);

    fs_ml_initialize_keymap();

    int k = 0;
    g_fs_ml_first_joystick_index = 0;

    g_fs_ml_input_devices[k].type = FS_ML_KEYBOARD;
    g_fs_ml_input_devices[k].index = k;
    g_fs_ml_input_devices[k].name = g_strdup("KEYBOARD");
    g_fs_ml_input_devices[k].alias = g_strdup("KEYBOARD");
    k += 1;

    g_fs_ml_input_device_count = k;
    fs_ml_mouse_init();
    k = g_fs_ml_input_device_count;

    g_fs_ml_first_joystick_index = g_fs_ml_input_device_count;

    int num_joysticks = SDL_NumJoysticks();
    fs_log("num joystick devices: %d\n", num_joysticks);
    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        fs_log("WARNING: Joystick module not initialized\n");
    }
    for (int i = 0; i < num_joysticks; i++) {
        if (k == FS_ML_INPUT_DEVICES_MAX) {
            fs_log("WARNING: reached max num devices\n");
            break;
        }
        SDL_Joystick *joystick = SDL_JoystickOpen(i);

#ifdef USE_SDL2
        char *name = fs_ml_input_fix_joystick_name(
            SDL_JoystickName(joystick), 1);
#else
        char *name = fs_ml_input_fix_joystick_name(
            SDL_JoystickName(i), 1);
#endif

        /* fs_ml_input_unique_device_name either returns name, or frees it
         * and return another name, so name must be malloced and owned by
         * caller. */
        name = fs_ml_input_unique_device_name(name);

        g_fs_ml_input_devices[k].type = FS_ML_JOYSTICK;
        g_fs_ml_input_devices[k].index = k;
        g_fs_ml_input_devices[k].name = name;
        if (i == 0) {
            g_fs_ml_input_devices[k].alias = g_strdup("JOYSTICK");
        } else {
            g_fs_ml_input_devices[k].alias = g_strdup_printf("JOYSTICK #%d",
                                                             i + 1);
        }

        g_fs_ml_input_devices[k].hats = SDL_JoystickNumHats(joystick);
        g_fs_ml_input_devices[k].buttons = SDL_JoystickNumButtons(joystick);
        g_fs_ml_input_devices[k].axes = SDL_JoystickNumAxes(joystick);
        g_fs_ml_input_devices[k].balls = SDL_JoystickNumBalls(joystick);

        fs_log("joystick device #%02d found: %s\n", i + 1, name);
        fs_log("- %d buttons %d hats %d axes %d balls\n",
               g_fs_ml_input_devices[k].buttons,
               g_fs_ml_input_devices[k].hats,
               g_fs_ml_input_devices[k].axes,
               g_fs_ml_input_devices[k].balls);
        k += 1;
    }

    g_fs_ml_input_device_count = k;

    fs_ml_initialize_keymap();
}
