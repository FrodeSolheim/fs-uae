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
#include "paths.h"

#ifndef WINDOWS
#undef main
#endif

char *g_fs_uae_config_file_path = "";
char *g_fs_uae_config_dir_path = "";

static char *joystick_config_name(const char* name, int with_number)
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
    printf("# fs-uae-device-helper %s #\n", PACKAGE_VERSION);
    printf("## keyboards ##\n");
    GList *list = fs_ml_input_list_custom_keyboards();
    GList *iterator = list;
    while (iterator) {
        printf("K: %s\n", (gchar *) iterator->data);
        iterator = g_list_next(iterator);
    }
    g_list_free_full(list, g_free);
    printf("## mice ##\n");
    printf("M: Mouse\n");
    int count = ManyMouse_Init();
    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            const char *name = ManyMouse_DeviceName(i);
            if (name[0] == 0 || g_ascii_strcasecmp(name, "mouse") == 0) {
                printf("M: Mouse: Unnamed Mouse\n");
            } else {
                printf("M: Mouse: %s\n", ManyMouse_DeviceName(i));
            }
        }
        ManyMouse_Quit();
    }
    printf("## joysticks ##\n");
#if 0
    printf("J: Fake Test Joystick %c%c\n", 0xc2, 0xae);
    printf("   Buttons: 0 Hats: 0 Axes: 0 Balls: 0\n");
#endif
#ifdef USE_SDL
    if (SDL_Init(SDL_INIT_JOYSTICK ) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK ) < 0\n");
        return;
    }
    printf("# SDL_NumJoysticks(): %d\n", SDL_NumJoysticks());
    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);

#ifdef USE_SDL2
        char *name = fs_ml_input_fix_joystick_name(
            SDL_JoystickName(joystick), 0);
#else
        char *name = fs_ml_input_fix_joystick_name(
            SDL_JoystickName(i), 0);
#endif
        printf("J: %s\n", name);
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
        SDL_JoystickClose(joystick);
    }
#else
    printf("# USE_SDL is not defined\n");
#endif
    printf("## end ##\n");
}

static void print_events(void)
{
#ifdef USE_SDL2
    printf("# Printing events\n");

    printf("# listing keyboards\n");
    GList *list = fs_ml_input_list_custom_keyboards();
    GList *iterator = list;
    int k = 0;
    while (iterator) {
        printf("{\"type\": \"keyboard-device-added\", \"device\": %d, "
               "\"name\": \"%s\"}\n",
               k++, (const char *) iterator->data);
        iterator = g_list_next(iterator);
    }
    g_list_free_full(list, g_free);

    printf("# listing mice\n");
    printf("{\"type\": \"mouse-device-added\", \"device\": %d, "
           "\"name\": \"%s\"}\n",
           0, "Mouse");
    fflush(stdout);

    int count = ManyMouse_Init();
    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            const char *name = ManyMouse_DeviceName(i);
            if (name[0] == 0 || g_ascii_strcasecmp(name, "mouse") == 0) {
                printf("{\"type\": \"mouse-device-added\", \"device\": %d, "
                       "\"name\": \"%s\"}\n",
                       i + 1, "Unnamed Mouse");
            } else {
                printf("{\"type\": \"mouse-device-added\", \"device\": %d, "
                       "\"name\": \"%s\"}\n",
                       i + 1, ManyMouse_DeviceName(i));
            }
            fflush(stdout);
        }
        ManyMouse_Quit();
    }

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_EVENTS)) < 0\n");
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
            printf("{\"type\": \"%s\", \"device\": %d, "
                   "\"button\": %d, \"state\": %d}\n",
                   event.type == SDL_JOYBUTTONDOWN ? "joy-button-down" :
                   "joy-button-up", event.jbutton.which, event.jbutton.button,
                   event.jbutton.state);
            break;
        case SDL_JOYHATMOTION:
            printf("{\"type\": \"joy-hat-motion\", \"device\": %d, "
                   "\"hat\": %d, \"state\": %d}\n",
                   event.jbutton.which, event.jhat.hat, event.jhat.value);
            break;
        case SDL_JOYAXISMOTION:
            if (startup > 0) {
                if (fs_get_monotonic_time() - startup < 1000.0) {
                    // at least on Linux, it has been observed that you get
                    // a (full negative) axis motion event per axis on
                    // startup.
                    printf("# ignored startup axis event\n");
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
            printf("{\"type\": \"joy-axis-motion\", \"device\": %d, "
                   "\"axis\": %d, \"state\": %d}\n",
                   event.jbutton.which, event.jaxis.axis, event.jaxis.value);
            break;
        case SDL_JOYDEVICEADDED:
            printf("# new joystick device added\n");
            SDL_Joystick *joystick = SDL_JoystickOpen(event.jdevice.which);

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
            printf("{\"type\": \"joy-device-added\", \"device\": %d, "
                   "\"name\": \"%s\", \"buttons\": %d, \"axes\": %d, "
                   "\"hats\": %d, \"balls\": %d}\n",
                   event.jdevice.which, name2,
                   SDL_JoystickNumButtons(joystick),
                   SDL_JoystickNumAxes(joystick),
                   SDL_JoystickNumHats(joystick),
                   SDL_JoystickNumBalls(joystick));
            free(name2);
            break;
        case SDL_JOYDEVICEREMOVED:
            printf("# new joystick device removed\n");
            printf("{\"type\": \"joy-device-removed\", \"device\": %d}\n",
                   event.jdevice.which);
            break;
        case SDL_QUIT:
            printf("# received quit signal\n");
            do_quit = 1;
            break;
        }
        fflush(stdout);
    }

#endif
}

static void print_state(SDL_Joystick* joystick, const char* name)
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
        SDL_Delay(100);
        fflush(stdout);
    }
}

#ifdef WINDOWS
// FIXME fix the main macro instead
int g_fs_ml_ncmdshow;
HINSTANCE g_fs_ml_hinstance;
#endif

int main(int argc, char* argv[])
{
    fs_set_argv(argc, argv);
    fs_set_data_dir(fs_uae_data_dir());

    if (argc != 2) {
        printf("usages:\n");
        printf("* fs-uae-device-helper --list\n");
        printf("* fs-uae-device-helper <device-name>\n");
        printf("* fs-uae-device-helper --events\n");
        return 1;
    }

    if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "list") == 0) {
        list_joysticks();
        return 0;
    }
    if (strcmp(argv[1], "--events") == 0) {
        print_events();
        return 0;
    }

    char* compare_name = joystick_config_name(argv[1], 1);

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK ) < 0\n");
        return 2;
    }

    int num_joysticks = SDL_NumJoysticks();
    for (int i = 0; i < num_joysticks; i++) {
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

        char* config_name = joystick_config_name(name, 1);
        printf("%s -- %s\n", config_name, compare_name);
        if (strcmp(config_name, compare_name) == 0) {
            print_state(joystick, config_name);
            SDL_JoystickClose(joystick);
            exit(0);
        }

        SDL_JoystickClose(joystick);
    }

    return 0;
}
