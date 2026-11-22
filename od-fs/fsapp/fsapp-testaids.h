/*
 * Test aids for headless runs of the main loop, turned on by environment variables: the simulated
 * display (FSAPP_SIMULATED_DISPLAY_HZ, with late swaps and a compositor's lead with presentation
 * feedback), the simulated drawing delay (FSAPP_SIMULATED_RENDER_US), test input
 * (FSAPP_TEST_INPUT) and the test display scale (FSAPP_TEST_DISPLAY_SCALE). They are off, and cost
 * one check each where the main loop asks, unless their variable is set. Main thread only (the test
 * input pushes its events from SDL's timer thread).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSAPP_TESTAIDS_H
#define FSAPP_TESTAIDS_H

#include <stdbool.h>
#include <stdint.h>

#include "fsapp-frame.h"

#ifdef __cplusplus
extern "C" {
#endif

// Turns on the test aids that their environment variables ask for. Called when the main loop
// starts.
void fsapp_testaids_init(void);

// Test input (FSAPP_TEST_INPUT=<ms>[,<SDL key name>]), a test aid for headless runs, where there is
// no input: every given number of milliseconds from when the main loop starts, an SDL timer pushes
// a key press (Left Shift, or the key named after the period) and then its release as SDL events,
// which go the way of any input event (the event hooks, the timing trace's `input: key`). Since the
// timer runs on a thread of its own, the events come whatever the main thread is doing, as a real
// keyboard's do, so how long the main thread leaves them waiting (the trace's `handled_after_us`)
// is measured as for real input. Turned on by fsapp_testaids_init; fsapp_testaids_quit stops it,
// called when the main loop has ended.
void fsapp_testaids_quit(void);

// The simulated display (FSAPP_SIMULATED_DISPLAY_HZ, such as 50 or 59.94), a developer and test
// aid for headless runs, where the swap returns at once (Mesa llvmpipe, SDL's offscreen driver): it
// makes the swap behave as on a display with a fixed refresh rate, so that timing with vsync on
// can be traced. While the swap interval is not 0, the main loop holds the swap until the next
// tick of that rate (fsapp_testaids_get_next_simulated_tick_ns), and that tick is the frame's host
// vertical blank (estimated); with swap interval 0 the swap is not held, as on a compositing
// desktop. The ticks are a fixed grid from when the main loop started, so the rate is exact and
// does not drift with the swaps. The host refresh rate told to the display hooks is the simulated
// one.
bool fsapp_testaids_is_display_simulated(void);
double fsapp_testaids_get_simulated_refresh_rate_hz(void);

// The simulated display's first tick after now_ns (on SDL's clock, in nanoseconds). Each tick is
// worked out from the first tick and its number, and rounded to whole nanoseconds, so the ticks do
// not drift.
int64_t fsapp_testaids_get_next_simulated_tick_ns(int64_t now_ns);

// Late swaps on the simulated display (FSAPP_SIMULATED_DISPLAY_LATE_US=<us>): how much later than
// the tick at tick_ns the held swap returns, in nanoseconds. That is the given time on about half
// the ticks, in a fixed pseudo-random pattern, and 0 on the others (and always 0 without the
// variable), so the swap returns, and its host vertical blank is taken, in two clusters, as
// measured on a MacBook Pro after a while without input. It tests that the pacer's planned starts
// do not follow late swaps.
int64_t fsapp_testaids_get_simulated_late_swap_ns(int64_t tick_ns);

// The compositor's lead on the simulated display (FSAPP_SIMULATED_DISPLAY_LEAD_US=<us>), in
// nanoseconds, 0 without it: the held swap returns that long before its tick (and that is its host
// vertical blank in the frame record, as a swap returns at the compositor's frame callback under
// Wayland), and the tick is reported as its presentation, a swap later
// (fsapp_testaids_take_simulated_presentation), as presentation feedback would report it. It
// tests the predicted host vertical blank's presentation path, with its lead, headless.
int64_t fsapp_testaids_get_simulated_lead_ns(void);

// With the simulated display's lead (fsapp_testaids_get_simulated_lead_ns): notes that swap
// swap_number was held for the tick at tick_ns (its presentation), or that a swap was not held
// (tick_ns 0: swap interval 0), and fills in the presentation of the swap held before it, if any,
// returning whether it did. Drawing side only.
bool fsapp_testaids_take_simulated_presentation(int64_t swap_number, int64_t tick_ns,
                                                fsapp_frame_presented_t *presented);

// The simulated drawing delay (FSAPP_SIMULATED_RENDER_US=<us>, or <us>@<swap number>), a test aid
// for headless runs: how much later (in nanoseconds) than its rendering is done the swap with this
// number (counted from 1, as the GL grab's swaps) has its rendering finished, as if drawing were
// slow. That is the given time on every swap, or only on the swap given after the @ (and 0 on the
// others, and always without the variable). The main loop waits for it before it takes the frame
// record's rendering finished, so the pacer sees a slow drawing. It tests missed swap deadlines and
// the slot margin.
int64_t fsapp_testaids_get_simulated_render_delay_ns(int64_t swap_number);

// Overrides the display scale SDL gives (display_scale) with the test display scale
// (FSAPP_TEST_DISPLAY_SCALE=<scale>, such as 1.5 or 2), a test aid for headless runs, where SDL's
// offscreen driver has a display scale of 1; without the variable, returns display_scale as it is.
// fsapp asks wherever it reads or predicts the display scale, so that the configures, the surface
// scales, the window's hit test and fsgui all follow it. The host window's size in device pixels
// and its pixel density are SDL's. The variable is read at the first call (logged there when set);
// a value that is not a scale more than 0 is logged and ignored. Main thread only.
float fsapp_testaids_override_display_scale(float display_scale);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_TESTAIDS_H */
