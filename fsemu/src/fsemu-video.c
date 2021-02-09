#define FSEMU_INTERNAL
#include "fsemu-video.h"

#include "fsemu-frame.h"
#include "fsemu-frameinfo.h"
#include "fsemu-glib.h"
#include "fsemu-glvideo.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layout.h"
#include "fsemu-module.h"
#include "fsemu-mutex.h"
#include "fsemu-option.h"
#include "fsemu-sdl.h"
#include "fsemu-sdlvideo.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-videothread.h"
#include "fsemu-window.h"

int fsemu_video_log_level = FSEMU_LOG_LEVEL_INFO;

#define FSEMU_VIDEO_MAX_FRAME_STATS (1 << 8)  // 256

#define FSEMU_VIDEO_FRAME_STATS(x) \
    fsemu_video.stats[(x) % FSEMU_VIDEO_MAX_FRAME_STATS]

#if 0
struct fsemu_video_thread_data {
    fsemu_size_t window_size;
    fsemu_rect_t client_area;
    fsemu_rect_t video_area;
    fsemu_rect_t video_rect;
};
#endif

static struct {
    bool initialized;
    fsemu_video_driver_t driver;
    fsemu_video_renderer_t renderer;
    bool renderer_decided;
    bool threaded;

    fsemu_video_format_t format;
    fsemu_video_frame_stats_t stats[FSEMU_VIDEO_MAX_FRAME_STATS];
    fsemu_rect_t rect;
    int ready;
    int emu_us_avg;
    int vsync;
    int64_t vsync_time;
    int rendered_frame;
    // 1 - disallow vsync, 2 - tried (and refused) to enable vsync
    int disallow_vsync;

    // Drawable size, used for layout and rendering. Not necessarily the same
    // as window size (for high-DPI windows on some platforms). Note, this
    // variable is not for the renderer. The video thread has its own copy.
    fsemu_size_t drawable_size;

    bool must_render_frame;
    int64_t must_render_frame_until;
    bool did_render_frame;

    fsemu_gui_item_t *gui_snapshot;
    fsemu_mutex_t *gui_snapshot_mutex;

#if 0
    // Mutex used to copy consistent data from UI thread over to video thread.
    fsemu_mutex_t *ui_video_mutex;
    // Data from UI thread
    struct fsemu_video_thread_data ui_thread_d;
    // Data (copy) for UI thread
    struct fsemu_video_thread_data thread_d;
#endif
} fsemu_video;

static GAsyncQueue *fsemu_video_frame_queue;

bool fsemu_video_is_threaded(void)
{
    return fsemu_video.threaded;
}

void fsemu_video_set_renderer(fsemu_video_renderer_t renderer)
{
    fsemu_video.renderer = renderer;
}

fsemu_video_renderer_t fsemu_video_get_renderer(void)
{
    return fsemu_video.renderer;
}

fsemu_video_format_t fsemu_video_format(void)
{
    return fsemu_video.format;
}

void fsemu_video_set_format(fsemu_video_format_t format)
{
    fsemu_video.format = format;
}

static int fsemu_video_default_format(void)
{
#ifdef FSEMU_LINUX_ARM
    return FSEMU_VIDEO_FORMAT_RGB565;
#else
    return FSEMU_VIDEO_FORMAT_BGRA;
#endif
}

// ----------------------------------------------------------------------------

void fsemu_video_drawable_size(fsemu_size_t *size)
{
    *size = fsemu_video.drawable_size;
}

void fsemu_video_set_drawable_size(fsemu_size_t *size)
{
    fsemu_video.drawable_size = *size;
    // Also automatically set layout size to drawable size?
    fsemu_layout_set_size(size);
}

// ----------------------------------------------------------------------------
#if 0
void fsemu_video_set_data_from_ui_thread(fsemu_size_t *window_size,
                                         fsemu_rect_t *client_area,
                                         fsemu_rect_t *video_area,
                                         fsemu_rect_t *video_rect)
{
    fsemu_mutex_lock(fsemu_video.ui_video_mutex);
    fsemu_video.ui_thread_d.window_size = *window_size;
    fsemu_video.ui_thread_d.client_area = *client_area;
    fsemu_video.ui_thread_d.video_area = *video_area;
    fsemu_video.ui_thread_d.video_rect = *video_rect;
    fsemu_mutex_unlock(fsemu_video.ui_video_mutex);
}

