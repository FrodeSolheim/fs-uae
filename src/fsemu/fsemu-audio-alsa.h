#ifndef FSEMU_AUDIO_ALSA_H_
#define FSEMU_AUDIO_ALSA_H_

#include "fsemu/fsemu-common.h"

#ifdef FSEMU_ALSA

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_audio_alsa_init(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_ALSA

#endif  // FSEMU_AUDIO_ALSA_H_
