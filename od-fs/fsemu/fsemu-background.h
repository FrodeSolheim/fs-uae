#ifndef FSEMU_BACKGROUND_H_
#define FSEMU_BACKGROUND_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_background_init(void);

#define fsemu_background_log(format, ...) fsemu_log("[FSE] [BCK] " format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_BACKGROUND_H_
