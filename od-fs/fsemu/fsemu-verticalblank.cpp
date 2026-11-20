/*
 * The predicted host vertical blank (see fsemu-verticalblank.h).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-verticalblank.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// A line fitted to observations numbered by time.

static void fsemu_verticalblank_fit_reset(fsemu_verticalblank_fit_t *fit)
{
    fit->predicted_ns = 0.0;
    fit->interval_ns = 0.0;
    fit->observations = 0;
    fit->off_observations = 0;
    fit->count = 0;
    fit->newest = 0;
    fit->off_count = 0;
    fit->off_newest = 0;
    fit->interval_kept = false;
}

// Sets up a line with its parameters (see fsemu_verticalblank_fit_t), with no observations.
static void fsemu_verticalblank_fit_init(fsemu_verticalblank_fit_t *fit, int window,
                                         int quantile_divisor, int early_run, int early_of,
                                         int late_run, int late_of,
                                         int rebase_interval_observations)
{
    memset(fit, 0, sizeof(*fit));
    fit->window = window;
    fit->quantile_divisor = quantile_divisor;
    fit->early_run = early_run;
    fit->early_of = early_of;
    fit->late_run = late_run;
    fit->late_of = late_of;
    fit->rebase_interval_observations = rebase_interval_observations;
    fsemu_verticalblank_fit_reset(fit);
}

// Whether the line is there: a point and an interval, from at least two observations.
static bool fsemu_verticalblank_fit_is_valid(const fsemu_verticalblank_fit_t *fit)
{
    return fit->predicted_ns != 0.0 && fit->interval_ns > 0.0 && fit->count >= 2;
}

// Whether the line is there and has its interval from the slopes (FSEMU_VERTICALBLANK_FIRST
// observations in its buffer): what the published line is made from.
static bool fsemu_verticalblank_fit_is_usable(const fsemu_verticalblank_fit_t *fit)
{
    return fsemu_verticalblank_fit_is_valid(fit) && fit->count >= FSEMU_VERTICALBLANK_FIRST;
}

// The buffer index of the i-th oldest observation in the buffer.
static int fsemu_verticalblank_index(const fsemu_verticalblank_fit_t *fit, int i)
{
    return (fit->newest - fit->count + 1 + i + fit->window) % fit->window;
}

static int fsemu_verticalblank_compare(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

// How many host refreshes there are in a time between two observations, at least one (one when
// there is no interval yet).
static int64_t fsemu_verticalblank_refreshes(double time_ns, double interval_ns)
{
    if (interval_ns <= 0.0) {
        return 1;
    }
    int64_t refreshes = llround(time_ns / interval_ns);
    return refreshes < 1 ? 1 : refreshes;
}

// Before the interval is measured from the slopes: the interval is the hint (the reported refresh)
// or the median of the times between the observations (most are one refresh apart, some two after
// a missed host vertical blank), and the observations are numbered again by it, so that an odd
// first time does not number them wrong for good.
static void fsemu_verticalblank_number_by_median(fsemu_verticalblank_fit_t *fit)
{
    double times_ns[FSEMU_VERTICALBLANK_FIRST];
    int n = 0;
    for (int i = 1; i < fit->count; i++) {
        times_ns[n++] = (double)(fit->observed_ns[fsemu_verticalblank_index(fit, i)] -
                                 fit->observed_ns[fsemu_verticalblank_index(fit, i - 1)]);
    }
    qsort(times_ns, n, sizeof(double), fsemu_verticalblank_compare);
    fit->interval_ns = fit->hint_ns > 0.0 ? fit->hint_ns : times_ns[n / 2];
    fit->refresh[fsemu_verticalblank_index(fit, 0)] = 0;
    for (int i = 1; i < fit->count; i++) {
        int index = fsemu_verticalblank_index(fit, i);
        int previous = fsemu_verticalblank_index(fit, i - 1);
        fit->refresh[index] =
            fit->refresh[previous] +
            fsemu_verticalblank_refreshes(
                (double)(fit->observed_ns[index] - fit->observed_ns[previous]), fit->interval_ns);
    }
}

// The interval from the slopes between the observations half the buffer apart: the mean of the
// middle half of them, so that late observations do not tilt it.
static void fsemu_verticalblank_measure_interval(fsemu_verticalblank_fit_t *fit)
{
    double slopes[FSEMU_VERTICALBLANK_WINDOW];
    int half = fit->count / 2;
    int n = 0;
    for (int i = 0; i + half < fit->count; i++) {
        int earlier = fsemu_verticalblank_index(fit, i);
        int later = fsemu_verticalblank_index(fit, i + half);
        slopes[n++] = (double)(fit->observed_ns[later] - fit->observed_ns[earlier]) /
                      (double)(fit->refresh[later] - fit->refresh[earlier]);
    }
    qsort(slopes, n, sizeof(double), fsemu_verticalblank_compare);
    int first = n / 4;
    int last = n - first;
    double sum = 0.0;
    for (int i = first; i < last; i++) {
        sum += slopes[i];
    }
    fit->interval_ns = sum / (last - first);
}

// Fits the line (offset_ns + slope_ns × refreshes, from the newest observation) to the observations
// within FSEMU_VERTICALBLANK_FIT_NS of it, by least squares. Leaves it as it is when fewer than a
// quarter of them are near it, or they are all at one host refresh.
static void fsemu_verticalblank_fit_line(const fsemu_verticalblank_fit_t *fit, double *offset_ns,
                                         double *slope_ns)
{
    int64_t newest_ns = fit->observed_ns[fit->newest];
    int64_t newest_refresh = fit->refresh[fit->newest];
    double sum_x = 0.0, sum_y = 0.0, sum_xx = 0.0, sum_xy = 0.0;
    int n = 0;
    for (int i = 0; i < fit->count; i++) {
        int index = fsemu_verticalblank_index(fit, i);
        // Relative to the newest observation, to keep the precision of the large times.
        double x = (double)(fit->refresh[index] - newest_refresh);
        double y = (double)(fit->observed_ns[index] - newest_ns);
        if (fabs(y - (*offset_ns + *slope_ns * x)) > FSEMU_VERTICALBLANK_FIT_NS) {
            continue;
        }
        sum_x += x;
        sum_y += y;
        sum_xx += x * x;
        sum_xy += x * y;
        n++;
    }
    double variance = n * sum_xx - sum_x * sum_x;
    if (n < 2 || n * 4 < fit->count || variance <= 0.0) {
        return;
    }
    *slope_ns = (n * sum_xy - sum_x * sum_y) / variance;
    *offset_ns = (sum_y - *slope_ns * sum_x) / n;
}

// The line: with the interval as its slope through the line's quantile of the observations (the
// first quartile, or the median), at the newest one's host refresh; once there are
// FSEMU_VERTICALBLANK_FIRST observations, fitted to the observations near that line.
static void fsemu_verticalblank_place(fsemu_verticalblank_fit_t *fit)
{
    int64_t newest_ns = fit->observed_ns[fit->newest];
    int64_t newest_refresh = fit->refresh[fit->newest];
    double offsets[FSEMU_VERTICALBLANK_WINDOW];
    for (int i = 0; i < fit->count; i++) {
        int index = fsemu_verticalblank_index(fit, i);
        // Where each observation puts the line, relative to the newest observation, to keep the
        // precision of the large times.
        offsets[i] = (double)(fit->observed_ns[index] - newest_ns) -
                     (double)(fit->refresh[index] - newest_refresh) * fit->interval_ns;
    }
    qsort(offsets, fit->count, sizeof(double), fsemu_verticalblank_compare);
    double offset_ns = offsets[fit->count / fit->quantile_divisor];
    if (fit->count >= FSEMU_VERTICALBLANK_FIRST) {
        double slope_ns = fit->interval_ns;
        for (int pass = 0; pass < FSEMU_VERTICALBLANK_FIT_PASSES; pass++) {
            fsemu_verticalblank_fit_line(fit, &offset_ns, &slope_ns);
        }
        fit->interval_ns = slope_ns;
    }
    fit->predicted_ns = (double)newest_ns + offset_ns;
}

// Whether `run` of the last `of` observations were more than FSEMU_VERTICALBLANK_FIT_NS off the
// line on one side (sign -1 for early, 1 for late).
static bool fsemu_verticalblank_is_run(const fsemu_verticalblank_fit_t *fit, int run, int of,
                                       double sign)
{
    if (run <= 0 || fit->off_count < of) {
        return false;
    }
    int n = 0;
    for (int i = 0; i < of; i++) {
        int index = (fit->off_newest - i + FSEMU_VERTICALBLANK_RUN_HISTORY) %
                    FSEMU_VERTICALBLANK_RUN_HISTORY;
        if (fit->off_ns[index] * sign > FSEMU_VERTICALBLANK_FIT_NS) {
            n++;
        }
    }
    return n >= run;
}

// Feeds an observation to a line. Returns false when it was left out (not at least half a refresh
// after the last one).
static bool fsemu_verticalblank_fit_update(fsemu_verticalblank_fit_t *fit, int64_t observed_ns)
{
    int64_t refresh = 0;
    int rebase = 0;
    if (fit->count > 0) {
        int64_t last_ns = fit->observed_ns[fit->newest];
        double time_ns = (double)(observed_ns - last_ns);
        // Not later than the last one, or the same host vertical blank.
        if (time_ns <= 0.0 || (fit->interval_ns > 0.0 && time_ns < fit->interval_ns / 2.0)) {
            return false;
        }
        refresh =
            fit->refresh[fit->newest] + fsemu_verticalblank_refreshes(time_ns, fit->interval_ns);
        if (fit->count >= FSEMU_VERTICALBLANK_FIRST) {
            double off_ns = (double)observed_ns -
                            (fit->predicted_ns +
                             (double)(refresh - fit->refresh[fit->newest]) * fit->interval_ns);
            fit->off_newest = (fit->off_newest + 1) % FSEMU_VERTICALBLANK_RUN_HISTORY;
            fit->off_ns[fit->off_newest] = off_ns;
            if (fit->off_count < FSEMU_VERTICALBLANK_RUN_HISTORY) {
                fit->off_count++;
            }
            if (fabs(off_ns) > fit->interval_ns / 4.0) {
                fit->off_observations++;
            } else {
                fit->off_observations = 0;
            }
            // A swap returns at most a few milliseconds before its host vertical blank, so one
            // a quarter of a refresh earlier than the line means that the host vertical blanks
            // moved earlier. Kept, the older observations would tilt the interval for a whole
            // buffer.
            bool early = off_ns < -fit->interval_ns / 4.0;
            if (early || fit->off_observations >= FSEMU_VERTICALBLANK_OFF_OBSERVATIONS) {
                // The host vertical blanks moved, or the interval changed: start afresh from this
                // observation.
                fsemu_verticalblank_fit_reset(fit);
                fit->restarts++;
                refresh = 0;
            } else if (fsemu_verticalblank_is_run(fit, fit->early_run, fit->early_of, -1.0)) {
                rebase = fit->early_of;
            } else if (fsemu_verticalblank_is_run(fit, fit->late_run, fit->late_of, 1.0)) {
                rebase = fit->late_of;
            }
        }
    }
    fit->newest = (fit->newest + 1) % fit->window;
    fit->observed_ns[fit->newest] = observed_ns;
    fit->refresh[fit->newest] = refresh;
    if (fit->count < fit->window) {
        fit->count++;
    }
    if (rebase > 0) {
        // A run of observations off the line on one side: a shift. Keeps only them, with their
        // numbering and the interval, and fits afresh to them. Not an interval more than
        // FSEMU_VERTICALBLANK_INTERVAL_AGREEMENT off the reported refresh (the presentations'):
        // measured in a disturbance and kept, the line drifts off the presentations and rebases
        // every few of them, never measuring it again (seen for 28 s after a switch to
        // fullscreen); it is measured again from the kept observations.
        if (fit->count > rebase) {
            fit->count = rebase;
        }
        fit->off_count = 0;
        fit->interval_kept =
            !(fit->hint_ns > 0.0 && fabs(fit->interval_ns - fit->hint_ns) >
                                        FSEMU_VERTICALBLANK_INTERVAL_AGREEMENT * fit->hint_ns);
        fit->rebases++;
    }
    if (fit->observations < INT_MAX) {
        fit->observations++;
    }
    if (fit->count < 2) {
        fit->predicted_ns = (double)observed_ns;
        fit->interval_ns = 0.0;
        return true;
    }
    if (fit->interval_kept && fit->count >= fit->rebase_interval_observations) {
        fit->interval_kept = false;
    }
    if (!fit->interval_kept) {
        if (fit->count <= FSEMU_VERTICALBLANK_FIRST) {
            fsemu_verticalblank_number_by_median(fit);
        }
        if (fit->count >= FSEMU_VERTICALBLANK_FIRST) {
            fsemu_verticalblank_measure_interval(fit);
        }
    }
    fsemu_verticalblank_place(fit);
    return true;
}

// ----------------------------------------------------------------------------
// The published line and the lead.

// The point of the line (anchor_ns, interval_ns) nearest t_ns.
static double fsemu_verticalblank_nearest(double anchor_ns, double interval_ns, double t_ns)
{
    return anchor_ns + nearbyint((t_ns - anchor_ns) / interval_ns) * interval_ns;
}

// d_ns wrapped into [low, low + 1) refreshes.
static double fsemu_verticalblank_wrap(double d_ns, double interval_ns, double low)
{
    return d_ns - floor(d_ns / interval_ns - low) * interval_ns;
}

void fsemu_verticalblank_init(fsemu_verticalblank_t *vb)
{
    memset(vb, 0, sizeof(*vb));
    fsemu_verticalblank_fit_init(&vb->returns, FSEMU_VERTICALBLANK_WINDOW, 4,
                                 FSEMU_VERTICALBLANK_EARLY_RUN, FSEMU_VERTICALBLANK_EARLY_OF,
                                 FSEMU_VERTICALBLANK_LATE_RUN, FSEMU_VERTICALBLANK_LATE_OF,
                                 FSEMU_VERTICALBLANK_REBASE_INTERVAL_OBSERVATIONS);
    fsemu_verticalblank_fit_init(
        &vb->presentations, FSEMU_VERTICALBLANK_PRESENTATION_WINDOW, 2,
        FSEMU_VERTICALBLANK_PRESENTATION_RUN, FSEMU_VERTICALBLANK_PRESENTATION_OF,
        FSEMU_VERTICALBLANK_PRESENTATION_RUN, FSEMU_VERTICALBLANK_PRESENTATION_OF,
        FSEMU_VERTICALBLANK_PRESENTATION_WINDOW / 2);
    vb->reported_refresh_ns = 0.0;
    fsemu_verticalblank_reset(vb);
}

void fsemu_verticalblank_reset(fsemu_verticalblank_t *vb)
{
    fsemu_verticalblank_fit_reset(&vb->returns);
    fsemu_verticalblank_fit_reset(&vb->presentations);
    vb->predicted_ns = 0.0;
    vb->interval_ns = 0.0;
    vb->observations = 0;
    vb->presented_ns = 0;
    vb->from_presentations = false;
    vb->source_lead_ns = 0.0;
    vb->far_updates = 0;
    vb->gliding_updates = 0;
    vb->glide_sign = 0;
    vb->updates = 0;
    vb->snaps = 0;
    vb->snap_next = 0;
    vb->lead_ns = 0.0;
    vb->lead_count = 0;
    vb->lead_next = 0;
    vb->has_previous = false;
    vb->previous_predicted_ns = 0.0;
    vb->previous_interval_ns = 0.0;
}

void fsemu_verticalblank_add_presentation(fsemu_verticalblank_t *vb, int64_t presented_ns,
                                          int64_t refresh_ns)
{
    if (presented_ns <= vb->presented_ns) {
        return;
    }
    double reported_ns = (double)refresh_ns;
    if (reported_ns > 0.0 && vb->reported_refresh_ns > 0.0 &&
        fabs(reported_ns - vb->reported_refresh_ns) >
            FSEMU_VERTICALBLANK_REFRESH_CHANGE * reported_ns) {
        // The reported refresh changed (another output, another mode): start afresh.
        fsemu_verticalblank_fit_reset(&vb->presentations);
    }
    if (reported_ns > 0.0) {
        vb->reported_refresh_ns = reported_ns;
    }
    // Numbered by time, as the swap returns are: the presentation's sequence is not a refresh
    // counter on every compositor (on one it counted presentations).
    vb->presentations.hint_ns = vb->reported_refresh_ns;
    vb->presented_ns = presented_ns;
    fsemu_verticalblank_fit_update(&vb->presentations, presented_ns);
}

// Whether the presentations are fresh at a swap return: their line has FSEMU_VERTICALBLANK_FIRST
// presentations, its interval agrees with the swap returns' line's, and the newest presentation is
// less than FSEMU_VERTICALBLANK_STALE_REFRESHES refreshes before the swap return.
static bool fsemu_verticalblank_are_presentations_fresh(const fsemu_verticalblank_t *vb,
                                                        int64_t observed_ns)
{
    const fsemu_verticalblank_fit_t *presentations = &vb->presentations;
    const fsemu_verticalblank_fit_t *returns = &vb->returns;
    if (vb->presented_ns == 0 || !fsemu_verticalblank_fit_is_usable(presentations)) {
        return false;
    }
    double interval_ns = presentations->interval_ns;
    if (fsemu_verticalblank_fit_is_usable(returns) &&
        fabs(interval_ns - returns->interval_ns) >
            FSEMU_VERTICALBLANK_INTERVAL_AGREEMENT * returns->interval_ns) {
        // Not of the display the swaps wait for (another output, a mode change in between).
        return false;
    }
    return (double)(observed_ns - vb->presented_ns) <
           FSEMU_VERTICALBLANK_STALE_REFRESHES * interval_ns;
}

// Snaps the published line to its source.
static void fsemu_verticalblank_snap(fsemu_verticalblank_t *vb, double anchor_ns,
                                     double interval_ns)
{
    vb->predicted_ns = anchor_ns;
    vb->interval_ns = interval_ns;
    vb->far_updates = 0;
    vb->gliding_updates = 0;
    vb->snaps++;
    vb->snap_updates[vb->snap_next] = vb->updates;
    vb->snap_next = (vb->snap_next + 1) % FSEMU_VERTICALBLANK_UNSTEADY_SNAPS;
}

// Moves the published line towards its source (anchor_ns, interval_ns) by at most
// FSEMU_VERTICALBLANK_GLIDE_NS, at the host vertical blank nearest ref_ns; snaps to it when it is
// the first, or when it has been more than a quarter of a refresh away for
// FSEMU_VERTICALBLANK_SNAP_UPDATES updates.
static void fsemu_verticalblank_glide(fsemu_verticalblank_t *vb, double anchor_ns,
                                      double interval_ns, double ref_ns)
{
    vb->updates++;
    if (vb->interval_ns <= 0.0) {
        fsemu_verticalblank_snap(vb, anchor_ns, interval_ns);
        return;
    }
    double here_ns = fsemu_verticalblank_nearest(vb->predicted_ns, vb->interval_ns, ref_ns);
    double there_ns = fsemu_verticalblank_nearest(anchor_ns, interval_ns, here_ns);
    double d_ns = there_ns - here_ns;
    if (fabs(d_ns) > interval_ns / 4.0) {
        vb->far_updates++;
        if (vb->far_updates >= FSEMU_VERTICALBLANK_SNAP_UPDATES) {
            fsemu_verticalblank_snap(vb, anchor_ns, interval_ns);
        } else {
            vb->predicted_ns = here_ns;
            vb->interval_ns = interval_ns;
        }
        return;
    }
    vb->far_updates = 0;
    double step_ns = fmax(-FSEMU_VERTICALBLANK_GLIDE_NS, fmin(FSEMU_VERTICALBLANK_GLIDE_NS, d_ns));
    int sign = (step_ns > 0.0) - (step_ns < 0.0);
    if (fabs(step_ns) >= FSEMU_VERTICALBLANK_GLIDE_NS && sign == vb->glide_sign) {
        vb->gliding_updates++;
    } else if (fabs(step_ns) >= FSEMU_VERTICALBLANK_GLIDE_NS) {
        vb->gliding_updates = 1;
    } else {
        vb->gliding_updates = 0;
    }
    vb->glide_sign = sign;
    vb->predicted_ns = here_ns + step_ns;
    vb->interval_ns = interval_ns;
}

// The lead target while the presentations are fresh: the third quartile of the distances from the
// last FSEMU_VERTICALBLANK_LEAD_WINDOW swap returns to the published line's nearest host vertical
// blank (the published line minus the swap return, wrapped to [-1/4, 3/4) of a refresh). The swap
// returns are kept, not their distances, so that the distances are measured against the published
// line as it is now.
static double fsemu_verticalblank_get_lead_target_ns(fsemu_verticalblank_t *vb, int64_t observed_ns)
{
    vb->lead_returns_ns[vb->lead_next] = observed_ns;
    vb->lead_next = (vb->lead_next + 1) % FSEMU_VERTICALBLANK_LEAD_WINDOW;
    if (vb->lead_count < FSEMU_VERTICALBLANK_LEAD_WINDOW) {
        vb->lead_count++;
    }
    double distances_ns[FSEMU_VERTICALBLANK_LEAD_WINDOW];
    for (int i = 0; i < vb->lead_count; i++) {
        double return_ns = (double)vb->lead_returns_ns[i];
        double nearest_ns =
            fsemu_verticalblank_nearest(vb->predicted_ns, vb->interval_ns, return_ns);
        distances_ns[i] = fsemu_verticalblank_wrap(nearest_ns - return_ns, vb->interval_ns, -0.25);
    }
    qsort(distances_ns, vb->lead_count, sizeof(double), fsemu_verticalblank_compare);
    return distances_ns[vb->lead_count * 3 / 4];
}

void fsemu_verticalblank_update(fsemu_verticalblank_t *vb, int64_t observed_ns)
{
    fsemu_verticalblank_fit_t *returns = &vb->returns;
    const fsemu_verticalblank_fit_t *presentations = &vb->presentations;
    if (!fsemu_verticalblank_fit_update(returns, observed_ns)) {
        // The same host vertical blank as the last one (or not later): changes nothing.
        return;
    }
    vb->observations = returns->observations;
    bool returns_usable = fsemu_verticalblank_fit_is_usable(returns);
    bool fresh = fsemu_verticalblank_are_presentations_fresh(vb, observed_ns);
    vb->from_presentations = fresh;
    // The source of the published line.
    bool has_source = true;
    double source_ns = 0.0;
    double source_interval_ns = 0.0;
    if (fresh) {
        if (returns_usable) {
            double here_ns = fsemu_verticalblank_nearest(returns->predicted_ns,
                                                         returns->interval_ns, (double)observed_ns);
            double there_ns = fsemu_verticalblank_nearest(presentations->predicted_ns,
                                                          presentations->interval_ns, here_ns);
            vb->source_lead_ns =
                fsemu_verticalblank_wrap(there_ns - here_ns, presentations->interval_ns, -0.25);
        }
        source_ns = presentations->predicted_ns;
        source_interval_ns = presentations->interval_ns;
    } else if (returns_usable) {
        // Presentations stale or absent: the swap returns' line, shifted by the last difference
        // between the two lines, so that losing the feedback does not move the published line.
        source_ns = returns->predicted_ns + vb->source_lead_ns;
        source_interval_ns = returns->interval_ns;
    } else {
        has_source = false;
    }
    int snaps = vb->snaps;
    if (has_source) {
        fsemu_verticalblank_glide(vb, source_ns, source_interval_ns, (double)observed_ns);
    }
    if (vb->interval_ns <= 0.0) {
        vb->lead_ns = 0.0;
        return;
    }
    double interval_ns = vb->interval_ns;
    double target_ns;
    if (fresh) {
        target_ns = fsemu_verticalblank_get_lead_target_ns(vb, observed_ns);
    } else if (returns_usable) {
        // What the glide has not caught up with yet: the published line minus the swap returns'.
        vb->lead_count = 0;
        vb->lead_next = 0;
        double here_ns = fsemu_verticalblank_nearest(returns->predicted_ns, returns->interval_ns,
                                                     (double)observed_ns);
        double nearest_ns = fsemu_verticalblank_nearest(vb->predicted_ns, interval_ns, here_ns);
        target_ns = fsemu_verticalblank_wrap(nearest_ns - here_ns, interval_ns, -0.25);
    } else {
        return;
    }
    // The swap deadline (the published line minus the lead) moves earlier at once and later by at
    // most the glide per update (as far as the published line moved in this update, plus the
    // glide).
    bool snapped = vb->snaps != snaps;
    double moved_ns = 0.0;
    if (vb->has_previous && !snapped) {
        double here_ns =
            fsemu_verticalblank_nearest(vb->predicted_ns, interval_ns, (double)observed_ns);
        moved_ns = here_ns - fsemu_verticalblank_nearest(vb->previous_predicted_ns,
                                                         vb->previous_interval_ns, here_ns);
    }
    vb->has_previous = true;
    vb->previous_predicted_ns = vb->predicted_ns;
    vb->previous_interval_ns = interval_ns;
    if (snapped || fabs(target_ns - vb->lead_ns) > interval_ns / 4.0 || target_ns > vb->lead_ns) {
        vb->lead_ns = target_ns;
    } else {
        vb->lead_ns = fmax(target_ns, vb->lead_ns + moved_ns - FSEMU_VERTICALBLANK_GLIDE_NS);
    }
}

int64_t fsemu_verticalblank_get_predicted_ns(const fsemu_verticalblank_t *vb,
                                             int64_t at_or_after_ns)
{
    if (vb->predicted_ns == 0.0 || vb->interval_ns <= 0.0) {
        return 0;
    }
    double periods = floor(((double)at_or_after_ns - vb->predicted_ns) / vb->interval_ns);
    int64_t predicted_ns = llround(vb->predicted_ns + periods * vb->interval_ns);
    // Rounding can leave it a little before at_or_after_ns.
    while (predicted_ns < at_or_after_ns) {
        periods += 1.0;
        predicted_ns = llround(vb->predicted_ns + periods * vb->interval_ns);
    }
    return predicted_ns;
}

int64_t fsemu_verticalblank_get_lead_ns(const fsemu_verticalblank_t *vb)
{
    if (vb->interval_ns <= 0.0) {
        return 0;
    }
    return llround(vb->lead_ns);
}

bool fsemu_verticalblank_is_settled(const fsemu_verticalblank_t *vb)
{
    return vb->observations >= FSEMU_VERTICALBLANK_SETTLE_OBSERVATIONS;
}

bool fsemu_verticalblank_is_steady(const fsemu_verticalblank_t *vb)
{
    if (vb->gliding_updates >= FSEMU_VERTICALBLANK_UNSTEADY_GLIDE_UPDATES) {
        return false;
    }
    int recent = 0;
    int remembered = vb->snaps < FSEMU_VERTICALBLANK_UNSTEADY_SNAPS
                         ? vb->snaps
                         : FSEMU_VERTICALBLANK_UNSTEADY_SNAPS;
    for (int i = 0; i < remembered; i++) {
        if (vb->updates - vb->snap_updates[i] < FSEMU_VERTICALBLANK_UNSTEADY_SNAP_UPDATES) {
            recent++;
        }
    }
    return recent < FSEMU_VERTICALBLANK_UNSTEADY_SNAPS;
}

bool fsemu_verticalblank_is_from_presentations(const fsemu_verticalblank_t *vb)
{
    return vb->from_presentations;
}
