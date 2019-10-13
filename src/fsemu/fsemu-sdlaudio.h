#ifndef FSEMU_SDLAUDIO_H_
#define FSEMU_SDLAUDIO_H_

#include "fsemu-common.h"
#include "fsemu-sdl.h"

#ifdef FSEMU_SDL

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlaudio_init(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDL

#endif  // FSEMU_SDLAUDIO_H_
