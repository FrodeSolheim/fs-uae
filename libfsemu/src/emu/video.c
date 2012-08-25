//#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "fs/ml.h"
#include <fs/glee.h>
#include <fs/glu.h>
#include "libfsemu.h"
#include "video.h"
#include "texture.h"
#include "util.h"
#include "menu.h"
#include "audio.h"
#include "font.h"

static int g_fs_emu_aspect_correction = 0;
static int g_fs_emu_disable_aspect_correction = 0;
/*
typedef struct fs_emu_video_buffer {
    int seq;
    void *data;
    int width;
    int height;
    int bpp;
    double aspect;
    fs_emu_rect crop;
} fs_emu_video_buffer;
*/
static fs_emu_video_buffer g_video_buffers[3] = {};
static GMutex* g_video_buffers_mutex = NULL;
static int g_video_buffers_initialized = 0;
static int g_video_buffer_next = 0;
static fs_emu_video_buffer *g_video_buffer_current = g_video_buffers;
static fs_emu_video_buffer *g_video_buffer_locked = NULL;
//static int g_video_buffer_locked = -1;
int g_fs_emu_video_format = 0;
int g_fs_emu_video_bpp = 0;
int g_fs_emu_texture_format = 0;

int g_fs_emu_scanlines = 0;
int g_fs_emu_scanlines_dark = 255 * 0.10;
int g_fs_emu_scanlines_light = 255 * 0.05;
int g_fs_emu_scanlines_dark_i = (255 - 255 * 0.10);
int g_fs_emu_scanlines_light_i = (255 - 255 * 0.05);

fs_emu_zoom_function g_toogle_zoom_function = NULL;

void update_video_stats_1();
void update_video_stats_2();

void fs_emu_set_toggle_zoom_function(fs_emu_zoom_function function) {
    g_toogle_zoom_function = function;
}

void fs_emu_toggle_zoom(int flags) {
    if (g_toogle_zoom_function) {
        g_toogle_zoom_function(flags);
    }
}

int fs_emu_initialize_video_buffers(int width, int height, int bpp) {
    // should only be called once, currently
    if (g_video_buffers_initialized) {
        return 0;
    }
    if (bpp == 0) {
        bpp = g_fs_emu_video_bpp;
    }
    for (int i = 0; i < 3; i++) {
        g_video_buffers[i].width = width;
        g_video_buffers[i].height = height;
        g_video_buffers[i].bpp = bpp;
        g_video_buffers[i].size = width * height * bpp;
        g_video_buffers[i].data = g_malloc0(g_video_buffers[i].size);
        //memset(g_video_buffers[i].data, 0, g_video_buffers[i].size);
        g_video_buffers[i].aspect = 1.0;
        //g_video_buffers[i].buffer_width = width;
        //g_video_buffers[i].buffer_height = height;
    }
    g_video_buffers_mutex = g_mutex_new();
    g_video_buffers_initialized = 1;
    return 1;
}

static int get_next_video_buffer() {
    g_video_buffer_next++;
    g_video_buffer_next = g_video_buffer_next % 3;
    if (g_video_buffers + g_video_buffer_next == g_video_buffer_locked) {
        g_video_buffer_next++;
        g_video_buffer_next = g_video_buffer_next % 3;
    }
    return g_video_buffer_next;
}

fs_emu_video_buffer *fs_emu_get_current_video_buffer() {
    return g_video_buffer_current;
}

/*
fs_emu_video_buffer *fs_emu_swap_offscreen_video_buffers() {
    get_next_video_buffer();
}
*/

fs_emu_video_buffer *fs_emu_get_available_video_buffer(int copy) {
    g_mutex_lock(g_video_buffers_mutex);
    int next_buffer = get_next_video_buffer();
    g_mutex_unlock(g_video_buffers_mutex);

    // FIXME: REMOVE
    //memset((g_video_buffers + next_buffer)->data, 0x0,
    //        (g_video_buffers + next_buffer)->size);

    if (copy) {
        if (g_video_buffer_current) {
            memcpy((g_video_buffers + next_buffer)->data,
                    g_video_buffer_current->data,
                    (g_video_buffers + next_buffer)->size);
        }
        else {
            memset((g_video_buffers + next_buffer)->data, 0x0,
                    (g_video_buffers + next_buffer)->size);
        }
    }
    return g_video_buffers + next_buffer;
}

