/*
 * The stripe recorder: what the emulation thread does along the real timeline, recorded per
 * emulated frame as the spans of the performance graph's background stripe (fsemu-graphrows.h),
 * with the frame's emulation done.
 *
 * The emulation thread's hooks (the frame's start, the start gate, the line waits, the extra CPU
 * blocks, posting slices, the frame's end and its end wait) say from when on the thread spends its
 * time in which category. A frame's record runs from the end of the frame before (so the work
 * between two frames is the next frame's other work) to the end of its own end wait, and is
 * published there for the main thread, which reads it for the frame's row. With the graph off, the
 * hooks do nothing but test fsemu_stripe_is_recording, decided once per frame at its start.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_STRIPE_H
#define FSEMU_STRIPE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// The categories of the emulation thread's time, in three groups (work, waiting and late). NONE is
// time outside the frame's records, drawn as the row's background.
typedef enum {
    FSEMU_STRIPE_CATEGORY_NONE = 0,
    // Work: the core up to the line's time, extra CPU time, and other emulation thread work (the
    // frame's start and end, messages, posting slices).
    FSEMU_STRIPE_CATEGORY_EMULATING,
    FSEMU_STRIPE_CATEGORY_EXTRA_CPU,
    FSEMU_STRIPE_CATEGORY_OTHER_WORK,
    // Waiting: a line wait (line sync) sleeping up to the planned wake time or busy-waiting,
    // waiting before the slot (the start gate, the start wait), and the frame's end wait (until its
    // planned end, sleeping or busy-waiting).
    FSEMU_STRIPE_CATEGORY_SLEEPING,
    FSEMU_STRIPE_CATEGORY_BUSY_WAITING,
    FSEMU_STRIPE_CATEGORY_BEFORE_SLOT,
    FSEMU_STRIPE_CATEGORY_END_WAIT,
    // Late: a sleep past its planned wake time, and an extra CPU block running past its line's
    // planned time.
    FSEMU_STRIPE_CATEGORY_OVERSLEEP,
    FSEMU_STRIPE_CATEGORY_EXTRA_CPU_LATE,
    FSEMU_STRIPE_CATEGORY_COUNT
} fsemu_stripe_category_t;

// A span: the emulation thread's time from start_ns to end_ns (on SDL's clock, in nanoseconds) was
// spent as category.
typedef struct fsemu_stripe_span_t {
    int64_t start_ns;
    int64_t end_ns;
    fsemu_stripe_category_t category;
} fsemu_stripe_span_t;

// What the recorder recorded of a frame by its end: emulation done (when the frame's last line was
// emulated, before any end wait; 0 when not known), and the spans in time order, not overlapping
// (borrowed: valid as long as the buffer they were read into).
typedef struct fsemu_stripe_frame_t {
    int64_t emulation_done_ns;
    const fsemu_stripe_span_t *spans;
    int span_count;
} fsemu_stripe_frame_t;

// The most spans a frame's record holds; later changes of category in a full record are left out
// (its last category runs on to the frame's end). Lines with line waits or extra CPU time take two
// or three each.
#define FSEMU_STRIPE_MAX_SPANS 2048

// A time argument that stands for the current time: the recorder reads the clock itself, and only
// while recording.
#define FSEMU_STRIPE_NOW INT64_MIN

// ----------------------------------------------------------------------------
// The emulation thread's side.

// Whether the current frame is recorded: the performance graph was on when it started. Read by the
// hooks before anything else, so that they cost one load and branch with the graph off.
extern bool fsemu_stripe_recording;

static inline bool fsemu_stripe_is_recording(void)
{
    return fsemu_stripe_recording;
}

// At the start of emulated frame frame_number (counted up by one per frame), before the start gate:
// whether the frame is recorded (the performance graph is on), and opens its record if the end of
// the frame before did not.
void fsemu_stripe_begin_frame(int frame_number, bool record);

// From time_ns on (not before the last change), the thread spends its time as category. Only while
// recording: the hooks use fsemu_stripe_set, fsemu_stripe_push and fsemu_stripe_pop below.
void fsemu_stripe_set_category(fsemu_stripe_category_t category, int64_t time_ns);

// The category the thread spends its time as now.
fsemu_stripe_category_t fsemu_stripe_get_category(void);

// From time_ns (or FSEMU_STRIPE_NOW) on, the thread spends its time as category.
static inline void fsemu_stripe_set(fsemu_stripe_category_t category, int64_t time_ns)
{
    if (fsemu_stripe_recording) {
        fsemu_stripe_set_category(category, time_ns);
    }
}

// For something the thread does in between (a wait, posting a slice): from time_ns (or
// FSEMU_STRIPE_NOW) on, it spends its time as category, until fsemu_stripe_pop returns to the
// category it was spending its time as before, which this returns.
static inline fsemu_stripe_category_t fsemu_stripe_push(fsemu_stripe_category_t category,
                                                        int64_t time_ns)
{
    if (!fsemu_stripe_recording) {
        return FSEMU_STRIPE_CATEGORY_NONE;
    }
    fsemu_stripe_category_t before = fsemu_stripe_get_category();
    fsemu_stripe_set_category(category, time_ns);
    return before;
}

static inline void fsemu_stripe_pop(fsemu_stripe_category_t before, int64_t time_ns)
{
    fsemu_stripe_set(before, time_ns);
}

// The frame's last line was emulated at time_ns (or FSEMU_STRIPE_NOW; only the first call in a
// frame counts): from then on, the frame's end is other work.
void fsemu_stripe_on_emulation_done(int64_t time_ns);

// The end of the frame's end wait (at time_ns, or FSEMU_STRIPE_NOW): publishes its record, and
// opens the next frame's record from then on.
void fsemu_stripe_end_frame(int64_t time_ns);

// ----------------------------------------------------------------------------
// The main thread's side (any thread but the emulation thread).

// The newest frame whose record has been published, -1 before the first.
int fsemu_stripe_get_ended_frame(void);

// Copies the published record of frame frame_number into frame, with its spans in spans (room for
// max_spans; FSEMU_STRIPE_MAX_SPANS holds any). False when there is none: the frame was not
// recorded, has not ended, or its record has been reused for later frames (the recorder keeps the
// newest frames' records, at least as many as the frame info does, unless they are long).
bool fsemu_stripe_read(int frame_number, fsemu_stripe_span_t *spans, int max_spans,
                       fsemu_stripe_frame_t *frame);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_STRIPE_H */
