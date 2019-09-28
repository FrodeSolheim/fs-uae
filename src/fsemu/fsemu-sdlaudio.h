#ifndef fsemu_sdlaudio_H_
#define fsemu_sdlaudio_H_

#include "fsemu/fsemu-common.h"
#include "fsemu/fsemu-sdl.h"

#ifdef FSEMU_SDL

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlaudio_init(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDL

#endif  // fsemu_sdlaudio_H_
