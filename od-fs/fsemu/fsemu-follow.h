/*
 * Whether the emulation follows the host vertical blank with vsync on, decided with hysteresis.
 * Kept apart from fsemu-frame.cpp, which decides it once per frame, so that it can be tested alone
 * (od-fs/tests/fsemu).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_FOLLOW_H
#define FSEMU_FOLLOW_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// How far (as a share) the host refresh rate may be from the emulated frame rate for the emulation
// to start following the host vertical blank.
// TODO: Make it a setting (about 1-2 %).
#define FSEMU_FOLLOW_TOLERANCE 0.02

// How much further apart (as a share) the rates may drift before the emulation stops following,
// so that a measured rate near the edge of the tolerance does not switch between timer-based and
// vsync-based timing from one frame to the next.
#define FSEMU_FOLLOW_HYSTERESIS 0.005

// How many frames in a row the rates must say otherwise before the decision changes, so that a
// host refresh rate measured from the first few swaps, or thrown off for a moment, does not
// switch it either.
#define FSEMU_FOLLOW_FRAMES 10

// The decision whether the emulation follows the host vertical blank (the start gate, and a frame
// period of the host refresh period). Otherwise the emulation keeps time on its own at the emulated
// frame rate, and each emulated frame is shown at the next host vertical blank. Whole multiples (a
// 50 Hz Amiga on 100 Hz) are not followed yet.
typedef struct fsemu_follow_t {
    bool following;
    // The frames in a row whose rates said otherwise.
    int frames_against;
} fsemu_follow_t;

// Sets up a decision that follows, as it does until the host refresh rate is measured.
void fsemu_follow_init(fsemu_follow_t *follow);

// Updates the decision with a frame's rates and whether the predicted host vertical blank is steady
// (fsemu_verticalblank_is_steady), and returns it: they say to follow when the prediction is steady
// and the host refresh rate is within FSEMU_FOLLOW_TOLERANCE of the emulated frame rate, or, while
// following, within FSEMU_FOLLOW_TOLERANCE + FSEMU_FOLLOW_HYSTERESIS, and also before the host
// refresh rate is measured (0 or less); the decision changes when they have said otherwise for
// FSEMU_FOLLOW_FRAMES frames in a row. A prediction that is not steady (a display whose
// presentations follow the swaps, as with adaptive sync) says not to follow, as a refresh rate out
// of tolerance does.
bool fsemu_follow_update(fsemu_follow_t *follow, double host_refresh_rate_hz, double frame_rate_hz,
                         bool steady);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_FOLLOW_H */
