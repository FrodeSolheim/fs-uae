#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// FIXME: make libfsml independent of libfsmeu
#include "../emu/util.h"
#include "../emu/video.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

#ifdef USE_SDL2
#define USE_SDL
#endif

#ifdef USE_SDL
#include <SDL.h>
#endif

#include <fs/conf.h>
#include <fs/i18n.h>
#include <fs/image.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#include <fs/ml.h>
//#include "fs/emu.h"
#include "ml_internal.h"

#include <fs/thread.h>

static int g_vblank_count = 0;
static int64_t g_measured_vblank_time = 0;
static int64_t g_adjusted_vblank_time = 0;
static int64_t g_estimated_next_vblank_time = 0;

// FIXME
static int g_estimated_upload_render_duration = 5000;

static fs_condition *g_frame_available_cond = NULL;
static fs_mutex *g_frame_available_mutex = NULL;

static fs_condition *g_buffer_swap_cond = NULL;
static fs_mutex *g_buffer_swap_mutex = NULL;

static volatile int g_start_new_frame = 0;
static fs_condition *g_start_new_frame_cond = NULL;
static fs_mutex *g_start_new_frame_mutex = NULL;

static int g_available_frame = -1;
static int g_uploaded_frame = -1;
static int g_rendered_frame = -1;

static int g_has_apple_fence = 0;
static int g_has_nv_fence = 0;
static int g_has_arb_sync = 0;

#define SYNC_FLAG_GLFINISH 1
#define SYNC_FLAG_SLEEP 2
#define SYNC_FLAG_FENCE 4

static int64_t g_sleep_until_vsync_last_time = 0;

#define VBLANK_COUNT 100
//static fs_emu_stat_queue g_measured_vblank_times;
static int64_t g_measured_vblank_times[VBLANK_COUNT] = {};
static int g_vblank_index = 0;

static fs_mutex *g_vblank_mutex = NULL;

static int64_t g_epoch = 0;

#define CHECK_GL_ERROR_MSG(msg)

void fs_ml_frame_update_begin(int frame) {
    if (g_fs_ml_video_sync) {

    }
    else if (g_fs_ml_vblank_sync) {
        // emulation running independently on the video renderer
    }
    else if (g_fs_ml_benchmarking) {
        // run as fast as possible
    }
    else {
        // video renderer is waiting for a new frame -signal that a new
        // frame is ready
        //fs_condition_signal(g_video_cond);
    }
}

void fs_ml_frame_update_end(int frame) {

    //printf("%d\n", frame);

    // in timed mode only (non-vsync), the video renderer is waiting for
    // a new frame signal
    fs_mutex_lock(g_frame_available_mutex);
    g_available_frame = frame;
    fs_condition_signal(g_frame_available_cond);
    fs_mutex_unlock(g_frame_available_mutex);

    if (g_fs_ml_video_sync) {
        fs_mutex_lock(g_start_new_frame_mutex);
        while (!g_start_new_frame) {
            fs_condition_wait (g_start_new_frame_cond,
                    g_start_new_frame_mutex);
        }
        g_start_new_frame = 0;
        fs_mutex_unlock(g_start_new_frame_mutex);
    }
    else if (g_fs_ml_vblank_sync) {
        // emulation running independently on the video renderer
    }
    else if (g_fs_ml_benchmarking) {
        // run as fast as possible
    }
    else {
        // video renderer is waiting for a new frame -signal that a new
        // frame is ready
        //fs_condition_signal(g_video_cond);
    }
}

