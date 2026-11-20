/*
 * The audio rate adjustment's controller: from the audio buffer level measured at each emulated
 * frame's end to the factor the top applies to Paula's sample clock (the rate feed-forward plus
 * the drift correction). Pure code with no SDL, clock or global state, so that it can be tested
 * alone against a simulated audio device (od-fs/tests/fsemu); fsemu-audio.cpp feeds it.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_AUDIORATE_H
#define FSEMU_AUDIORATE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// The drift correction's limit (as a share of the rate, 0.1 %), so that no correction is audible.
#define FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION 0.001

// How fast the drift correction may change (as a share of the rate per second, 0.01 %/s).
#define FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION_PER_S 0.0001

// The share of the rate limit that a step uses, so that the measured rate of change stays below
// FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION_PER_S (the timing trace rounds the drift correction).
#define FSEMU_AUDIORATE_SLEW_MARGIN 0.99

// The excess rule (fsemu_audiorate_check_excess): the band the level settles in (this share of
// the target either side of it), and how many frames, counting the one with audio underruns, the
// rule looks for the level rising above the band after them (about half a second, longer than a
// catch-up lasts before the timeline is reset).
#define FSEMU_AUDIORATE_BAND 0.25
#define FSEMU_AUDIORATE_AFTER_UNDERRUNS_FRAMES 25

// The controller's state, owned by the caller. Only fsemu-audiorate.cpp looks inside.
typedef struct fsemu_audiorate_t {
    // The drift correction's integrator (a share of the rate) and its last value.
    double integral;
    double drift_correction;
} fsemu_audiorate_t;

// The excess rule's state, owned by the caller, apart from the controller's (the rule never
// changes what the controller has learnt). Only fsemu-audiorate.cpp looks inside.
typedef struct fsemu_audiorate_excess_rule_t {
    // Whether the audio device was overdue at the last frame's end, and the frames left in which
    // a level above the band is taken for the silence that audio underruns handed over.
    bool device_was_overdue;
    int frames_after_underruns;
} fsemu_audiorate_excess_rule_t;

// What the excess rule decided for a frame.
typedef struct fsemu_audiorate_excess_t {
    // Drop the oldest audio of the audio ring buffer down to the target, as an audio overrun does
    // (with a crossfade across the cut). The rule never adds silence.
    bool drop;
    // Hold the controller (frozen) for the frame's level.
    bool hold;
} fsemu_audiorate_excess_t;

// The audio rate adjustment and its parts, all shares of the rate (0.001 is 0.1 %): Paula's sample
// interval is the nominal one divided by (1 + adjustment).
typedef struct fsemu_audiorate_adjustment_t {
    double adjustment;
    double rate_feed_forward;
    double drift_correction;
} fsemu_audiorate_adjustment_t;

// Sets up a controller with no drift correction (the device's clock taken to match the host's).
void fsemu_audiorate_init(fsemu_audiorate_t *controller);

// The rate feed-forward: the ratio of the emulated frame rate the core runs at (the vblank_hz
// passed to fsemu's frame start) to fsemu's timeline rate, minus one, so that a timeline running
// faster than the emulated frame rate slows Paula's sample clock. Exactly 0 when the two are the
// same (no vsync), and 0 when either rate is unknown (0 or less).
double fsemu_audiorate_get_rate_feed_forward(double emulated_hz, double timeline_hz);

// One step of the controller, once per emulated frame: the audio buffer level measured at the end
// of the frame before (ns), the target audio buffer level (ns), the rate feed-forward, the time
// the step stands for (one frame period on fsemu's timeline, ns), the real time since the step
// before (ns) and whether the controller is frozen (during a stall, when the measured level says
// nothing about the clocks). The drift correction is a PI controller on the level's error,
// clamped to FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION and changing by at most
// FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION_PER_S of real time (counting at most a frame period per
// step, with FSEMU_AUDIORATE_SLEW_MARGIN); while frozen it is held, integrator and all.
fsemu_audiorate_adjustment_t fsemu_audiorate_update(fsemu_audiorate_t *controller, double level_ns,
                                                    double target_ns, double rate_feed_forward,
                                                    double period_ns, double elapsed_ns,
                                                    bool frozen);

// Sets up the excess rule with nothing to look for.
void fsemu_audiorate_init_excess_rule(fsemu_audiorate_excess_rule_t *rule);

// The excess rule, once per emulated frame after its end, with the audio buffer level measured
// then (ns), the target (ns), whether the frame had audio underruns, whether the audio device is
// overdue by more than a frame period, and whether the frame ended on time (not catching up after
// a stall). It deals with an excess that the drift correction, at most 0.1 %, would take tens of
// seconds to make up, and that is no clock drift:
//
// - When the audio device calls back again after being overdue (after it was opened or resumed,
//   or a long hiccup): the audio written meanwhile, up to the ceiling, is dropped down to the
//   target (when the level is above it).
// - After audio underruns: the silence they handed over stays in the level once the emulation has
//   caught up. For FSEMU_AUDIORATE_AFTER_UNDERRUNS_FRAMES frames the controller is held while the
//   level is above the target, and a level above the band in a frame on time is dropped down to
//   the target.
//
// Below the target, nothing: the controller and audio underruns deal with it.
fsemu_audiorate_excess_t fsemu_audiorate_check_excess(fsemu_audiorate_excess_rule_t *rule,
                                                      double level_ns, double target_ns,
                                                      bool underruns, bool device_overdue,
                                                      bool on_time);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_AUDIORATE_H */