fs_emu_video_buffer *fs_emu_lock_video_buffer() {
    g_mutex_lock(g_video_buffers_mutex);
    g_video_buffer_locked = g_video_buffer_current;
    g_mutex_unlock(g_video_buffers_mutex);
    return g_video_buffer_locked;
}

void fs_emu_unlock_video_buffer() {
    g_mutex_lock(g_video_buffers_mutex);
    g_video_buffer_locked = NULL;
    g_mutex_unlock(g_video_buffers_mutex);
}

int fs_emu_grow_render_buffer(fs_emu_video_buffer *buffer, int width,
        int height) {
    int needed_size = width * height * buffer->bpp;
    if (buffer->size >= needed_size) {
        return 0;
    }
    fs_log("fs_emu_grow_render_buffer %dx%d (%d bytes)\n", width, height,
            needed_size);
    g_free(buffer->data);
    buffer->size = needed_size;
    buffer->data = g_malloc0(buffer->size);
    return 1;
}

void copy_buffer_data(fs_emu_video_buffer *new_buffer,
        fs_emu_video_buffer *old_buffer) {
    if (!old_buffer) {
        return;
    }

    int stride = new_buffer->width * new_buffer->bpp;
    unsigned char *src, *dst;
    int width;
    int first_line, last_line;

    //int crop = g_fs_emu_video_crop_mode;
    int crop = 0;

    if (crop) {
        width = new_buffer->crop.w;
        first_line = new_buffer->crop.y;
        last_line = new_buffer->crop.y + new_buffer->crop.h - 1;

        src = old_buffer->data + stride * first_line + new_buffer->crop.x * g_fs_emu_video_bpp;
        dst = new_buffer->data + stride * first_line + new_buffer->crop.x * g_fs_emu_video_bpp;
    }
    else {
        // no cropping; must copy the entire line
        width = new_buffer->width;
        first_line = 0;
        last_line = new_buffer->height - 1;

        src = old_buffer->data;
        dst = new_buffer->data;
    }
#if 0
    if (!g_fs_emu_scanlines) {
#endif
        for (int y = first_line; y <= last_line; y++) {
            if (new_buffer->line[y]) {
                memcpy(dst, src, width * g_fs_emu_video_bpp);
            }
            src += stride;
            dst += stride;
        }
#if 0
    }
    else {
        for (int y = first_line; y <= last_line; y++) {
            if (new_buffer->line[y]) {
                memcpy(dst, src, width * g_fs_emu_video_bpp);
            }
            else {
                unsigned char *p = dst;
                if ((y % 2) == 0) {
                    if (g_fs_emu_scanlines_dark == 0) {
                        continue;
                    }
                    for (int x = 0; x < width; x++) {
                        *p = (*p * g_fs_emu_scanlines_dark_i) / 256;
                        p++;
                        *p = (*p * g_fs_emu_scanlines_dark_i) / 256;
                        p++;
                        *p = (*p * g_fs_emu_scanlines_dark_i) / 256;
                        // FIXME: depends on bpp
                        p += 2;
                    }
                }
                else {
                    if (g_fs_emu_scanlines_light == 0) {
                        continue;
                    }
                    for (int x = 0; x < width; x++) {
                        *(p) = (*p * g_fs_emu_scanlines_light_i) / 256
                                + g_fs_emu_scanlines_light;
                        p++;
                        *(p) = (*p * g_fs_emu_scanlines_light_i) / 256
                                + g_fs_emu_scanlines_light;
                        p++;
                        *(p) = (*p * g_fs_emu_scanlines_light_i) / 256
                                + g_fs_emu_scanlines_light;
                        // FIXME: depends on bpp
                        p += 2;
                    }
                }
            }
            src += stride;
            dst += stride;
        }
    }
#endif
}

