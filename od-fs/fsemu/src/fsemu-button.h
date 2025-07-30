#ifndef FSEMU_BUTTON_H_
#define FSEMU_BUTTON_H_

#include "fsemu-common.h"
#include "fsemu-key.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    // FSEMU_KEY_NUM_KEYS_2 = FSEMU_KEY_NUM_KEYS * 2,
    // FIXME: Space for Mod and Shift+Mod combinations?
    FSEMU_KEY_NUM_KEYS_3 = FSEMU_KEY_NUM_KEYS * 3,

    FSEMU_BUTTON_BEFORE_FIRST,

    // Buttons in the same order as SDL2 gamecontroller
    FSEMU_BUTTON_A,
    FSEMU_BUTTON_B,
    FSEMU_BUTTON_X,
    FSEMU_BUTTON_Y,
    FSEMU_BUTTON_BACK,
    FSEMU_BUTTON_GUIDE,
    FSEMU_BUTTON_START,
    FSEMU_BUTTON_LEFTSTICK,
    FSEMU_BUTTON_RIGHTSTICK,
    FSEMU_BUTTON_LEFTSHOULDER,
    FSEMU_BUTTON_RIGHTSHOULDER,
    FSEMU_BUTTON_DPUP,
    FSEMU_BUTTON_DPDOWN,
    FSEMU_BUTTON_DPLEFT,
    FSEMU_BUTTON_DPRIGHT,

    FSEMU_BUTTON_AFTER_LAST,
};

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_BUTTON_H_
