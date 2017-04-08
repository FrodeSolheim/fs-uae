#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu.h>
#include <fs/emu/video.h>
#include <stdio.h>
#include <string.h>
#include <fs/glib.h>
#include <fs/ml.h>
#include <fs/thread.h>
#include <math.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#include "audio.h"
#include "emu_lua.h"
#include "font.h"
#include "libfsemu.h"
#include "menu.h"
#include "texture.h"
#include "theme.h"
#include "util.h"
#include "video.h"
#include "video_buffer.h"
#include "xml_shader.h"

static int g_fs_emu_aspect_correction = 0;
int g_fs_emu_disable_aspect_correction = 0;

int g_fs_emu_video_format = 0;
int g_fs_emu_video_bpp = 0;
int g_fs_emu_texture_format = 0;

int g_fs_emu_scanlines = 0;
int g_fs_emu_scanlines_dark = 255 * 0.10;
int g_fs_emu_scanlines_light = 255 * 0.05;

fs_emu_zoom_function g_toogle_zoom_function = NULL;

int g_fs_emu_video_debug = 0;
int g_fs_emu_video_fullscreen = 0;
char *g_fs_emu_video_fullscreen_mode_string = NULL;
int g_fs_emu_video_fullscreen_mode = -1;
int g_fs_emu_video_crop_mode = 1;
int g_fs_emu_screenshot = 0;

int g_fs_emu_benchmarking = 0;
int64_t g_fs_emu_benchmark_start_time = 0;
int g_fs_emu_total_emu_frames = 0;
int g_fs_emu_total_sys_frames = 0;

#define FRAME_TIME_LIST_COUNT 256
// time is specified in microseconds
fs_emu_stat_queue g_fs_emu_emu_frame_times = {};
fs_emu_stat_queue g_fs_emu_emu2_frame_times = {};
fs_emu_stat_queue g_fs_emu_sys_frame_times = {};
int g_fs_emu_lost_frames = 0;
int g_fs_emu_repeated_frames = 0;
int g_fs_emu_lost_vblanks = 0;
int g_fs_emu_audio_buffer_underruns = 0;
int64_t g_fs_emu_lost_frame_time = 0;
int64_t g_fs_emu_repeated_frame_time = 0;
int64_t g_fs_emu_video_mode_change_time = 0;

// this is used to make sure that changes to menu etc is not done while
// rendering it... handling of input events for menu actions must be done
// when holding this lock
static fs_mutex *g_video_render_mutex;

int g_fs_emu_video_sync_to_vblank = 0;
int g_fs_emu_video_allow_full_sync = 0;
int g_fs_emu_video_frame_rate_host = 0;

// this is the target frame rate for the video (emulator output)
static double g_video_frame_rate = 0;

// this is the aspect ratio of the video frame from the emulator, defaults
// to 1.0 (1:1 pixels)
static double g_video_aspect_ratio = 1.0;

//static int g_emu_video_struct_seq_no = 0;
static GQueue *g_emu_video_struct_queue = NULL;
static fs_mutex *g_emu_video_struct_mutex = NULL;

void fs_emu_set_toggle_zoom_function(fs_emu_zoom_function function) {
    g_toogle_zoom_function = function;
}

void fs_emu_toggle_zoom(int flags) {
    if (g_toogle_zoom_function) {
        g_toogle_zoom_function(flags);
    }
}

void fs_emu_set_pixel_aspect_ratio(double ratio) {
    g_video_aspect_ratio = ratio;
}

double fs_emu_get_video_frame_rate(void)
{
    return g_video_frame_rate;
}

void fs_emu_set_video_frame_rate(double frame_rate)
{
    static double last_frame_rate = 0;
    static int last_frame_rate_host = 0;
    if (frame_rate == last_frame_rate
            && last_frame_rate_host == g_fs_emu_video_frame_rate_host) {
        return;
    }
    last_frame_rate = frame_rate;
    last_frame_rate_host = g_fs_emu_video_frame_rate_host;
    int frame_rate_i = round(frame_rate);
    fs_log("[VIDEO] fs_emu_set_video_frame_rate: %0.2f (%d)\n",
           frame_rate, frame_rate_i);
    g_video_frame_rate = frame_rate;

    fs_log("[DISPLAY] Sync: g_fs_emu_video_sync_to_vblank = %d\n",
            g_fs_emu_video_sync_to_vblank);
    if (g_fs_emu_video_sync_to_vblank) {
        fs_log("[DISPLAY] Sync: g_fs_emu_video_allow_full_sync = %d\n",
                g_fs_emu_video_allow_full_sync);
        if (g_fs_emu_video_allow_full_sync) {
            if (frame_rate && (frame_rate_i == g_fs_emu_video_frame_rate_host
                    || frame_rate_i == g_fs_emu_video_frame_rate_host + 1)) {
                fs_log("[DISPLAY] Sync: Frame rate (%0.2f) close enough to "
                       "screen refresh (%d)\n",
                       frame_rate, g_fs_emu_video_frame_rate_host);
                fs_ml_video_sync_enable(1);
            } else {
                fs_log("[DISPLAY] Sync: Frame rate (%0.2f) does not equal "
                       "screen refresh (%d)\n",
                       frame_rate, g_fs_emu_video_frame_rate_host);
                fs_ml_video_sync_enable(0);
            }
        }
    }
}

