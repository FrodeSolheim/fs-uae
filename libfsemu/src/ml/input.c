#include <SDL.h>
#include <stdlib.h>
#include <fs/log.h>
#include <fs/string.h>
#include <fs/queue.h>
#include <fs/hashtable.h>
#include "ml_internal.h"

static fs_queue *g_input_queue = NULL;
static fs_mutex *g_input_mutex = NULL;
static fs_ml_input_function g_input_function = NULL;

int g_fs_ml_first_keyboard_index = 0;
int g_fs_ml_first_mouse_index = 0;
int g_fs_ml_first_joystick_index = 0;

fs_ml_event* fs_ml_alloc_event() {
    return malloc(sizeof(fs_ml_event));
}

void fs_ml_input_event_free(fs_ml_event *event) {
    free(event);
}

void fs_ml_set_input_function(fs_ml_input_function function) {
    g_input_function = function;
}

int fs_ml_post_event(fs_ml_event* event) {
    if (event->type == FS_ML_KEYDOWN || event->type == FS_ML_KEYUP) {
        if (fs_ml_handle_keyboard_shortcut(event)) {
            return 1;
        }
    }
    if (g_input_function) {
        g_input_function(event);
    }
    /*
    fs_mutex_lock(g_input_mutex);
    fs_queue_push_tail(g_input_queue, event);
    fs_mutex_unlock(g_input_mutex);
    */
    return 1;
}

char *fs_ml_input_unique_device_name(char *name) {
    // fs_hash_table_destroy(device_counts);
    static fs_hash_table *device_counts = NULL;
    if (device_counts == NULL) {
        device_counts = fs_hash_table_new_full(
                fs_str_hash, fs_str_equal, free, NULL);
    }

    int count = FS_POINTER_TO_INT(fs_hash_table_lookup(
            device_counts, name));
    count++;
    fs_hash_table_replace(device_counts, fs_strdup(name),
            FS_INT_TO_POINTER(count));
    if (count > 1) {
        char *result = fs_strdup_printf("%s #%d", name, count);
        free(name);
        name = result;
    }
    return name;
}

void fs_ml_input_init() {
    FS_ML_INIT_ONCE;

    SDL_Init(SDL_INIT_JOYSTICK);

    fs_log("fs_ml_input_init\n");

    g_input_queue = fs_queue_new();
    g_input_mutex = fs_mutex_create();

    fs_log("calling fs_ml_video_init\n");
    fs_ml_video_init();

    int size = sizeof(fs_ml_input_device) * FS_ML_INPUT_DEVICES_MAX;
    // allocate zeroed memory
    g_fs_ml_input_devices = fs_malloc0(size);

    fs_ml_initialize_keymap();

    int k = 0;
    g_fs_ml_first_joystick_index = 0;

    g_fs_ml_input_devices[k].type = FS_ML_KEYBOARD;
    g_fs_ml_input_devices[k].index = k;
    g_fs_ml_input_devices[k].name = fs_strdup("KEYBOARD");
    g_fs_ml_input_devices[k].alias = fs_strdup("KEYBOARD");
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
        char* name = fs_ascii_strup(SDL_JoystickName(joystick), -1);
#else
        char* name = fs_ascii_strup(SDL_JoystickName(i), -1);
#endif
        name = fs_strstrip(name);
        if (name[0] == '\0') {
            free(name);
            name = fs_ascii_strup("Unnamed", -1);
        }

        // fs_ml_input_unique_device_name either returns name, or frees it
        // and return another name, so name must be malloced and owned by
        // caller
        name = fs_ml_input_unique_device_name(name);

        g_fs_ml_input_devices[k].type = FS_ML_JOYSTICK;
        g_fs_ml_input_devices[k].index = k;
        g_fs_ml_input_devices[k].name = name;
        if (i == 0) {
            g_fs_ml_input_devices[k].alias = fs_strdup("JOYSTICK");
        }
        else {
            g_fs_ml_input_devices[k].alias = fs_strdup_printf("JOYSTICK #%d",
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
