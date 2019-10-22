#define FSEMU_INTERNAL
#include "fsemu-video.h"

#include "fsemu-frame.h"
#include "fsemu-glib.h"
#include "fsemu-glvideo.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layout.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-sdl.h"
#include "fsemu-sdlvideo.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-time.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-window.h"

#define FSEMU_VIDEO_MAX_FRAME_STATS (1 << 8)  // 256

static struct {
    fsemu_video_frame_stats_t stats[FSEMU_VIDEO_MAX_FRAME_STATS];
    fsemu_rect_t rect;
    int ready;
    int emu_us_avg;
    int renderer;
    int vsync;
    int64_t vsync_time;
    int rendered_frame;
} fsemu_video;

bool fsemu_video_log_enabled = false;

static GAsyncQueue *fsemu_video_frame_queue;

void fsemu_video_set_renderer(int renderer)
{
    fsemu_video.renderer = renderer;
}

void fsemu_video_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_layout_init();
    fsemu_frame_init();

    fsemu_option_read_bool_default(
        FSEMU_OPTION_LOG_VIDEO, &fsemu_video_log_enabled, false);
    fsemu_video_frame_queue = g_async_queue_new();

    // fsemu_video.renderer = FSEMU_VIDEO_RENDERER_OPENGL;

    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_OPENGL) {
        fsemu_glvideo_init();
    } else {
        fsemu_sdlvideo_init();
    }
}

int fsemu_video_vsync(void)
{
    return fsemu_video.vsync;
}

void fsemu_video_set_vsync(int vsync)
{
    fsemu_video.vsync = vsync;
}

void fsemu_video_toggle_vsync(void)
{
    fsemu_video_set_vsync(!fsemu_video_vsync());
}

int64_t fsemu_video_vsync_time(void)
{
    return fsemu_video.vsync_time;
}

void fsemu_video_set_vsync_time(int64_t vsync_time)
{
    fsemu_video.vsync_time = vsync_time;
    fsemu_video_set_frame_swapped_at(fsemu_video_rendered_frame(), vsync_time);
}

void fsemu_video_background_color_rgb(int *r, int *g, int *b)
{
    *r = 0x0c;
    *g = 0x0c;
    *b = 0x0c;
}

void fsemu_video_work(int timeout_us)
{
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_OPENGL) {
        fsemu_glvideo_work(timeout_us);
    } else {
        fsemu_sdlvideo_work(timeout_us);
    }
}

void fsemu_video_render(void)
{
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_OPENGL) {
        fsemu_glvideo_render();
    } else {
        fsemu_sdlvideo_render();
    }
    fsemu_video.ready = false;
}

bool fsemu_video_ready(void)
{
    return fsemu_video.ready;
}

void fsemu_video_set_ready(bool ready)
{
    fsemu_video.ready = ready;
}

void fsemu_video_display(void)
{
    // fsemu_video_log("--- display --- [draw]\n");
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_OPENGL) {
        fsemu_glvideo_display();
    } else {
        fsemu_sdlvideo_display();
    }
}

void fsemu_video_post_frame(fsemu_video_frame_t *frame)
{
    frame->number = fsemu_frame_counter();
    g_async_queue_push(fsemu_video_frame_queue, frame);
}

fsemu_video_frame_t *fsemu_video_get_frame(int timeout_us)
{
    if (timeout_us == 0) {
        // Probably not necessary, but did not test timeout 0
        return (fsemu_video_frame_t *) g_async_queue_try_pop(
            fsemu_video_frame_queue);
    }
    return (fsemu_video_frame_t *) g_async_queue_timeout_pop(
        fsemu_video_frame_queue, timeout_us);
}

// FIXME: COPY WINDOW / LAYOUT SIZE on main <-> video sync ??
/*
static void fsemu_video_convert_coordinates(SDL_Rect *out,
                                            fsemu_rect *in,
                                            int coordinates)
{
    // FIXME: Use copy of window size? Layout?
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);

    if (coordinates == FSEMU_COORD_REAL) {
        out->w = in->w;
        out->h = in->h;
        out->x = in->x;
        out->y = in->y;
    } else if (coordinates == FSEMU_COORD_1080P_LEFT) {
        double scale = window_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = in->x * scale;
        out->y = in->y * scale;

    } else if (coordinates == FSEMU_COORD_1080P_RIGHT) {
        double scale = window_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = window_size.w - (1920 - in->x) * scale;
        out->y = in->y * scale;
    }
}
*/

