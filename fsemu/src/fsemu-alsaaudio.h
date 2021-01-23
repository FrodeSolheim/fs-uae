#ifndef FSEMU_ALSAAUDIO_H_
#define FSEMU_ALSAAUDIO_H_

#include "fsemu-common.h"

#ifdef FSEMU_ALSA

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_alsaaudio_init(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_ALSA

#endif  // FSEMU_ALSAAUDIO_H_
