/*
 * The predicted host vertical blank: the host vertical blanks as FS-UAE expects them, a steady
 * sequence worked out from the swap returns of the recent swaps (the frame records' host vertical
 * blanks) and, where the windowing system reports them, the presentations. While the emulation
 * follows the host vertical blank, the emulated frames' origins are on it (the published line),
 * and the swap deadlines are the compositor's lead before it, so it must not jump.
 *
 * Two lines are fitted, each to the last few seconds of its observations, with the host refresh
 * interval as its slope: one to the swap returns and one to the presentations. A rough line goes
 * through the first quartile of the swap returns (the median of the presentations, which have no
 * late cluster), with the interval from the slopes between them; the line is then fitted (least
 * squares) to the observations near it, leaving out the others, a few times over. A rough interval
 * alone is not enough: it changes by a few microseconds as observations come and go, which moves a
 * line through observations hundreds of refreshes back by hundreds of microseconds.
 *
 * Most swaps return at their host vertical blank (or, under a compositor, at the start of the
 * repaint that takes them in), but not all: on macOS, about half the swaps were seen to return
 * 1.75 ms late after a while without input, and with the render thread, about one in twelve 0.5 to
 * 1 ms earlier than the rest. The first quartile follows the swaps that return on time when about
 * half return late, and leaves out a few early ones. A real shift of the swap returns (a compositor
 * that moves its repaint) is a run of them off the line on one side: 12 of the last 16 more than
 * FSEMU_VERTICALBLANK_FIT_NS early, or 30 of the last 32 as late, rebase the fit on that run, which
 * scattered early or late returns do not make.
 *
 * The published line is the presentations' line while they are fresh (the real host vertical
 * blanks), else the swap returns' line, shifted by the last difference between the two (0 where
 * there never were presentations: Windows, macOS, X11 without Present). It glides towards that
 * line by at most FSEMU_VERTICALBLANK_GLIDE_NS per update, so the origins never jump, and snaps
 * to it when it is more than a quarter of a refresh off for FSEMU_VERTICALBLANK_SNAP_UPDATES
 * updates in a row (a mode switch, another output).
 *
 * The compositor's lead is how long before the published line's host vertical blanks the swaps
 * return (the latest swap for that repaint): the swap deadline is the published line minus it. It
 * is measured from the last FSEMU_VERTICALBLANK_LEAD_WINDOW swap returns against the published
 * line (their third quartile) while the presentations are fresh, otherwise it is how far the
 * published line is from the swap returns' line. It goes up at once, so that the swap deadline is
 * never kept later than the latest swap, and down by at most FSEMU_VERTICALBLANK_GLIDE_NS per
 * update. Without presentations it is 0 once the glide has caught up.
 *
 * Kept apart, with no dependency on the performance graph, the GPU or SDL, so that it can be tested
 * alone (od-fs/tests/fsemu).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_VERTICALBLANK_H
#define FSEMU_VERTICALBLANK_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// How many of the recent swap returns their line is fitted to (4.8 s at 50 Hz): the interval is the
// slope of each pair half of them apart, the middle half of those slopes averaged, so that neither
// a late observation nor a few in a row tilt it; and the line goes through their first quartile.
#define FSEMU_VERTICALBLANK_WINDOW 240

// How many of the recent presentations their line is fitted to (about a second): short, so that
// it follows a display whose presentations slide, through their median.
#define FSEMU_VERTICALBLANK_PRESENTATION_WINDOW 48

// How far (ns) from the line an observation may be to be fitted: the swaps that return late (1.75
// ms on macOS) or early (0.5 ms or more) are not, while the jitter of those that return on time (a
// few tens of microseconds) is. Also how far off a line an observation counts for a rebase.
#define FSEMU_VERTICALBLANK_FIT_NS 250000.0

// How many times the line is fitted, each time to the observations near the line before.
#define FSEMU_VERTICALBLANK_FIT_PASSES 3

// How many observations it takes before the interval is measured from the slopes. Until then, it
// is the median of the times between the observations (for the presentations, the refresh the
// windowing system reports, when it does), by which they are numbered again each time.
#define FSEMU_VERTICALBLANK_FIRST 8

// How many observations in a row more than a quarter of a refresh from a line start it afresh (the
// host vertical blanks moved later, or the host refresh interval changed). One more than a quarter
// of a refresh before it is enough (they moved earlier).
#define FSEMU_VERTICALBLANK_OFF_OBSERVATIONS 16

// The rebase of the swap returns' line: when FSEMU_VERTICALBLANK_EARLY_RUN of the last
// FSEMU_VERTICALBLANK_EARLY_OF swap returns were more than FSEMU_VERTICALBLANK_FIT_NS before the
// line (a shift earlier, within 12 frames), or FSEMU_VERTICALBLANK_LATE_RUN of the last
// FSEMU_VERTICALBLANK_LATE_OF as far after it (a shift later, within 30 frames: half the swaps
// late, as on macOS, never make such a run), the fit keeps only those observations, with their
// numbering and the interval, and fits afresh to them. It does not measure the interval from the
// slopes again until it has FSEMU_VERTICALBLANK_REBASE_INTERVAL_OBSERVATIONS (the least squares fit
// still refines it).
#define FSEMU_VERTICALBLANK_EARLY_RUN 12
#define FSEMU_VERTICALBLANK_EARLY_OF 16
#define FSEMU_VERTICALBLANK_LATE_RUN 30
#define FSEMU_VERTICALBLANK_LATE_OF 32
#define FSEMU_VERTICALBLANK_REBASE_INTERVAL_OBSERVATIONS 120

// The rebase of the presentations' line: FSEMU_VERTICALBLANK_PRESENTATION_RUN of the last
// FSEMU_VERTICALBLANK_PRESENTATION_OF presentations more than FSEMU_VERTICALBLANK_FIT_NS off the
// line on one side. After it, the interval is measured from the slopes again once the line has half
// its window of presentations, or at once when it is more than
// FSEMU_VERTICALBLANK_INTERVAL_AGREEMENT off the reported refresh (measured in a disturbance, it
// would keep the line rebasing).
#define FSEMU_VERTICALBLANK_PRESENTATION_RUN 6
#define FSEMU_VERTICALBLANK_PRESENTATION_OF 8

// The presentations are fresh, and their line is the source of the published line, while it has
// FSEMU_VERTICALBLANK_FIRST presentations, the newest less than this many of its refreshes before
// the swap return, and its interval within FSEMU_VERTICALBLANK_INTERVAL_AGREEMENT (a share) of the
// swap returns' line's (else they are of another display than the one the swaps wait for). A
// reported refresh that changes by more than FSEMU_VERTICALBLANK_REFRESH_CHANGE (a share) starts
// the presentations' line afresh.
#define FSEMU_VERTICALBLANK_STALE_REFRESHES 25
#define FSEMU_VERTICALBLANK_INTERVAL_AGREEMENT 0.01
#define FSEMU_VERTICALBLANK_REFRESH_CHANGE 0.01

// The glide: how far (ns) the published line moves towards its source in one update at most (0.5 %
// of a refresh at 50 Hz, under a column of the performance graph), and after how many updates in a
// row more than a quarter of a refresh from it the published line snaps to it.
#define FSEMU_VERTICALBLANK_GLIDE_NS 100000.0
#define FSEMU_VERTICALBLANK_SNAP_UPDATES 4

// How many of the last swap returns the compositor's lead is measured from (their third quartile,
// so that a few early ones do not raise it and a few late ones do not lower it).
#define FSEMU_VERTICALBLANK_LEAD_WINDOW 16

// The prediction is not steady after FSEMU_VERTICALBLANK_UNSTEADY_SNAPS snaps within
// FSEMU_VERTICALBLANK_UNSTEADY_SNAP_UPDATES updates (5 s at 50 Hz), or after
// FSEMU_VERTICALBLANK_UNSTEADY_GLIDE_UPDATES updates in a row of the published line moving by the
// whole glide in one direction: what a display whose presentations follow the swaps (adaptive sync)
// does, and a fixed display does not.
#define FSEMU_VERTICALBLANK_UNSTEADY_SNAPS 3
#define FSEMU_VERTICALBLANK_UNSTEADY_SNAP_UPDATES 250
#define FSEMU_VERTICALBLANK_UNSTEADY_GLIDE_UPDATES 50

// How many observations it takes after a reset (or a restart of the swap returns' line) before the
// prediction says that it has settled.
#define FSEMU_VERTICALBLANK_SETTLE_OBSERVATIONS 32

// How many of a line's recent distances from it are kept, for the rebase's runs.
#define FSEMU_VERTICALBLANK_RUN_HISTORY 32

// A line fitted to observations (the swap returns, or the presentations), numbered by time. Part
// of fsemu_verticalblank_t; not used on its own.
typedef struct fsemu_verticalblank_fit_t {
    // A point of the line (the line's time at the newest observation's host refresh), 0 before the
    // first observation.
    double predicted_ns;
    // The line's slope, the host refresh interval, 0 before the second observation.
    double interval_ns;
    // The observations taken since the line was started afresh.
    int observations;
    // How many observations in a row were more than a quarter of a refresh from the line.
    int off_observations;
    // The recent observations (a ring buffer of `window` of them, the newest at `newest`, `count`
    // of them), and the host refresh each was at, counted from the first one in the buffer.
    int count;
    int newest;
    int64_t observed_ns[FSEMU_VERTICALBLANK_WINDOW];
    int64_t refresh[FSEMU_VERTICALBLANK_WINDOW];
    // How far the recent observations were from the line before each (a ring buffer, the newest at
    // `off_newest`, `off_count` of them), for the rebase.
    int off_count;
    int off_newest;
    double off_ns[FSEMU_VERTICALBLANK_RUN_HISTORY];
    // Whether the interval is kept from before a rebase (not measured from the slopes).
    bool interval_kept;
    // A refresh interval to number the first observations by, instead of the median of the times
    // between them, 0 for none (the presentations' reported refresh).
    double hint_ns;
    // The parameters: the buffer's length, the quantile the rough line goes through (1 / divisor),
    // the rebase's runs (a count of observations more than FSEMU_VERTICALBLANK_FIT_NS early or late
    // of the last so many), and how many observations a rebased line needs before it measures the
    // interval from the slopes again.
    int window;
    int quantile_divisor;
    int early_run;
    int early_of;
    int late_run;
    int late_of;
    int rebase_interval_observations;
    // How many times the line was started afresh, and rebased (for the tests and the trace).
    int restarts;
    int rebases;
} fsemu_verticalblank_fit_t;

// The prediction. All times are on SDL's clock, in nanoseconds.
typedef struct fsemu_verticalblank_t {
    // The published line: the predicted host vertical blank nearest the last swap return, 0 before
    // there is a line (FSEMU_VERTICALBLANK_FIRST swap returns after a reset), and the host refresh
    // interval, 0 before there is a line.
    double predicted_ns;
    double interval_ns;
    // The swap returns taken since the reset, or since their line was last started afresh.
    int observations;
    // The swap returns' line and the presentations' line.
    fsemu_verticalblank_fit_t returns;
    fsemu_verticalblank_fit_t presentations;
    // The newest presentation fed, 0 for none since the reset, and the refresh the windowing system
    // last reported (0 when it never did; kept across a reset, as it is the same display).
    int64_t presented_ns;
    double reported_refresh_ns;
    // Whether the published line was from the presentations at the last update.
    bool from_presentations;
    // The presentations' line minus the swap returns' line, the last time both were there.
    double source_lead_ns;
    // The glide: how many updates in a row the source was more than a quarter of a refresh away,
    // how many in a row moved by the whole glide in one direction (and that direction), the
    // updates and snaps since the reset, and the update number of the last
    // FSEMU_VERTICALBLANK_UNSTEADY_SNAPS snaps (a ring buffer, the next at `snap_next`).
    int far_updates;
    int gliding_updates;
    int glide_sign;
    int updates;
    int snaps;
    int snap_next;
    int snap_updates[FSEMU_VERTICALBLANK_UNSTEADY_SNAPS];
    // The published compositor's lead, the last swap returns it is measured from (a ring buffer,
    // the next at `lead_next`, `lead_count` of them), and the published line at the update before.
    double lead_ns;
    int lead_count;
    int lead_next;
    int64_t lead_returns_ns[FSEMU_VERTICALBLANK_LEAD_WINDOW];
    bool has_previous;
    double previous_predicted_ns;
    double previous_interval_ns;
} fsemu_verticalblank_t;

// Sets up a prediction with no observations: it predicts nothing and has not settled.
void fsemu_verticalblank_init(fsemu_verticalblank_t *vb);

// Starts the prediction afresh (a session start, resuming from pause): it forgets its observations,
// predicts nothing until it has FSEMU_VERTICALBLANK_FIRST swap returns again and has not settled.
void fsemu_verticalblank_reset(fsemu_verticalblank_t *vb);

// Feeds a presentation from a frame record (presented_ns, when it was shown; refresh_ns, the
// refresh the windowing system reported, 0 when it did not): oldest first, only those that were
// shown, and before that record's fsemu_verticalblank_update. One not after the last one is left
// out.
void fsemu_verticalblank_add_presentation(fsemu_verticalblank_t *vb, int64_t presented_ns,
                                          int64_t refresh_ns);

// Feeds a swap return (the host vertical blank of a frame record) and updates the published line
// and the lead. A swap return a whole refresh late (a missed host vertical blank) is taken as the
// next host vertical blank, and does not move the prediction. One that is not at least half a
// refresh after the last one (the same host vertical blank) is left out, and changes nothing. One
// more than a quarter of a refresh before the swap returns' line, or
// FSEMU_VERTICALBLANK_OFF_OBSERVATIONS in a row more than a quarter of a refresh from it, start
// that line afresh from that observation; the published line glides on (and snaps where the new
// line is far from it).
void fsemu_verticalblank_update(fsemu_verticalblank_t *vb, int64_t observed_ns);

// The predicted host vertical blank (on the published line) at or after at_or_after_ns, or 0 when
// there is no prediction.
int64_t fsemu_verticalblank_get_predicted_ns(const fsemu_verticalblank_t *vb,
                                             int64_t at_or_after_ns);

// The compositor's lead: how long before a predicted host vertical blank the latest swap for it is
// (the swap deadline is the predicted host vertical blank minus this). 0 before there is a
// prediction, and where the swaps return at the host vertical blank; negative where they return
// after it (a composited window whose frames are shown a refresh later).
int64_t fsemu_verticalblank_get_lead_ns(const fsemu_verticalblank_t *vb);

// Whether the prediction has settled: FSEMU_VERTICALBLANK_SETTLE_OBSERVATIONS swap returns since
// the reset (or since the swap returns' line last started afresh).
bool fsemu_verticalblank_is_settled(const fsemu_verticalblank_t *vb);

// Whether the prediction is steady: not FSEMU_VERTICALBLANK_UNSTEADY_SNAPS snaps within
// FSEMU_VERTICALBLANK_UNSTEADY_SNAP_UPDATES updates, nor FSEMU_VERTICALBLANK_UNSTEADY_GLIDE_UPDATES
// updates in a row gliding in one direction. Steady before there is a prediction.
bool fsemu_verticalblank_is_steady(const fsemu_verticalblank_t *vb);

// Whether the published line was from the presentations at the last update (they were fresh).
bool fsemu_verticalblank_is_from_presentations(const fsemu_verticalblank_t *vb);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_VERTICALBLANK_H */