static void fsemu_video_copy_thread_data(void)
{
    fsemu_mutex_lock(fsemu_video.ui_video_mutex);
    fsemu_video.thread_d = fsemu_video.ui_thread_d;
    fsemu_mutex_unlock(fsemu_video.ui_video_mutex);
}

void fsemu_video_thread_window_size(fsemu_size_t *size)
{
    *size = fsemu_video.thread_d.window_size;
}

void fsemu_video_thread_client_area(fsemu_rect_t *rect)
{
    *rect = fsemu_video.thread_d.client_area;
}

void fsemu_video_thread_video_area(fsemu_rect_t *rect)
{
    *rect = fsemu_video.thread_d.video_area;
}

void fsemu_video_thread_video_rect(fsemu_rect_t *rect)
{
    *rect = fsemu_video.thread_d.video_rect;
}
#endif
// ----------------------------------------------------------------------------

void fsemu_video_set_size_2(int width, int height)
{
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
        fsemu_glvideo_set_size_2(width, height);
    }
}

int fsemu_video_vsync(void)
{
    return fsemu_video.vsync;
}

void fsemu_video_disallow_vsync(int disallow_vsync)
{
    fsemu_video.disallow_vsync = disallow_vsync;
}

bool fsemu_video_vsync_prevented(void)
{
    return fsemu_video.disallow_vsync == 2;
}

void fsemu_video_set_vsync(int vsync)
{
    // fsemu_log("Set vsync=%d\n", vsync);
    if (vsync && fsemu_video.disallow_vsync) {
        // fsemu_log("Set vsync=%d disabled\n", vsync);
        fsemu_video_log("Wanted to enable vsync, but disabled by emulator");
        fsemu_video.disallow_vsync = 2;
        return;
    }
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

// void fsemu_video_set_vsync_time(int frame_number, int64_t vsync_time)
// {
//     // FSEMU_VIDEO_FRAME_STATS(frame_number).vsync_at = vsync_time;
//     fsemu_video_set_frame_vsync_at(frame_number, vsync_time);

//     // FIXME: Not nedded?
//     fsemu_video.vsync_time = vsync_time;
//     // fsemu_video_set_frame_swapped_at(fsemu_video_rendered_frame(),
//     vsync_time);
//     fsemu_video_set_frame_swapped_at(fsemu_video_rendered_frame(),
//     vsync_time);
// }

void fsemu_video_background_color_rgb(int *r, int *g, int *b)
{
    *r = 0x0c;
    *g = 0x0c;
    *b = 0x0c;
}

void fsemu_video_work(int timeout_us)
{
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_SDL) {
        fsemu_sdlvideo_work(timeout_us);
    } else if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
        fsemu_glvideo_work(timeout_us);
    }
}

void fsemu_video_render(void)
{
    fsemu_frame_number_rendering = fsemu_frame_number_posted;

    if (fsemu_video_vsync()) {
        // printf("fsemu_video_render (vsync)\n");
    }
    if (fsemu_video_can_skip_rendering_this_frame()) {
        fsemu_video.did_render_frame = false;
    } else {
        fsemu_frame_log_epoch("Render\n");
        if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_SDL) {
            fsemu_sdlvideo_render();
        } else if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
            fsemu_glvideo_render();
        }
        fsemu_video.did_render_frame = true;
    }
    fsemu_video.ready = false;
    // Reset this variable, will be set again if a new (actual) frame is
    // posted.
    // FIXME: Check if this is still needed / used
    fsemu_video.must_render_frame = false;

    fsemu_frame_number_rendered = fsemu_frame_number_rendering;

    fsemu_window_notify_frame_rendered_vt();
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
    if (!fsemu_video.did_render_frame) {
        // Did not render frame, nothing to display
        printf("No frame rendered, skipping display\n");
        if (fsemu_video_vsync()) {
            printf("fsemu_video_display NOT (vsync)\n");
        }
        return;
    }

    fsemu_frame_number_displaying = fsemu_frame_number_rendered;

    if (fsemu_video_vsync()) {
        // printf("fsemu_video_display (vsync)\n");
    }

    fsemu_frame_log_epoch("Display\n");
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_SDL) {
        fsemu_sdlvideo_display();
    } else if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
        fsemu_glvideo_display();
    }

    // fsemu_assert(fsemu_frame_number_displayed == fsemu_frame_number_posted -
    // 1);
    fsemu_frame_number_displayed = fsemu_frame_number_displaying;
}

