//#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <fs/ml.h>
#include <fs/queue.h>
#include <fs/thread.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#include "audio.h"
#include "font.h"
#include "libfsemu.h"
#include "menu.h"
#include "texture.h"
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
char *g_fs_emu_video_fullscreen_mode = NULL;
int g_fs_emu_video_fullscreen_window = -1;
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
int g_fs_emu_repeated_frames = 0;
int g_fs_emu_lost_frames = 0;
int g_fs_emu_lost_vblanks = 0;
int g_fs_emu_audio_buffer_underruns = 0;

// this is used to make sure that changes to menu etc is not done while
// rendering it... handling of input events for menu actions must be done
// when holding this lock
static fs_mutex *g_video_render_mutex;

// this is the target frame rate for the video (emulator output)
static int g_video_frame_rate = 0;

// this is the aspect ratio of the video frame from the emulator, defaults
// to 1.0 (1:1 pixels)
static double g_video_aspect_ratio = 1.0;

//static int g_emu_video_struct_seq_no = 0;
static fs_queue *g_emu_video_struct_queue = NULL;
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

int fs_emu_get_video_frame_rate() {
    return g_video_frame_rate;
}

void fs_emu_set_video_frame_rate(int frame_rate) {
    g_video_frame_rate = frame_rate;
}

static void initialize() {
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

void fs_emu_video_init() {
    fs_log("fs_emu_video_init\n");
    fs_emu_video_init_options();

    g_video_render_mutex = fs_mutex_create();
    g_emu_video_struct_queue = fs_queue_new();
    g_emu_video_struct_mutex = fs_mutex_create();

    fs_emu_init_render();
}

int fs_emu_get_video_format() {
    return g_fs_emu_video_format;
}

int fs_emu_get_texture_format() {
    return g_fs_emu_texture_format;
}

void fs_emu_video_init_opengl() {
    fs_emu_initialize_opengl();
    initialize();
    fs_emu_menu_init_opengl();
#ifdef WITH_XML_SHADER
    fs_emu_xml_shader_init();
#endif
}

void fs_emu_toggle_fullscreen() {
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

void update_video_stats_system_video() {
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

void fs_emu_video_after_update() {
    fs_emu_video_buffer_unlock();

//void fs_emu_swap_buffers() {
    if (fs_emu_pointer_is_visible_to() > 0) {
        if (fs_emu_pointer_is_visible_to() < fs_emu_monotonic_time()) {
            //fs_log("%lld\n", fs_emu_monotonic_time());
            fs_emu_show_pointer(0);
        }
    }

    update_video_stats_system_video();

    if (g_fs_emu_benchmark_start_time) {
        static int64_t last_report = 0;
        int64_t t2 = fs_emu_monotonic_time();
        if (t2 - last_report > 5000000) {
            double ttime = ((t2 - g_fs_emu_benchmark_start_time) / 1000000);
            double sys_fps = g_fs_emu_total_sys_frames / ttime;
            double emu_fps = g_fs_emu_total_emu_frames / ttime;
            //fs_log("average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
            printf("average fps sys: %0.1f emu: %0.1f\n", sys_fps, emu_fps);
            last_report = t2;
        }
    }
}
