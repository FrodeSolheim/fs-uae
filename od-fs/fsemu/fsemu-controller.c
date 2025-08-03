#define FSEMU_INTERNAL
#include "fsemu-controller.h"

#include "fsemu-action.h"
#include "fsemu-input.h"
#include "fsemu-sdl.h"

// back
// dpdown
// dpleft
// dpright
// dpup
// guide
// leftshoulder
// leftstick
// lefttrigger
// leftx
// lefty
// rightshoulder
// rightstick
// righttrigger
// rightx
// righty
// start
// x
// y

#define FSEMU_CONTROLLER_MAX_COUNT 8

static struct {
    bool initialized;
    fsemu_controller_t controllers[FSEMU_CONTROLLER_MAX_COUNT];
    SDL_Gamepad *sdl_controllers[FSEMU_CONTROLLER_MAX_COUNT];
} fsemu_controller;

void fsemu_controller_init(void)
{
    return;

    if (fsemu_controller.initialized) {
        return;
    }
    fsemu_controller.initialized = true;
    fsemu_input_log("Initializing controller module\n");

    // FIXME: Test quality code
    for (int i = 0; i < FSEMU_CONTROLLER_MAX_COUNT; i++) {
        printf("Opening SDL game controller %d\n", i);
        SDL_Gamepad *sdl_controller = SDL_OpenGamepad(i);
        if (sdl_controller) {
            printf(
                "Opening SDL game controller %d -> %p\n", i, sdl_controller);
            fsemu_controller.sdl_controllers[i] = sdl_controller;
        }
    }
}

static void fsemu_controller_manual_controller_update(
    int index, SDL_Gamepad *sdl_controller)
{
    int slot = 0;
    int16_t state = 0;
    for (int button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; button++) {
        state = SDL_GetGamepadButton(sdl_controller, button);
        state = state ? FSEMU_ACTION_STATE_MAX : 0;
        slot = FSEMU_CONTROLLER_BEFORE_FIRST_BUTTON + 1 + button;

        if (fsemu_controller.controllers[index].last_state[slot] != state) {
            printf("state change slot %d: %d -> %d\n",
                   slot,
                   fsemu_controller.controllers[index].last_state[slot],
                   state);
            fsemu_controller.controllers[index].last_state[slot] = state;
            // FIXME: trigger something...
        }
    }
}

static void fsemu_controller_manual_update(void)
{
    for (int i = 0; i < FSEMU_CONTROLLER_MAX_COUNT; i++) {
        SDL_Gamepad *sdl_controller = fsemu_controller.sdl_controllers[i];
        if (sdl_controller) {
            fsemu_controller_manual_controller_update(i, sdl_controller);
        }
    }
}

void fsemu_controller_update(void)
{
    return;

    fsemu_controller_manual_update();
}
