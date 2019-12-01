#ifndef FSEMU_CONTROL_H_
#define FSEMU_CONTROL_H_

#include <stdbool.h>

#include "fsemu-common.h"

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

#ifdef __cplusplus
}
#endif

#define fsemu_control_log(format, ...) \
    fsemu_log("[FSEMU] [CNTRL] " format, ##__VA_ARGS__)

#endif  // FSEMU_CONTROL_H_
