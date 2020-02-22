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
    double frame_hz;
    bool frame_warp;

    int overshoot_us;
    int wait_us;
    int gui_us;  // Only used when emulation thread == video thread
    int emu_us;
    int render_us;  // Only used when emulation thread == video thread
    int sleep_us;
    int extra_us;
    int other_us;

    int64_t origin_at;
    int64_t began_at;
    int64_t rendered_at;
    int64_t swapped_at;
} fsemu_video_frame_stats_t;

#define FSEMU_VIDEO_RENDERER_SDL 0
#define FSEMU_VIDEO_RENDERER_OPENGL 1

#ifdef __cplusplus
extern "C" {
#endif

// This - if called - must be called before fsemu_video_init.
void fsemu_video_set_renderer(int renderer);

// Can be called before fsemu_video_init and fsemu_window_init.
void fsemu_video_disallow_vsync(int disallow_vsync);

// Can be called before fsemu_video_init and fsemu_window_init.
void fsemu_video_set_vsync(int vsync);

// Initialize the video subsystem. Can safely be called more than once.
void fsemu_video_init(void);

// ---------------------------------------------------------------------------

// This is not for the renderer. The video thread has its own copy.

void fsemu_video_drawable_size(fsemu_size_t *size);

void fsemu_video_set_drawable_size(fsemu_size_t *size);

// ---------------------------------------------------------------------------
#if 0
// Copy of window size, safe to call from the video thread.
void fsemu_video_thread_window_size(fsemu_size_t *size);

// Copy of client area, safe to call from the video thread.
void fsemu_video_thread_client_area(fsemu_rect_t *rect);

// Copy of video area, safe to call from the video thread.
void fsemu_video_thread_video_area(fsemu_rect_t *rect);

// Copy of video rect, safe to call from the video thread. This is the video
// output rectangle in window coordinates.
void fsemu_video_thread_video_rect(fsemu_rect_t *rect);

void fsemu_video_set_data_from_ui_thread(fsemu_size_t *window_size,
                                         fsemu_rect_t *client_area,
                                         fsemu_rect_t *video_area,
                                         fsemu_rect_t *video_rect);
#endif
// ---------------------------------------------------------------------------

void fsemu_video_set_size_2(int width, int height);

int fsemu_video_vsync(void);
void fsemu_video_toggle_vsync(void);
bool fsemu_video_vsync_prevented(void);

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

// void fsemu_video_fix_right_edge(uint8_t *pixels, int vy, int vw, int vh, int
// tw, int wh); void fsemu_video_fix_bottom_edge(uint8_t *pixels, int vw, int
// vh, int tw, int wh);

#endif

#ifdef __cplusplus
}
#endif

extern bool fsemu_video_log_enabled;

#define fsemu_video_log(format, ...)                         \
    if (fsemu_video_log_enabled) {                           \
        fsemu_log("[FSEMU] [VIDEO] " format, ##__VA_ARGS__); \
    }

// FIXME: Move to some color module?

// #define FSEMU_RGB(c) ((((uint32_t) c) >> 8) | (0xff - (c & 0xff)) << 24)
#define FSEMU_RGB(c)                                                    \
    (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | \
     0xff000000)

#define FSEMU_RGBA(c)                                     \
    (((c & 0xff000000) >> 24) | ((c & 0x00ff0000) >> 8) | \
     ((c & 0x0000ff00) << 8) | ((c & 0x000000ff) << 24))

#define FSEMU_RGB_A(c, a)                                               \
    (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | \
     ((a & 0x000000ff) << 24))

static inline void fsemu_color_set_alpha(uint32_t *color, int alpha)
{
    *color = (*color & 0xffffff) | ((alpha & 0x000000ff) << 24);
}

#endif  // FSEMU_VIDEO_H_
