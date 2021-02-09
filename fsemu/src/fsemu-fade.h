#ifndef FSEMU_FADE_H_
#define FSEMU_FADE_H_

#include <stdint.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_fade_init(void);
void fsemu_fade_update(void);
void fsemu_fade_set_color(uint32_t color);
#if 0
void fsemu_fade_force(bool force);
#endif

#ifdef FSEMU_INTERNAL

#define fsemu_fade_log(format, ...) \
    fsemu_log("[FSE] [FAD] " format, ##__VA_ARGS__)

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_FADE_H_
