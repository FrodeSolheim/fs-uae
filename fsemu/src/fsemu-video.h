#ifndef FSEMU_VIDEO_H_
#define FSEMU_VIDEO_H_

#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-gui.h"
#include "fsemu-types.h"
#include "fsemu-util.h"

enum {
    FSEMU_VIDEO_FORMAT_UNKNOWN,
    FSEMU_VIDEO_FORMAT_RGBA,
    FSEMU_VIDEO_FORMAT_BGRA,
    FSEMU_VIDEO_FORMAT_RGB565
};

int fsemu_video_format(void);

/** This can be specified before fsemu_video_init. */
void fsemu_video_set_format(int format);

typedef struct {
    int layer;
    uint8_t *buffer;
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
    // No actual frame data, used in pause mode
    bool dummy;
} fsemu_video_frame_t;

// FIXME: Move to fsemu-frame?
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

typedef enum {
    FSEMU_VIDEO_DRIVER_NULL,
    FSEMU_VIDEO_DRIVER_SDL
} fsemu_video_driver_t;

typedef enum {
    FSEMU_VIDEO_RENDERER_NULL,
    FSEMU_VIDEO_RENDERER_SDL,
    FSEMU_VIDEO_RENDERER_GL
} fsemu_video_renderer_t;

// Deprecated name
#define FSEMU_VIDEO_RENDERER_OPENGL FSEMU_VIDEO_RENDERER_GL

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
    return FSEMU_UTIL_MALLOC0(fsemu_video_frame_t);
    // return (fsemu_video_frame_t *) malloc(sizeof(fsemu_video_frame_t));
}
static inline void fsemu_video_free_frame(fsemu_video_frame_t *frame)
{
    free(frame);
}

void fsemu_video_post_frame(fsemu_video_frame_t *frame);

#ifdef FSEMU_INTERNAL

// Called from fsemu_window_init in order to choose window driver based on
// video driver.
void fsemu_video_decide_driver(void);

// Returns the video driver.
fsemu_video_driver_t fsemu_video_driver(void);

/**
 * Called from fsemu_frame_end (from emulation thread).
 */
void fsemu_video_end_frame(void);

// void fsemu_video_fix_right_edge(uint8_t *pixels, int vy, int vw, int vh, int
// tw, int wh); void fsemu_video_fix_bottom_edge(uint8_t *pixels, int vw, int
// vh, int tw, int wh);

// Use this to manually specify that something has been rendered, so that the
// display function will actually display.
// void fsemu_video_set_did_render();
void fsemu_video_force_display(void);

bool fsemu_video_can_skip_rendering_this_frame(void);

void fsemu_video_must_render_frame(void);
void fsemu_video_must_render_frame_until(int64_t until_us);

#endif

#ifdef __cplusplus
}
#endif

extern bool fsemu_video_log_enabled;

#define fsemu_video_log(format, ...)                         \
    if (fsemu_video_log_enabled) {                           \
        fsemu_log("[FSEMU] [VIDEO] " format, ##__VA_ARGS__); \
    }

#endif  // FSEMU_VIDEO_H_
