/*
 * The pacer: the part of a program with content on its own clock (FS-UAE's emulation, a video
 * player) that decides when each frame of the host window is finished and swapped.
 *
 * The program sets and clears it with fsapp_mainthread_set_pacer (fsapp-mainthread.h, whose top
 * comment describes how the main loop calls it).
 *
 * Its wait hook is where the program hands over its content: it uploads the slices of the paced
 * surface's streamed content as they come and commits a frame once it is whole (fsapp-surface.h,
 * "Streamed content"), so that the main loop draws the new frame into the picture rectangle right
 * after the wait; the bars around the picture were filled before the wait.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_PACER_H
#define FSAPP_PACER_H

#include <stdbool.h>

#include "fsapp-frame.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_surface;

typedef enum fsapp_pacer_wait_result {
    // Draw the paced surface from its new content.
    FSAPP_PACER_NEW_CONTENT,
    // Finish the frame with the paced surface's old content (a repeated frame).
    FSAPP_PACER_NO_NEW_CONTENT,
    // Quit: end the frame without drawing or swapping.
    FSAPP_PACER_QUIT,
} fsapp_pacer_wait_result_t;

typedef struct fsapp_pacer {
    // Called in the wait phase on the drawing side (with the GL context current), once per frame
    // while the pacer is set. Returns when the frame should be finished and swapped, and must do
    // so within a bounded time (about a frame), also when its content does not come: fsapp does
    // not enforce it, but the window stops updating while it waits. Wait with fsapp_frame_wait,
    // which pumps the events, and return FSAPP_PACER_QUIT when it says quit.
    fsapp_pacer_wait_result_t (*wait)(void);
    // Called after each swap on the drawing side while the pacer is set, with the frame's record
    // (only valid during the call), in the phase after the swap. Can be NULL.
    void (*swapped)(const fsapp_frame_record_t *record);
    // The surface it paces (the paced surface), which the main loop draws after the wait from its
    // streamed content (see fsapp_surface_attach_streamed_content) whenever it is visible. The
    // content belongs to the surface: without a pacer, the main loop draws a visible surface that
    // has streamed content all the same, so its old content stays on screen.
    struct fsapp_surface *surface;
    // The swap interval when the pacer is set (see SDL_GL_SetSwapInterval; no adaptive vsync), and
    // whether the pacer follows the host vertical blank, which makes fsapp call glFinish() after
    // the swap to find it (see fsapp_frame_host_vertical_blank_t). Change them with
    // fsapp_mainthread_set_pacer_swap_interval.
    int swap_interval;
    bool follows_host_vertical_blank;
} fsapp_pacer_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_PACER_H */