void fsemu_video_post_frame(fsemu_video_frame_t *frame)
{
    static int64_t last;
    int64_t t = fsemu_time_us();
    // printf("START_OR_POST post %d %d + %lld\n", frame->partial,
    // frame->height, lld((t - last) / 1000));
    last = t;
    // fsemu_assert(fsemu_frame_number_posted == fsemu_frame_number_began - 1);

    // FIXME: fsemu_frame_counter conflict with new vars
    // frame->number = fsemu_frame_counter();
    frame->number = fsemu_frame_number_began;
    g_async_queue_push(fsemu_video_frame_queue, frame);

    // if (frame->partial == 0 || frame->partial == frame->height) {
    //     fsemu_frame_number_posted = fsemu_frame_number_began;
    // }
}

fsemu_video_frame_t *fsemu_video_get_frame(int timeout_us)
{
    fsemu_assert(timeout_us >= -1);
    fsemu_video_frame_t *frame;
    if (timeout_us == 0) {
        // Maybe not necessary, but did not test timeout 0
        frame = g_async_queue_try_pop(fsemu_video_frame_queue);
    } else if (timeout_us == -1) {
        frame = g_async_queue_pop(fsemu_video_frame_queue);
    } else {
        frame = g_async_queue_timeout_pop(fsemu_video_frame_queue, timeout_us);
    }
#if 0
    // See if there are more events, and only consider the last one.
    fsemu_video_frame_t *frame2;
    while ((frame2 = g_async_queue_try_pop(fsemu_video_frame_queue))) {
        // FIXME: Can also check if an entire frame has been skipped here.
        printf("WARNING: Had to skip frame event\n");
        fsemu_video_free_frame(frame);
        frame = frame2;
    }
#endif
    return frame;
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
    fsemu_frame_log_epoch("Render GUI (early)\n");

    // FIXME: Consider moving this elsewhere.
    fsemu_videothread_copy_thread_data();

    if (items == NULL) {
        printf("WARNING: fsemu_video_render_gui_early items=NULL\n");
        return;
    }
    // fsemu_video_log_debug("render_gui_early\n");
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_SDL) {
        fsemu_sdlvideo_render_gui_early(items);
    } else if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
        fsemu_glvideo_render_gui_early(items);
    }
}

void fsemu_video_render_gui(fsemu_gui_item_t *items)
{
    fsemu_frame_log_epoch("Render GUI\n");
    if (items == NULL) {
        printf("WARNING: fsemu_video_render_gui items=NULL\n");
        return;
    }
#if 0
    // fsemu_video_log_debug("render_gui\n");
    printf("fsemu_video_render_gui %p\n", items);
#endif
    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_SDL) {
        fsemu_sdlvideo_render_gui(items);
    } else if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
        fsemu_glvideo_render_gui(items);
    }
}

