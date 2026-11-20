/*
 * The audio ring buffer: the audio on its way from the emulation thread to the audio callback.
 *
 * A lock-free single-producer, single-consumer ring buffer of stereo frames of 16-bit samples
 * (native byte order), with atomic read and write positions (acquire and release). Exactly one
 * thread writes (the writer: the emulation thread) and exactly one thread reads (the reader: the
 * audio callback); every function says which side calls it. Reading allocates nothing and takes no
 * lock.
 *
 * - An audio underrun: a read hands over what there is and silence for exactly the missing part,
 *   and counts the audio underrun. Nothing else is added.
 * - An audio overrun: the ceiling is FSEMU_AUDIORING_CEILING_FACTOR times the target. A write that
 *   would take the level (with the level outside the ring buffer, as the writer tells it) above it
 * drops the oldest audio, so that the level after the write is the target, in one step, and counts
 * the audio overrun. The writer decides the cut; the next read carries it out with a short
 * crossfade from the dropped audio into the audio after the cut, so that it does not click. The
 * reader never moves the read position on its own.
 * - Clearing (a reset): the writer drops everything written so far; the next read fades the
 *   dropped audio out.
 * - A reset to the target: the writer drops the oldest audio down to the target, carried out as
 *   for an audio overrun, or adds silence after the newest audio up to it, faded out from the
 *   newest audio so that it does not click. A drop to the target only drops.
 *
 * Kept apart, with no dependency on SDL, the audio device or the frame timing, so that it can be
 * tested alone (od-fs/tests/fsemu).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_AUDIORING_H
#define FSEMU_AUDIORING_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// The size of a frame: two channels of 16-bit samples. Sizes are in bytes and are whole frames
// (the functions leave out a partial frame at the end).
#define FSEMU_AUDIORING_FRAME_BYTES 4

// The ceiling of the level, as a multiple of the target.
#define FSEMU_AUDIORING_CEILING_FACTOR 4

// The length of the crossfade across the cut of an audio overrun (and of the fade-out after
// clearing), in frames: about 1.5 ms at 44.1 kHz. Shorter when less audio is dropped, or when the
// read is shorter.
#define FSEMU_AUDIORING_FADE_FRAMES 64

typedef struct fsemu_audioring_t fsemu_audioring_t;

// What a read handed over, for the reader's measurements.
typedef struct fsemu_audioring_read_t {
    // The silence handed over for the missing part (0 when there was no audio underrun).
    int missing_bytes;
    // The read position after the read, and the write position the read saw (positions count the
    // bytes of the stream from its start, so the difference is what is left in the ring buffer).
    int64_t read_position;
    int64_t write_position;
} fsemu_audioring_read_t;

// Creates an empty ring buffer with room for at least capacity_bytes (rounded up to a power of
// two) and no target (no ceiling). Allocates; call it before the reader and the writer start.
fsemu_audioring_t *fsemu_audioring_create(int capacity_bytes);

// Releases the ring buffer, when neither the reader nor the writer uses it any more.
void fsemu_audioring_destroy(fsemu_audioring_t *ring);

// ----------------------------------------------------------------------------
// The writer
// ----------------------------------------------------------------------------

// Sets the target level (the ceiling is FSEMU_AUDIORING_CEILING_FACTOR times it); 0 for none.
void fsemu_audioring_set_target_bytes(fsemu_audioring_t *ring, int target_bytes);

// Sets the level outside the ring buffer as the writer estimates it (below 0 counts as 0): the
// audio the reader has handed on and that is not yet played (the audio in flight), and audio the
// writer has not written yet. The ceiling, an audio overrun and the target count it with the ring
// buffer's own level.
void fsemu_audioring_set_outside_bytes(fsemu_audioring_t *ring, int outside_bytes);

// Writes the audio. Returns how much audio it dropped: on an audio overrun, the oldest audio down
// to the target (counted as an audio overrun; a reader that is carrying out the last cut at the
// time may still hand over the rest of its read of it); and when the ring buffer is full because
// the reader has stopped reading, the audio written now (counted once, when it becomes full). 0
// otherwise.
int fsemu_audioring_write(fsemu_audioring_t *ring, const void *data, int size);

// Writes silence, as fsemu_audioring_write does.
int fsemu_audioring_write_silence(fsemu_audioring_t *ring, int size);

// Drops all the audio written so far (counted as a reset). Returns how much it dropped.
int fsemu_audioring_clear(fsemu_audioring_t *ring);

// Resets the level (with the level outside) to the target (counted as a reset): above it, drops
// the oldest audio, carried out with a crossfade as for an audio overrun, and returns how much it
// dropped; below it, writes silence after the newest audio up to the target (faded out from the
// newest audio that the reader has not read yet, as fsemu_audioring_fill_to_target does), and
// sets *silence_bytes (when silence_bytes is not NULL) to how much.
int fsemu_audioring_reset_to_target(fsemu_audioring_t *ring, int *silence_bytes);

// Drops the oldest audio down to the target (with the level outside), carried out with a
// crossfade as for an audio overrun (counted as a reset), and returns how much it dropped; at or
// below the target, does nothing (adds no silence) and returns 0.
int fsemu_audioring_drop_to_target(fsemu_audioring_t *ring);

// Writes silence after the newest audio up to the target (with the level outside), as for
// holding the level at the target while the audio is muted (not counted as a reset). After audio
// that the reader has not read yet, the silence starts with a fade-out from its last frame
// (FSEMU_AUDIORING_FADE_FRAMES, or fewer when less silence is written), so that it does not
// click. Returns how much it wrote: 0 at or above the target.
int fsemu_audioring_fill_to_target(fsemu_audioring_t *ring);

// The level as the writer sees it: the audio written and not yet read, less the audio dropped.
int fsemu_audioring_get_level_bytes(fsemu_audioring_t *ring);

// The write position, and the position the reader goes on from (the read position, or the cut
// of an audio overrun or a reset that the reader has not carried out yet).
int64_t fsemu_audioring_get_write_position(fsemu_audioring_t *ring);
int64_t fsemu_audioring_get_read_position(fsemu_audioring_t *ring);

// ----------------------------------------------------------------------------
// The reader
// ----------------------------------------------------------------------------

// Hands over exactly size bytes into data: the audio there is, after carrying out a cut the writer
// asked for, and silence for the missing part (counted as an audio underrun). Fills in result
// when it is not NULL.
void fsemu_audioring_read(fsemu_audioring_t *ring, void *data, int size,
                          fsemu_audioring_read_t *result);

// ----------------------------------------------------------------------------
// Either side
// ----------------------------------------------------------------------------

// The audio underruns, audio overruns and resets so far.
int64_t fsemu_audioring_get_underrun_count(fsemu_audioring_t *ring);
int64_t fsemu_audioring_get_overrun_count(fsemu_audioring_t *ring);
int64_t fsemu_audioring_get_reset_count(fsemu_audioring_t *ring);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_AUDIORING_H */