static void save_screenshot_of_opengl_framebuffer(const char *path) {
#if 0
    static int count = 0;
    count += 1;

    time_t t = time(NULL);
#ifdef WINDOWS
    struct tm *tm_p = localtime(&t);
#else
    struct tm tm_struct;
    struct tm *tm_p = &tm_struct;
    localtime_r(&t, tm_p);
#endif
    char strbuf[20];
    strftime(strbuf, 20, "%Y-%m-%d-%H-%M", tm_p);
    char *name = g_strdup_printf("%s-%s-%03d.png",
            g_fs_ml_video_screenshots_prefix,
            strbuf, g_fs_ml_video_screenshot);
    char *path = g_build_filename(g_fs_ml_video_screenshots_dir, name, NULL);
#endif
    fs_log("writing screenshot to %s\n", path);

    int w = fs_ml_video_width();
    int h = fs_ml_video_height();
    fs_log("reading opengl frame buffer (%d x %d)\n", w, h);
    void *out_data = g_malloc(w * h * 4);

    // when using GL_RGB, remeber to temporarily set GL_UNPACK_ALIGNMENT so
    // all rows will be contiguous (the OpenGL default is to align rows on
    // 4-byte boundaries
    //GLint unpack_alignment;
    //glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, out_data);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

    // flip image vertically
    int stride = w * 4;
    void *tmp = g_malloc(stride);
    void *line1 = out_data;
    void *line2 = out_data + stride * (h - 1);
    for (int i = 0; i < h / 2; i++) {
        memcpy(tmp, line1, stride);
        memcpy(line1, line2, stride);
        memcpy(line2, tmp, stride);
        line1 += stride;
        line2 -= stride;
    }
    g_free(tmp);

    int result = fs_image_save_data(path, out_data, w, h, 4);
    if (result) {
        fs_log("saved screenshot\n");
    }
    else {
        fs_log("error saving screenshot\n");
    }
    g_free(out_data);
#if 0
    g_free(name);
    g_free(path);
#endif
}

int fs_ml_get_vblank_count() {
    return g_vblank_count;
}

int64_t fs_ml_get_vblank_time() {
    return g_measured_vblank_time;
}

void fs_ml_stop() {
    g_fs_ml_running = 0;
    // signal g_frame_available_cond because video (main) thread may be
    // blocking on this condition
    // fs_condition_signal(g_frame_available_cond);
}

#if 0
static int eltime(int64_t t) {
    return (int) ((t - g_epoch));
}
#endif

static void update_frame() {
    if (g_fs_ml_video_update_function) {
        g_uploaded_frame = g_fs_ml_video_update_function();
    }
}

static void render_frame() {
    if (g_fs_ml_video_render_function) {
        g_fs_ml_video_render_function();
        g_rendered_frame = g_uploaded_frame;
    }
}

#ifdef USE_SDL2
extern SDL_Window* g_fs_ml_window;
#endif

static void swap_opengl_buffers() {
    //int64_t t1 = fs_get_monotonic_time();
#if defined(USE_SDL2)
    SDL_GL_SwapWindow(g_fs_ml_window);
#elif defined(USE_SDL)
    SDL_GL_SwapBuffers();
#else
    printf("ERROR: no swap\n");
#endif
    //int64_t t2 = fs_get_monotonic_time();
}

static void gl_finish() {
    //int64_t t1 = fs_get_monotonic_time();
    fs_gl_finish();
    CHECK_GL_ERROR_MSG("render_frame");
    //int64_t t3 = fs_get_monotonic_time();
    //printf("          %lld : %lld\n", t2 - t1, t3 - t2);
}

//void fs_ml_wait_vblank() {
//}

static void sleep_until_vsync()
{
    int sleep_time = 5000;
    int64_t t = fs_emu_monotonic_time();
    //int64_t sleep_until = 0;
    if (g_fs_ml_target_frame_time > 0) {
        // calculate sleep time based on frame rate
        // (allowing 4 ms busy-waiting by OpenGL driver, if necessary)
        //sleep_time = g_fs_ml_target_frame_time - 4000;
        sleep_time = g_sleep_until_vsync_last_time +
                g_fs_ml_target_frame_time - t - 5000;
    }
    if (sleep_time > g_fs_ml_target_frame_time - 4000) {
        sleep_time = 0;
    }
    if (sleep_time > 0) {
        //printf("sleep %d\n", sleep_time);
        fs_ml_usleep(sleep_time);
    }
}

