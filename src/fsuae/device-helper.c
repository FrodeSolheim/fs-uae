#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>

#ifdef USE_SDL2
#define USE_SDL
#endif

#ifdef USE_SDL
// we must include SDL first before emu.h, so libfsemu's #definition of main
// is the current one (on Windows) when main is encountered further down
#include <SDL.h>
#endif

#include <fs/emu.h>
#include <fs/glib.h>

#ifndef WINDOWS
#undef main
#endif

#define DEVICE_HELPER
#include "fsuae-path.c"

int fsuae_log_level = FSEMU_LOG_LEVEL_WARNING;

char *g_fs_uae_config_file_path = "";
char *g_fs_uae_config_dir_path = "";

static void flush_stdout(void)
{
    fflush(stdout);
}

static char *joystick_config_name(const char *name, int with_number)
{
    const char *in = name;
    char *result = malloc(strlen(name) + 1);
    char *out = result;
    int other = 0;
    while (*in) {
        if (!with_number) {
            if (*in == '#') {
                // remove #2, #3 from name
                break;
            }
        }
        char c = g_ascii_tolower(*in);
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            if (other) {
                *(out++) = '_';
            }
            *(out++) = c;
            other = 0;
        } else {
            other = 1;
        }
        in++;
    }
    *out = '\0';
    if (g_str_has_suffix(result, "_")) {
        result[strlen(result) - 1] = '\0';
    }
    return result;
}

int ManyMouse_Init(void);
void ManyMouse_Quit(void);
const char *ManyMouse_DeviceName(unsigned int index);

static void list_joysticks(void)
{
    printf("# Keyboards:\n");
    flush_stdout();
    GList *list = fs_ml_input_list_custom_keyboards();
    GList *iterator = list;
    while (iterator) {
        printf("K: %s\n", (gchar *) iterator->data);
        flush_stdout();
        iterator = g_list_next(iterator);
    }
    g_list_free_full(list, g_free);
    printf("# Mice:\n");
    printf("M: Mouse\n");
    flush_stdout();
#if 0
    int count = ManyMouse_Init();
    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            const char *name = ManyMouse_DeviceName(i);
            if (name[0] == 0 || g_ascii_strcasecmp(name, "mouse") == 0) {
                printf("M: Mouse: Unnamed Mouse\n");
                flush_stdout();
            } else {
                printf("M: Mouse: %s\n", ManyMouse_DeviceName(i));
                flush_stdout();
            }
        }
        ManyMouse_Quit();
    }
#endif
    printf("# Joysticks:\n");
    flush_stdout();
    if (getenv("FSGS_FAKE_JOYSTICKS")) {
        int fake_joysticks = 0;
        sscanf(getenv("FSGS_FAKE_JOYSTICKS"), "%d", &fake_joysticks);
        for (int i = 0; i < fake_joysticks; i++) {
            printf("J: Fake Joystick\n");
            printf("   Buttons: 0 Hats: 0 Axes: 0 Balls: 0 GUID: %s\n",
                   "4300a03b56ae4b6dbdbf2030995ec9b0");
            printf("   SDLName: \"Fake Joystick\"\n");
            flush_stdout();
        }
    }
#if 0
    printf("J: Fake Test Joystick %c%c\n", 0xc2, 0xae);
    printf("   Buttons: 0 Hats: 0 Axes: 0 Balls: 0 GUID: %s\n",
           "4300a03b56ae4b6dbdbf2030995ec9b0");
    printf("   SDLName: \"Fake Test Joystick %c%c\"\n", 0xc2, 0xae);
    flush_stdout();
#endif
#ifdef USE_SDL
    printf("# SDL_Init(SDL_INIT_JOYSTICK)\n");
    flush_stdout();

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK) < 0\n");
        flush_stdout();
        return;
    }
    printf("# SDL_NumJoysticks(): %d\n", SDL_NumJoysticks());
    flush_stdout();
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);

