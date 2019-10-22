#ifndef FSEMU_CONTROL_H_
#define FSEMU_CONTROL_H_

#include "fsemu-common.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool fsemu_control_warp(void);
void fsemu_control_set_warp(bool warp);
void fsemu_control_toggle_warp(void);

#ifdef __cplusplus
}
#endif

#define fsemu_control_log(format, ...) \
    fsemu_log("[FSEMU] [CNTRL] " format, ##__VA_ARGS__)

#endif  // FSEMU_CONTROL_H_
