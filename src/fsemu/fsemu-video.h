#ifndef FSEMU_VIDEO_H_
#define FSEMU_VIDEO_H_

#include "fsemu/fsemu-common.h"
#include "fsemu/fsemu-gui.h"
#include "fsemu/fsemu-types.h"

typedef struct {
    int layer;
    void *buffer;
    int stride;
    int depth;
    int width;
    int height;
    int partial;  // how much of the height is valid
    double frequency;
    fsemu_rect limits;
} fsemu_video_frame_t;

typedef struct {
    int wait_us;
    int emu_us;
    int sleep_us;
    int extra_us;
    int other_us;
} fsemu_video_frame_stats_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_video_init(int flags);
void fsemu_video_work(int timeout_us);

bool fsemu_video_ready(void);

void fsemu_video_display(void);

void fsemu_video_render_gui_early(fsemu_gui_item_t *items);
void fsemu_video_render(void);
void fsemu_video_render_gui(fsemu_gui_item_t *items);

void fsemu_video_frame_stats(int frame, fsemu_video_frame_stats_t *stats);

// From main or video thread

fsemu_video_frame_t *fsemu_video_get_frame(int timeout_us);

// These functions can be called from other threads (i.e. emulation threads)

static inline fsemu_video_frame_t *fsemu_video_alloc_frame(void)
{
    return (fsemu_video_frame_t *) malloc(sizeof(fsemu_video_frame_t));
}
static inline fsemu_video_frame_t *fsemu_video_free_frame(void)
{
    return (fsemu_video_frame_t *) malloc(sizeof(fsemu_video_frame_t));
}

void fsemu_video_post_frame(fsemu_video_frame_t *frame);

#ifdef FSEMU_INTERNAL

/**
 * Called from fsemu_frame_end (from emulation thread).
 */
void fsemu_video_end_frame(void);

#endif

#ifdef __cplusplus
}
#endif

#define fsemu_video_log(format, ...) \
    fsemu_log("[FSEMU] [VIDEO] " format, ##__VA_ARGS__)

// #define FSEMU_RGB(c) ((((uint32_t) c) >> 8) | (0xff - (c & 0xff)) << 24)
#define FSEMU_RGB(c)                                                    \
    (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | \
     0xff000000)

#define FSEMU_RGBA(c)                                     \
    (((c & 0xff000000) >> 24) | ((c & 0x00ff0000) >> 8) | \
     ((c & 0x0000ff00) << 8) | ((c & 0x000000ff) << 24))

#endif  // FSEMU_VIDEO_H_