#if 0
static void full_sleep_until_vsync()
{
    // FIXME: use this instead of sleep_until_vsync
    int sleep_time = 0;
    int time_left = 3000;
    int64_t t = fs_emu_monotonic_time();
    if (g_fs_ml_target_frame_time > 0) {
        sleep_time = g_sleep_until_vsync_last_time +
                g_fs_ml_target_frame_time - t - time_left;
    }
    if (sleep_time > g_fs_ml_target_frame_time - time_left) {
        sleep_time = 0;
    }
    if (sleep_time > 0) {
        fs_ml_usleep(sleep_time);
    }
}
#endif

#ifdef USE_GLEE
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;

static void (APIENTRYP glWaitSync)(GLsync sync, GLbitfield flags,
        GLuint64 timeout) = NULL;
static GLenum (APIENTRYP glClientWaitSync)(GLsync sync, GLbitfield flags,
        GLuint64 timeout) = NULL;
static GLsync (APIENTRYP glFenceSync)(GLenum condition, 
        GLbitfield flags) = NULL;
#endif

#define GL_OBJECT_TYPE                 0x9112
#define GL_SYNC_CONDITION              0x9113
#define GL_SYNC_STATUS                 0x9114
#define GL_SYNC_FLAGS                  0x9115
#define GL_SYNC_GPU_COMMANDS_COMPLETE  0x9117
#define GL_ALREADY_SIGNALED            0x911A
#define GL_TIMEOUT_EXPIRED             0x911B
#define GL_CONDITION_SATISFIED         0x911C
#define GL_WAIT_FAILED                 0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT     0x00000001
#define GL_UNSIGNALED                  0x9118
#define GL_SIGNALED                    0x9119
#define TIMEOUT_IGNORED                0xFFFFFFFFFFFFFFFFull

#define FENCE_SET 1
#define FENCE_WAIT 2

#ifdef USE_GLES

#else
static GLuint g_fence;
static GLsync g_sync;
#endif

void * __GLeeGetProcAddress(const char *extname);

#define SYNC_SWAP 1
#define SYNC_SWAP_FINISH 2
#define SYNC_FINISH_SWAP_FINISH 3
#define SYNC_SLEEP_SWAP_FINISH 4
#define SYNC_FINISH_SLEEP_SWAP_FINISH 5

#define SYNC_SWAP_FENCE_START 6
#define SYNC_SWAP_FENCE 7
#define SYNC_SWAP_SLEEP_FENCE 8

static int g_sync_method = 0;

static int check_sync_method(const char *a, const char *b) {
    if (a && g_ascii_strcasecmp(a, b) == 0) {
        return 1;
    }
    return 0;
}

