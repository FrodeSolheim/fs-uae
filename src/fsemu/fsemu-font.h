#ifndef FSEMU_FONT_H_
#define FSEMU_FONT_H_

#include "fsemu/fsemu-common.h"
#include "fsemu/fsemu-log.h"

typedef struct {
} fsemu_font_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_font_init(void);

#ifdef __cplusplus
}
#endif

#define fsemu_font_log(format, ...) \
    fsemu_log("[FSEMU] [FONT ] " format, ##__VA_ARGS__)

#endif  // FSEMU_FONT_H_
