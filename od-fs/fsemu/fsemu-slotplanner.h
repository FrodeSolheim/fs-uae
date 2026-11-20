/*
 * The slot planner: plans each emulated frame's emulation slot while the emulation follows the
 * host vertical blank, from the predicted host vertical blank (fsemu-verticalblank.h).
 *
 * Each emulated frame is meant for a predicted host vertical blank, one host refresh after the
 * previous frame's; its swap deadline is the compositor's lead before it (the latest swap for that
 * host vertical blank), and its origin is the predicted host vertical blank one refresh before it.
 * Its slot starts at its planned start, the event pump allowance after the
 * origin, so that the main thread has returned from the swap and pumped events before the frame's
 * first lines run, and ends the slot margin before the deadline. Whole-frame slots (fsemu's
 * default, so that slow hosts keep up) start at the previous slot's end instead, with a fixed slot
 * margin. Otherwise the slot margin adapts to what the host needs, measured from the swapped
 * frames: up at once after a missed or nearly missed deadline, down a small step per frame to a
 * floor. A late slot start catches up (the scanline pacer runs its lines without waiting until they
 * are back on their planned times), and the main thread holds a frame that became ready after its
 * deadline until the next frame's hold time; when catching up fails for several slots in a row
 * (they start late, or whole-frame slots, which always start after the emulation thread's work
 * between frames, end late), or the emulation stalls, the timeline is reset.
 *
 * The planning (fsemu_slotplanner_t) is done by the emulation thread at each slot start, and the
 * slot margin (fsemu_slotplanner_margin_t) is measured by the main thread from the swapped frames.
 * Neither has a clock, SDL or global state: times are passed in, so that the rules can be tested
 * alone (od-fs/tests/fsemu).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_SLOTPLANNER_H
#define FSEMU_SLOTPLANNER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// How many observations the predicted host vertical blank must have at least (its interval
// measured from the slopes) before slots are first planned from it. Until then, the start gate
// places them.
#define FSEMU_SLOTPLANNER_MIN_OBSERVATIONS 8

// The time a slot waits after the predicted host vertical blank, so that the main thread has
// returned from the swap and pumped events before the frame's first lines run.
#define FSEMU_SLOTPLANNER_EVENT_PUMP_ALLOWANCE_NS 1000000

// The slot margin of whole-frame slots, a share of the host refresh interval: fixed, so that the
// emulation keeps one phase against the latest swap (the swap deadline), from this share of a
// refresh before the swap deadline a refresh before to as long before the swap deadline (8 ms at
// 50 Hz: from 8 ms before the host vertical blank to 12 ms after it, the lead earlier). The start
// gate's whole-frame slots have the same phase.
#define FSEMU_SLOTPLANNER_WHOLE_FRAME_MARGIN 0.4

// The adaptive slot margin (slots after the host vertical blank) at a session start, its floor,
// and how much it goes down per swapped frame while the needed margin is below it.
#define FSEMU_SLOTPLANNER_STARTING_MARGIN_NS 5000000
#define FSEMU_SLOTPLANNER_MARGIN_FLOOR_NS 2000000
#define FSEMU_SLOTPLANNER_MARGIN_DOWN_STEP_NS 10000

// A frame whose rendering finished less than this before its swap deadline nearly missed it.
#define FSEMU_SLOTPLANNER_NEAR_MISS_NS 500000

// The allowance for jitter added to the largest measured time, for the needed margin and the
// drawing time.
#define FSEMU_SLOTPLANNER_JITTER_ALLOWANCE_NS 1000000

// How many of the last swapped frames with a swap deadline the needed margin and the drawing time
// are measured over.
#define FSEMU_SLOTPLANNER_HISTORY 64

// How many late slots in a row reset the timeline: slots that started a tenth of a host refresh or
// more after their planned start, or whole-frame slots whose emulation was done as late after their
// planned end (a whole-frame slot starts where the last one was planned to end, so always after the
// emulation thread's work between frames, and catches up).
#define FSEMU_SLOTPLANNER_RESET_LATE_SLOTS 10

// The predicted host vertical blank, as the main thread publishes it (a consistent copy of
// fsemu_verticalblank_t's prediction). All times are on SDL's clock, in nanoseconds.
typedef struct fsemu_slotplanner_prediction_t {
    // A predicted host vertical blank (any one: the others are whole intervals from it).
    double predicted_ns;
    // The host refresh interval, 0 when there is none yet.
    double interval_ns;
    // The compositor's lead: how long before a predicted host vertical blank the latest swap for it
    // is (fsemu_verticalblank_get_lead_ns). The swap deadlines are this much before the predicted
    // host vertical blanks.
    int64_t lead_ns;
    // The observations the prediction has taken since it was started.
    int observations;
    // Whether the interval is the one from before the predicted host vertical blank restarted
    // (fsemu_slotplanner_follow_prediction): slots are planned from it although it has fewer than
    // FSEMU_SLOTPLANNER_MIN_OBSERVATIONS observations.
    bool interval_kept;
} fsemu_slotplanner_prediction_t;

// How far (a share of the emulated frame period) the prediction's interval may be from the emulated
// frame period for slots to be planned from it. The emulation follows only a host refresh rate near
// its own, so an interval further off was measured from swaps that came every one or two refreshes
// (as at a session start on a busy host); planned from, it would pace the emulation at that rate.
#define FSEMU_SLOTPLANNER_INTERVAL_TOLERANCE 0.1

// Whether slots can be planned from the prediction for an emulated frame period of
// frame_interval_ns: it has FSEMU_SLOTPLANNER_MIN_OBSERVATIONS observations and an interval, or it
// kept its interval through a restart, and the interval is within
// FSEMU_SLOTPLANNER_INTERVAL_TOLERANCE of the frame period.
bool fsemu_slotplanner_can_plan(const fsemu_slotplanner_prediction_t *prediction,
                                double frame_interval_ns);

// Takes the predicted host vertical blank after it was updated (predicted, a copy of
// fsemu_verticalblank_t's prediction) into the prediction the slots are planned from (planning),
// or starts that afresh when predicted is NULL or has no observations (a session start, resuming
// from pause). Once slots can be planned from it, they can be until it starts afresh: when the
// predicted host vertical blank restarts (on an early observation or a changed interval) and has
// fewer than FSEMU_SLOTPLANNER_MIN_OBSERVATIONS observations again, planning takes its predicted
// host vertical blank but keeps the interval and the lead from before (and the predicted host
// vertical blank too when the restarted one has none), so that the swap deadlines snap to it with
// no timeline reset and the start gate does not take over again.
void fsemu_slotplanner_follow_prediction(fsemu_slotplanner_prediction_t *planning,
                                         const fsemu_slotplanner_prediction_t *predicted);

// ----------------------------------------------------------------------------
// The planning, on the emulation thread
// ----------------------------------------------------------------------------

// A planned emulation slot.
typedef struct fsemu_slotplanner_slot_t {
    // The origin of the slot's emulated frame: the predicted host vertical blank one host refresh
    // before the one the frame is meant for, which the frame's other times are measured from (the
    // swap deadline is the lead less than a refresh after it).
    int64_t origin_ns;
    // The planned start of the slot.
    int64_t planned_start_ns;
    // The planned end of the slot, the slot margin before the swap deadline. The scanline pacer
    // spreads the frame's lines evenly from the planned start to here.
    int64_t planned_end_ns;
    // The swap deadline: the predicted host vertical blank the emulated frame is meant for, minus
    // the lead.
    int64_t deadline_ns;
    // The slot margin the slot was planned with (capped, see fsemu_slotplanner_plan_slot).
    int64_t margin_ns;
    // Whether the timeline was reset to plan this slot (not at a session start or on resuming).
    bool timeline_reset;
} fsemu_slotplanner_slot_t;

// The planning of the slots.
typedef struct fsemu_slotplanner_t {
    // The swap deadline of the last planned slot, 0 before the first one, and the predicted host
    // vertical blank it was meant for.
    int64_t deadline_ns;
    int64_t vertical_blank_ns;
    // The planned end of the last planned slot.
    int64_t planned_end_ns;
    // How many slots in a row were late (FSEMU_SLOTPLANNER_RESET_LATE_SLOTS).
    int late_slots;
    // When the last planned slot's emulation was done (fsemu_slotplanner_on_emulation_done), 0 when
    // not known.
    int64_t emulation_done_ns;
    // Whether the slots are whole-frame slots.
    bool whole_frame_slots;
} fsemu_slotplanner_t;

// Sets up a planner with no slot planned yet, without whole-frame slots.
void fsemu_slotplanner_init(fsemu_slotplanner_t *planner);

// Starts the planning afresh (a session start, resuming from pause): the next slot is the first
// whose planned start is not yet past when it is planned. Not a timeline reset.
void fsemu_slotplanner_reset(fsemu_slotplanner_t *planner);

// Sets whether the slots are whole-frame slots (fsemu's default, FSEMU_WHOLE_FRAME_SLOTS=0 for
// not): each starts at the previous slot's end, with no gap, instead of the event pump allowance
// after its origin; the first is a whole host refresh long. Their margin is
// FSEMU_SLOTPLANNER_WHOLE_FRAME_MARGIN of a host refresh, whatever margin
// fsemu_slotplanner_plan_slot is given.
void fsemu_slotplanner_set_whole_frame_slots(fsemu_slotplanner_t *planner, bool whole_frame_slots);

// The last planned slot's emulation was done at emulation_done_ns (its last line was emulated):
// whether a whole-frame slot was late is judged by it, when the next slot is planned.
void fsemu_slotplanner_on_emulation_done(fsemu_slotplanner_t *planner, int64_t emulation_done_ns);

// Plans the next slot when the emulation thread is ready to start it (now_ns: after the last slot's
// emulation done, before waiting for the planned start), with the current prediction and slot
// margin. The margin is capped so that the slot is never shorter than half a host refresh.
//
// The slot is for the first predicted host vertical blank at least half a host refresh after the
// last slot's, so that the deadlines snap to a prediction that moved; its swap deadline is that
// minus the lead (a lead that goes up makes that slot shorter by as much). The first slot (and the
// one after a timeline reset) is the first whose planned start is not yet past. The timeline is
// reset when the slot would start after its own planned end (a stall), or when it is the
// FSEMU_SLOTPLANNER_RESET_LATE_SLOTS-th late slot in a row (this slot started late, or for
// whole-frame slots, the last one's emulation was done late): that slot is then planned as a first
// slot, and says so.
void fsemu_slotplanner_plan_slot(fsemu_slotplanner_t *planner,
                                 const fsemu_slotplanner_prediction_t *prediction,
                                 int64_t margin_ns, int64_t now_ns, fsemu_slotplanner_slot_t *slot);

// ----------------------------------------------------------------------------
// The slot margin, on the main thread
// ----------------------------------------------------------------------------

// What the slot margin is measured from: a swapped emulated frame's points in time (from its frame
// info and frame record).
typedef struct fsemu_slotplanner_frame_t {
    // The frame's swap deadline, 0 when it had none (the start gate's frames).
    int64_t deadline_ns;
    // When the emulation thread finished the frame's last line.
    int64_t emulation_done_ns;
    // When the frame was ready (its last slice posted).
    int64_t frame_ready_ns;
    // When the main thread had finished rendering it, before the swap.
    int64_t rendering_finished_ns;
    // The host vertical blank it was presented at (observed or estimated).
    int64_t vertical_blank_ns;
    // Whether the main thread held it, as it became ready after its swap deadline.
    bool held;
} fsemu_slotplanner_frame_t;

// The slot margin and the times it is measured from.
typedef struct fsemu_slotplanner_margin_t {
    // The slot margin to plan the next slot with.
    int64_t margin_ns;
    // The measured times of the last swapped frames that had a swap deadline (ring buffers of
    // `count` of them, the next one written at `next`): from emulation done to rendering finished,
    // and from frame ready to rendering finished (the drawing time).
    int count;
    int next;
    int64_t done_to_finished_ns[FSEMU_SLOTPLANNER_HISTORY];
    int64_t drawing_ns[FSEMU_SLOTPLANNER_HISTORY];
    // Whether the slots are whole-frame slots, whose margin is fixed.
    bool whole_frame_slots;
} fsemu_slotplanner_margin_t;

// Sets up a slot margin with the starting margin and no measurements (a session start), for slots
// after the host vertical blank.
void fsemu_slotplanner_margin_init(fsemu_slotplanner_margin_t *margin);

// Sets whether the slots are whole-frame slots: their margin is fixed (the swapped frames are still
// measured, for the hold time), and the hold time expects the next frame by it.
void fsemu_slotplanner_margin_set_whole_frame_slots(fsemu_slotplanner_margin_t *margin,
                                                    bool whole_frame_slots);

// Takes in a swapped frame's measurements and updates the slot margin: up at once to the starting
// margin or the needed margin (the largest measured time from emulation done to rendering finished
// of the last FSEMU_SLOTPLANNER_HISTORY swapped frames, plus the jitter allowance), whichever is
// larger, when the frame missed its swap deadline (presented more than half a refresh after it, or
// held) or nearly missed it; otherwise down a step towards the needed margin, never below the
// floor. Capped so that a slot is never shorter than half a host refresh. Not changed for
// whole-frame slots. A frame with no swap
// deadline (the start gate's) is left out: it is not measured and does not change the margin.
void fsemu_slotplanner_margin_on_swapped(fsemu_slotplanner_margin_t *margin,
                                         const fsemu_slotplanner_prediction_t *prediction,
                                         const fsemu_slotplanner_frame_t *frame);

// The slot margin to plan the next slot with.
int64_t fsemu_slotplanner_margin_get_slot_margin_ns(const fsemu_slotplanner_margin_t *margin);

// How long the main thread holds a frame that became ready after its swap deadline
// (held_deadline_ns): until the next frame's swap deadline (as fsemu_slotplanner_plan_slot plans
// it) minus the drawing time, the largest time from frame ready to rendering finished of the last
// FSEMU_SLOTPLANNER_HISTORY swapped frames with a swap deadline, plus the jitter allowance; or,
// when that is earlier, until the jitter allowance after the next frame is expected (its slot end
// with the slot margin), so that a next frame on time overtakes the held one. The next frame's
// swap deadline is the lead before the first predicted host vertical blank at least half a refresh
// after the held frame's (its deadline plus the lead). Without an interval
// (the prediction started afresh), until held_deadline_ns: the frame is not held.
int64_t fsemu_slotplanner_margin_get_hold_until_ns(const fsemu_slotplanner_margin_t *margin,
                                                   const fsemu_slotplanner_prediction_t *prediction,
                                                   int64_t held_deadline_ns);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_SLOTPLANNER_H */