#ifdef USE_SDL2
        char *name =
            fs_ml_input_fix_joystick_name(SDL_JoystickName(joystick), 0);
#else
        char *name = fs_ml_input_fix_joystick_name(SDL_JoystickName(i), 0);
#endif
        printf("J: %s\n", name);
        flush_stdout();
        g_free(name);

        char guid_str[33];
        SDL_JoystickGUID guid = SDL_JoystickGetGUID(joystick);
        SDL_JoystickGetGUIDString(guid, guid_str, 33);
        guid_str[32] = '\0';

        printf("   Buttons: %d Hats: %d Axes: %d Balls: %d GUID: %s\n",
               SDL_JoystickNumButtons(joystick),
               SDL_JoystickNumHats(joystick),
               SDL_JoystickNumAxes(joystick),
               SDL_JoystickNumBalls(joystick),
               guid_str);
        printf("   SDLName: \"%s\"\n", SDL_JoystickName(joystick));
        flush_stdout();
        SDL_JoystickClose(joystick);
    }
#else
    printf("# USE_SDL is not defined\n");
    flush_stdout();
#endif
}

static void print_events(void)
{
#ifdef USE_SDL2
    printf("# Printing events\n");

    printf("# Keyboards:\n");
    flush_stdout();
    GList *list = fs_ml_input_list_custom_keyboards();
    GList *iterator = list;
    int k = 0;
    while (iterator) {
        printf(
            "{\"type\": \"keyboard-device-added\", \"device\": %d, \"name\": "
            "\"%s\"}\n",
            k++,
            (const char *) iterator->data);
        flush_stdout();
        iterator = g_list_next(iterator);
    }
    g_list_free_full(list, g_free);

    printf("# Mice:\n");
    flush_stdout();
    printf(
        "{\"type\": \"mouse-device-added\", \"device\": %d, \"name\": "
        "\"%s\"}\n",
        0,
        "Mouse");
    flush_stdout();

#if 0
    int count = ManyMouse_Init();
    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            const char *name = ManyMouse_DeviceName(i);
            if (name[0] == 0 || g_ascii_strcasecmp(name, "mouse") == 0) {
                printf(
                    "{\"type\": \"mouse-device-added\", \"device\": %d, "
                    "\"name\": \"%s\"}\n",
                    i + 1,
                    "Unnamed Mouse");
            } else {
                printf(
                    "{\"type\": \"mouse-device-added\", \"device\": %d, "
                    "\"name\": \"%s\"}\n",
                    i + 1,
                    ManyMouse_DeviceName(i));
            }
            flush_stdout();
        }
        ManyMouse_Quit();
    }
#endif

    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");

    // With recent SDL2 versions (SDL 2.0.14+?), it seems that the video
    // subsystem must be initialized to get events from xinput controllers.
    printf("# SDL_Init(SDL_INIT_EVERYTHING)\n");
    flush_stdout();
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("# SDL_Init(SDL_INIT_EVERYTHING) < 0\n");
        flush_stdout();
        return;
    }

    int64_t startup = fs_get_monotonic_time();

    SDL_Event event;
    int do_quit = 0;
    while (do_quit == 0) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                printf(
                    "{\"type\": \"%s\", \"device\": %d, "
                    "\"sdlInstanceId\": %d, \"button\": %d, \"state\": %d}\n",
                    event.type == SDL_JOYBUTTONDOWN ? "joy-button-down"
                                                    : "joy-button-up",
                    event.jbutton.which,
                    event.jbutton.which,
                    event.jbutton.button,
                    event.jbutton.state);
                break;
            case SDL_JOYHATMOTION:
                printf(
                    "{\"type\": \"joy-hat-motion\", \"device\": %d, "
                    "\"sdlInstanceId\": %d, \"hat\": %d, \"state\": %d}\n",
                    event.jbutton.which,
                    event.jbutton.which,
                    event.jhat.hat,
                    event.jhat.value);
                break;
            case SDL_JOYAXISMOTION:
                if (startup > 0) {
                    if (fs_get_monotonic_time() - startup < 1000.0) {
                        // at least on Linux, it has been observed that you get
                        // a (full negative) axis motion event per axis on
                        // startup.
                        printf("# Ignored startup axis event\n");
                        break;
                    } else {
                        startup = 0;
                    }
                }
