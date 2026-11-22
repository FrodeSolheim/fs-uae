/*
 * fsemu's part of fsapp's main loop: the emulation surface, fsemu's hooks and its pacer.
 *
 * The emulation surface has streamed content (fsapp-surface.h, "Streamed content"). fsemu's pacer
 * paces the main loop by the emulated frames: its wait hook takes in their slices and commits each
 * whole frame to fsapp (fsemu-video.h). fsemu's drawing hook refits the picture rectangle to the
 * surface's rectangle in every frame, also while paused, and drops the content when the session
 * has stopped. fsapp draws the content and fills the bars around it with the fill colour; fsemu
 * makes no GL calls.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSEMU_MAINLOOP_H
#define FSEMU_MAINLOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

#include "fsemu-slotplanner.h"

struct fsapp_surface;

// Create the emulation surface (with streamed content: the emulated frames, which fsemu's pacer
// takes in and commits; focusable, and fsapp's emulation surface, fsapp-input.h) and register
// fsemu's shortcuts and its hooks with fsapp's main loop: its event hook (below), frame hook (input
// reconfiguration, setting or clearing fsemu's pacer, and telling fsapp whether a session runs),
// drawing hook (the video setup at the drawing side's first frame, and the picture's fit in every
// frame) and display hook. fsemu's pacer paces the main loop by the emulated frames: it is set from
// the session's first emulated frame until pause or the session's end, and again on resume
// (fsemu_mainloop_set_session and _set_paused); otherwise the main loop runs without a pacer. Call
// on the main thread before the UI thread starts and before the program registers its own hooks.
//
// The event hook gets only the input that fsapp's routing gives the emulation (fsapp-input.h):
// keys and gamepad buttons and axes while the emulation surface has keyboard focus or the input
// grab is applied, mouse input while the grab is applied and the click on the Amiga that turns it
// on (fsemu tells fsapp whether a session runs, for it), each release where its press went;
// and every hotplug event (keyboards, mice, joysticks and gamepads added or removed). It is not
// called for shortcuts, for the menu input (Guide, or Start and Select together: Start pressed
// first reaches it, and gets a release made by fsapp when Select completes the chord), for input
// that goes to a UI surface or nowhere, or for other events.
void fsemu_mainloop_init(void);

// True once fsemu's video is initialised, which its drawing hook does at the main loop's first
// drawing side frame (with the host window and its OpenGL context). The emulation must not post
// frames before. Thread safe.
bool fsemu_mainloop_video_initialized(void);

// No emulation session is coming (a launch failed): the emulation surface keeps its fill colour
// (it gets no content, and fsemu never sets its pacer without a session), and the performance
// graph goes. Thread safe.
void fsemu_mainloop_on_no_session_coming(void);

// The emulation thread's session (from its first emulated frame until it ends) and pause, which
// decide whether fsemu wants its pacer (while the session runs and is not paused). Each change
// wakes the main loop, whose next frame hook sets or clears the pacer. Called by fsemu_frame on
// the emulation thread.
void fsemu_mainloop_set_session(bool running);
void fsemu_mainloop_set_paused(bool paused);

// The emulation's decision whether to follow the host vertical blank with vsync on has changed
// (fsemu_frame_is_following_host_vertical_blank): tells fsapp, so that the main loop's pacer
// follows the host vertical blank (fsapp finds it with a glFinish() after the swap) only while the
// emulation does. Called by fsemu_frame on the emulation thread.
void fsemu_mainloop_on_following_changed(void);

// The slot timing starts afresh (a session start, which fsemu_mainloop_set_session does itself,
// and resuming from pause): the main loop starts its predicted host vertical blank and the slot
// margin afresh, and fsemu_mainloop_get_slot_timing says there is nothing to plan from until the
// main loop has done so. Not on a timeline reset. Thread safe.
void fsemu_mainloop_restart_slot_timing(void);

// The slot timing the main thread publishes for the emulation thread's slot planner
// (fsemu-slotplanner.h) after each swap: a consistent copy of the prediction the slots are planned
// from (the predicted host vertical blank, its interval and its observations, kept plannable
// through the prediction's own restarts by fsemu_slotplanner_follow_prediction, with the
// compositor's lead the swap deadlines are planned that much before it), and the slot
// margin measured from the swapped frames, including the planned frames' swap deadlines, emulation
// done and rendering finished (in their frame info and frame records), both in nanoseconds as the
// slot planner takes them. Returns false when what is published is from before the last restart
// (fsemu_mainloop_restart_slot_timing). Thread safe.
bool fsemu_mainloop_get_slot_timing(fsemu_slotplanner_prediction_t *prediction, int64_t *margin_ns);

// The predicted host vertical blank (fsemu-verticalblank.h), fed from the frame records with vsync
// on (their swap returns, and the presentations they report), in nanoseconds: the predicted one at
// or after at_or_after_ns (0 when there is no prediction), the refresh interval it predicts with
// (0 when there is none), whether it has settled, and whether it is steady (true before there is a
// prediction). The first is drawing side only (the pacer's hooks), like the prediction itself; the
// others are as published after the last swap, for any thread (the emulation thread's start gate
// reads the interval and whether it has settled, and its decision to follow the host vertical
// blank whether it is steady).
int64_t fsemu_mainloop_get_predicted_host_vertical_blank_ns(int64_t at_or_after_ns);
double fsemu_mainloop_get_predicted_host_refresh_interval_ns(void);
bool fsemu_mainloop_is_predicted_host_vertical_blank_settled(void);
bool fsemu_mainloop_is_predicted_host_vertical_blank_steady(void);

// The emulation surface: one for the whole process, created by fsemu_mainloop_init.
struct fsapp_surface *fsemu_mainloop_emulation_surface(void);

void fsemu_mainloop_handle_event(SDL_Event *event);

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_MAINLOOP_H */
