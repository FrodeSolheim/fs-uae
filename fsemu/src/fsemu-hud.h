#ifndef FSEMU_HUD_H_
#define FSEMU_HUD_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_hud_init(void);
void fsemu_hud_update(void);

#ifdef FSEMU_INTERNAL

#define fsemu_hud_log(format, ...) \
    fsemu_log("[FSEMU] [ HUD ] " format, ##__VA_ARGS__)

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HUD_H_
