/*
 * Presentation feedback: when the windowing system actually showed each swap, as it reports it.
 *
 * Before each swap, the drawing side asks for feedback on it, and the windowing system later
 * reports when it was presented (or that it was discarded: replaced before it was shown). On Linux,
 * by SDL's video driver: on wayland, the presentation-time protocol (wp_presentation;
 * fsapp-presentation-wayland.cpp); on x11 (also XWayland), the Present extension's completion
 * events for the GL driver's presents (fsapp-presentation-x11.cpp). The client libraries are not
 * linked, and their headers are not needed: they are used as SDL or the GL driver loaded them
 * (fsapp-presentation-backend.h).
 *
 * macOS has no presentation feedback for OpenGL (only Metal's drawables report when they were
 * shown). There, on SDL's cocoa driver, the display's refreshes are reported instead: the real
 * vertical blanks, from the hardware's timestamps (CVDisplayLink, on the display SDL's own vsync
 * follows; fsapp-presentation-macos.cpp), but not which swap was shown at which. Elsewhere it does
 * nothing.
 *
 * The presentations and display refreshes go into the frame records (fsapp-frame.h) and the timing
 * trace, and the performance graph marks them. The pacer uses the presentations for timing: fsemu
 * fits its predicted host vertical blank to them. The display refreshes are not used for timing
 * yet. The host vertical blank in the frame record is found as before (the swap's return).
 *
 * Called on the drawing side only (one thread at a time).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSAPP_PRESENTATION_H
#define FSAPP_PRESENTATION_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL3/SDL.h>

#include "fsapp-frame.h"

#ifdef __cplusplus
extern "C" {
#endif

// Asks for feedback on the presentation of the window's next swap, which is swap swap_number.
// Called just before the swap (the feedback is for the surface's next commit, which the swap
// makes). The first call sets the feedback up, when the video driver has it, and logs whether it
// did; it does nothing on the platforms without it.
void fsapp_presentation_request(SDL_Window *window, int64_t swap_number);

// Takes the presentations reported since the last call (those already received from the windowing
// system; it does not wait), oldest first, at most max of them, and returns how many. Those that
// do not fit are taken by the next call.
int fsapp_presentation_take(fsapp_frame_presented_t *presented, int max);

// The display refreshes reported since the last call, oldest first: the newest max of them, the
// older ones dropped. Those received by the last fsapp_presentation_take (call it first).
int fsapp_presentation_take_display_refreshes(fsapp_frame_display_refresh_t *refreshes, int max);

// The swaps asked about that have not been reported yet (-1 without presentation feedback, also on
// macOS, where only the display's refreshes are reported). Taken
// right after a swap (and fsapp_presentation_take), it counts that swap too.
int fsapp_presentation_get_waiting(void);

// Whether presentation feedback (or on macOS, the display's refreshes) has been set up (after the
// first request).
bool fsapp_presentation_is_available(void);

// Drops the feedback still to come and what was set up, while the window and the windowing
// system's connection still exist (when the drawing side ends).
void fsapp_presentation_quit(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_PRESENTATION_H */
