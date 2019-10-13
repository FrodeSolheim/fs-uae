#ifndef FSEMU_VIDEO_H_
#define FSEMU_VIDEO_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-types.h"

typedef struct {
    int layer;
    void *buffer;
    int stride;
    int depth;
    int width;
    int height;
    int partial;  // how much of the height is valid
    double frequency;
    fsemu_rect_t limits;
    int flags;
    // This is the frame number, automatically set to the frame counter when
    // posting a frame. The "client" does not have to set it.
    int number;
} fsemu_video_frame_t;

#define FSEMU_FRAME_FLAG_TURBO (1 << 0)

typedef struct {
    int wait_us;
    int emu_us;
    int sleep_us;
    int extra_us;
    int other_us;
    int64_t origin_at;
    int64_t began_at;
    int64_t rendered_at;
    int64_t swapped_at;
} fsemu_video_frame_stats_t;

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_VIDEO_RENDERER_SDL 0
#define FSEMU_VIDEO_RENDERER_OPENGL 1

/** This - if called - must be called before fsemu_video_init. */
void fsemu_video_set_renderer(int renderer);

/** Can be called before fsemu_video_init and fsemu_window_init. */
void fsemu_video_set_vsync(int vsync);

void fsemu_video_init(void);

int fsemu_video_vsync(void);
void fsemu_video_toggle_vsync(void);

int64_t fsemu_video_vsync_time(void);
void fsemu_video_set_vsync_time(int64_t vsync_time);

void fsemu_video_work(int timeout_us);

void fsemu_video_background_color_rgb(int *r, int *g, int *b);

bool fsemu_video_ready(void);
void fsemu_video_set_ready(bool ready);

void fsemu_video_display(void);

void fsemu_video_render_gui_early(fsemu_gui_item_t *items);
void fsemu_video_render(void);
void fsemu_video_render_gui(fsemu_gui_item_t *items);

/** Returns the frame number of the latest rendered frame. */
int fsemu_video_rendered_frame(void);

void fsemu_video_set_frame_began_at(int frame, int64_t began_at);
void fsemu_video_set_frame_rendered_at(int frame, int64_t rendered_at);
void fsemu_video_set_frame_swapped_at(int frame, int64_t swapped_at);

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
