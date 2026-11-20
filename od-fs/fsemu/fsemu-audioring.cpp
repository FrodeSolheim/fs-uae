/*
 * The audio ring buffer (see fsemu-audioring.h).
 *
 * The read and write positions count the bytes of the stream from its start (they never wrap), so
 * the level is their difference; the byte in the buffer is at the position modulo the capacity (a
 * power of two). The writer publishes the audio with a release store of the write position, which
 * the reader loads with acquire before reading the audio; the reader gives the room back with a
 * release store of the read position, which the writer loads with acquire before writing over it.
 *
 * Dropping audio (an audio overrun, a reset) moves where the reader goes on from, which only the
 * reader may change: the writer publishes the cut (the position of the first byte kept) after the
 * write position it belongs to, and the reader takes it at its next read. Until then the writer
 * counts the level from the cut, but the room up to the read position stays the reader's.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-audioring.h"

#include <string.h>

#include <atomic>
#include <vector>

struct fsemu_audioring_t {
    std::vector<uint8_t> data;
    int64_t mask;

    std::atomic<int64_t> write_position;
    std::atomic<int64_t> read_position;
    // The cut the writer asked for and the reader has not taken yet; 0 for none.
    std::atomic<int64_t> cut;

    std::atomic<int64_t> underruns;
    std::atomic<int64_t> overruns;
    std::atomic<int64_t> resets;

    // The writer's own: the target, the level outside (0 or more), the last cut it asked for,
    // and whether the ring buffer is full because the reader has stopped reading.
    int target_bytes;
    int outside_bytes;
    int64_t last_cut;
    bool full;
};

static int whole_frames(int size)
{
    if (size <= 0) {
        return 0;
    }
    return size - size % FSEMU_AUDIORING_FRAME_BYTES;
}

static int64_t max64(int64_t a, int64_t b)
{
    return a > b ? a : b;
}

static int min_int(int a, int b)
{
    return a < b ? a : b;
}

fsemu_audioring_t *fsemu_audioring_create(int capacity_bytes)
{
    int64_t capacity = FSEMU_AUDIORING_FRAME_BYTES;
    while (capacity < capacity_bytes) {
        capacity *= 2;
    }
    fsemu_audioring_t *ring = new fsemu_audioring_t();
    ring->data.resize(capacity);
    ring->mask = capacity - 1;
    ring->write_position.store(0);
    ring->read_position.store(0);
    ring->cut.store(0);
    ring->underruns.store(0);
    ring->overruns.store(0);
    ring->resets.store(0);
    ring->target_bytes = 0;
    ring->outside_bytes = 0;
    ring->last_cut = 0;
    ring->full = false;
    return ring;
}

void fsemu_audioring_destroy(fsemu_audioring_t *ring)
{
    delete ring;
}

// ----------------------------------------------------------------------------

// Copies size bytes into the buffer from position on, across the end of the buffer if need be.
// With data NULL, copies silence.
static void copy_in(fsemu_audioring_t *ring, int64_t position, const uint8_t *data, int size)
{
    int64_t capacity = ring->mask + 1;
    int offset = (int)(position & ring->mask);
    int first = (int)(capacity - offset < size ? capacity - offset : size);
    if (data) {
        memcpy(ring->data.data() + offset, data, first);
        memcpy(ring->data.data(), data + first, size - first);
    } else {
        memset(ring->data.data() + offset, 0, first);
        memset(ring->data.data(), 0, size - first);
    }
}

// Copies size bytes out of the buffer from position on, across the end of the buffer if need be.
static void copy_out(fsemu_audioring_t *ring, int64_t position, uint8_t *data, int size)
{
    int64_t capacity = ring->mask + 1;
    int offset = (int)(position & ring->mask);
    int first = (int)(capacity - offset < size ? capacity - offset : size);
    memcpy(data, ring->data.data() + offset, first);
    memcpy(data + first, ring->data.data(), size - first);
}

// The sample at a byte position (a multiple of the sample size).
static int16_t sample_at(fsemu_audioring_t *ring, int64_t position)
{
    int16_t sample;
    memcpy(&sample, ring->data.data() + (position & ring->mask), sizeof(sample));
    return sample;
}

// ----------------------------------------------------------------------------
// The writer
// ----------------------------------------------------------------------------

// Where the reader goes on from, as the writer sees it.
static int64_t effective_read_position(fsemu_audioring_t *ring)
{
    return max64(ring->read_position.load(std::memory_order_acquire), ring->last_cut);
}

// The ring buffer's own level at the target: the target less the level outside, 0 or more.
static int64_t level_at_target(fsemu_audioring_t *ring)
{
    int64_t level = ring->target_bytes - ring->outside_bytes;
    return level > 0 ? level : 0;
}

static void ask_for_cut(fsemu_audioring_t *ring, int64_t cut)
{
    ring->last_cut = cut;
    ring->cut.store(cut, std::memory_order_release);
}

static int write_bytes(fsemu_audioring_t *ring, const uint8_t *data, int size)
{
    size = whole_frames(size);
    if (size == 0) {
        return 0;
    }
    int64_t write = ring->write_position.load(std::memory_order_relaxed);
    int64_t read = ring->read_position.load(std::memory_order_acquire);
    if (write + size - read > ring->mask + 1) {
        // The reader has stopped reading (a cut is taken only when it reads), so there is no room
        // for the audio: drop it, counted once.
        if (!ring->full) {
            ring->full = true;
            ring->overruns.fetch_add(1, std::memory_order_relaxed);
        }
        return size;
    }
    ring->full = false;

    int dropped = 0;
    int64_t cut = 0;
    int64_t level = write - max64(read, ring->last_cut);
    int64_t target = ring->target_bytes;
    if (target > 0 &&
        level + ring->outside_bytes + size > target * FSEMU_AUDIORING_CEILING_FACTOR) {
        // An audio overrun: drop the oldest audio, so that the level after this write, with the
        // level outside, is the target.
        cut = write + size - level_at_target(ring);
        dropped = (int)(cut - (write - level));
        ring->overruns.fetch_add(1, std::memory_order_relaxed);
    }

    copy_in(ring, write, data, size);
    ring->write_position.store(write + size, std::memory_order_release);
    if (cut) {
        // After the write position, so that a reader that takes the cut sees the audio after it.
        ask_for_cut(ring, cut);
    }
    return dropped;
}

void fsemu_audioring_set_target_bytes(fsemu_audioring_t *ring, int target_bytes)
{
    ring->target_bytes = whole_frames(target_bytes);
}

void fsemu_audioring_set_outside_bytes(fsemu_audioring_t *ring, int outside_bytes)
{
    ring->outside_bytes = outside_bytes > 0 ? whole_frames(outside_bytes) : 0;
}

int fsemu_audioring_write(fsemu_audioring_t *ring, const void *data, int size)
{
    return write_bytes(ring, (const uint8_t *)data, size);
}

int fsemu_audioring_write_silence(fsemu_audioring_t *ring, int size)
{
    return write_bytes(ring, NULL, size);
}

int fsemu_audioring_clear(fsemu_audioring_t *ring)
{
    int64_t write = ring->write_position.load(std::memory_order_relaxed);
    int dropped = (int)(write - effective_read_position(ring));
    if (dropped > 0) {
        ask_for_cut(ring, write);
    }
    ring->resets.fetch_add(1, std::memory_order_relaxed);
    return dropped;
}

// Writes silence up to the target (with the level outside). Returns how much it wrote. After audio
// that the reader has not read yet, the silence fades out from its last frame, so that it does not
// click.
static int fill_to_target(fsemu_audioring_t *ring)
{
    int64_t write = ring->write_position.load(std::memory_order_relaxed);
    int64_t level = write - effective_read_position(ring);
    int64_t silence = level_at_target(ring) - level;
    if (silence <= 0) {
        return 0;
    }
    int fade_bytes = 0;
    if (level > 0) {
        // The fade: from the last frame towards 0, the weight going from fade out of fade + 1
        // down to 1 out of fade + 1.
        uint8_t fade_data[FSEMU_AUDIORING_FADE_FRAMES * FSEMU_AUDIORING_FRAME_BYTES];
        int fade =
            min_int(FSEMU_AUDIORING_FADE_FRAMES, (int)(silence / FSEMU_AUDIORING_FRAME_BYTES));
        for (int c = 0; c < 2; c++) {
            int last = sample_at(ring, write - FSEMU_AUDIORING_FRAME_BYTES + c * 2);
            for (int i = 0; i < fade; i++) {
                int16_t sample = (int16_t)(last * (fade - i) / (fade + 1));
                memcpy(fade_data + i * FSEMU_AUDIORING_FRAME_BYTES + c * 2, &sample,
                       sizeof(sample));
            }
        }
        fade_bytes = fade * FSEMU_AUDIORING_FRAME_BYTES;
        if (write_bytes(ring, fade_data, fade_bytes) != 0) {
            return 0;
        }
    }
    if (silence > fade_bytes && write_bytes(ring, NULL, (int)silence - fade_bytes) != 0) {
        return fade_bytes;
    }
    return (int)silence;
}

int fsemu_audioring_reset_to_target(fsemu_audioring_t *ring, int *silence_bytes)
{
    int64_t write = ring->write_position.load(std::memory_order_relaxed);
    int64_t level = write - effective_read_position(ring);
    int64_t kept = level_at_target(ring);
    int dropped = 0;
    int silence = 0;
    if (level > kept) {
        dropped = (int)(level - kept);
        ask_for_cut(ring, write - kept);
    } else {
        silence = fill_to_target(ring);
    }
    ring->resets.fetch_add(1, std::memory_order_relaxed);
    if (silence_bytes) {
        *silence_bytes = silence;
    }
    return dropped;
}

int fsemu_audioring_drop_to_target(fsemu_audioring_t *ring)
{
    int64_t write = ring->write_position.load(std::memory_order_relaxed);
    int64_t level = write - effective_read_position(ring);
    int64_t kept = level_at_target(ring);
    if (level <= kept) {
        return 0;
    }
    ask_for_cut(ring, write - kept);
    ring->resets.fetch_add(1, std::memory_order_relaxed);
    return (int)(level - kept);
}

int fsemu_audioring_fill_to_target(fsemu_audioring_t *ring)
{
    return fill_to_target(ring);
}

int64_t fsemu_audioring_get_write_position(fsemu_audioring_t *ring)
{
    return ring->write_position.load(std::memory_order_relaxed);
}

int64_t fsemu_audioring_get_read_position(fsemu_audioring_t *ring)
{
    return effective_read_position(ring);
}

int fsemu_audioring_get_level_bytes(fsemu_audioring_t *ring)
{
    return (int)(fsemu_audioring_get_write_position(ring) - effective_read_position(ring));
}

// ----------------------------------------------------------------------------
// The reader
// ----------------------------------------------------------------------------

// Carries out a cut at the start of a read: hands over a crossfade from the dropped audio (from
// the read position on, which goes on from what was handed over before) into the audio after the
// cut, or a fade-out when there is none yet (after clearing). Returns the frames handed over, and
// moves the read position past the audio after the cut that the crossfade used.
static int carry_out_cut(fsemu_audioring_t *ring, int64_t *read, int64_t cut, int64_t write,
                         uint8_t *data, int frames)
{
    const int frame_bytes = FSEMU_AUDIORING_FRAME_BYTES;
    int dropped_frames = (int)((cut - *read) / frame_bytes);
    int after_frames = (int)((write - cut) / frame_bytes);
    int fade = min_int(min_int(FSEMU_AUDIORING_FADE_FRAMES, dropped_frames), frames);
    if (after_frames > 0) {
        fade = min_int(fade, after_frames);
    }
    for (int i = 0; i < fade; i++) {
        // The weight of the audio after the cut goes from 1 to fade out of fade + 1; the result
        // lies between the two samples (integer division rounds towards zero).
        int weight = i + 1;
        for (int c = 0; c < 2; c++) {
            int old_sample = sample_at(ring, *read + i * frame_bytes + c * 2);
            int new_sample = after_frames > 0 ? sample_at(ring, cut + i * frame_bytes + c * 2) : 0;
            int16_t sample =
                (int16_t)((old_sample * (fade + 1 - weight) + new_sample * weight) / (fade + 1));
            memcpy(data + i * frame_bytes + c * 2, &sample, sizeof(sample));
        }
    }
    *read = cut + (after_frames > 0 ? (int64_t)fade * frame_bytes : 0);
    return fade;
}

void fsemu_audioring_read(fsemu_audioring_t *ring, void *void_data, int size,
                          fsemu_audioring_read_t *result)
{
    uint8_t *data = (uint8_t *)void_data;
    size = whole_frames(size);
    int64_t read = ring->read_position.load(std::memory_order_relaxed);
    // The cut before the write position: the writer publishes it after the write position it
    // belongs to.
    int64_t cut = ring->cut.exchange(0, std::memory_order_acquire);
    int64_t write = ring->write_position.load(std::memory_order_acquire);

    int done = 0;
    if (cut > read && cut <= write) {
        int frames =
            carry_out_cut(ring, &read, cut, write, data, size / FSEMU_AUDIORING_FRAME_BYTES);
        done = frames * FSEMU_AUDIORING_FRAME_BYTES;
    }

    int take = (int)(write - read) < size - done ? (int)(write - read) : size - done;
    copy_out(ring, read, data + done, take);
    read += take;
    done += take;
    ring->read_position.store(read, std::memory_order_release);

    int missing = size - done;
    if (missing > 0) {
        memset(data + done, 0, missing);
        ring->underruns.fetch_add(1, std::memory_order_relaxed);
    }

    if (result) {
        result->missing_bytes = missing;
        result->read_position = read;
        result->write_position = write;
    }
}

// ----------------------------------------------------------------------------
// Either side
// ----------------------------------------------------------------------------

int64_t fsemu_audioring_get_underrun_count(fsemu_audioring_t *ring)
{
    return ring->underruns.load(std::memory_order_relaxed);
}

int64_t fsemu_audioring_get_overrun_count(fsemu_audioring_t *ring)
{
    return ring->overruns.load(std::memory_order_relaxed);
}

int64_t fsemu_audioring_get_reset_count(fsemu_audioring_t *ring)
{
    return ring->resets.load(std::memory_order_relaxed);
}
