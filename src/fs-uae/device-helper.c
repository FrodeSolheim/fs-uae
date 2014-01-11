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
#include <fs/string.h>

#ifndef WINDOWS
#undef main
#endif

static char *joystick_config_name(const char* name, int with_number) {
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
        char c = fs_ascii_tolower(*in);
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            if (other) {
                *(out++) = '_';
            }
            *(out++) = c;
            other = 0;
        }
        else {
            other = 1;
        }
        in++;
    }
    *out = '\0';
    if (fs_str_has_suffix(result, "_")) {
        result[strlen(result) - 1] = '\0';
    }
    return result;
}

int ManyMouse_Init(void);
void ManyMouse_Quit(void);
const char *ManyMouse_DeviceName(unsigned int index);

void list_joysticks() {
    // printf("# FS-UAE VERSION %s\n", g_fs_uae_version);
    printf("# listing keyboards\n");
    printf("K: Keyboard\n");
    printf("# listing mice\n");
    printf("M: Mouse\n");
    int count = ManyMouse_Init();
    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            const char *name = ManyMouse_DeviceName(i);
            if (name[0] == 0 || fs_ascii_strcasecmp(name, "mouse") == 0) {
                printf("M: Unnamed Mouse\n");
            }
            else {
                printf("M: %s\n", ManyMouse_DeviceName(i));
            }
        }
        ManyMouse_Quit();
    }

    printf("# listing joysticks\n");
#ifdef USE_SDL
    if (SDL_Init(SDL_INIT_JOYSTICK ) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK ) < 0\n");
        return;
    }
    printf("# SDL_NumJoysticks(): %d\n", SDL_NumJoysticks());
    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        SDL_Joystick *joystick = SDL_JoystickOpen(i);
#ifdef USE_SDL2
        if (SDL_JoystickName(joystick)[0] == '\0') {
#else
        if (SDL_JoystickName(i)[0] == '\0') {
#endif
            printf("J: Unnamed\n");
        }
        else {
#ifdef USE_SDL2
            printf("J: %s\n", SDL_JoystickName(joystick));
#else
            printf("J: %s\n", SDL_JoystickName(i));
#endif
        }
        printf("   Buttons: %d Hats: %d Axes: %d Balls: %d\n",
            SDL_JoystickNumButtons(joystick),
            SDL_JoystickNumHats(joystick),
            SDL_JoystickNumAxes(joystick),
            SDL_JoystickNumBalls(joystick));
        SDL_JoystickClose(joystick);
    }
#else
    printf("# USE_SDL is not defined\n");
#endif
    printf("# listing joysticks done\n");
}

void print_events(SDL_Joystick* joystick, const char* name) {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            printf("%s button %d state %d\n",
                name, event.jbutton.button, event.jbutton.state);
            break;
        case SDL_JOYHATMOTION:
            printf("%s hat %d state %d\n",
                name, event.jhat.hat, event.jhat.value);
            break;
        case SDL_JOYAXISMOTION:
            printf("%s axis %d state %d\n",
                name, event.jaxis.axis, event.jaxis.value);
            break;
        }
    }
}

void print_state(SDL_Joystick* joystick, const char* name) {

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

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("usages:\n");
        printf("* fs-uae-device-helper list\n");
        printf("* fs-uae-device-helper <device-name>\n");
        return 1;
    }

    if (strcmp(argv[1], "list") == 0) {
        list_joysticks();
        return 0;
    }

    char* compare_name = joystick_config_name(argv[1], 1);

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK ) < 0\n");
        return 1;
    }

    // SDL_Init(SDL_INIT_EVERYTHING);

    int num_joysticks = SDL_NumJoysticks();
    for (int i = 0; i < num_joysticks; i++) {
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
