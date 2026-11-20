#ifndef FSEMU_AUDIOBUFFER_H
#define FSEMU_AUDIOBUFFER_H

// fsemu's audio ring buffer (fsemu-audioring.h): the one between the emulation thread (the writer)
// and the audio callback (the reader), with the target audio buffer level, and audio overruns and
// resets recorded in the timing trace.

#include "fsemu-audioring.h"
#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Creates the audio ring buffer. May be called more than once.
void fsemu_audiobuffer_init(void);

// ----------------------------------------------------------------------------
// The writer (the emulation thread)
// ----------------------------------------------------------------------------

// Writes audio. The first write starts at the target audio buffer level (counting the level
// outside the audio ring buffer), with silence before it. A write that would take the level above
// the ceiling is an audio overrun (fsemu-audioring.h). While warping, the audio is dropped (muted).
void fsemu_audiobuffer_update(const void *data, int size);

void fsemu_audiobuffer_write_silence(int size);
void fsemu_audiobuffer_write_silence_ns(int64_t duration_ns);

// Drops all the audio written so far (a reset of the audio ring buffer).
void fsemu_audiobuffer_clear(void);

// The audio buffer level outside the audio ring buffer, as estimated now (bytes): the audio in
// flight and the audio the emulator has not written yet. Counted with the ring buffer's own level
// against the ceiling and by the functions below.
void fsemu_audiobuffer_set_outside_bytes(int bytes);

// Resets the audio buffer level to the target (a reset of the audio ring buffer): drops the oldest
// audio, crossfading, or adds silence after the newest, faded out from it. Nothing before the
// first write.
void fsemu_audiobuffer_reset_to_target(void);

// Drops the oldest audio down to the target, crossfading (a reset of the audio ring buffer), when
// the audio buffer level is above it; adds no silence. Nothing before the first write.
void fsemu_audiobuffer_drop_to_target(void);

// Drops all the audio written so far and fills the audio ring buffer to the target with silence
// (a reset of the audio ring buffer). Nothing before the first write.
void fsemu_audiobuffer_clear_to_target(void);

// Fills the audio ring buffer to the target with silence, if it is below it (not a reset).
// Nothing before the first write.
void fsemu_audiobuffer_fill_to_target(void);

// The write position, and the position the reader goes on from (fsemu-audioring.h).
int64_t fsemu_audiobuffer_get_write_position(void);
int64_t fsemu_audiobuffer_get_read_position(void);

// ----------------------------------------------------------------------------
// The reader (the audio callback)
// ----------------------------------------------------------------------------

// Hands over exactly size bytes: the audio there is and silence for the missing part (an audio
// underrun). Fills in result when it is not NULL.
void fsemu_audiobuffer_read(void *data, int size, fsemu_audioring_read_t *result);

// ----------------------------------------------------------------------------
// Either side
// ----------------------------------------------------------------------------

// The audio underruns, audio overruns and resets of the audio ring buffer so far.
int64_t fsemu_audiobuffer_get_underrun_count(void);
int64_t fsemu_audiobuffer_get_overrun_count(void);
int64_t fsemu_audiobuffer_get_reset_count(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_AUDIOBUFFER_H */
