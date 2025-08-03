#ifndef FSEMU_CONTROL_H_
#define FSEMU_CONTROL_H_

#include <stdbool.h>

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_control_quit(void);

bool fsemu_control_paused(void);
void fsemu_control_set_paused(bool paused);
void fsemu_control_toggle_paused(void);

bool fsemu_control_warp(void);
void fsemu_control_set_warp(bool warp);
void fsemu_control_toggle_warp(void);

// Alias for function in savestate module.
void fsemu_control_load_state(int slot);

// Alias for function in savestate module.
void fsemu_control_save_state(int slot);

bool fsemu_control_soft_reset_allowed(void);
void fsemu_control_set_soft_reset_allowed(bool allowed);

bool fsemu_control_hard_reset_allowed(void);
void fsemu_control_set_hard_reset_allowed(bool allowed);

#define fsemu_control_log(format, ...) \
    fsemu_log("[FSE] [CTL] " format, ##__VA_ARGS__)

#ifdef FSEMU_INTERNAL

void fsemu_control_set_paused_internal(bool paused);

void fsemu_control_set_warp_internal(bool warping);

#endif

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_CONTROL_H_
