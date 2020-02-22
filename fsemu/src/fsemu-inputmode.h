#ifndef FSEMU_INPUTMODE_H_
#define FSEMU_INPUTMODE_H_

#include <stdint.h>

#include "fsemu-common.h"
#include "fsemu-inputdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fsemu_inputmode fsemu_inputmode_t;

fsemu_inputmode_t *fsemu_inputmode_new(void);
// fsemu_inputmode_t *fsemu_inputmode_alloc(void);
void fsemu_inputmode_init(fsemu_inputmode_t *mode);

const char *fsemu_inputmode_name(fsemu_inputmode_t *mode);
void fsemu_inputmode_set_name(fsemu_inputmode_t *mode, const char *name);

typedef struct fsemu_inputport_mapping {
    int device_type;   // keyboard, mouse, joystick or gamecontroller?
    int device_input;  // device type specific offset (key number, joystick
                       // button, etc)
    // -- or, combined device_type and offset into
    // int input; no, cannot do, device index is not known here...
    int action;
} fsemu_inputport_mapping_t;

#if 0
// void fsemu_inputport_mode_set_default_controller(fsemu_inputport_mode_t *mode);
void fsemu_inputmode_set_default_mapping(fsemu_inputport_mode_t *mode);

void fsemu_inputmode_add_controller_mapping(fsemu_inputport_mode_t *mode);
void fsemu_inputmode_add_keyboard_mapping(fsemu_inputport_mode_t *mode);
void fsemu_inputmode_add_mapping(fsemu_inputport_mode_t *mode);
#endif

// void fsemu_inputmode_map_controller(fsemu_inputport_mode_t *mode);
// void fsemu_inputmode_map_key(fsemu_inputport_mode_t *mode);

// Examples:
// fsemu_inputmode_map(
//     mode, FSEMU_INPUT_KEYBOARD, FSEMU_KEY_LEFT, C64_JOYSTICK2_FIRE)
// fsemu_inputmode_map(
//     mode, FSEMU_INPUT_CONTROLLER, FSEMU_CONTROLLER_A, C64_JOYSTICK2_FIRE)

typedef void (*fsemu_inputmode_map_f)(fsemu_inputmode_t *, int, int, int);

// void fsemu_inputmode_map(fsemu_inputmode_t *mode,
//                          int device_type,
//                          int device_input,
//                          int action);

void fsemu_inputmode_map(fsemu_inputmode_t *mode, int input, int action);

#ifdef FSEMU_INTERNAL

struct fsemu_inputmode {
    char *name;
    uint16_t mapping[FSEMU_INPUTDEVICE_MAX];
};

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_INPUTMODE_H_
