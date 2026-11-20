/*
 * The audio rate adjustment's controller (see fsemu-audiorate.h).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-audiorate.h"

#include <math.h>

void fsemu_audiorate_init(fsemu_audiorate_t *controller)
{
    controller->integral = 0.0;
    controller->drift_correction = 0.0;
}

double fsemu_audiorate_get_rate_feed_forward(double emulated_hz, double timeline_hz)
{
    if (emulated_hz <= 0.0 || timeline_hz <= 0.0 || emulated_hz == timeline_hz) {
        return 0.0;
    }
    return emulated_hz / timeline_hz - 1.0;
}

// The drift correction's gains, for the level's error in seconds: a critically damped loop with a
// time constant of 1 / FSEMU_AUDIORATE_OMEGA seconds (proportional gain 2 omega, integral gain
// omega squared).
// REVIEW: Tuned with the simulation (od-fs/tests/fsemu/audiorate_test.cpp); to be checked against
// real audio devices with the timing trace.
#define FSEMU_AUDIORATE_OMEGA 0.05
#define FSEMU_AUDIORATE_KP (2.0 * FSEMU_AUDIORATE_OMEGA)
#define FSEMU_AUDIORATE_KI (FSEMU_AUDIORATE_OMEGA * FSEMU_AUDIORATE_OMEGA)

static double fsemu_audiorate_clamp(double value, double limit)
{
    return value < -limit ? -limit : (value > limit ? limit : value);
}

fsemu_audiorate_adjustment_t fsemu_audiorate_update(fsemu_audiorate_t *controller, double level_ns,
                                                    double target_ns, double rate_feed_forward,
                                                    double period_ns, double elapsed_ns,
                                                    bool frozen)
{
    if (!frozen && period_ns > 0.0) {
        double error_s = (target_ns - level_ns) / 1e9;
        double period_s = period_ns / 1e9;
        double integral = controller->integral + FSEMU_AUDIORATE_KI * error_s * period_s;
        // Anti-windup: the integrator stays within the limit, and does not grow further while the
        // drift correction is at the limit in its direction.
        integral = fsemu_audiorate_clamp(integral, FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION);
        double wanted = FSEMU_AUDIORATE_KP * error_s + integral;
        bool at_limit = fabs(wanted) >= FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION &&
                        (wanted > 0) == (integral > controller->integral);
        if (!at_limit) {
            controller->integral = integral;
        }
        wanted = fsemu_audiorate_clamp(FSEMU_AUDIORATE_KP * error_s + controller->integral,
                                       FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION);
        // Per real time since the step before, counting at most a frame period (a step after a
        // pause or a stall changes no more than one in time does).
        double slew_ns = elapsed_ns < period_ns ? (elapsed_ns > 0.0 ? elapsed_ns : 0.0) : period_ns;
        double max_change = FSEMU_AUDIORATE_MAX_DRIFT_CORRECTION_PER_S *
                            FSEMU_AUDIORATE_SLEW_MARGIN * slew_ns / 1e9;
        double change = fsemu_audiorate_clamp(wanted - controller->drift_correction, max_change);
        controller->drift_correction += change;
    }
    fsemu_audiorate_adjustment_t result;
    result.rate_feed_forward = rate_feed_forward;
    result.drift_correction = controller->drift_correction;
    result.adjustment = rate_feed_forward + controller->drift_correction;
    return result;
}

void fsemu_audiorate_init_excess_rule(fsemu_audiorate_excess_rule_t *rule)
{
    rule->device_was_overdue = false;
    rule->frames_after_underruns = 0;
}

fsemu_audiorate_excess_t fsemu_audiorate_check_excess(fsemu_audiorate_excess_rule_t *rule,
                                                      double level_ns, double target_ns,
                                                      bool underruns, bool device_overdue,
                                                      bool on_time)
{
    fsemu_audiorate_excess_t result = {false, false};
    if (device_overdue) {
        rule->device_was_overdue = true;
        rule->frames_after_underruns = 0;
        return result;
    }
    if (rule->device_was_overdue) {
        // The audio device calls back again (after its start, a pause or a long hiccup): the
        // audio written meanwhile, up to the ceiling, would stay in the level.
        rule->device_was_overdue = false;
        result.drop = level_ns > target_ns;
        result.hold = result.drop;
        return result;
    }
    if (underruns) {
        rule->frames_after_underruns = FSEMU_AUDIORATE_AFTER_UNDERRUNS_FRAMES;
    }
    if (rule->frames_after_underruns > 0) {
        // The level is raised by the silence the audio underruns handed over as soon as the
        // emulation has caught up: held while above the target, so that it is not learnt as
        // clock drift, and dropped once the frames are on time again. Below the target the
        // controller answers (the device may drain faster than the drift correction makes up
        // for).
        rule->frames_after_underruns--;
        result.hold = level_ns > target_ns;
        if (on_time && level_ns > target_ns * (1.0 + FSEMU_AUDIORATE_BAND)) {
            result.drop = true;
            rule->frames_after_underruns = 0;
        }
    }
    return result;
}
