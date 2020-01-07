#ifndef FSEMU_INPUT_H_
#define FSEMU_INPUT_H_

#include "fsemu-common.h"
#include "fsemu-keyboard.h"
// #include "fsemu-scancodes.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_input_init(void);

void fsemu_input_work(int timeout);

typedef struct {
    int key;
    int mod;
    int action;
} fsemu_input_configure_keyboard_t;

void fsemu_input_configure_keyboard(
    fsemu_input_configure_keyboard_t mapping[]);

typedef struct {
    const char *name;
    uint16_t value;
    int flags;
} fsemu_input_action_t;

bool fsemu_input_next_action(int *action, int *state);

void fsemu_input_register_actions(fsemu_input_action_t actions[]);

#define FSEMU_ACTION_STATE_MIN -0x7fff
#define FSEMU_ACTION_STATE_MAX 0x7fff

// Internal function used by fsemu_sdlinput.
void fsemu_input_handle_keyboard(fsemu_keyboard_key_t scancode, bool pressed);
void fsemu_input_process_action(int action, int state);

extern int fsemu_input_log_level;

#define fsemu_input_log(format, ...)                         \
    if (fsemu_input_log_level > 0) {                         \
        fsemu_log("[FSEMU] [INPUT] " format, ##__VA_ARGS__); \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_INPUT_H_