static void decide_opengl_sync_method()
{
    fs_log("[OPENGL] Deciding video sync method\n");
    const char *c = fs_config_get_const_string("video_sync_method");
    if (check_sync_method(c, "auto")) {
    } else if (check_sync_method(c, "swap")) {
        fs_log("[OPENGL] SYNC_SWAP\n");
        g_sync_method = SYNC_SWAP;
    } else if (check_sync_method(c, "swap-finish")) {
        fs_log("[OPENGL] SYNC_SWAP_FINISH\n");
        g_sync_method = SYNC_SWAP_FINISH;
    } else if (check_sync_method(c, "finish-swap-finish")) {
        fs_log("[OPENGL] SYNC_FINISH_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SWAP_FINISH;
    } else if (check_sync_method(c, "sleep-swap-finish")) {
        fs_log("[OPENGL] SYNC_SLEEP_SWAP_FINISH\n");
        g_sync_method = SYNC_SLEEP_SWAP_FINISH;
    } else if (check_sync_method(c, "finish-sleep-swap-finish")) {
        fs_log("[OPENGL] SYNC_FINISH_SLEEP_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SLEEP_SWAP_FINISH;
    } else if (check_sync_method(c, "swap-fence")) {
        fs_log("[OPENGL] SYNC_SWAP_FENCE\n");
        g_sync_method = SYNC_SWAP_FENCE;
    } else if (check_sync_method(c, "swap-sleep-fence")) {
        fs_log("[OPENGL] SYNC_SWAP_SLEEP_FENCE\n");
        g_sync_method = SYNC_SWAP_SLEEP_FENCE;
    } else if (c) {
        fs_log("[OPENGL] Unknown sync method specified\n");
        g_sync_method = 0;
    }
    int fence_support = g_has_nv_fence || g_has_apple_fence || g_has_arb_sync;
    if (g_sync_method >= SYNC_SWAP_FENCE_START && !fence_support) {
        fs_log("[OPENGL] No fence support, cannot use this sync method\n");
        g_sync_method = 0;
    }
    if (g_sync_method == 0) {
        fs_log("[OPENGL] Using default sync method\n");
#if defined(WINDOWS) || defined(MACOSX)
        fs_log("- SYNC_FINISH_SLEEP_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SLEEP_SWAP_FINISH;
#else
        fs_log("- SYNC_FINISH_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SWAP_FINISH;
#endif
    }
}

static void check_opengl_sync_capabilities() {
    fs_log("checking OpenGL capabilities\n");
    const char *ext = (const char *) glGetString(GL_EXTENSIONS);
    if (ext) {
        if (strstr(ext, "GL_NV_fence") != NULL) {
            g_has_nv_fence = 1;
            fs_log("GL_NV_fence extension found \n");
        }
        if (strstr(ext, "GL_APPLE_fence") != NULL) {
            g_has_apple_fence = 1;
            fs_log("GL_APPLE_fence extension found\n");
        }
        if (strstr(ext, "GL_ARB_sync") != NULL) {
            fs_log("GL_ARB_sync extension found\n");
#ifdef USE_GLEE
            glFenceSync = __GLeeGetProcAddress("glFenceSync");
            glWaitSync = __GLeeGetProcAddress("glWaitSync");
            glClientWaitSync = __GLeeGetProcAddress("glClientWaitSync");
#endif
            if (glFenceSync && glClientWaitSync) {
                g_has_arb_sync = 1;
            }
            else {
                fs_log("error looking up functions\n");
            }
        }
    }
}

static void initialize_opengl_sync() {
    check_opengl_sync_capabilities();
    decide_opengl_sync_method();
#ifdef USE_GLES

#else
    if (g_has_nv_fence) {
        glGenFencesNV(1, &g_fence);
        CHECK_GL_ERROR_MSG("glGenFencesNV(1, &g_fence)");
    }
    else if (g_has_apple_fence) {
        glGenFencesAPPLE(1, &g_fence);
        CHECK_GL_ERROR_MSG("glGenFencesAPPLE(1, &g_fence)");
    }
#endif
}

static void opengl_fence(int command) {
#ifdef USE_GLES

#else
    if (command == FENCE_SET) {
        if (g_has_nv_fence) {
            //printf("...\n");
            glSetFenceNV(g_fence, GL_ALL_COMPLETED_NV);
            CHECK_GL_ERROR_MSG("glSetFenceNV(g_fence, GL_ALL_COMPLETED_NV)");
        }
        else if (g_has_apple_fence) {
            glSetFenceAPPLE(g_fence);
            CHECK_GL_ERROR_MSG("glSetFenceAPPLE(g_fence)");
        }
        else if (g_has_arb_sync) {
            g_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
            CHECK_GL_ERROR_MSG("glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)");
        }
    }
    else if (command == FENCE_WAIT) {
        if (g_has_nv_fence) {
            //printf("-- f --\n");
            //glFinishFenceNV(g_fence);
            //int64_t t1 = fs_get_monotonic_time();
            //fs_ml_usleep(10000);
            while (!glTestFenceNV(g_fence)) {
                CHECK_GL_ERROR_MSG("glTestFenceNV(g_fence)");
                //printf("-> %lld\n", fs_get_monotonic_time() - t1);
                //printf("%d\n", glGetError());
                fs_ml_usleep(1000);
                //printf("-> %lld\n", fs_get_monotonic_time() - t1);
            }
            CHECK_GL_ERROR_MSG("glTestFenceNV(g_fence)");
        }
        else if (g_has_apple_fence) {
            while (!glTestFenceAPPLE(g_fence)) {
                CHECK_GL_ERROR_MSG("glTestFenceAPPLE(g_fence)");
                fs_ml_usleep(1000);
            }
            CHECK_GL_ERROR_MSG("glTestFenceAPPLE(g_fence)");
        }
        else if (g_has_arb_sync) {
            int flags = GL_SYNC_FLUSH_COMMANDS_BIT;
            while (glClientWaitSync(g_sync, flags, 0)
                    == GL_TIMEOUT_EXPIRED) {
                CHECK_GL_ERROR_MSG("glClientWaitSync(g_sync, flags, 0)");
                flags = 0;
                fs_ml_usleep(1000);
            }
            CHECK_GL_ERROR_MSG("glClientWaitSync(g_sync, flags, 0)");
        }
    }
#endif
}

static void opengl_swap_synchronous() {
    if (g_sync_method == SYNC_SWAP) {
        swap_opengl_buffers();
    }
    else if (g_sync_method == SYNC_SWAP_FINISH) {
        swap_opengl_buffers();
        gl_finish();
    }
    else if (g_sync_method == SYNC_FINISH_SWAP_FINISH) {
        gl_finish();
        swap_opengl_buffers();
        gl_finish();
    }
    else if (g_sync_method == SYNC_SLEEP_SWAP_FINISH) {
        sleep_until_vsync();
        swap_opengl_buffers();
        gl_finish();
    }
    else if (g_sync_method == SYNC_FINISH_SLEEP_SWAP_FINISH) {
        gl_finish();
        sleep_until_vsync();
        swap_opengl_buffers();
        gl_finish();
    }
    else if (g_sync_method == SYNC_SWAP_FENCE) {
        swap_opengl_buffers();
        opengl_fence(FENCE_SET);
        glFlush();
        opengl_fence(FENCE_WAIT);
    }
    else if (g_sync_method == SYNC_SWAP_SLEEP_FENCE) {
        swap_opengl_buffers();
        sleep_until_vsync();
        opengl_fence(FENCE_SET);
        glFlush();
        opengl_fence(FENCE_WAIT);
    }
}

static void render_iteration_vsync()
{
    if (g_fs_ml_video_sync_low_latency) {
        int current_frame_at_start = g_available_frame;

        //int64_t t1 = fs_ml_monotonic_time();

        int sleep_time = 0;
        int time_left = g_estimated_upload_render_duration;
        int64_t t = fs_emu_monotonic_time();
        if (g_fs_ml_target_frame_time > 0) {
            sleep_time = g_estimated_next_vblank_time - t - time_left;
        }
        if (sleep_time > g_fs_ml_target_frame_time - time_left) {
            sleep_time = 0;
        }
        if (sleep_time > 0) {
            fs_ml_usleep(sleep_time);
        }

        if (g_available_frame > current_frame_at_start) {
            //printf("low latency %d\n", g_available_frame);
        }
        else {
            //printf("...\n");
        }
    }

    update_frame();
    CHECK_GL_ERROR_MSG("update_frame");
    render_frame();
    CHECK_GL_ERROR_MSG("render_frame");

    //opengl_fence(FENCE_SET);
    //glFlush();
    //opengl_fence(FENCE_WAIT);
    //int64_t upload_render_time = fs_ml_monotonic_time() - t1;
    //printf("urt %lld\n", upload_render_time);

    opengl_swap_synchronous();

    g_measured_vblank_time = fs_ml_monotonic_time();
    g_vblank_count++;
    fs_mutex_lock(g_vblank_mutex);
    g_measured_vblank_times[g_vblank_index] = g_measured_vblank_time;
    g_vblank_index = (g_vblank_index + 1) % VBLANK_COUNT;
    fs_mutex_unlock(g_vblank_mutex);

    // FIXME: adjust g_measured_vblank_time based on historical data (smooth out
    // irregularities) and save the result in g_adjusted_vblank_time
    g_adjusted_vblank_time = g_measured_vblank_time;

    g_sleep_until_vsync_last_time = g_adjusted_vblank_time;
    g_estimated_next_vblank_time = g_adjusted_vblank_time + \
            g_fs_ml_target_frame_time;

    // g_start_new_frame_cond is used to signal that a new frame can be
    // generated when the emulation is running in sync - this is not used
    // when only display flipping is synced to vblank

    fs_mutex_lock(g_start_new_frame_mutex);
    g_start_new_frame = 1;
    fs_condition_signal(g_start_new_frame_cond);
    fs_mutex_unlock(g_start_new_frame_mutex);
}

void fs_ml_render_iteration() {
    static int first = 1;
    if (first) {
        first = 0;
        initialize_opengl_sync();
    }

    if (g_fs_ml_vblank_sync) {
        render_iteration_vsync();
    }
    else if (g_fs_ml_benchmarking) {
        update_frame();
        render_frame();
        swap_opengl_buffers();
    }
    else {
        // when vsync is off, we wait until a new frame is ready and
        // then we display it immediately

        if (fs_ml_is_quitting()) {
            // but when the emulation is quitting, we can't expect any new
            // frames so there's no point waiting for them. Instead, we just
            // sleep a bit to throttle the frame rate for the quit animation
            fs_ml_usleep(10000);
        } else {
            // wait max 33 ms to allow the user interface to work even if
            // the emu hangs
            // int64_t dest_time = fs_get_real_time() + 33 * 1000;
            int64_t end_time = fs_condition_get_wait_end_time(33 * 1000);
            int64_t check_time = 0;

            fs_mutex_lock(g_frame_available_mutex);
            // fs_log("cond wait until %lld\n", end_time);
            while (g_rendered_frame == g_available_frame) {
                fs_condition_wait_until(
                    g_frame_available_cond, g_frame_available_mutex, end_time);
                check_time = fs_condition_get_wait_end_time(0);
                if (check_time >= end_time) {
                    // fs_log("timed out at %lld\n", check_time);
                    break;
                } else {
                    // fs_log("wake-up at %lld (end_time = %lld)\n", check_time, end_time);
                }
            }
            fs_mutex_unlock(g_frame_available_mutex);
        }

        update_frame();
        render_frame();
        swap_opengl_buffers();
        //gl_finish();
    }

    if (g_fs_ml_video_screenshot_path) {
        fs_mutex_lock(g_fs_ml_video_screenshot_mutex);
        if (g_fs_ml_video_screenshot_path) {
            save_screenshot_of_opengl_framebuffer(
                    g_fs_ml_video_screenshot_path);
            g_free(g_fs_ml_video_screenshot_path);
            g_fs_ml_video_screenshot_path = NULL;
        }
        fs_mutex_unlock(g_fs_ml_video_screenshot_mutex);
    }

    if (g_fs_ml_video_post_render_function) {
        g_fs_ml_video_post_render_function();
    }
}

void fs_ml_render_init()
{
    g_frame_available_cond = fs_condition_create();
    g_frame_available_mutex = fs_mutex_create();

    g_start_new_frame_cond = fs_condition_create();
    g_start_new_frame_mutex = fs_mutex_create();

    g_buffer_swap_cond = fs_condition_create();
    g_buffer_swap_mutex = fs_mutex_create();

    g_epoch = fs_get_monotonic_time();
    g_vblank_mutex = fs_mutex_create();
    //fs_emu_stat_queue_init(&g_measured_vblank_times, VBLANK_TIMES_COUNT);

    if (fs_config_get_boolean("low_latency_vsync") == 0) {
        fs_log("disabling use of low latency vsync\n");
        g_fs_ml_video_sync_low_latency = 0;
    } else {
        fs_log("using low latency vsync when full vsync is enabled\n");
        g_fs_ml_video_sync_low_latency = 1;
    }
}
