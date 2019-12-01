#ifndef FSEMU_MOUSE_H_
#define FSEMU_MOUSE_H_

#include <stdbool.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the mouse subsystem. Note, this is a higher-level subsystem,
 * the actual mouse input is handled by the fsemu_input and/or fsemu_window.
 */
void fsemu_mouse_init(void);

/**
 * Returns true if the mouse cursor is (or will be) captured.
 */
bool fsemu_mouse_captured(void);

/**
 * This function does not actually capture the mouse. Instead, it just sets
 * a flag, and the window driver will (at some point) check if the mouse
 * should be captured or released, compare with the actual state, and do the
 * actual capture/release if needed.
 */
void fsemu_mouse_set_captured(bool captured);
void fsemu_mouse_toggle_captured(void);

extern int fsemu_mouse_log_level;

#ifdef FSEMU_INTERNAL

#if 0
#define fsemu_mouse_log(format, ...) \
    fsemu_log("[FSEMU] [MOUSE] " format, ##__VA_ARGS__)
#endif

#define fsemu_mouse_log(level, format, ...)                  \
    if (fsemu_mouse_log_level >= level) {                    \
        fsemu_log("[FSEMU] [AUDIO] " format, ##__VA_ARGS__); \
    }

#if 0

void fsemu_mouse_handle_position(int window_x, int window_y);
void fsemu_mouse_handle_click(int button,
                              int state,
                              int window_x,
                              int window_y);
#endif

typedef struct fsemu_mouse_event {
    int pressed;
    int released;
    bool moved;
    int x;
    int y;

    // bool buttons[4];
    // deprecated:
    int button;
    int state;
} fsemu_mouse_event_t;

bool fsemu_mouse_handle_mouse(fsemu_mouse_event_t *event);

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MOUSE_H_