static void fsemu_video_update_stats(void)
{
    fsemu_thread_assert_emu();

    static int64_t last;
    int64_t now = fsemu_time_us();
#if 0
    if (last == 0) {  // if (fsemu_frame_emu_duration > 100000) {
        fsemu_video_log_debug("FIXME: first timing is off...\n");
        last = now;
        return;
    }
#endif

    // printf("fsemu_video_update_stats frame %d\n", fsemu_frame_counter());

    fsemu_video_frame_stats_t *stats =
        &fsemu_video
             .stats[fsemu_frame_counter_mod(FSEMU_VIDEO_MAX_FRAME_STATS)];

    stats->frame_hz = fsemu_frame_hz;
    // stats->frame_warp = fsemu_frame_warp;
    stats->frame_warp = fsemu_frame_warping();

    stats->overshoot_us = fsemu_frame_overshoot_duration;
    stats->wait_us = fsemu_frame_wait_duration;
    stats->gui_us = fsemu_frame_gui_duration;
    stats->emu_us = fsemu_frame_emu_duration;
    stats->render_us = fsemu_frame_render_duration;
    stats->sleep_us = fsemu_frame_sleep_duration;
    stats->extra_us = fsemu_frame_extra_duration;

    stats->origin_at = fsemu_frame_origin_at;
    stats->began_at = fsemu_frame_begin_at;

    if (last != 0) {
        stats->other_us = (now - last) - stats->overshoot_us - stats->wait_us -
                          stats->emu_us - stats->sleep_us - stats->extra_us -
                          stats->gui_us - stats->render_us;
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
    // Currently only set via fsemu_video_set_frame_rendered_at
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

void fsemu_video_set_frame_vsync_at(int frame, int64_t vsync_at)
{
    // FSEMU_VIDEO_FRAME_STATS(frame).vsync_at = vsync_at;
    FSEMU_FRAMEINFO(frame).vsync_at = vsync_at;
}

void fsemu_video_set_frame_swapped_at(int frame, int64_t swapped_at)
{
    FSEMU_VIDEO_FRAME_STATS(frame).swapped_at = swapped_at;

    FSEMU_FRAMEINFO(frame).swapped_at = swapped_at;
}

#endif

void fsemu_video_end_frame(void)
{
    fsemu_video_update_stats();

    int next_frame = (fsemu_frame_counter() + 1) % FSEMU_VIDEO_MAX_FRAME_STATS;
    fsemu_video.stats[next_frame].rendered_at = 0;
    fsemu_video.stats[next_frame].swapped_at = 0;
}

void fsemu_video_frame_stats(int frame, fsemu_video_frame_stats_t *stats)
{
    // printf("%d:%lld\n", frame, (long long) fsemu_video.stats[frame %
    // FSEMU_VIDEO_MAX_FRAME_STATS].rendered_at);
    memcpy(stats,
           &fsemu_video.stats[frame % FSEMU_VIDEO_MAX_FRAME_STATS],
           sizeof(fsemu_video_frame_stats_t));
}

#if 0
void fsemu_video_fix_right_edge(uint8_t *pixels, int vy, int vw, int vh, int tw, int wh)
{
    if (vw >= tw) {
        if (vw > tw) {
            printf("WARNING: vw > tw\n");
        }
        return;
    }
    // int pitch = tw * 4;
    pixels += (tw * 4) * vy + vw * 4;
    for (int i = vy; i < vh; i++) {
        pixels[4] = pixels[0];
        pixels[5] = pixels[1];
        pixels[6] = pixels[2];
        pixels[7] = pixels[3];
        pixels += tw * 4;
    }
}

void fsemu_video_fix_bottom_edge(uint8_t *pixels, int vw, int vh, int tw, int wh)
{

}

#endif

bool fsemu_video_can_skip_rendering_this_frame(void)
{
    if (fsemu_video.must_render_frame) {
        // printf("must render frame\n");
        return false;
    }
    if (fsemu_time_us() < fsemu_video.must_render_frame_until) {
        printf("fsemu_time_us() < fsemu_video.must_render_frame_until\n");
        return false;
    }
    // FIXME: Possible check state such as adaptive sync / v-sync / fullscreen
    // window mode / focused / minimized in order to determine whether
    // rendering is necessary.
    printf("can skip!\n");
    return true;
}

void fsemu_video_must_render_frame(void)
{
    fsemu_video.must_render_frame = true;
}

void fsemu_video_must_render_frame_until(int64_t until_us)
{
    fsemu_video.must_render_frame_until = until_us;
}

void fsemu_video_force_display(void)
{
    fsemu_frame_number_rendering += 1;
    fsemu_frame_number_rendered += 1;
    fsemu_video.did_render_frame = true;
    fsemu_video_display();
}

fsemu_video_driver_t fsemu_video_driver(void)
{
    return fsemu_video.driver;
}

void fsemu_video_decide_driver(void)
{
    const char *driver = fsemu_option_const_string(FSEMU_OPTION_VIDEO_DRIVER);
    if (driver) {
        fsemu_video_log("Want video driver: %s\n", driver);
    }
    if (0) {
#ifdef FSEMU_OPENAL
    } else if (!driver || strcmp(driver, "openal") == 0) {
        fse_init_openal_audio();
#endif
#ifdef FSEMU_ALSA
    } else if (!driver || strcmp(driver, "alsa") == 0) {
        fsemu_audio_alsa_init();
#endif
#ifdef FSEMU_PULSE
    } else if (!driver || strcmp(driver, "pulse") == 0) {
        fsemu_audio_pulse_init();
#endif
#ifdef FSEMU_SDL
    } else if (!driver || strcmp(driver, "sdl") == 0) {
        fsemu_video.driver = FSEMU_VIDEO_DRIVER_SDL;
        // fsemu_sdlaudio_init();
#endif
    } else {
        fsemu_video.driver = FSEMU_VIDEO_DRIVER_NULL;
        // fsemu_nullaudio_init();
        // fse_init_dummy_audio();
    }

    const char *chosen_driver_name;
    if (fsemu_video.driver == FSEMU_VIDEO_DRIVER_SDL) {
        chosen_driver_name = "SDL";
    } else {
        chosen_driver_name = "Null";
    }

    fsemu_video_log("Video driver: %s\n", chosen_driver_name);
    fsemu_video.renderer_decided = true;
}

// ----------------------------------------------------------------------------

void fsemu_video_set_gui_snapshot_mt(fsemu_gui_item_t *snapshot)
{
    fsemu_thread_assert_main();
    fsemu_mutex_lock(fsemu_video.gui_snapshot_mutex);
    if (fsemu_video.gui_snapshot) {
        fsemu_gui_free_snapshot(fsemu_video.gui_snapshot);
    }
    fsemu_video.gui_snapshot = snapshot;
    fsemu_mutex_unlock(fsemu_video.gui_snapshot_mutex);
}

fsemu_gui_item_t *fsemu_video_get_gui_snapshot_vt(void)
{
    fsemu_thread_assert_video();
    fsemu_mutex_lock(fsemu_video.gui_snapshot_mutex);
    fsemu_gui_item_t *snapshot = fsemu_video.gui_snapshot;
    fsemu_video.gui_snapshot = NULL;
    fsemu_mutex_unlock(fsemu_video.gui_snapshot_mutex);
    return snapshot;
}

void fsemu_video_set_startup_done_mt(void)
{
    if (fsemu_video_is_threaded()) {
        fsemu_videothread_start();
    }
}

// ----------------------------------------------------------------------------

static void fsemu_video_quit(void)
{
}

// ----------------------------------------------------------------------------

void fsemu_video_init(void)
{
    if (FSEMU_MODULE_INIT(video)) {
        return;
    }
    fsemu_layout_init();
    fsemu_frame_init();
    fsemu_videothread_init();

    fsemu_video.gui_snapshot_mutex = fsemu_mutex_create();

    // Video renderer is already known, The fsemu-window module should have
    // called fsemu_video_decide_driver by now.
    fsemu_assert(fsemu_video.renderer_decided);

    // FIXME:
    // fsemu_option_read_bool_default(
    //     FSEMU_OPTION_LOG_VIDEO, &fsemu_video_log_enabled, false);

    fsemu_video_frame_queue = g_async_queue_new();

    // fsemu_video.renderer = FSEMU_VIDEO_RENDERER_GL;
    if (fsemu_video.format == FSEMU_VIDEO_FORMAT_UNKNOWN) {
        fsemu_video.format = fsemu_video_default_format();
    }

    if (fsemu_video.driver == FSEMU_VIDEO_DRIVER_NULL) {
        fsemu_video.renderer = FSEMU_VIDEO_RENDERER_NULL;
    }

    if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_SDL) {
        fsemu_sdlvideo_init();
    } else if (fsemu_video.renderer == FSEMU_VIDEO_RENDERER_GL) {
        if (fsemu_option_enabled(FSEMU_OPTION_VIDEO_THREAD)) {
            fsemu_video.threaded = true;
        }
        fsemu_glvideo_init();
    }

    if (fsemu_option_enabled(FSEMU_OPTION_VSYNC)) {
        fsemu_video_log("Enabling vsync (via option)\n");
        fsemu_video_set_vsync(true);
    }

    // Calling update to ensure v-sync mode is synchronized.
    // FIXME: Should go via fsemu_window
    fsemu_sdlwindow_update();

    // FIXME: Move to here from sdlwindow?
    // if (fsemu_sdlwindow_kmsdrm()) {
    //     // Also setting vsync. FIXME: Unconditionally?
    //     fsemu_video_set_vsync(true);
    // }

    // FIXME: Started by fsemu_helper_startup_loop for now...
    // if (fsemu_video_is_threaded()) {
    //     fsemu_videothread_start();
    // }

#if 0
    fsemu_video.ui_video_mutex = fsemu_mutex_create();
#endif
}