static void initialize()
{
    static int initialized = 0;
    if (initialized == 1) {
        return;
    }
    initialized = 1;
    fs_emu_stat_queue_init(&g_fs_emu_emu_frame_times, FRAME_TIME_LIST_COUNT);
    fs_emu_stat_queue_init(&g_fs_emu_emu2_frame_times, FRAME_TIME_LIST_COUNT);
    fs_emu_stat_queue_init(&g_fs_emu_sys_frame_times, FRAME_TIME_LIST_COUNT);
}

int fs_emu_video_get_aspect_correction() {
    return g_fs_emu_aspect_correction && !g_fs_emu_disable_aspect_correction;
}

void fs_emu_video_set_aspect_correction(int correct) {
    g_fs_emu_aspect_correction = correct;
}

void fs_emu_video_render_mutex_lock() {
    if (!g_video_render_mutex) {
        return;
    }
    fs_mutex_lock(g_video_render_mutex);
}

void fs_emu_video_render_mutex_unlock() {
    if (!g_video_render_mutex) {
        return;
    }
    fs_mutex_unlock(g_video_render_mutex);
}

void fse_init_video_2(void)
{
    fs_log("[FSE] fse_init_video_2\n");
    fse_init_video_options();

    g_video_render_mutex = fs_mutex_create();
    g_emu_video_struct_queue = g_queue_new();
    g_emu_video_struct_mutex = fs_mutex_create();
}

int fs_emu_get_video_format()
{
    return g_fs_emu_video_format;
}

int fs_emu_get_texture_format()
{
    return g_fs_emu_texture_format;
}

void fse_init_video_opengl() {
    fs_log("fse_init_video_opengl\n");
    fs_emu_initialize_opengl();
    initialize();
    fs_emu_menu_init_opengl();
#ifdef WITH_XML_SHADER
    fs_emu_xml_shader_init();
#endif
#ifdef WITH_LUA
    fs_emu_lua_run_handler("on_fs_emu_init_video");
#endif
}

void fs_emu_toggle_fullscreen(void)
{
    fs_ml_toggle_fullscreen();
}

static int64_t g_frame_time_last = 0;
static int64_t g_frame_time_first = 0;

void fs_emu_update_video_stats_1() {
    int t = fs_emu_monotonic_time();
    if (g_frame_time_first == 0) {
        return;
    }
    int time_ms = (t - g_frame_time_first) / 1000;

    int dt = (int) (time_ms - g_frame_time_last);
    // more than 5 seconds => do not record entry (abnormality)
    fs_emu_stat_queue_add_entry(&g_fs_emu_emu2_frame_times, dt, 5 * 1000);
}

void fs_emu_update_video_stats_2() {
    int t = fs_emu_monotonic_time();
    if (g_frame_time_first == 0) {
        g_frame_time_first = t;
    }
    int time_ms = (t - g_frame_time_first) / 1000;
    fs_emu_audio_video_sync(time_ms);

    int dt = (int) (time_ms - g_frame_time_last);
    // more than 5 seconds => do not record entry (abnormality)
    fs_emu_stat_queue_add_entry(&g_fs_emu_emu_frame_times, dt, 5 * 1000);
    g_frame_time_last = time_ms;
}

static void update_video_stats_system_video()
{
    if (g_fs_emu_benchmark_start_time > 0) {
        g_fs_emu_total_sys_frames++;
    }

    static int64_t frame_time_first = 0;
    static int64_t frame_time_last = 0;
    int t = fs_emu_monotonic_time();
    if (frame_time_first == 0) {
        frame_time_first = t;
    }
    int time_ms = (t - frame_time_first) / 1000;
    int dt = (int) (time_ms - frame_time_last);
    // more than 5 seconds => do not record entry (abnormality)
    fs_emu_stat_queue_add_entry(&g_fs_emu_sys_frame_times, dt, 5 * 1000);
    frame_time_last = time_ms;
    double refresh_rate = fs_ml_get_refresh_rate();
    // check if we have missed a vblank internal, but only after 2 seconds
    // after first render
    if (time_ms > 2000 && refresh_rate > 0) {
        if (dt > 1.5 * 1000.0 / refresh_rate) {
            g_fs_emu_lost_vblanks++;
        }
    }
}

double fs_emu_get_average_emu_fps() {
    return 1000.0 / (((double) g_fs_emu_emu_frame_times.total) /
            ((double) g_fs_emu_emu_frame_times.count));
}

double fs_emu_get_average_sys_fps() {
    return 1000.0 / (((double) g_fs_emu_sys_frame_times.total) /
            ((double) g_fs_emu_sys_frame_times.count));
}


