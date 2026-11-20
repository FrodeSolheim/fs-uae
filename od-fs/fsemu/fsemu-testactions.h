/*
 * Test actions (FSEMU_TEST_ACTIONS), a test aid for headless runs, where there is no one to press
 * the keys: pause, resume, warp, savestates and the performance graph's mode at given times, so
 * that the timing trace can show how the emulation and its audio go through them. Off, and costing
 * a loop over no actions per pass of the main loop's host window side, unless the variable is set.
 * Main thread only.
 *
 * FSEMU_TEST_ACTIONS is a comma-separated list of <seconds>:<action>, the seconds counted from when
 * the main loop starts (fractions allowed), with the actions pause, resume, warp, unwarp, save<N>
 * and load<N> (a savestate slot from 1 to 9) and graph (the performance graph's next mode, as
 * Alt+O: off, sidebars, detailed), such as "20:pause,25:resume,30:save1,35:load1".
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_TESTACTIONS_H
#define FSEMU_TESTACTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

// Turns on the test actions that FSEMU_TEST_ACTIONS asks for. Called when the main loop starts;
// may be called more than once.
void fsemu_testactions_init(void);

// Carries out the test actions that are due. Called by fsemu's frame hook.
void fsemu_testactions_run_due(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_TESTACTIONS_H */
