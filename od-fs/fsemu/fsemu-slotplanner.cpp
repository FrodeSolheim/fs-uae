/*
 * The slot planner (see fsemu-slotplanner.h).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-slotplanner.h"

#include <math.h>
#include <stddef.h>

// Whether the predicted host vertical blank has the observations and the interval to plan from on
// its own.
static bool fsemu_slotplanner_has_observations(const fsemu_slotplanner_prediction_t *prediction)
{
    return prediction->observations >= FSEMU_SLOTPLANNER_MIN_OBSERVATIONS &&
           prediction->interval_ns > 0.0;
}

// Whether slots can be planned from the prediction, whatever its interval.
static bool fsemu_slotplanner_plannable(const fsemu_slotplanner_prediction_t *prediction)
{
    return prediction->interval_kept || fsemu_slotplanner_has_observations(prediction);
}

bool fsemu_slotplanner_can_plan(const fsemu_slotplanner_prediction_t *prediction,
                                double frame_interval_ns)
{
    return fsemu_slotplanner_plannable(prediction) &&
           fabs(prediction->interval_ns - frame_interval_ns) <=
               FSEMU_SLOTPLANNER_INTERVAL_TOLERANCE * frame_interval_ns;
}

void fsemu_slotplanner_follow_prediction(fsemu_slotplanner_prediction_t *planning,
                                         const fsemu_slotplanner_prediction_t *predicted)
{
    if (predicted == NULL || predicted->observations <= 0) {
        planning->predicted_ns = 0.0;
        planning->interval_ns = 0.0;
        planning->lead_ns = 0;
        planning->observations = 0;
        planning->interval_kept = false;
        return;
    }
    // A restart of the predicted host vertical blank after slots could be planned from it. Its
    // interval from fewer observations can be off (a late observation among the first few).
    bool keep =
        fsemu_slotplanner_plannable(planning) && !fsemu_slotplanner_has_observations(predicted);
    if (!keep || predicted->predicted_ns != 0.0) {
        planning->predicted_ns = predicted->predicted_ns;
    }
    if (!keep) {
        planning->interval_ns = predicted->interval_ns;
        planning->lead_ns = predicted->lead_ns;
    }
    planning->observations = predicted->observations;
    planning->interval_kept = keep;
}

void fsemu_slotplanner_init(fsemu_slotplanner_t *planner)
{
    planner->whole_frame_slots = false;
    fsemu_slotplanner_reset(planner);
}

void fsemu_slotplanner_reset(fsemu_slotplanner_t *planner)
{
    planner->deadline_ns = 0;
    planner->vertical_blank_ns = 0;
    planner->planned_end_ns = 0;
    planner->late_slots = 0;
    planner->emulation_done_ns = 0;
}

// The predicted host vertical blank a number of host refreshes from the prediction's.
static int64_t fsemu_slotplanner_predicted_ns(const fsemu_slotplanner_prediction_t *prediction,
                                              double refreshes)
{
    return llround(prediction->predicted_ns + refreshes * prediction->interval_ns);
}

// The number of host refreshes from the prediction's host vertical blank to the first one at or
// after at_or_after_ns.
static double
fsemu_slotplanner_refreshes_at_or_after(const fsemu_slotplanner_prediction_t *prediction,
                                        double at_or_after_ns)
{
    double refreshes = ceil((at_or_after_ns - prediction->predicted_ns) / prediction->interval_ns);
    // Rounding can leave it a little before at_or_after_ns.
    while ((double)fsemu_slotplanner_predicted_ns(prediction, refreshes) < at_or_after_ns) {
        refreshes += 1.0;
    }
    return refreshes;
}

// The largest slot margin for the interval: one that leaves a slot of half a host refresh after the
// event pump allowance.
static int64_t fsemu_slotplanner_margin_cap_ns(double interval_ns)
{
    return (int64_t)floor(interval_ns / 2.0) - FSEMU_SLOTPLANNER_EVENT_PUMP_ALLOWANCE_NS;
}

// The fixed slot margin of whole-frame slots for the interval.
static int64_t fsemu_slotplanner_whole_frame_margin_ns(double interval_ns)
{
    return (int64_t)floor(FSEMU_SLOTPLANNER_WHOLE_FRAME_MARGIN * interval_ns);
}

// The number of host refreshes from the prediction's host vertical blank to the one the slot after
// a slot for vertical_blank_ns is for: the first predicted host vertical blank at least half a
// refresh after it.
static double fsemu_slotplanner_next_refreshes(const fsemu_slotplanner_prediction_t *prediction,
                                               int64_t vertical_blank_ns)
{
    return fsemu_slotplanner_refreshes_at_or_after(prediction, (double)vertical_blank_ns +
                                                                   prediction->interval_ns / 2.0);
}

// The planned start of a slot that has no previous one to follow, from its origin: the event pump
// allowance after it, or for a whole-frame slot, a host refresh and the slot margin before the
// swap deadline (the lead and the slot margin before the origin).
static int64_t
fsemu_slotplanner_first_planned_start_ns(const fsemu_slotplanner_t *planner,
                                         const fsemu_slotplanner_prediction_t *prediction,
                                         int64_t origin_ns, int64_t margin_ns)
{
    if (planner->whole_frame_slots) {
        return origin_ns - prediction->lead_ns - margin_ns;
    }
    return origin_ns + FSEMU_SLOTPLANNER_EVENT_PUMP_ALLOWANCE_NS;
}

// Places the slot for the predicted host vertical blank a number of host refreshes from the
// prediction's: its swap deadline the lead before it. A whole-frame slot starts at the previous
// slot's end, unless it is the first.
static void fsemu_slotplanner_place(const fsemu_slotplanner_t *planner,
                                    const fsemu_slotplanner_prediction_t *prediction,
                                    double refreshes, int64_t margin_ns, bool first,
                                    fsemu_slotplanner_slot_t *slot, int64_t *vertical_blank_ns)
{
    *vertical_blank_ns = fsemu_slotplanner_predicted_ns(prediction, refreshes);
    slot->deadline_ns = *vertical_blank_ns - prediction->lead_ns;
    slot->origin_ns = fsemu_slotplanner_predicted_ns(prediction, refreshes - 1.0);
    if (planner->whole_frame_slots && !first) {
        slot->planned_start_ns = planner->planned_end_ns;
    } else {
        slot->planned_start_ns = fsemu_slotplanner_first_planned_start_ns(
            planner, prediction, slot->origin_ns, margin_ns);
    }
    slot->planned_end_ns = slot->deadline_ns - margin_ns;
    slot->margin_ns = margin_ns;
}

// Places the first slot (at a session start, or after a timeline reset): the one for the first
// predicted host vertical blank whose planned start is not yet past.
static void fsemu_slotplanner_place_first(const fsemu_slotplanner_t *planner,
                                          const fsemu_slotplanner_prediction_t *prediction,
                                          int64_t margin_ns, int64_t now_ns,
                                          fsemu_slotplanner_slot_t *slot,
                                          int64_t *vertical_blank_ns)
{
    // The first origin (a host refresh before the host vertical blank the slot is for) whose
    // planned start is at or after now_ns.
    int64_t offset_ns = fsemu_slotplanner_first_planned_start_ns(planner, prediction, 0, margin_ns);
    double refreshes =
        fsemu_slotplanner_refreshes_at_or_after(prediction, (double)(now_ns - offset_ns));
    fsemu_slotplanner_place(planner, prediction, refreshes + 1.0, margin_ns, true, slot,
                            vertical_blank_ns);
}

void fsemu_slotplanner_set_whole_frame_slots(fsemu_slotplanner_t *planner, bool whole_frame_slots)
{
    planner->whole_frame_slots = whole_frame_slots;
}

void fsemu_slotplanner_on_emulation_done(fsemu_slotplanner_t *planner, int64_t emulation_done_ns)
{
    planner->emulation_done_ns = emulation_done_ns;
}

// Whether the slot just placed (after the last planned one, which was planned to end at
// last_end_ns) makes a late slot: it starts a tenth of a host refresh or more after its planned
// start, or for whole-frame slots, the last one's emulation was done as late after its planned end.
static bool fsemu_slotplanner_is_late(const fsemu_slotplanner_t *planner,
                                      const fsemu_slotplanner_slot_t *slot, int64_t last_end_ns,
                                      double interval_ns, int64_t now_ns)
{
    if (planner->whole_frame_slots) {
        return planner->emulation_done_ns != 0 &&
               (double)(planner->emulation_done_ns - last_end_ns) >= interval_ns / 10.0;
    }
    return (double)(now_ns - slot->planned_start_ns) >= interval_ns / 10.0;
}

void fsemu_slotplanner_plan_slot(fsemu_slotplanner_t *planner,
                                 const fsemu_slotplanner_prediction_t *prediction,
                                 int64_t margin_ns, int64_t now_ns, fsemu_slotplanner_slot_t *slot)
{
    double interval_ns = prediction->interval_ns;
    if (planner->whole_frame_slots) {
        margin_ns = fsemu_slotplanner_whole_frame_margin_ns(interval_ns);
    }
    int64_t cap_ns = fsemu_slotplanner_margin_cap_ns(interval_ns);
    if (margin_ns > cap_ns) {
        margin_ns = cap_ns;
    }
    slot->timeline_reset = false;
    int64_t vertical_blank_ns = 0;
    if (planner->deadline_ns == 0) {
        fsemu_slotplanner_place_first(planner, prediction, margin_ns, now_ns, slot,
                                      &vertical_blank_ns);
    } else {
        double refreshes = fsemu_slotplanner_next_refreshes(prediction, planner->vertical_blank_ns);
        int64_t last_end_ns = planner->planned_end_ns;
        fsemu_slotplanner_place(planner, prediction, refreshes, margin_ns, false, slot,
                                &vertical_blank_ns);
        bool reset = false;
        if (now_ns > slot->planned_end_ns) {
            // A stall: the slot would start after its own planned end.
            reset = true;
        } else if (fsemu_slotplanner_is_late(planner, slot, last_end_ns, interval_ns, now_ns)) {
            planner->late_slots++;
            reset = planner->late_slots >= FSEMU_SLOTPLANNER_RESET_LATE_SLOTS;
        } else {
            planner->late_slots = 0;
        }
        if (reset) {
            fsemu_slotplanner_place_first(planner, prediction, margin_ns, now_ns, slot,
                                          &vertical_blank_ns);
            slot->timeline_reset = true;
            planner->late_slots = 0;
        }
    }
    planner->deadline_ns = slot->deadline_ns;
    planner->vertical_blank_ns = vertical_blank_ns;
    planner->planned_end_ns = slot->planned_end_ns;
    planner->emulation_done_ns = 0;
}

void fsemu_slotplanner_margin_init(fsemu_slotplanner_margin_t *margin)
{
    margin->margin_ns = FSEMU_SLOTPLANNER_STARTING_MARGIN_NS;
    margin->count = 0;
    margin->next = 0;
    margin->whole_frame_slots = false;
}

void fsemu_slotplanner_margin_set_whole_frame_slots(fsemu_slotplanner_margin_t *margin,
                                                    bool whole_frame_slots)
{
    margin->whole_frame_slots = whole_frame_slots;
}

// The largest of the times in a ring buffer of the last swapped frames, plus the jitter allowance.
static int64_t fsemu_slotplanner_largest_ns(const fsemu_slotplanner_margin_t *margin,
                                            const int64_t *times_ns)
{
    int64_t largest_ns = 0;
    for (int i = 0; i < margin->count; i++) {
        if (times_ns[i] > largest_ns) {
            largest_ns = times_ns[i];
        }
    }
    return largest_ns + FSEMU_SLOTPLANNER_JITTER_ALLOWANCE_NS;
}

void fsemu_slotplanner_margin_on_swapped(fsemu_slotplanner_margin_t *margin,
                                         const fsemu_slotplanner_prediction_t *prediction,
                                         const fsemu_slotplanner_frame_t *frame)
{
    // The start gate's frames (at a session start) were not planned against a deadline, and their
    // times include start-up work: kept, they would set the needed margin and the drawing time for
    // the history's length. Without an interval (the prediction started afresh), a frame planned
    // before cannot be judged.
    if (frame->deadline_ns == 0 || prediction->interval_ns <= 0.0) {
        return;
    }
    margin->done_to_finished_ns[margin->next] =
        frame->rendering_finished_ns - frame->emulation_done_ns;
    margin->drawing_ns[margin->next] = frame->rendering_finished_ns - frame->frame_ready_ns;
    margin->next = (margin->next + 1) % FSEMU_SLOTPLANNER_HISTORY;
    if (margin->count < FSEMU_SLOTPLANNER_HISTORY) {
        margin->count++;
    }
    if (margin->whole_frame_slots) {
        return;
    }
    double interval_ns = prediction->interval_ns;
    int64_t needed_ns = fsemu_slotplanner_largest_ns(margin, margin->done_to_finished_ns);
    bool missed =
        frame->held || (double)(frame->vertical_blank_ns - frame->deadline_ns) > interval_ns / 2.0;
    bool near_miss =
        frame->deadline_ns - frame->rendering_finished_ns < FSEMU_SLOTPLANNER_NEAR_MISS_NS;
    int64_t margin_ns = margin->margin_ns;
    if (missed || near_miss) {
        // REVIEW: A miss sets the margin at once to the larger of the starting margin and the
        // needed margin, but a margin already above both (still stepping down from a slow frame)
        // is kept rather than lowered on a miss.
        if (margin_ns < FSEMU_SLOTPLANNER_STARTING_MARGIN_NS) {
            margin_ns = FSEMU_SLOTPLANNER_STARTING_MARGIN_NS;
        }
        if (margin_ns < needed_ns) {
            margin_ns = needed_ns;
        }
    } else if (needed_ns < margin_ns) {
        margin_ns -= FSEMU_SLOTPLANNER_MARGIN_DOWN_STEP_NS;
        if (margin_ns < needed_ns) {
            margin_ns = needed_ns;
        }
        if (margin_ns < FSEMU_SLOTPLANNER_MARGIN_FLOOR_NS) {
            margin_ns = FSEMU_SLOTPLANNER_MARGIN_FLOOR_NS;
        }
    }
    int64_t cap_ns = fsemu_slotplanner_margin_cap_ns(interval_ns);
    if (margin_ns > cap_ns) {
        margin_ns = cap_ns;
    }
    margin->margin_ns = margin_ns;
}

int64_t fsemu_slotplanner_margin_get_slot_margin_ns(const fsemu_slotplanner_margin_t *margin)
{
    return margin->margin_ns;
}

int64_t fsemu_slotplanner_margin_get_hold_until_ns(const fsemu_slotplanner_margin_t *margin,
                                                   const fsemu_slotplanner_prediction_t *prediction,
                                                   int64_t held_deadline_ns)
{
    if (prediction->interval_ns <= 0.0) {
        // Nothing to plan from (the prediction started afresh): not held.
        return held_deadline_ns;
    }
    int64_t next_deadline_ns =
        fsemu_slotplanner_predicted_ns(
            prediction,
            fsemu_slotplanner_next_refreshes(prediction, held_deadline_ns + prediction->lead_ns)) -
        prediction->lead_ns;
    int64_t until_ns = next_deadline_ns - fsemu_slotplanner_largest_ns(margin, margin->drawing_ns);
    // REVIEW: Holding only until the next deadline minus the drawing time is not enough. The
    // margin and the drawing time are both a largest time of the history plus the jitter allowance,
    // so the next frame, planned with the margin, is ready about when the drawing time allows, and
    // after it when the margin is capped (a drawing time above half a refresh) or was raised after
    // the next frame was planned. The held frame would then be swapped, the main thread would take
    // the next frame in only after that swap, after its deadline, and every frame would be held and
    // swapped a refresh late for as long as the slow frame is in the history.
    int64_t margin_ns = margin->whole_frame_slots
                            ? fsemu_slotplanner_whole_frame_margin_ns(prediction->interval_ns)
                            : margin->margin_ns;
    int64_t expected_ns = next_deadline_ns - margin_ns + FSEMU_SLOTPLANNER_JITTER_ALLOWANCE_NS;
    return until_ns > expected_ns ? until_ns : expected_ns;
}