int g_fs_emu_audio_enabled;

static void update_leds(int64_t t) {
    if (g_fs_emu_video_mode_change_time == 0) {
        // we use this variable to ignore sync warnings for a short while
        // after the emulation has started and/or video mode changes, since
        // it will be temporarily "unstable" then. (normal)
        g_fs_emu_video_mode_change_time = t;
    }

    int vsync_led_state = 0;
    int fps_led_state = 0;
    int audio_led_state = 0;

    double diff;
    int ignore_lossed_frames = 0;
    int ignore_repeated_frames = 0;
    if (fs_ml_get_vblank_sync()) {
        if (g_fs_emu_video_frame_rate_host == 0) {
            // ?
        }
        else if (g_fs_emu_video_frame_rate_host == g_video_frame_rate) {
            // should ideally not lose / get repeated frames
        }
        else if (g_fs_emu_video_frame_rate_host > g_video_frame_rate) {
            ignore_repeated_frames = 1;
        }
        else if (g_fs_emu_video_frame_rate_host < g_video_frame_rate) {
            ignore_lossed_frames = 1;
        }
        diff = g_fs_emu_video_frame_rate_host - fs_emu_get_average_sys_fps();
    }
    else {
        diff = g_video_frame_rate - fs_emu_get_average_sys_fps();
    }

    if (g_fs_emu_video_frame_rate_host == 0) {
        // unknown host frame rate
        diff = 0;
    }
    if (diff < 0) {
        diff = diff * -1;
    }

    if (diff > 0.2) {
        vsync_led_state = 3;
    }
    else if (fs_ml_get_vblank_sync()) {
        if (fs_ml_get_video_sync()) {
            vsync_led_state = 1;
        }
        else {
            vsync_led_state = 2;
        }
    }
    else {
        // leave at 0
    }

    diff = g_video_frame_rate - fs_emu_get_average_emu_fps();
    if (diff < 0) {
        diff = diff * -1;
    }

    if (diff > 0.1) {
        fps_led_state = 3;
    }
    else if (!ignore_lossed_frames &&
            t - g_fs_emu_lost_frame_time < 100000) {
        fps_led_state = 3;
    }
    else if (!ignore_repeated_frames &&
            t - g_fs_emu_repeated_frame_time < 100000) {
        fps_led_state = 3;
    }
    else if (g_video_frame_rate == 60) {
        fps_led_state = 2;
    }
    else {
        fps_led_state = 1;
    }

    if (t - g_fs_emu_audio_buffer_underrun_time < 100000) {
        audio_led_state = 3;
    }
    else {
        audio_led_state = g_fs_emu_audio_stream_playing[0];
    }

    int64_t time_since_change = t - g_fs_emu_video_mode_change_time;
    if (time_since_change < 6000000) { // 6 seconds
        //int state = ((t - g_fs_emu_video_mode_change_time) / 250000) % 2;
        //vsync_led_state = state ? vsync_led_state : 0;
        //fps_led_state = state ? fps_led_state : 0;
        //audio_led_state = state;
        fps_led_state = 0;
    }
    if (time_since_change < 5000000) {
        vsync_led_state = 0;
    }
    if (time_since_change < 2000000) {
        audio_led_state = 0;
    }

    fs_emu_set_overlay_state(FS_EMU_VSYNC_LED_OVERLAY, vsync_led_state);
    fs_emu_set_overlay_state(FS_EMU_FPS_LED_OVERLAY, fps_led_state);
    fs_emu_set_overlay_state(FS_EMU_AUDIO_LED_OVERLAY, audio_led_state);

    // adding 0.1 so 49.9 is rounded up to 50
    int emu_fps = fs_emu_get_average_emu_fps() + 0.1;
    int digit;
    digit = emu_fps / 10;
    if (digit == 0) digit = 10;
    fs_emu_set_overlay_state(FS_EMU_FPS_D1_OVERLAY, digit);
    digit = emu_fps % 10;
    if (digit == 0) digit = 10;
    fs_emu_set_overlay_state(FS_EMU_FPS_D0_OVERLAY, digit);
}

void fs_emu_video_after_update() {
    fs_emu_video_buffer_unlock();
    int64_t t = fs_emu_monotonic_time();

    if (fs_emu_cursor_is_visible_to() > 0) {
        if (fs_emu_cursor_is_visible_to() < fs_emu_monotonic_time()) {
            //fs_log("%lld\n", fs_emu_monotonic_time());
            fs_emu_show_cursor(0);
        }
    }

    update_leds(t);

    update_video_stats_system_video();

    if (g_fs_emu_benchmark_start_time) {
        static int64_t last_report = 0;
        if (t - last_report > 5000000) {
            double ttime = ((t - g_fs_emu_benchmark_start_time) / 1000000.0);
            double sys_fps = g_fs_emu_total_sys_frames / ttime;
            double emu_fps = g_fs_emu_total_emu_frames / ttime;
            //fs_log("average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
            printf("average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
            last_report = t;
        }
    }
}
