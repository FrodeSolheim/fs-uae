#ifndef FSEMU_SDLAUDIO_H
#define FSEMU_SDLAUDIO_H

#include "fsemu-common.h"
#include "fsemu-sdl.h"

#ifdef FSEMU_SDL

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlaudio_init(void);

// Pauses the audio device (no more callbacks) and clears the stream of what it has not played.
void fsemu_sdlaudio_pause(void);

// Resumes the audio device.
void fsemu_sdlaudio_resume(void);

// Clears the stream of what the device has not taken from it yet (on a reset of the audio ring
// buffer). Any thread.
void fsemu_sdlaudio_clear(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // FSEMU_SDL

#endif /* FSEMU_SDLAUDIO_H */