void fs_emu_set_video_buffer(fs_emu_video_buffer *buffer) {
    if (buffer == NULL) {
        return;
    }
    if (g_fs_emu_benchmark_start_time > 0) {
        g_fs_emu_total_emu_frames++;
    }

    copy_buffer_data(buffer, g_video_buffer_current);

    update_video_stats_1();

    fs_ml_frame_update_begin();

    g_mutex_lock(g_video_buffers_mutex);
    g_video_buffer_current = buffer;
    g_mutex_unlock(g_video_buffers_mutex);

    fs_ml_frame_update_end();

    update_video_stats_2();

    /*
    fs_ml_lock_buffer_swap();
    fs_ml_signal_buffer_swap();
    fs_ml_unlock_buffer_swap();
*/
}

// command line argument --video-sync
//gchar *g_fs_emu_video_vsync_mode_arg = NULL;

int g_fs_emu_video_debug = 0;
int g_fs_emu_video_fullscreen = 0;
gchar *g_fs_emu_video_fullscreen_mode = NULL;
int g_fs_emu_video_fullscreen_window = -1;
int g_fs_emu_video_crop_mode = 1;
//gboolean g_fs_emu_video_vsync = -1;
int g_fs_emu_screenshot = 0;

int g_fs_emu_benchmarking = 0;
int64_t g_fs_emu_benchmark_start_time = 0;
int g_fs_ml_benchmarking = 0;
int g_fs_emu_total_emu_frames = 0;
int g_fs_emu_total_sys_frames = 0;

// current size of window (or last window size)
//static int g_window_width = 752;
//static int g_window_height = 574;

// current size of window or fullscreen view
//static int g_video_width = 0;
//static int g_video_height = 0;

//static GLuint g_frame_texture = 0;

#define FRAME_TIME_LIST_COUNT 256
// time is specified in microseconds
fs_emu_stat_queue g_fs_emu_emu_frame_times = {};
fs_emu_stat_queue g_fs_emu_emu2_frame_times = {};
fs_emu_stat_queue g_fs_emu_sys_frame_times = {};
int g_fs_emu_repeated_frames = 0;
int g_fs_emu_lost_frames = 0;
int g_fs_emu_lost_vblanks = 0;
int g_fs_emu_audio_buffer_underruns = 0;

//#define VIDEO_BUFFER_MAX_SIZE (1024*1024*4)
//static uint8_t g_buffer_data[VIDEO_BUFFER_MAX_SIZE] = {};
//static int g_buffer_width = 0;
//static int g_buffer_height = 0;
//static int g_buffer_bpp = 0;
//static fs_emu_rect g_buffer_crop = {};

//static GMutex *g_video_mutex;
//static uint8_t g_video_raw_data[VIDEO_BUFFER_MAX_SIZE] = {};
//static fs_emu_video_raw_function g_video_raw_function = NULL;
//static void* g_video_raw_user_data = NULL;

// this is used to make sure that changes to menu etc is not done while
// rendering it... handling of input events for menu actions must be done
// when holding this lock
static GMutex *g_video_render_mutex;

// this is the target frame rate for the video (emulator output)
static int g_video_frame_rate = 0;

// this is the aspect ratio of the video frame from the emulator, defaults
// to 1.0 (1:1 pixels)
static double g_video_aspect_ratio = 1.0;

//static int g_emu_video_struct_seq_no = 0;
static GQueue *g_emu_video_struct_queue = NULL;
static GMutex *g_emu_video_struct_mutex = NULL;

#if 0
static fs_emu_video_data *alloc_video_struct() {
    fs_emu_video_data *info = g_malloc0(sizeof(struct fs_emu_video_data));
    return info;
}
#endif

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
    //g_frame_times.last = g_list_last(g_frame_times.list);
}

