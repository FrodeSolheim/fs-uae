#ifndef FSEMU_AXIS_H_
#define FSEMU_AXIS_H_

#include "fsemu-button.h"
#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    FSEMU_AXIS_BEFORE_FIRST = FSEMU_BUTTON_AFTER_LAST,

    // Axes in the same order as SDL2 gamecontroller
    FSEMU_AXIS_LEFTX,
    FSEMU_AXIS_LEFTY,
    FSEMU_AXIS_RIGHTX,
    FSEMU_AXIS_RIGHTY,
    FSEMU_AXIS_LEFTTRIGGER,
    FSEMU_AXIS_RIGHTTRIGGER,

    FSEMU_AXIS_BEFORE_FIRST_HALF_AXIS,
    // Half-axes (no corresponding concept in SDL2 gamecontroller).
    FSEMU_AXIS_LEFTXNEG,
    FSEMU_AXIS_LEFTXPOS,
    FSEMU_AXIS_LEFTYNEG,
    FSEMU_AXIS_LEFTYPOS,
    FSEMU_AXIS_RIGHTXNEG,
    FSEMU_AXIS_RIGHTXPOS,
    FSEMU_AXIS_RIGHTYNEG,
    FSEMU_AXIS_RIGHTYPOS,
    FSEMU_AXIS_AFTER_LAST_HALF_AXIS,

    FSEMU_AXIS_AFTER_LAST,
};

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_AXIS_H_
