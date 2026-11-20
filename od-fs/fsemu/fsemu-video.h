/*
 * fsemu's video: the emulated frames, from the emulation thread to fsapp.
 *
 * The emulation thread posts each emulated frame in slices to a queue (fsemu_video_post_frame).
 * On the main loop's drawing side, fsemu's pacer takes them in from its wait hook
 * (fsemu_video_work) and hands them to fsapp as the emulation surface's streamed content
 * (fsapp-surface.h, "Streamed content"): a frame is begun with its upload region, each slice is
 * uploaded, and at frame ready the frame is committed with its source rectangle (the crop) and its
 * picture rectangle, which fsemu's layout code works out from the crop and fit policies
 * (fsemu-layout.h). fsapp draws it and fills the bars around the picture with the surface's fill
 * colour. fsemu makes no GL calls.
 *
 * Copyright (c) 2019-2026 Frode Solheim
 */

#ifndef FSEMU_VIDEO_H
#define FSEMU_VIDEO_H

#include "fs/log.h"
#include "fsapp-util.h"
#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-layout.h"
#include "fsemu-types.h"

// ----------------------------------------------------------------------------
// Types
// ----------------------------------------------------------------------------

typedef enum {
    FSEMU_VIDEO_FORMAT_UNKNOWN,
    FSEMU_VIDEO_FORMAT_BGRA,
    FSEMU_VIDEO_FORMAT_RGB565
} fsemu_video_format_t;

struct fsemu_video_frame_t;

typedef struct fsemu_video_frame_t {
    int layer;
    uint8_t *buffer;
    int stride;
    int depth;
    int width;
    int height;
    int partial; // how much of the height is valid
    double frequency;
    fsemu_rect_t limits;
    int flags;
    // The emulated frame whose lines the picture has, set by the client before posting it
    // (fsemu_video_get_posting_frame_number).
    int number;

    void (*finalize)(struct fsemu_video_frame_t *frame);
    void *finalize_data;
} fsemu_video_frame_t;

// FIXME: Move to fsemu-frame?
#define FSEMU_FRAME_FLAG_TURBO (1 << 0)
// The frame was cropped under the auto crop policy (fsemu_video_get_frame_crop_policy). Set by
// fsemu_video_post_frame, not by the client.
#define FSEMU_FRAME_FLAG_AUTO_CROP (1 << 1)

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------
// Pre-initialization
// ----------------------------------------------------------------------------

fsemu_video_format_t fsemu_video_format(void);

// This can be specified before fsemu_video_init.
void fsemu_video_set_format(fsemu_video_format_t format);

// ----------------------------------------------------------------------------
// Initialization functions
// ----------------------------------------------------------------------------

// Can be called before fsemu_video_init and fsemu_window_init.
void fsemu_video_disallow_vsync(int disallow_vsync);

// Can be called before fsemu_video_init and fsemu_window_init.
void fsemu_video_set_vsync(int vsync);

// Reads the options the video setup uses, with the modules it sets up first that read options of
// their own (the layout, the frame and the screenshots), once, on the main thread, where fsemu's
// options are read (fsemu-option.h). fsemu_video_init calls it first; in render thread mode, where
// fsemu_video_init runs on the render thread (fsemu's drawing hook), the frame hook calls it
// before. Can safely be called more than once.
void fsemu_video_read_options(void);

// Initialize the video subsystem. Can safely be called more than once.
void fsemu_video_init(void);

// ----------------------------------------------------------------------------
// Other/unsorted...
// ----------------------------------------------------------------------------

void fsemu_video_drawable_size(fsemu_size_t *size);

void fsemu_video_set_drawable_size(fsemu_size_t *size);

// ----------------------------------------------------------------------------

void fsemu_video_set_size_2(int width, int height, int drawable_width, int drawable_height);

int fsemu_video_vsync(void);
void fsemu_video_toggle_vsync(void);
bool fsemu_video_vsync_prevented(void);

// The measured host refresh interval in nanoseconds, rounded down to a whole nanosecond
// (fsemu_video_get_host_refresh_interval_ns), and the host refresh rate it gives in whole Hz.
int64_t fsemu_video_get_vsync_interval_ns(void);
int fsemu_video_vsync_frequency(void);

// The measured host refresh interval in nanoseconds: the trimmed mean of the intervals between the
// host vertical blanks of swapped emulated frames (fsemu_video_set_frame_vsync), unrounded, or 0
// before it is measured. Written on the drawing side, read on any thread.
double fsemu_video_get_host_refresh_interval_ns(void);

// Takes in the next slice of an emulated frame, waiting up to timeout_ns for one (0: only if one
// has come), and hands it to fsapp as the emulation surface's streamed content: at the first slice
// of a frame not begun, the frame is begun with the upload region (the crop with a fixed crop
// policy, the whole frame with auto-crop, the frame's crop policy as its first slice is marked);
// each slice's rows of the region are
// uploaded; at the last slice (frame ready) the frame is committed with its source rectangle and
// picture rectangle. Returns true if it took one in. Drawing side (fsemu's pacer's wait hook).
bool fsemu_video_work(int64_t timeout_ns);

// The picture in this frame, from fsemu's drawing hook (drawing side, before drawing below):
// refits the content shown to the emulation surface's rectangle in this frame under the fit
// policy (also while paused), places the performance graph's sidebars, passes on the filter, and
// drops the content when the session has stopped (and commits no frame while it is). pixel_scale
// is the window's pixels per window coordinate in this frame's snapshot.
void fsemu_video_update_picture(bool session_stopped, double pixel_scale);

