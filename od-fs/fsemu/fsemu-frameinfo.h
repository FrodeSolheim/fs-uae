#ifndef FSEMU_FRAMEINFO_H
#define FSEMU_FRAMEINFO_H

#include <stdint.h>

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_FRAMEINFO_COUNT 256

typedef struct {
    double frame_hz;
    bool frame_warp;

    // The frame's time spent in each part (in nanoseconds, as all of fsemu's frame timing).
    int64_t overshoot_ns;
    int64_t wait_ns;
    int64_t gui_ns; // Only used when emulation thread == video thread
    int64_t emu_ns;
    int64_t render_ns; // Only used when emulation thread == video thread
    int64_t sleep_ns;
    int64_t extra_ns;
    int64_t other_ns;

    // The times below are on SDL's clock in nanoseconds (SDL_GetTicksNS), 0 when not set.
    // 1 in vsync_allow_start_ns is not a time but lets the frame start at once.
    volatile int64_t vsync_allow_start_ns;

    // The frame's origin (the moment its times are measured from, x = 0 in the performance graph),
    // its planned start, and when its emulation slot actually started (the overshoot after the
    // planned start), written at the frame's start.
    int64_t origin_ns;
    int64_t planned_start_ns;
    int64_t slot_started_ns;

    int64_t begins_ns;

    int64_t began_ns;

    // When the frame is scheduled to end
    int64_t ends_ns;
    // When the frame actully ended
    int64_t ended_ns;
    // Emulation done: when the emulation thread finished the frame's last line (the slot's actual
    // end), before its last slice is posted. The slot margin is measured from it.
    int64_t emulation_done_ns;

    // While the emulation follows the host vertical blank and the frame's slot was planned
    // (fsemu-slotplanner.h): its swap deadline, and the slot margin it was planned with. Written at
    // the slot start, before the frame's first slice is posted; 0 for frames that were not planned
    // (the start gate's, and every frame when not following).
    int64_t deadline_ns;
    int64_t slot_margin_ns;

    // When the frame's last slice (the whole picture) was posted (written on the emulation thread,
    // before the push that hands it over), and frame ready: when the drawing side had taken it in
    // and committed it (written there).
    int64_t last_slice_posted_ns;
    int64_t frame_ready_ns;

    // Rendering finished and the swap's return, from the frame record of the swap that first
    // presented the frame (written on the main thread).
    int64_t rendered_ns;
    int64_t swapped_ns;
    int64_t vsync_estimated_ns;
    int64_t vsync_ns;
    // The predicted host vertical blank nearest vsync_ns, once the prediction has settled, else 0
    // (written on the main thread, before vsync_ns). The start gate places the planned start from
    // it, so that one late swap does not move the planned start.
    int64_t vsync_predicted_ns;

    bool render_skipped;

    // Events for the performance graph: the frame was used up by a pause (it has no picture), is
    // the session's first, or its start was a timeline reset.
    bool paused;
    bool session_start;
    bool timeline_reset;

} fsemu_frameinfo_t;

extern fsemu_frameinfo_t fsemu_frameinfo[FSEMU_FRAMEINFO_COUNT];

#define FSEMU_FRAMEINFO(frame) (fsemu_frameinfo[(frame) % FSEMU_FRAMEINFO_COUNT])

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_FRAMEINFO_H */
