/*
 * Whether the emulation follows the host vertical blank with vsync on (see fsemu-follow.h).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-follow.h"

void fsemu_follow_init(fsemu_follow_t *follow)
{
    follow->following = true;
    follow->frames_against = 0;
}

// Whether a frame's rates say to follow, from whether the emulation follows now.
static bool fsemu_follow_should_follow(bool following, double host_refresh_rate_hz,
                                       double frame_rate_hz)
{
    if (host_refresh_rate_hz <= 0.0 || frame_rate_hz <= 0.0) {
        return true;
    }
    double tolerance = FSEMU_FOLLOW_TOLERANCE + (following ? FSEMU_FOLLOW_HYSTERESIS : 0.0);
    double ratio = host_refresh_rate_hz / frame_rate_hz;
    return ratio > 1.0 - tolerance && ratio < 1.0 + tolerance;
}

bool fsemu_follow_update(fsemu_follow_t *follow, double host_refresh_rate_hz, double frame_rate_hz,
                         bool steady)
{
    bool should_follow = steady && fsemu_follow_should_follow(follow->following,
                                                              host_refresh_rate_hz, frame_rate_hz);
    if (should_follow == follow->following) {
        follow->frames_against = 0;
    } else if (++follow->frames_against >= FSEMU_FOLLOW_FRAMES) {
        follow->following = !follow->following;
        follow->frames_against = 0;
    }
    return follow->following;
}
