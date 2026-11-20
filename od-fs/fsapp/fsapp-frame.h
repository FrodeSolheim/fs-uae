/*
 * The frames of the host window: their phases and their record, the frame wait and wake, and
 * drawing without a pacer (keep drawing and the request for one frame).
 *
 * The main loop (fsapp-mainthread.h) runs the frames; its top comment describes them, and its
 * implementation file implements these calls.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_FRAME_H
#define FSAPP_FRAME_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------
// The phases and the record.

// The phases of a frame (see the top of fsapp-mainthread.h), in order. Their names
// (fsapp_frame_get_phase_name) are the enumerators' in lower case, and are used in the log and the
// performance graph.
typedef enum fsapp_frame_phase {
    FSAPP_FRAME_PHASE_EVENTS,
    FSAPP_FRAME_PHASE_FRAME_WORK,
    FSAPP_FRAME_PHASE_UI_UPLOAD,
    FSAPP_FRAME_PHASE_DRAWING_BELOW,
    FSAPP_FRAME_PHASE_WAIT,
    FSAPP_FRAME_PHASE_DRAWING_PACED,
    FSAPP_FRAME_PHASE_DRAWING_ABOVE,
    FSAPP_FRAME_PHASE_RENDERING_FINISHED,
    FSAPP_FRAME_PHASE_SWAP,
    FSAPP_FRAME_PHASE_AFTER_SWAP,
    FSAPP_FRAME_PHASE_COUNT,
} fsapp_frame_phase_t;

// The phase's name ("events", "frame_work", ...), or "unknown".
const char *fsapp_frame_get_phase_name(fsapp_frame_phase_t phase);

// How the host vertical blank in a frame record is known.
typedef enum fsapp_frame_host_vertical_blank {
    // Not known (the host refresh rate is not known); its time is 0.
    FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN,
    // Estimated: the return of a glFinish() after the swap where that has not been shown to line
    // up with the refresh, or else the swap plus one host refresh period.
    FSAPP_FRAME_HOST_VERTICAL_BLANK_ESTIMATED,
    // Observed: the return of a glFinish() after the swap, on a platform where the performance
    // graph has shown it to line up with the refresh.
    FSAPP_FRAME_HOST_VERTICAL_BLANK_OBSERVED,
} fsapp_frame_host_vertical_blank_t;

// A swap's presentation, as the windowing system reported it (fsapp-presentation.h): when it was
// shown, or that it was discarded (replaced before it was shown, or skipped).
typedef struct fsapp_frame_presented {
    // The swap it is about (the frame record's swap number).
    int64_t swap_number;
    // When it was shown (converted to SDL's clock), 0 when it was discarded.
    int64_t presented_ns;
    // The refresh period the windowing system reported (0 when not known, or discarded).
    int64_t refresh_ns;
    // The display's refresh counter at the presentation (Wayland's sequence, X11 Present's MSC; 0
    // when it has none).
    uint64_t sequence;
    // How it was shown, on Wayland: wp_presentation_feedback's kind bits (FSAPP_FRAME_PRESENTED_*,
    // 0 on X11).
    uint32_t flags;
    // How it was shown, on X11: the Present extension's completion mode
    // (FSAPP_FRAME_PRESENTED_MODE_*, NONE on Wayland).
    int mode;
} fsapp_frame_presented_t;

// The kind bits of a presentation (Wayland's wp_presentation_feedback.kind).
#define FSAPP_FRAME_PRESENTED_VSYNC 0x1
#define FSAPP_FRAME_PRESENTED_HW_CLOCK 0x2
#define FSAPP_FRAME_PRESENTED_HW_COMPLETION 0x4
#define FSAPP_FRAME_PRESENTED_ZERO_COPY 0x8

// The completion modes of an X11 Present (xcb_present_complete_mode_t): copied to the window,
// flipped (scanned out from the swap's buffer), skipped (never shown, replaced by a later one;
// reported as discarded), or copied where a flip was possible. NONE where there is no mode.
#define FSAPP_FRAME_PRESENTED_MODE_NONE -1
#define FSAPP_FRAME_PRESENTED_MODE_COPY 0
#define FSAPP_FRAME_PRESENTED_MODE_FLIP 1
#define FSAPP_FRAME_PRESENTED_MODE_SKIP 2
#define FSAPP_FRAME_PRESENTED_MODE_SUBOPTIMAL_COPY 3

// The most presentations one frame record carries.
#define FSAPP_FRAME_MAX_PRESENTED 4

// A refresh of the display, as the windowing system reported it from the hardware's timestamps
// (fsapp-presentation.h; macOS only, from CVDisplayLink): the display's real vertical blank, not
// tied to a swap.
typedef struct fsapp_frame_display_refresh {
    // When the display refreshed (converted to SDL's clock).
    int64_t refresh_ns;
    // The refresh period the display reported (0 when not known).
    int64_t period_ns;
    // When the windowing system expected a frame drawn at this refresh to be shown (CoreVideo's
    // output time; 0 when not known). Its own estimate, not a reported presentation.
    int64_t output_ns;
} fsapp_frame_display_refresh_t;

// The most display refreshes one frame record carries (the newest ones).
#define FSAPP_FRAME_MAX_DISPLAY_REFRESHES 8

// What happened in a frame, given to the pacer's swapped hook (fsapp-pacer.h). All times are on
// SDL's clock (SDL_GetTicksNS) in nanoseconds. Frame ready and dropped frames are the pacer's own
// and are not in it.
typedef struct fsapp_frame_record {
    // The swaps counted from 1 at the first swap (the GL grab's swap numbers).
    int64_t swap_number;
    // When each phase started (indexed by fsapp_frame_phase_t), in phase order.
    int64_t phase_ns[FSAPP_FRAME_PHASE_COUNT];
    // Rendering finished: when the glFinish() before the swap returned.
    int64_t rendering_finished_ns;
    // When the swap call returned.
    int64_t swap_ns;
    // The host vertical blank (presentation), and how it is known (0 when unknown).
    int64_t host_vertical_blank_ns;
    fsapp_frame_host_vertical_blank_t host_vertical_blank;
    // The UI frame lag: the swaps since the last record of a UI frame was taken in (0 when this
    // frame took one in), and the time from then (the start of that frame's UI upload) to this
    // frame's swap.
    int64_t ui_frame_lag_swaps;
    int64_t ui_frame_lag_ns;
    // The presentations of earlier swaps (sometimes this one) the windowing system reported since
    // the record before, oldest first (fsapp-presentation.h; none where it has no presentation
    // feedback). Traced, and fed by the pacer (fsemu) to its predicted host vertical blank.
    int presented_count;
    fsapp_frame_presented_t presented[FSAPP_FRAME_MAX_PRESENTED];
    // The swaps not yet reported as presented when the record was made, this one included (-1
    // where there is no presentation feedback): 1 when every earlier swap has been shown, more when
    // swaps are queued ahead of the display (each one more frame of latency).
    int presentation_waiting;
    // The display's refreshes the windowing system reported since the record before, oldest first:
    // the newest FSAPP_FRAME_MAX_DISPLAY_REFRESHES when more came (none where it reports none; only
    // macOS so far). Recorded and traced only, as the presentations.
    int display_refresh_count;
    fsapp_frame_display_refresh_t display_refreshes[FSAPP_FRAME_MAX_DISPLAY_REFRESHES];
} fsapp_frame_record_t;

// ----------------------------------------------------------------------------
// The frame wait and wake.

typedef enum fsapp_frame_wait_result {
    // fsapp_frame_wake was called (from any thread).
    FSAPP_FRAME_WAIT_WOKEN,
    // until_ns was reached.
    FSAPP_FRAME_WAIT_TIME_REACHED,
    // Quit was requested: the main loop is to end.
    FSAPP_FRAME_WAIT_QUIT,
} fsapp_frame_wait_result_t;

// Blocks until woken (fsapp_frame_wake), until until_ns (on SDL's clock, SDL_GetTicksNS, in
// nanoseconds) or until quit is requested, and says which. Returns at once with
// FSAPP_FRAME_WAIT_TIME_REACHED (after handling the pending events, in one-thread mode) when
// until_ns has already passed. Called on the drawing side only. In one-thread mode, the events are
// pumped while it blocks, each handled on the host window side (the event hooks, fsapp, the copy
// for the UI thread), and quit is checked after every pump. In render thread mode it runs on the
// render thread and pumps no events (the main thread does): it blocks on the render thread's own
// condition, and quit is when the main loop has ended and asks the render thread to stop. In both,
// it waits coarsely while more than about a millisecond is left and busy-waits the rest. A wake
// that comes while nothing waits may end the next wait at once, so check for what you are waiting
// for before waiting.
fsapp_frame_wait_result_t fsapp_frame_wait(int64_t until_ns);

// Ends the frame wait (fsapp_frame_wait), or the next one if none is in progress, and the main
// loop's idle wait: in render thread mode it wakes both the render thread and the main thread.
// Callable from any thread. Sending on the main channel (g_fsapp_main_channel)
// calls it. The idle wait (no frame needed) blocks until an event comes, so whatever changes what
// the main loop looks at from another thread (makes a frame needed, sets state that a frame hook
// reads, asks to quit) calls it, or the change is seen only when the idle wait next looks again on
// its own, after a while (see IDLE_WAIT_MS in fsapp-mainthread.cpp).
void fsapp_frame_wake(void);

// ----------------------------------------------------------------------------
// Drawing without a pacer (see the top of fsapp-mainthread.h).

// Keep drawing: a counted switch that makes the main loop draw a frame every host refresh period
// while no pacer is set, instead of only when a frame is needed. Whoever needs steady frames takes
// it and releases it again, in pairs, from any thread; both wake the loop. Held by the GL grab
// while a grab is still to come (it counts swaps); the UI thread asks for the frames it needs
// instead (fsapp_pythonthread_request_ui_frame).
void fsapp_frame_take_keep_drawing(void);
void fsapp_frame_release_keep_drawing(void);

// Asks for one frame to be drawn, so that a change is shown without keeping the loop drawing.
// Callable from any thread; wakes the loop. While a pacer is set every frame is drawn anyway.
void fsapp_frame_request(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_FRAME_H */