/*
int fs_emu_get_vblank_vsync() {
    return fs_ml_get_vblank_vsync();
}

int fs_emu_get_video_vsync() {
    return fs_ml_get_video_vsync();
}
*/

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
    g_mutex_lock(g_video_render_mutex);
}

void fs_emu_video_render_mutex_unlock() {
    if (!g_video_render_mutex) {
        return;
    }
    g_mutex_unlock(g_video_render_mutex);
}

static void configure_scanlines() {
    double dval;

    if (fs_config_get_boolean("scanlines") == 1) {
        g_fs_emu_scanlines = 1;
    }
    dval = fs_config_get_double_clamped("scanlines_light", 0, 100);
    if (dval != FS_CONFIG_NONE) {
        g_fs_emu_scanlines_light = 255.0 * dval / 100.0;
        g_fs_emu_scanlines_light_i = 255 - g_fs_emu_scanlines_light;
    }
    dval = fs_config_get_double_clamped("scanlines_dark", 0, 100);
    if (dval != FS_CONFIG_NONE) {
        g_fs_emu_scanlines_dark = 255.0 * dval / 100.0;
        g_fs_emu_scanlines_dark_i = 255 - g_fs_emu_scanlines_dark;
    }
}

void fs_emu_video_init() {
    fs_log("fs_emu_video_init\n");

    //int frame_rate = 0;
    //int auto_display_mode = 1;
    //int vsync = 0;

    //int sync_mode = SYNC_MODE_AUTO;
    int auto_sync_mode = 1;
    int sync_to_vblank = 0;
    int sync_with_emu = 0;
    gchar *sync_mode_str = NULL;

    int fsaa = fs_config_get_int_clamped("fsaa", 0, 4);
    if (fsaa != FS_CONFIG_NONE) {
        fs_log("setting full-scene anti-aliasing (FSAA) to %dx%d\n",
                fsaa, fsaa);
        fs_ml_set_video_fsaa(fsaa);
    }
    else {
        fs_log("full-scene anti-aliasing is not requested\n");
    }

    int frame_rate = fs_config_get_int("frame_rate");
    if (frame_rate != FS_CONFIG_NONE) {
        fs_log("WARNING: video/frame_rate set manually to %d\n",
                frame_rate);
        fs_emu_set_video_frame_rate(frame_rate);
    }
    else {
        frame_rate = fs_emu_get_video_frame_rate();
    }

    fs_ml_video_mode mode;
    memset(&mode, 0, sizeof(fs_ml_video_mode));
    if (fs_ml_video_mode_get_current(&mode) == 0) {
        fs_log("current display mode is %dx%d@%dhz\n", mode.width,
                mode.height, mode.fps);
    }
    else {
        fs_log("could not get display mode\n");
    }

    fs_log("checking video sync mode\n");
    /*
    if (g_fs_emu_video_vsync_mode_arg) {
        fs_log("--video-sync was specified on the command line\n");
        sync_mode_str = g_strdup(g_fs_emu_video_vsync_mode_arg);
    }
    else {
        // FIXME: change config key name
    }
    */
    sync_mode_str = fs_config_get_string("video_sync");
    if (!sync_mode_str) {
        // compatibility key
        sync_mode_str = fs_config_get_string("sync");
    }
    if (sync_mode_str) {
        if (g_ascii_strcasecmp(sync_mode_str, "auto") == 0) {

        }
        else if (g_ascii_strcasecmp(sync_mode_str, "off") == 0) {
            auto_sync_mode = 0;
        }
        else if (g_ascii_strcasecmp(sync_mode_str, "vblank") == 0) {
            auto_sync_mode = 0;
            sync_to_vblank = 1;
        }
        else if (g_ascii_strcasecmp(sync_mode_str, "full") == 0) {
            auto_sync_mode = 0;
            sync_to_vblank = 1;
            sync_with_emu = 1;
        }
        else {
            fs_log("WARNING: invalid value for video-sync: %s\n",
                    sync_mode_str);
        }
        g_free(sync_mode_str);
    }
    else {
        fs_log("not specified: using automatic video sync mode\n");
    }

    if (auto_sync_mode) {
        fs_log("auto sync mode is enabled\n");
        if (frame_rate && frame_rate == mode.fps) {
            fs_log("frame rate (%d) equals screen refresh (%d)\n",
                    frame_rate, mode.fps);
            sync_to_vblank = 1;
            sync_with_emu = 1;
        }
        else {
            fs_log("frame rate (%d) does not equal screen refresh (%d)\n",
                    frame_rate, mode.fps);
            sync_to_vblank = 1;
        }
    }

    if (fs_emu_netplay_enabled()) {
        fs_log("netplay is enabled, disabling full video/emulator sync\n");
        sync_with_emu = 0;
        //sync_to_vblank = 0;
    }

    if (fs_config_get_boolean("benchmark") != FS_CONFIG_NONE) {
        fs_log("benchmarking enable, disabling video sync\n");
        sync_to_vblank = 0;
        sync_with_emu = 0;
        g_fs_emu_benchmarking = 1;
        g_fs_ml_benchmarking = 1;
    }

    if (sync_with_emu && !g_fs_emu_full_sync_allowed) {
        fs_log("full video/emu sync is not allowed in this mode\n");
        sync_with_emu = 0;
    }

    if (sync_with_emu) {
        fs_log("will sync emulation and video with vblank\n");
        fs_ml_video_sync_enable();

        if (frame_rate && mode.fps) {
            double pitch = (1.0 * mode.fps) / frame_rate;
            fs_log("changing audio pitch to %0.2f based on frame rates\n",
                    pitch);
            fs_emu_audio_set_default_pitch(pitch);
        }
    }
    else if (sync_to_vblank) {
        fs_log("will sync video output only to vblank (no tearing)\n");
        fs_ml_vblank_sync_enable();
    }
    else {
        fs_log("no video sync\n");
    }

    if (fs_config_get_boolean("disable_aspect_correction") == 1) {
        g_fs_emu_disable_aspect_correction = 1;
    }
    else if (fs_config_get_boolean("keep_aspect") == 1) {
        fs_emu_video_set_aspect_correction(1);
    }

    const char *s = fs_config_get_const_string("video_format");
    if (s) {
        if (g_ascii_strcasecmp(s, "bgra") == 0) {
            fs_log("using video format BGRA\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_BGRA;
            g_fs_emu_video_bpp = 4;
        }
        else if (g_ascii_strcasecmp(s, "rgba") == 0) {
            fs_log("using video format RGBA\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_RGBA;
            g_fs_emu_video_bpp = 4;
        }
        else if (g_ascii_strcasecmp(s, "rgb") == 0) {
            fs_log("using video format RGB\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_RGB;
            g_fs_emu_video_bpp = 3;
        }
        else if (g_ascii_strcasecmp(s, "rgb565") == 0) {
            fs_log("using video format RGB565\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_R5G6B5;
            g_fs_emu_video_bpp = 2;
        }
        else if (g_ascii_strcasecmp(s, "rgba5551") == 0) {
            fs_log("using video format RGBA5551\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_R5G5B5A1;
            g_fs_emu_video_bpp = 2;
        }
        else {
            fs_emu_warning("Unknown video format");
        }
    }
    if (!g_fs_emu_video_format) {
        fs_log("using default video format BGRA\n");
        g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_BGRA;
        g_fs_emu_video_bpp = 4;
    }

    s = fs_config_get_const_string("texture_format");
    if (s) {
        if (g_ascii_strcasecmp(s, "rgb") == 0) {
            fs_log("using texture format RGB\n");
            g_fs_emu_texture_format = GL_RGB;
        }
        else if (g_ascii_strcasecmp(s, "rgb8") == 0) {
            fs_log("using texture format RGB8\n");
            g_fs_emu_texture_format = GL_RGB8;
        }
        else if (g_ascii_strcasecmp(s, "rgba") == 0) {
            fs_log("using texture format RGBA\n");
            g_fs_emu_texture_format = GL_RGBA;
        }
        else if (g_ascii_strcasecmp(s, "rgba8") == 0) {
            fs_log("using texture format RGBA8\n");
            g_fs_emu_texture_format = GL_RGBA8;
        }
        else if (g_ascii_strcasecmp(s, "rgb5") == 0) {
            fs_log("using texture format RGB5\n");
            g_fs_emu_texture_format = GL_RGB5;
        }
        else if (g_ascii_strcasecmp(s, "rgb5_a1") == 0) {
            fs_log("using texture format RGB5_A1\n");
            g_fs_emu_texture_format = GL_RGB5_A1;
        }
        else {
            fs_emu_warning("Unknown texture format");
        }
    }
    if (!g_fs_emu_texture_format) {
        fs_log("using default texture format RGB\n");
        g_fs_emu_texture_format = GL_RGB;
    }


    //g_video_mutex = g_mutex_new();
    g_video_render_mutex = g_mutex_new();
    g_emu_video_struct_queue = g_queue_new();
    g_emu_video_struct_mutex = g_mutex_new();

    configure_scanlines();

    fs_emu_init_render();
}

int fs_emu_get_video_format() {
    return g_fs_emu_video_format;
}

int fs_emu_get_texture_format() {
    return g_fs_emu_texture_format;
}

void fs_emu_video_init_opengl() {
    //g_video_width = 1024;
    //g_video_height = 600;
    fs_emu_initialize_opengl();
    initialize();
    fs_emu_menu_init_opengl();

    fs_emu_init_shader();
}

void fs_emu_toggle_fullscreen() {
    //fs_log("WARNING: toggle fullscreen is not implemented\n");
    fs_ml_toggle_fullscreen();
}

static int64_t g_frame_time_last = 0;
static int64_t g_frame_time_first = 0;

void update_video_stats_1() {
    int t = fs_emu_monotonic_time();
    if (g_frame_time_first == 0) {
        return;
    }
    int time_ms = (t - g_frame_time_first) / 1000;

    int dt = (int) (time_ms - g_frame_time_last);
    // more than 5 seconds => do not record entry (abnormality)
    fs_emu_stat_queue_add_entry(&g_fs_emu_emu2_frame_times, dt, 5 * 1000);
}

void update_video_stats_2() {
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

void fs_emu_video_render(int allow_wait) {
    // FIXME: REMOVE
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
    fs_emu_unlock_video_buffer();

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

    /*
    //fs_gl_finish();
    //SDL_GL_SwapBuffers();
    int64_t t1 = fs_emu_monotonic_time();
    fs_ml_swap_buffers();
    fs_gl_finish();
    int64_t t2 = fs_emu_monotonic_time();
    int dt = (t2 - t1) / 1000;
    // FIXME: CHECK HERZ / VSYNC
    if (dt >= 17) {
        fs_log("WARNING: swap time: %d ms\n", dt);
    }
    */

    //g_fps =  1000000.0 / (((double) g_frame_times.total) /
    //        ((double) g_frame_times.count));
    //if (g_frame_times.seq_no % 10 == 0) {
    //    fs_log("avg fps: %0.2f\n", fps);
    //}

    /*
    static int64_t last_time = 0;
    int64_t t = fs_emu_monotonic_time();
    int dt = (int) (t - last_time);
    if (dt > 20000 + 500) {
        fs_log("dt %0.2f\n", dt / 1000.0);
    }
    last_time = t;
    */

    /*
    static Uint32 last_time_sdl = 0;
    Uint32 t_sdl = SDL_GetTicks();
    int dt_sdl = t_sdl - last_time_sdl;
    fs_log("dt %0.2f\n", dt_sdl / 1.0);
    last_time_sdl = t_sdl;
    */
}

#if 0
void fs_emu_screenshot(const char *path, int crop) {
    fs_log("fs_emu_screenshot %s crop=%d\n", path, crop);
    fs_log("FIXME\n");
}
#endif