// True once fsemu_video_init has been called (on the main thread).
bool fsemu_video_initialized(void);

// Frame ready: the slices of an emulated frame have all been taken in.
bool fsemu_video_ready(void);
void fsemu_video_set_ready(bool ready);

// Takes the ready emulated frame (frame ready, already committed to fsapp, which draws it until the
// next one). Called by fsemu's pacer.
void fsemu_video_take_frame(void);

// void fsemu_video_set_frame_began_at(int frame, int64_t began_at);

// Rendering finished and the swap's return for the frame, from the frame record of the swap that
// first presented it (drawing side), in nanoseconds.
void fsemu_video_set_frame_rendered(int frame, int64_t rendered_ns);
void fsemu_video_set_frame_swapped(int frame, int64_t swapped_ns);
// The frame's host vertical blank (or the swap's return when it is not known), and the predicted
// host vertical blank nearest it (0 when the prediction has not settled), in nanoseconds, for the
// start gate of the frame two later (drawing side).
void fsemu_video_set_frame_vsync(int frame, int64_t vsync_ns, int64_t vsync_predicted_ns);

// From the main thread (the drawing side). Waits up to timeout_ns for a slice (0: only if one has
// come; -1: until one comes).

fsemu_video_frame_t *fsemu_video_get_frame(int64_t timeout_ns);

// These functions can be called from other threads (i.e. emulation threads)

static inline fsemu_video_frame_t *fsemu_video_alloc_frame(void)
{
    return FSAPP_UTIL_MALLOC0(fsemu_video_frame_t);
    // return (fsemu_video_frame_t *) malloc(sizeof(fsemu_video_frame_t));
}

// static inline void fsemu_video_free_frame(fsemu_video_frame_t *frame)
// {
//     free(frame);
// }

void fsemu_video_finalize_and_free_frame(fsemu_video_frame_t *frame);

// The emulated frame whose lines a picture posted now has, which the client tags it with
// (frame->number): the frame being emulated (fsemu_frame_number_began) for a picture posted while
// that frame's lines are drawn, in slices up to the last one at the end of its lines, or with
// after_frame_end, the frame before, for a picture posted whole after that frame's end (a few
// lines into the next frame, at the display's vertical sync). With vsync on, the start gate of
// frame N waits for the host vertical blank that shows picture N - 2 (fsemu-frame.cpp), the one
// posted at the end of frame N - 2's lines. Emulation thread.
int fsemu_video_get_posting_frame_number(bool after_frame_end);

// The crop policy of emulated frame `number` (fsemu_video_get_posting_frame_number), by which the
// emulation thread chooses the frame's crop (its limits): read at the frame's first slice (the
// first call for a frame number not seen before) and the same for all its later slices, so that a
// frame is cropped and uploaded under one policy even when the policy changes halfway; a change
// takes effect at the next frame. fsemu_video_post_frame marks each slice with it
// (FSEMU_FRAME_FLAG_AUTO_CROP), and the drawing side decides the frame's upload region by its first
// slice's mark. Emulation thread.
fsemu_layout_crop_policy_t fsemu_video_get_frame_crop_policy(int number);

// Posts a slice of emulated frame frame->number (frame->partial is the rows drawn so far, the
// frame's height for its last slice, or 0 for a whole frame at once) to fsemu's queue, from the
// emulation thread, and wakes the main loop's frame wait so fsemu's pacer takes it in. Each slice
// of a frame must have more rows than the one before. Never blocks: when the main thread falls
// behind, the queue drops the older frames.
void fsemu_video_post_frame(fsemu_video_frame_t *frame);

// The emulated frame whose picture was last posted whole (its last slice, or the whole picture at
// once), -1 before the first. A picture whose last rows are drawn only in the next frame's first
// lines (short frames) is posted whole from the next frame's WinUAE framewait. Emulation thread.
int fsemu_video_get_last_whole_posted_frame(void);

#ifdef FSEMU_INTERNAL

/**
 * Called from fsemu_frame_end (from emulation thread).
 */
void fsemu_video_end_frame(void);

// void fsemu_video_fix_right_edge(uint8_t *pixels, int vy, int vw, int vh, int
// tw, int wh); void fsemu_video_fix_bottom_edge(uint8_t *pixels, int vw, int
// vh, int tw, int wh);

bool fsemu_video_can_skip_rendering_this_frame(void);

void fsemu_video_must_render_frame(void);
void fsemu_video_must_render_frame_until(int64_t until_ns);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_video_log_level;

#define fsemu_video_log(format, ...)                                                               \
    FS_LOG_INFO(fsemu_video_log_level, "[FSE] [VID]", format, ##__VA_ARGS__)

#define fsemu_video_log_debug(format, ...)                                                         \
    FS_LOG_DEBUG(fsemu_video_log_level, "[FSE] [VID]", format, ##__VA_ARGS__)

#define fsemu_video_log_error(format, ...)                                                         \
    FS_LOG_ERROR(fsemu_video_log_level, "[FSE] [VID]", format, ##__VA_ARGS__)

#define fsemu_video_log_info(format, ...)                                                          \
    FS_LOG_INFO(fsemu_video_log_level, "[FSE] [VID]", format, ##__VA_ARGS__)

#define fsemu_video_log_trace(format, ...)                                                         \
    FS_LOG_TRACE(fsemu_video_log_level, "[FSE] [VID]", format, ##__VA_ARGS__)

#define fsemu_video_log_warning(format, ...)                                                       \
    FS_LOG_WARNING(fsemu_video_log_level, "[FSE] [VID]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_VIDEO_H */
