/*
 * The stripe recorder (see fsemu-stripe.h).
 *
 * Each frame's record is a list of changes of category (from an offset after the record's start
 * on, the time is spent as a category), ending with a change to NONE at the record's end. The
 * changes of all records follow each other in one ring, so that short frames (warp) take little
 * room and many of them are kept, and the records (where a frame's changes start, and how many)
 * are kept in a ring of the newest frames. Both are written by the emulation thread only. Each
 * record says which frame it holds once it is complete; the emulation thread clears that before
 * reusing it, and says which changes it is about to overwrite before it does. The main thread
 * checks both after copying a record, so that it never takes one that was being reused or
 * overwritten while it copied.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#define FSEMU_INTERNAL
#include "fsemu-stripe.h"

#include <string.h>

#include <algorithm>
#include <atomic>

#include <SDL3/SDL.h>

// How many frames' records are kept: as many as the frame info (fsemu-frameinfo.h) keeps, which
// is how far back the graph looks, also for the frames between two swaps while warping.
#define FSEMU_STRIPE_FRAMES 256

// The changes of all records: room for 31 frames of the most changes a record holds, and for far
// more of a frame's usual few hundred (a line wait or extra CPU block takes two or three) or a
// warped frame's few. Overwritten a block at a time.
#define FSEMU_STRIPE_CHANGES 65536
#define FSEMU_STRIPE_CHANGE_BLOCK 256

// No frame (a record being written or never written).
#define FSEMU_STRIPE_NO_FRAME -1

typedef struct {
    // The frame whose complete record this is, FSEMU_STRIPE_NO_FRAME while it is written.
    std::atomic<int> frame;
    int64_t start_ns;
    int64_t emulation_done_ns;
    // The record's changes: count changes from change number first (counted from the first change
    // ever written; change n is at n % FSEMU_STRIPE_CHANGES in the ring).
    uint64_t first;
    int count;
} fsemu_stripe_record_t;

bool fsemu_stripe_recording;

static struct {
    fsemu_stripe_record_t records[FSEMU_STRIPE_FRAMES];
    // The changes: from offsets_ns[i] after its record's start on, categories[i].
    int64_t offsets_ns[FSEMU_STRIPE_CHANGES];
    uint8_t categories[FSEMU_STRIPE_CHANGES];
    // Changes before this number may have been overwritten (or are being overwritten).
    std::atomic<uint64_t> overwritten;
    // The emulation thread's: the record being written and its frame (FSEMU_STRIPE_NO_FRAME when
    // none is open), its current category, and the number of the next change.
    fsemu_stripe_record_t *record;
    int record_frame;
    fsemu_stripe_category_t category;
    uint64_t next_change;
    // The newest published frame.
    std::atomic<int> ended_frame;
} fsemu_stripe = {{}, {},  {}, {0}, NULL, FSEMU_STRIPE_NO_FRAME, FSEMU_STRIPE_CATEGORY_NONE,
                  0,  {-1}};

// ----------------------------------------------------------------------------
// The emulation thread's side.

// Appends a change to the open record, at its next change number: before the first change of each
// block, says that the changes the block overwrites are gone.
static void fsemu_stripe_append(int64_t offset_ns, fsemu_stripe_category_t category)
{
    uint64_t n = fsemu_stripe.next_change++;
    if (n % FSEMU_STRIPE_CHANGE_BLOCK == 0 &&
        n + FSEMU_STRIPE_CHANGE_BLOCK > FSEMU_STRIPE_CHANGES) {
        fsemu_stripe.overwritten.store(n + FSEMU_STRIPE_CHANGE_BLOCK - FSEMU_STRIPE_CHANGES,
                                       std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_release);
    }
    fsemu_stripe.offsets_ns[n % FSEMU_STRIPE_CHANGES] = offset_ns;
    fsemu_stripe.categories[n % FSEMU_STRIPE_CHANGES] = (uint8_t)category;
    fsemu_stripe.record->count += 1;
}

// Opens frame's record from now_ns on, with the frame's work before its start gate (other work).
static void fsemu_stripe_open(int frame, int64_t now_ns)
{
    fsemu_stripe_record_t *record = &fsemu_stripe.records[frame % FSEMU_STRIPE_FRAMES];
    record->frame.store(FSEMU_STRIPE_NO_FRAME, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    record->start_ns = now_ns;
    record->emulation_done_ns = 0;
    record->first = fsemu_stripe.next_change;
    record->count = 0;
    fsemu_stripe.record = record;
    fsemu_stripe.record_frame = frame;
    fsemu_stripe.category = FSEMU_STRIPE_CATEGORY_OTHER_WORK;
    fsemu_stripe_append(0, FSEMU_STRIPE_CATEGORY_OTHER_WORK);
}

void fsemu_stripe_begin_frame(int frame_number, bool record)
{
    fsemu_stripe_recording = record;
    if (!fsemu_stripe_recording) {
        fsemu_stripe.record_frame = FSEMU_STRIPE_NO_FRAME;
        return;
    }
    if (fsemu_stripe.record_frame != frame_number) {
        // The first recorded frame (or the numbers jumped): from its start.
        fsemu_stripe_open(frame_number, (int64_t)SDL_GetTicksNS());
    }
}

// The time a hook gave, or now for FSEMU_STRIPE_NOW.
static int64_t fsemu_stripe_time(int64_t time_ns)
{
    return time_ns == FSEMU_STRIPE_NOW ? (int64_t)SDL_GetTicksNS() : time_ns;
}

// Appends a change to category at time_ns (not before the last change) to the record.
static void fsemu_stripe_add(fsemu_stripe_category_t category, int64_t time_ns)
{
    fsemu_stripe_record_t *record = fsemu_stripe.record;
    int64_t offset_ns = time_ns - record->start_ns;
    int last = (int)((record->first + record->count - 1) % FSEMU_STRIPE_CHANGES);
    if (offset_ns <= fsemu_stripe.offsets_ns[last]) {
        // At (or before) the last change: it changes to this category instead.
        fsemu_stripe.categories[last] = (uint8_t)category;
        return;
    }
    if (fsemu_stripe.categories[last] == category) {
        return;
    }
    if (record->count == FSEMU_STRIPE_MAX_SPANS && category != FSEMU_STRIPE_CATEGORY_NONE) {
        // Full, but for the end (NONE).
        // REVIEW: The last category is then drawn on to the frame's end.
        return;
    }
    fsemu_stripe_append(offset_ns, category);
}

void fsemu_stripe_set_category(fsemu_stripe_category_t category, int64_t time_ns)
{
    if (fsemu_stripe.record_frame == FSEMU_STRIPE_NO_FRAME) {
        return;
    }
    fsemu_stripe.category = category;
    fsemu_stripe_add(category, fsemu_stripe_time(time_ns));
}

fsemu_stripe_category_t fsemu_stripe_get_category(void)
{
    return fsemu_stripe.category;
}

void fsemu_stripe_on_emulation_done(int64_t time_ns)
{
    if (!fsemu_stripe_recording || fsemu_stripe.record_frame == FSEMU_STRIPE_NO_FRAME ||
        fsemu_stripe.record->emulation_done_ns != 0) {
        return;
    }
    int64_t now_ns = fsemu_stripe_time(time_ns);
    fsemu_stripe.record->emulation_done_ns = now_ns;
    fsemu_stripe_set_category(FSEMU_STRIPE_CATEGORY_OTHER_WORK, now_ns);
}

void fsemu_stripe_end_frame(int64_t time_ns)
{
    int frame = fsemu_stripe.record_frame;
    if (!fsemu_stripe_recording || frame == FSEMU_STRIPE_NO_FRAME) {
        return;
    }
    int64_t now_ns = fsemu_stripe_time(time_ns);
    fsemu_stripe_add(FSEMU_STRIPE_CATEGORY_NONE, now_ns);
    fsemu_stripe.record->frame.store(frame, std::memory_order_release);
    fsemu_stripe.ended_frame.store(frame, std::memory_order_release);
    fsemu_stripe_open(frame + 1, now_ns);
}

// ----------------------------------------------------------------------------
// The main thread's side.

int fsemu_stripe_get_ended_frame(void)
{
    return fsemu_stripe.ended_frame.load(std::memory_order_acquire);
}

bool fsemu_stripe_read(int frame_number, fsemu_stripe_span_t *spans, int max_spans,
                       fsemu_stripe_frame_t *frame)
{
    if (frame_number < 0) {
        return false;
    }
    fsemu_stripe_record_t *record = &fsemu_stripe.records[frame_number % FSEMU_STRIPE_FRAMES];
    if (record->frame.load(std::memory_order_acquire) != frame_number) {
        return false;
    }
    int64_t start_ns = record->start_ns;
    uint64_t first = record->first;
    int changes = std::min(record->count, FSEMU_STRIPE_MAX_SPANS + 1);
    int count = 0;
    for (int i = 0; i + 1 < changes && count < max_spans; i++) {
        int c = (int)((first + i) % FSEMU_STRIPE_CHANGES);
        int next = (int)((first + i + 1) % FSEMU_STRIPE_CHANGES);
        if (fsemu_stripe.categories[c] == FSEMU_STRIPE_CATEGORY_NONE) {
            continue;
        }
        spans[count].start_ns = start_ns + fsemu_stripe.offsets_ns[c];
        spans[count].end_ns = start_ns + fsemu_stripe.offsets_ns[next];
        spans[count].category = (fsemu_stripe_category_t)fsemu_stripe.categories[c];
        count++;
    }
    frame->emulation_done_ns = record->emulation_done_ns;
    std::atomic_thread_fence(std::memory_order_acquire);
    if (record->frame.load(std::memory_order_relaxed) != frame_number ||
        first < fsemu_stripe.overwritten.load(std::memory_order_relaxed)) {
        // Reused for a later frame, or its changes overwritten, while it was copied (or before).
        return false;
    }
    frame->spans = spans;
    frame->span_count = count;
    return true;
}
