#ifndef FSEMU_VIDEOTHREAD_H_
#define FSEMU_VIDEOTHREAD_H_

// This module contains data which can be safely used from the video renderer,
// whether running in a separate video rendering thread or not. The point is
// that (if threaded), the video renderer should not access variables used by
// the UI/window system, since the renderer could then get inconsistent data.

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the videothread subsystem. Can safely be called more than once.
// Will be called from fsemu_video_init, so no reason to call this from other
// contexts.
void fsemu_videothread_init(void);

void fsemu_videothread_lock(void);
void fsemu_videothread_unlock(void);

#if 0
// Copy of window size, safe to call from the video renderer/thread.
void fsemu_videothread_window_size(fsemu_size_t *size);
#endif

// Copy of drawable size, safe to call from the video renderer/thread.
void fsemu_videothread_drawable_size(fsemu_size_t *size);

// Copy of client area, safe to call from the video renderer/thread.
void fsemu_videothread_client_area(fsemu_rect_t *rect);

// Copy of video area, safe to call from the video renderer/thread.
void fsemu_videothread_video_area(fsemu_rect_t *rect);

// Copy of video rect, safe to call from the video renderer/thread. This is
// the video output rectangle in window coordinates.
void fsemu_videothread_video_rect(fsemu_rect_t *rect);

// Internal function. Automatically acquires the videothread lock.
void fsemu_videothread_copy_thread_data(void);

// Internal function. Remember to acquire the videothread lock first.
void fsemu_videothread_set_data_from_ui_thread(fsemu_size_t *window_size,
                                               fsemu_size_t *drawable_size,
                                               fsemu_rect_t *client_area,
                                               fsemu_rect_t *video_area,
                                               fsemu_rect_t *video_rect);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_VIDEOTHREAD_H_