#if 0
            if (event.jaxis.value > -2000 && event.jaxis.value < 2000) {
                break;
            }
#endif
                printf(
                    "{\"type\": \"joy-axis-motion\", \"device\": %d, "
                    "\"sdlInstanceId\": %d, \"axis\": %d, \"state\": %d}\n",
                    event.jbutton.which,
                    event.jbutton.which,
                    event.jaxis.axis,
                    event.jaxis.value);
                break;
            case SDL_JOYDEVICEADDED:
                printf("# Joystick device added\n");
                SDL_Joystick *joystick = SDL_JoystickOpen(event.jdevice.which);

                char guid[33];
                SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), guid, 33);

                char *name = fs_ml_input_fix_joystick_name(
                    SDL_JoystickName(joystick), 0);

                char *name2 = strdup(name);
                char *c = name2;
                while (*c) {
                    // simple hack, replacing a couple of chars to (easily)
                    // make the name valid json.
                    if (*c == '\"') {
                        *c = '\'';
                    }
                    if (*c == '\\') {
                        *c = '/';
                    }
                    c++;
                }

                char *name3 = strdup(SDL_JoystickName(joystick));
                // FIXME: Properly escape name
                c = name3;
                while (*c) {
                    // simple hack, replacing a couple of chars to (easily)
                    // make the name valid json.
                    if (*c == '\"') {
                        *c = '\'';
                    }
                    if (*c == '\\') {
                        *c = '/';
                    }
                    c++;
                }

                printf(
                    "{\"type\": \"joy-device-added\", \"device\": %d, "
                    "\"name\": \"%s\", \"buttons\": %d, \"axes\": %d, "
                    "\"hats\": %d, \"balls\": %d, \"sdlInstanceId\": %d, "
                    "\"sdlGuid\": \"%s\", \"sdlName\": \"%s\"}\n",
                    event.jdevice.which,
                    name2,
                    SDL_JoystickNumButtons(joystick),
                    SDL_JoystickNumAxes(joystick),
                    SDL_JoystickNumHats(joystick),
                    SDL_JoystickNumBalls(joystick),
                    SDL_JoystickInstanceID(joystick),
                    guid,
                    name3);
                free(name2);
                free(name3);
                break;
            case SDL_JOYDEVICEREMOVED:
                printf("# Joystick device removed\n");
                printf("{\"type\": \"joy-device-removed\", \"device\": %d, "
                       "\"sdlInstanceId\": %d}\n",
                       event.jdevice.which,
                       event.jdevice.which);
                break;
            case SDL_QUIT:
                printf("# Received quit signal\n");
                do_quit = 1;
                break;
        }
        flush_stdout();
    }

#endif
}

static void print_state(SDL_Joystick *joystick, const char *name)
{
    int num_buttons = SDL_JoystickNumButtons(joystick);
    int num_hats = SDL_JoystickNumHats(joystick);
    int num_axes = SDL_JoystickNumAxes(joystick);
    while (1) {
        SDL_JoystickUpdate();
        printf("%s buttons", name);
        for (int i = 0; i < num_buttons; i++) {
            printf(" %d", SDL_JoystickGetButton(joystick, i));
        }
        printf("\n");
        printf("%s hats", name);
        for (int i = 0; i < num_hats; i++) {
            printf(" %d", SDL_JoystickGetHat(joystick, i));
        }
        printf("\n");
        printf("%s axes", name);
        for (int i = 0; i < num_axes; i++) {
            printf(" %d", SDL_JoystickGetAxis(joystick, i));
        }
        printf("\n");
        flush_stdout();
        SDL_Delay(100);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                printf("# Received quit signal\n");
                return;
            }
        }
    }
}

