/*
 * Presentation feedback's backends (private to fsapp-presentation*.cpp): one per windowing system
 * that reports presentations or the display's refreshes, chosen by SDL's video driver.
 *
 * A backend uses the windowing system's client library with dlopen (on Linux, as SDL or the GL
 * driver has already loaded it, with RTLD_NOLOAD; on macOS the system framework), so FS-UAE does
 * not link it, and on Linux does not need its headers to build.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSAPP_PRESENTATION_BACKEND_H
#define FSAPP_PRESENTATION_BACKEND_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL3/SDL.h>

#include "fsapp-frame.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fsapp_presentation_backend {
    // Sets up the feedback for the window; logs why and returns false when it cannot.
    bool (*init)(SDL_Window *window);
    // Asks for feedback on the window's next swap, swap swap_number (called just before it).
    void (*request)(SDL_Window *window, int64_t swap_number);
    // Handles what the windowing system has reported (reading it without waiting), adding each
    // presentation with fsapp_presentation_add_done and each display refresh with
    // fsapp_presentation_add_display_refresh.
    void (*poll)(void);
    // The swaps asked about that have not been reported yet (-1 for a backend that reports no
    // presentations).
    int (*get_waiting)(void);
    // Frees what init made, and drops the feedback still to come. Also called after a failed init.
    void (*quit)(void);
} fsapp_presentation_backend_t;

// The backends, NULL where not built (Wayland and X11 only on Linux, macOS only on macOS).
const fsapp_presentation_backend_t *fsapp_presentation_get_wayland_backend(void);
const fsapp_presentation_backend_t *fsapp_presentation_get_x11_backend(void);
const fsapp_presentation_backend_t *fsapp_presentation_get_macos_backend(void);

// Adds a reported presentation, for fsapp_presentation_take.
void fsapp_presentation_add_done(const fsapp_frame_presented_t *presented);

// Adds a reported display refresh, for fsapp_presentation_take_display_refreshes.
void fsapp_presentation_add_display_refresh(const fsapp_frame_display_refresh_t *refresh);

// A time on the clock clock_id (a clockid_t, such as CLOCK_MONOTONIC, or on macOS
// CLOCK_UPTIME_RAW), in nanoseconds, moved to
// SDL's clock by the two clocks' difference now (0 when the clock cannot be read). Only for times
// a little in the past, so that the clocks' drift does not matter.
int64_t fsapp_presentation_to_sdl_clock(int clock_id, int64_t time_ns);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_PRESENTATION_BACKEND_H */
