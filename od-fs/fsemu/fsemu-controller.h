#ifndef FSEMU_CONTROLLER_H_
#define FSEMU_CONTROLLER_H_

#include <stdbool.h>

#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_controller_init(void);
void fsemu_controller_update(void);

#define FSEMU_CONTROLLER_MAX_COUNT 8

// The order of the controller enum entries must correspond with SDL2 game
// controller button and axis order. So do not mess with the indices
// (maybe)

enum {
    // FSEMU_CONTROLLER_UNKNOWN,
    FSEMU_CONTROLLER_BEFORE_FIRST_BUTTON,
    // Buttons in the same order as SDL2 gamecontroller
    // FSEMU_CONTROLLER_INVALID_BUTTON,
    FSEMU_CONTROLLER_A,
    FSEMU_CONTROLLER_B,
    FSEMU_CONTROLLER_X,
    FSEMU_CONTROLLER_Y,
    FSEMU_CONTROLLER_BACK,
    FSEMU_CONTROLLER_GUIDE,
    FSEMU_CONTROLLER_START,
    FSEMU_CONTROLLER_LEFTSTICK,
    FSEMU_CONTROLLER_RIGHTSTICK,
    FSEMU_CONTROLLER_LEFTSHOULDER,
    FSEMU_CONTROLLER_RIGHTSHOULDER,
    FSEMU_CONTROLLER_DPUP,
    FSEMU_CONTROLLER_DPDOWN,
    FSEMU_CONTROLLER_DPLEFT,
    FSEMU_CONTROLLER_DPRIGHT,
    FSEMU_CONTROLLER_AFTER_LAST_BUTTON,

    FSEMU_CONTROLLER_BEFORE_FIRST_AXIS,
    // Axes in the same order as SDL2 gamecontroller
    // FSEMU_CONTROLLER_INVALID_AXIS,
    FSEMU_CONTROLLER_LEFTX,
    FSEMU_CONTROLLER_LEFTY,
    FSEMU_CONTROLLER_RIGHTX,
    FSEMU_CONTROLLER_RIGHTY,
    FSEMU_CONTROLLER_LEFTTRIGGER,
    FSEMU_CONTROLLER_RIGHTTRIGGER,
    FSEMU_CONTROLLER_AFTER_LAST_AXIS,

    FSEMU_CONTROLLER_BEFORE_FIRST_HALF_AXIS,
    // Half-axes (no corresponding concept in SDL2 gamecontroller).
    FSEMU_CONTROLLER_LEFTXNEG,
    FSEMU_CONTROLLER_LEFTXPOS,
    FSEMU_CONTROLLER_LEFTYNEG,
    FSEMU_CONTROLLER_LEFTYPOS,
    FSEMU_CONTROLLER_RIGHTXNEG,
    FSEMU_CONTROLLER_RIGHTXPOS,
    FSEMU_CONTROLLER_RIGHTYNEG,
    FSEMU_CONTROLLER_RIGHTYPOS,
    FSEMU_CONTROLLER_AFTER_LAST_HALF_AXIS,

    FSEMU_CONTROLLER_MAX_SLOTS,
};

// typedef struct fsemu_action {
// } fsemu_action_t;

struct fsemu_controller {
    const char *name;
    int sdl_joystick_index;
    int sdl_instance_id;

    int16_t last_state[FSEMU_CONTROLLER_MAX_SLOTS];

#if 0
    fsemu_action_t __0;            // FSEMU_CONTROLLER_UNKNOWN,
    fsemu_action_t __1;            // FSEMU_CONTROLLER_BEFORE_FIRST_BUTTON
    fsemu_action_t a;              // FSEMU_CONTROLLER_A
    fsemu_action_t b;              // FSEMU_CONTROLLER_B
    fsemu_action_t x;              // FSEMU_CONTROLLER_X
    fsemu_action_t y;              // FSEMU_CONTROLLER_Y
    fsemu_action_t back;           // FSEMU_CONTROLLER_BACK
    fsemu_action_t guide;          // FSEMU_CONTROLLER_GUIDE
    fsemu_action_t start;          // FSEMU_CONTROLLER_START
    fsemu_action_t leftstick;      // FSEMU_CONTROLLER_LEFTSTICK
    fsemu_action_t rightstick;     // FSEMU_CONTROLLER_RIGHTSTICK
    fsemu_action_t leftshoulder;   // FSEMU_CONTROLLER_LEFTSHOULDER
    fsemu_action_t rightshoulder;  // FSEMU_CONTROLLER_RIGHTSHOULDER
    fsemu_action_t dpup;           // FSEMU_CONTROLLER_DPUP
    fsemu_action_t dpdown;         // FSEMU_CONTROLLER_DPDOWN
    fsemu_action_t dpleft;         // FSEMU_CONTROLLER_DPLEFT
    fsemu_action_t dpright;        // FSEMU_CONTROLLER_DPRIGHT
    fsemu_action_t __2;            // FSEMU_CONTROLLER_AFTER_LAST_BUTTON
    fsemu_action_t __3;            // FSEMU_CONTROLLER_BEFORE_FIRST_AXIS
    fsemu_action_t leftx;          // FSEMU_CONTROLLER_LEFTX
    fsemu_action_t lefy;           // FSEMU_CONTROLLER_LEFTY
    fsemu_action_t rightx;         // FSEMU_CONTROLLER_RIGHTX
    fsemu_action_t righty;         // FSEMU_CONTROLLER_RIGHTY
    fsemu_action_t lefttrigger;    // FSEMU_CONTROLLER_LEFTTRIGGER
    fsemu_action_t righttrigger;   // FSEMU_CONTROLLER_RIGHTTRIGGER
    fsemu_action_t __4;            // FSEMU_CONTROLLER_AFTER_LAST_AXIS
    fsemu_action_t __5;            // FSEMU_CONTROLLER_BEFORE_FIRST_HALF_AXIS
    fsemu_action_t leftxneg;       // FSEMU_CONTROLLER_LEFTXNEG
    fsemu_action_t leftxpos;       // FSEMU_CONTROLLER_LEFTXPOS
    fsemu_action_t leftyneg;       // FSEMU_CONTROLLER_LEFTYNEG
    fsemu_action_t leftypos;       // FSEMU_CONTROLLER_LEFTYPOS
    fsemu_action_t rightxneg;      // FSEMU_CONTROLLER_RIGHTXNEG
    fsemu_action_t rightxpos;      // FSEMU_CONTROLLER_RIGHTXPOS
    fsemu_action_t rightyneg;      // FSEMU_CONTROLLER_RIGHTYNEG
    fsemu_action_t rightypos;      // FSEMU_CONTROLLER_RIGHTYPOS
    fsemu_action_t __6;            // FSEMU_CONTROLLER_AFTER_LAST_HALF_AXIS
#endif
};

typedef struct fsemu_controller fsemu_controller_t;

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_CONTROLLER_H_