void fsemu_video_render_gui_early(fsemu_gui_item_t *items)
{
    // fsemu_video_log("render_gui_early\n");
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_OPENGL) {
        fsemu_glvideo_render_gui_early(items);
    } else {
        fsemu_sdlvideo_render_gui_early(items);
    }
}

void fsemu_video_render_gui(fsemu_gui_item_t *items)
{
    // fsemu_video_log("render_gui\n");
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_OPENGL) {
        fsemu_glvideo_render_gui(items);
    } else {
        fsemu_sdlvideo_render_gui(items);
    }
}

static void fsemu_video_update_stats(void)
{
    static int64_t last;
    int64_t now = fsemu_time_us();
#if 0
    if (last == 0) {  // if (fsemu_frame_emu_duration > 100000) {
        fsemu_video_log("FIXME: first timing is off...\n");
        last = now;
        return;
    }
#endif

    // printf("fsemu_video_update_stats frame %d\n", fsemu_frame_counter());

    fsemu_video_frame_stats_t *stats =
        &fsemu_video
             .stats[fsemu_frame_counter_mod(FSEMU_VIDEO_MAX_FRAME_STATS)];
    stats->wait_us = fsemu_frame_wait_duration;
    stats->emu_us = fsemu_frame_emu_duration;
    stats->sleep_us = fsemu_frame_sleep_duration;
    stats->extra_us = fsemu_frame_extra_duration;

    stats->origin_at = fsemu_frame_origin_at;
    stats->began_at = fsemu_frame_begin_at;
    stats->rendered_at = 0;
    stats->swapped_at = 0;

    if (last != 0) {
        stats->other_us = (now - last) - stats->wait_us - stats->emu_us -
                          stats->sleep_us - stats->extra_us;
    }

    static fsemu_mavgi_t emu_us_mavgi;
    static int emu_us_mavgi_values[FSEMU_VIDEO_MAX_FRAME_STATS];
    // stats->emu_us_mavg = fsemu_mavgi(
    //     &emu_us_mavgi, emu_us_mavgi_values, 60, stats->emu_us);
    fsemu_video.emu_us_avg = fsemu_mavgi(&emu_us_mavgi,
                                         emu_us_mavgi_values,
                                         FSEMU_VIDEO_MAX_FRAME_STATS,
                                         stats->emu_us);

    last = now;
}

// FIXME: Should probably be fsemu_frame_emutime_avg_us
// FIXME: move
int fsemu_frame_emutime_avg_us(void)
{
    return fsemu_video.emu_us_avg;
}

#if 1

int fsemu_video_rendered_frame(void)
{
    // Currently onlyy set via fsemu_video_set_frame_rendered_at
    return fsemu_video.rendered_frame;
}

void fsemu_video_set_frame_began_at(int frame, int64_t began_at)
{
    fsemu_video.stats[frame % FSEMU_VIDEO_MAX_FRAME_STATS].began_at = began_at;
}

void fsemu_video_set_frame_rendered_at(int frame, int64_t rendered_at)
{
    // printf("set rendered at for frame %d to %lld\n", frame, (long long)
    // rendered_at);
    fsemu_video.rendered_frame = frame;
    fsemu_video.stats[frame % FSEMU_VIDEO_MAX_FRAME_STATS].rendered_at =
        rendered_at;
    // printf("%d:%lld\n", frame, (long long) fsemu_video.stats[frame %
    // FSEMU_VIDEO_MAX_FRAME_STATS].rendered_at);
}

void fsemu_video_set_frame_swapped_at(int frame, int64_t swapped_at)
{
    fsemu_video.stats[frame % FSEMU_VIDEO_MAX_FRAME_STATS].swapped_at =
        swapped_at;
}

#endif

void fsemu_video_end_frame(void)
{
    fsemu_video_update_stats();
}

void fsemu_video_frame_stats(int frame, fsemu_video_frame_stats_t *stats)
{
    // printf("%d:%lld\n", frame, (long long) fsemu_video.stats[frame %
    // FSEMU_VIDEO_MAX_FRAME_STATS].rendered_at);
    memcpy(stats,
           &fsemu_video.stats[frame % FSEMU_VIDEO_MAX_FRAME_STATS],
           sizeof(fsemu_video_frame_stats_t));
}