#ifdef WITH_MIDI

#include "portmidi.h"

static void print_name_escaped(const char *value)
{
    const char *c = value;
    while (*c) {
        if (*c == '\n') {
            fputs("%0a", stdout);
        } else if (*c == '\"') {
            fputs("%22", stdout);
        } else {
            putchar(*c);
        }
        c++;
    }
}

static void list_portmidi_devices(void)
{
    printf("# Calling Pm_Initialize\n");
    PmError error = Pm_Initialize();
    if (error) {
        printf("# PortMidi error %d\n", error);
        return;
    }
    int count = Pm_CountDevices();
    printf("# PortMidi reports %d devices:\n\n", count);
    for (int i = 0; i < count; i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        if (info != NULL) {
            // printf("\"%s\"", info->name);
            putchar('"');
            print_name_escaped(info->name);
            putchar('"');
            if (info->input) {
                fputs(" IN", stdout);
            }
            if (info->output) {
                fputs(" OUT", stdout);
            }
            printf(" (%s)\n", info->interf);
        }
    }
    printf("\n");
}

#endif  // WITH_MIDI

#ifdef WINDOWS
// FIXME fix the main macro instead
int g_fs_ml_ncmdshow;
HINSTANCE g_fs_ml_hinstance;
#endif



static void print_usage(void)
{
    printf("\nUsage:\n");
    printf("  fs-uae-device-helper --list\n");
    printf("  fs-uae-device-helper <device-name>\n");
    printf("  fs-uae-device-helper --events\n");
#ifdef WITH_MIDI
    printf("  fs-uae-device-helper list-portmidi-devices\n");
#endif
    printf("\n");
}


int main(int argc, char *argv[])
{
    printf("# FS-UAE Device Helper %s\n", PACKAGE_VERSION);
    flush_stdout();

    fs_set_argv(argc, argv);
    fs_set_data_dir(fsuae_path_data_dir());

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        print_usage();
        flush_stdout();
        return 1;
    }

    if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "list") == 0) {
        list_joysticks();
        printf("# End\n");
        flush_stdout();
        return 0;
    }
#ifdef WITH_MIDI
    if (strcmp(argv[1], "list-portmidi-devices") == 0) {
        list_portmidi_devices();
        printf("# End\n");
        flush_stdout();
        return 0;
    }
#endif
    if (strcmp(argv[1], "--events") == 0) {
        print_events();
        printf("# End\n");
        flush_stdout();
        return 0;
    }

    char *compare_name = joystick_config_name(argv[1], 1);

    // With recent SDL2 versions (SDL 2.0.14+?), it seems that the video
    // subsystem must be initialized to get events from xinput controllers.
    printf("# SDL_Init(SDL_INIT_EVERYTHING)\n");
    flush_stdout();
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("# SDL_Init(SDL_INIT_EVERYTHING) < 0\n");
        flush_stdout();
        return 2;
    }
    int num_joysticks = SDL_NumJoysticks();
    for (int i = 0; i < num_joysticks; i++) {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
#ifdef USE_SDL2
        char *name =
            fs_ml_input_fix_joystick_name(SDL_JoystickName(joystick), 1);
#else
        char *name = fs_ml_input_fix_joystick_name(SDL_JoystickName(i), 1);
#endif
        /* fs_ml_input_unique_device_name either returns name, or frees it
         * and return another name, so name must be malloced and owned by
         * caller. */
        name = fs_ml_input_unique_device_name(name);
        char *config_name = joystick_config_name(name, 1);
        printf("# %s -- %s\n", config_name, compare_name);
        if (strcmp(config_name, compare_name) == 0) {
            print_state(joystick, config_name);
            SDL_JoystickClose(joystick);
            printf("# End\n");
            flush_stdout();
            exit(0);
        }
        SDL_JoystickClose(joystick);
    }
    printf("# No devices matched\n");
    printf("# End\n");
    flush_stdout();
    return 0;
}
