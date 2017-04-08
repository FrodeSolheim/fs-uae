#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SDL.h>
#include <stdlib.h>
#include <fs/conf.h>
#include <fs/lazyness.h>
#include <fs/log.h>
#include <fs/glib.h>
#include <fs/ml/options.h>
#include "ml_internal.h"

// For fs_emu_mouse_integration
#include <fs/emu.h>

static GQueue *g_input_queue = NULL;
static fs_mutex *g_input_mutex = NULL;
static fs_ml_input_function g_input_function = NULL;

int g_fs_log_input = 0;
int g_fs_ml_first_keyboard_index = 0;
int g_fs_ml_first_mouse_index = 0;
int g_fs_ml_first_joystick_index = 0;

int fs_ml_first_mouse_index(void)
{
    return g_fs_ml_first_mouse_index;
}

/* maps SDL joystick indices to fs_ml indices */
SDL_JoystickID g_fs_ml_sdl_joystick_index_map[MAX_SDL_JOYSTICK_IDS];

#define CURSOR_MODE_ON 1
#define CURSOR_MODE_OFF 0
#define CURSOR_MODE_AUTO -1
static int g_cursor_mode = CURSOR_MODE_ON;

static int g_mouse_integration = 0;

bool fs_ml_mouse_integration(void)
{
    return g_mouse_integration;
}

