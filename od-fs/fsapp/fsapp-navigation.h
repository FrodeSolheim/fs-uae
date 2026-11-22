/*
 * Navigation: the mapping from gamepads and keys to navigation events for the surface with keyboard
 * focus, and the menu input recogniser, which sees every gamepad button before routing
 * (fsapp-routing.h).
 *
 * A pure unit with no SDL, clock, thread or other module, so that its tests (od-fs/tests/fsapp)
 * compile its source file alone. The caller passes SDL3's values (the constants below) and times
 * in nanoseconds on SDL's clock.
 *
 * The mapper's rules:
 *
 * - Gamepads: the d-pad and the left stick move, the left and right shoulders page back and
 *   forward, south accepts and east goes back. Other buttons, the right stick and the triggers
 *   give nothing (Guide, Start and Select are the recogniser's).
 * - A stick presses a direction beyond 0.66 of full scale and releases it inside 0.33.
 * - Keys (pressed, the keyboard as one device): the arrows move, Return and keypad Enter accept,
 *   Escape and Backspace go back, Page Up and Page Down page, Home and End go to the first and last
 *   item. Their repeats are the host's key repeats.
 * - Directions and pages held on a gamepad repeat 300 ms after the press and then every 75 ms;
 *   accept and back never repeat, from gamepads or keys. A repeat that is late is given once, and
 *   the next is the next one due after it.
 *
 * The recogniser's rules, per gamepad: Guide is the menu input. Select pressed while Start is down,
 * or Start while Select is down, is the menu input too, and the button that completes the chord is
 * consumed with its release. Start pressed first has been passed on (to the Amiga, while it has the
 * input), so the recogniser asks for a release of Start to be sent after it, before the menu input,
 * and consumes Start's later release. Start alone and Select alone are passed on.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_NAVIGATION_H
#define FSAPP_NAVIGATION_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// The gamepad buttons and axes used, with SDL3's values (SDL_GAMEPAD_BUTTON_SOUTH and so on;
// BACK is Select).
#define FSAPP_NAVIGATION_BUTTON_SOUTH 0
#define FSAPP_NAVIGATION_BUTTON_EAST 1
#define FSAPP_NAVIGATION_BUTTON_BACK 4
#define FSAPP_NAVIGATION_BUTTON_GUIDE 5
#define FSAPP_NAVIGATION_BUTTON_START 6
#define FSAPP_NAVIGATION_BUTTON_LEFT_SHOULDER 9
#define FSAPP_NAVIGATION_BUTTON_RIGHT_SHOULDER 10
#define FSAPP_NAVIGATION_BUTTON_DPAD_UP 11
#define FSAPP_NAVIGATION_BUTTON_DPAD_DOWN 12
#define FSAPP_NAVIGATION_BUTTON_DPAD_LEFT 13
#define FSAPP_NAVIGATION_BUTTON_DPAD_RIGHT 14
#define FSAPP_NAVIGATION_AXIS_LEFTX 0
#define FSAPP_NAVIGATION_AXIS_LEFTY 1

// The keys used, with SDL3's keycodes (SDLK_UP and so on).
#define FSAPP_NAVIGATION_KEY_UP 0x40000052u
#define FSAPP_NAVIGATION_KEY_DOWN 0x40000051u
#define FSAPP_NAVIGATION_KEY_LEFT 0x40000050u
#define FSAPP_NAVIGATION_KEY_RIGHT 0x4000004fu
#define FSAPP_NAVIGATION_KEY_RETURN 0x0000000du
#define FSAPP_NAVIGATION_KEY_KP_ENTER 0x40000058u
#define FSAPP_NAVIGATION_KEY_ESCAPE 0x0000001bu
#define FSAPP_NAVIGATION_KEY_BACKSPACE 0x00000008u
#define FSAPP_NAVIGATION_KEY_PAGEUP 0x4000004bu
#define FSAPP_NAVIGATION_KEY_PAGEDOWN 0x4000004eu
#define FSAPP_NAVIGATION_KEY_HOME 0x4000004au
#define FSAPP_NAVIGATION_KEY_END 0x4000004du

// The navigation values, also Python's fsapp.surfaceapi.v1.Navigation (generated from this enum by
// od-fs/tools/gen_surface_enums.py: keep one value per line).
typedef enum fsapp_navigation_value {
    FSAPP_NAVIGATION_UP = 1,
    FSAPP_NAVIGATION_DOWN = 2,
    FSAPP_NAVIGATION_LEFT = 3,
    FSAPP_NAVIGATION_RIGHT = 4,
    FSAPP_NAVIGATION_PAGE_BACK = 5,
    FSAPP_NAVIGATION_PAGE_FORWARD = 6,
    FSAPP_NAVIGATION_FIRST = 7,
    FSAPP_NAVIGATION_LAST = 8,
    FSAPP_NAVIGATION_ACCEPT = 9,
    FSAPP_NAVIGATION_BACK = 10,
    FSAPP_NAVIGATION_MENU = 11,
} fsapp_navigation_value_t;

typedef enum fsapp_navigation_device_kind {
    FSAPP_NAVIGATION_KEYBOARD,
    FSAPP_NAVIGATION_GAMEPAD,
} fsapp_navigation_device_kind_t;

// The input device that produced a navigation event.
typedef struct fsapp_navigation_device {
    fsapp_navigation_device_kind_t kind;
    // A gamepad's instance ID (SDL_JoystickID); 0 for the keyboard.
    uint32_t id;
} fsapp_navigation_device_t;

typedef struct fsapp_navigation_event {
    fsapp_navigation_value_t value;
    fsapp_navigation_device_t device;
    bool repeat;
} fsapp_navigation_event_t;

// What the recogniser does with a gamepad button.
typedef struct fsapp_navigation_menu_result {
    // Pass the button on to routing (else it is consumed).
    bool pass;
    // First send routing a release of the same gamepad's Start.
    bool release_start;
    // Then the menu input, to routing.
    bool menu;
} fsapp_navigation_menu_result_t;

typedef struct fsapp_navigation fsapp_navigation_t;

// Creates a mapper and recogniser with nothing held.
fsapp_navigation_t *fsapp_navigation_create(void);

void fsapp_navigation_destroy(fsapp_navigation_t *navigation);

// The mapper: a gamepad's button pressed or released at time_ns. Returns true and writes event
// when it gives a navigation event.
bool fsapp_navigation_handle_button(fsapp_navigation_t *navigation, uint32_t device, int button,
                                    bool down, int64_t time_ns, fsapp_navigation_event_t *event);

// The mapper: a gamepad's axis at value (SDL's range, -32768 to 32767) at time_ns. Returns true and
// writes event when it gives a navigation event.
bool fsapp_navigation_handle_axis(fsapp_navigation_t *navigation, uint32_t device, int axis,
                                  int value, int64_t time_ns, fsapp_navigation_event_t *event);

// The mapper: a key (SDL_Keycode) pressed, with repeat for the host's key repeat. Returns true and
// writes event when it gives a navigation event.
bool fsapp_navigation_handle_key(fsapp_navigation_t *navigation, uint32_t key, bool repeat,
                                 fsapp_navigation_event_t *event);

// The mapper: the repeats due at now_ns. Returns their number and points events at them, valid
// until the next call.
int fsapp_navigation_update(fsapp_navigation_t *navigation, int64_t now_ns,
                            const fsapp_navigation_event_t **events);

// The mapper: the time of the next repeat. Returns false when nothing repeats.
bool fsapp_navigation_get_deadline_ns(const fsapp_navigation_t *navigation, int64_t *deadline_ns);

// The mapper: forgets what is held, so nothing repeats (when keyboard focus leaves a UI surface).
// The recogniser keeps its state.
void fsapp_navigation_reset(fsapp_navigation_t *navigation);

// Forgets what a gamepad holds, in the mapper and the recogniser (when it is removed).
void fsapp_navigation_remove_device(fsapp_navigation_t *navigation, uint32_t device);

// The recogniser: a gamepad's button pressed or released, before routing. Writes what to do with
// it to result.
void fsapp_navigation_recognise_menu_input(fsapp_navigation_t *navigation, uint32_t device,
                                           int button, bool down,
                                           fsapp_navigation_menu_result_t *result);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_NAVIGATION_H */