bool fs_ml_cursor_allowed(void)
{
    return g_cursor_mode != CURSOR_MODE_OFF;
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
    if (g_input_function) {
        g_input_function(event);
    }
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

static int fs_ml_check_joystick_blacklist_by_guid(const char *guid)
{
    if (false) {
#if 0
    } else if (strcasecmp(guid, "efbeedfe000000000000504944564944") == 0) {
        /* VIRTUAL HID DEVICE */
        return 1;
#endif
#if 0
    } else if (strcasecmp(guid, "030000000b0400003365000000010000") == 0) {
        /* MOSIC      SPEED-LINK COMPETITION PRO */
        return 1;
#endif
    }
    return 0;
}

GList *fs_ml_input_list_custom_keyboards(void)
{
#if 0
    char *keyboards_path = g_build_filename(
        fs_data_dir(), "Devs", NULL);
#else
    char *keyboards_path = g_build_filename(
        fs_data_dir(), "Devs", "Keyboards", NULL);
#endif
    GList *list = NULL;
    GDir *dir = g_dir_open(keyboards_path, 0, NULL);
    if (dir) {
        const gchar *name;
        while ((name = g_dir_read_name(dir))) {
            if (!g_str_has_suffix(name, ".ini")) {
                continue;
            }
            gchar *path = g_build_filename(keyboards_path, name, NULL);
            GKeyFile *key_file = g_key_file_new();
            if (!g_key_file_load_from_file(
                    key_file, path, G_KEY_FILE_NONE, NULL)) {
                fs_log("WARNING: Could not load %s\n", path);
                continue;
            }
            gchar *type = g_key_file_get_string(
                        key_file, "device", "type", NULL);
            if (!type || strcmp(type, "keyboard") != 0) {
                g_free(type);
                continue;
            }
            g_free(type);
            gchar *attached = g_key_file_get_string(
                        key_file, "device", "attached", NULL);
            if (!attached || strcmp(attached, "1") != 0) {
                g_free(attached);
                continue;
            }
            g_free(attached);
            char *name2 = g_strdup(name);
            name2[strlen(name) - 4] = '\0';
            gchar *disable_name = g_strdup_printf("%s.disabled", name2);
            gchar *disable_path = g_build_filename(
                keyboards_path, disable_name, NULL);
            g_free(disable_name);
            if (g_file_test(disable_path, G_FILE_TEST_EXISTS)) {
                g_free(disable_path);
                continue;
            }
            g_free(disable_path);
            list = g_list_append(list, name2);
        }
        g_dir_close(dir);
    }
    list = g_list_sort(list, (GCompareFunc) g_ascii_strcasecmp);
    gchar *disable_path = g_build_filename(
                keyboards_path, "Keyboard.disabled", NULL);
    if (!g_file_test(disable_path, G_FILE_TEST_EXISTS)) {
        list = g_list_prepend(list, g_strdup("Keyboard"));
    }
    g_free(disable_path);
    g_free(keyboards_path);
    return list;
}

void fs_ml_input_init()
{
    FS_ML_INIT_ONCE;

    SDL_Init(SDL_INIT_JOYSTICK);

    fs_log("[INPUT] fs_ml_input_init\n");

    g_fs_log_input = getenv("FS_DEBUG_INPUT") && \
            getenv("FS_DEBUG_INPUT")[0] == '1';

    if (fs_config_get_boolean(OPTION_LOG_INPUT) == 1) {
        fs_log("[INPUT] enable input event logging\n");
        g_fs_log_input = 1;
    }

    if (fs_config_get_boolean(OPTION_MOUSE_INTEGRATION) == 1) {
        g_mouse_integration = 1;
    }

    g_cursor_mode = fs_config_get_boolean(OPTION_CURSOR);
    if (fs_config_check_auto(OPTION_CURSOR, FS_CONFIG_AUTO)) {
        if (fs_emu_mouse_integration()) {
            g_cursor_mode = CURSOR_MODE_OFF;
        } else {
            g_cursor_mode = CURSOR_MODE_AUTO;
        }
    }

    g_input_queue = g_queue_new();
    g_input_mutex = fs_mutex_create();

    int size = sizeof(fs_ml_input_device) * FS_ML_INPUT_DEVICES_MAX;
    /* Allocate zeroed memory. */
    g_fs_ml_input_devices = g_malloc0(size);

    fs_ml_initialize_keymap();

    int k = 0;
    g_fs_ml_first_joystick_index = 0;
#if 0
    g_fs_ml_input_devices[k].type = FS_ML_KEYBOARD;
    g_fs_ml_input_devices[k].index = k;
    g_fs_ml_input_devices[k].name = g_strdup("KEYBOARD");
    g_fs_ml_input_devices[k].alias = g_strdup("KEYBOARD");
    k += 1;
#endif
    GList *list = fs_ml_input_list_custom_keyboards();
    GList *iterator = list;
    while (iterator) {
        const char *name = (const char *) iterator->data;
        g_fs_ml_input_devices[k].type = FS_ML_KEYBOARD;
        g_fs_ml_input_devices[k].index = k;
        g_fs_ml_input_devices[k].name = g_strdup(name);
        g_fs_ml_input_devices[k].alias = g_strdup(name);
        k += 1;
        iterator = g_list_next(iterator);
    }
    g_list_free_full(list, g_free);

    g_fs_ml_input_device_count = k;
    fs_ml_mouse_init();
    k = g_fs_ml_input_device_count;

    g_fs_ml_first_joystick_index = g_fs_ml_input_device_count;

    int num_joysticks = SDL_NumJoysticks();
    fs_log("[INPUT] Joystick device count: %d\n", num_joysticks);
    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        fs_log("[INPUT] WARNING: Joystick module not initialized\n");
    }
    for (int i = 0; i < num_joysticks; i++) {
        if (k == FS_ML_INPUT_DEVICES_MAX) {
            fs_log("[INPUT] WARNING: Reached max num devices\n");
            break;
        }

        char guid_str[33];
        SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(i);
        SDL_JoystickGetGUIDString(guid, guid_str, 33);
        guid_str[32] = '\0';
        fs_log("[INPUT] SDL_JoystickGetDeviceGUID(%d) = %s\n", i, guid_str);
        if (fs_ml_check_joystick_blacklist_by_guid(guid_str)) {
            fs_log("[INPUT] Blacklisted joystick, not opening!\n");
            continue;
        }

        SDL_Joystick *joystick = SDL_JoystickOpen(i);
        fs_log("[INPUT] SDL_JoystickOpen(%d)\n", i);

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

        fs_log("[INPUT] Joystick device #%02d found: %s\n", i + 1, name);
        fs_log("[INPUT] %d buttons %d hats %d axes %d balls\n",
               g_fs_ml_input_devices[k].buttons,
               g_fs_ml_input_devices[k].hats,
               g_fs_ml_input_devices[k].axes,
               g_fs_ml_input_devices[k].balls);

        SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);
        fs_log("[INPUT] Joystick instance ID = %d\n", instance_id);
        if (instance_id >= MAX_SDL_JOYSTICK_IDS) {
            fs_log("[INPUT] SDL_JoystickID > %d\n", MAX_SDL_JOYSTICK_IDS);
            fs_log("[INPUT] Closing joystick\n");
            SDL_JoystickClose(joystick);
            continue;
        }

        g_fs_ml_sdl_joystick_index_map[instance_id] = k;
        k += 1;
    }

    g_fs_ml_input_device_count = k;

    fs_ml_initialize_keymap();
}
