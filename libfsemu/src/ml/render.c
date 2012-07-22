#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <glib.h>
#include <SDL.h>

#include <fs/config.h>
#include <fs/glee.h>
#include <fs/ml.h>
//#include "fs/emu.h"
#include "ml_internal.h"

#include <fs/thread.h>
// FIXME: make libfsml independent of libfsmeu
#include "../emu/util.h"
#include "../emu/video.h"

static int g_vblank_count = 0;
static int64_t g_vblank_time = 0;

static GCond *g_video_cond = NULL;
static GMutex *g_video_mutex = NULL;

static GCond *g_buffer_swap_cond = NULL;
static GMutex *g_buffer_swap_mutex = NULL;

static int g_has_apple_fence = 0;
static int g_has_nv_fence = 0;
static int g_has_arb_sync = 0;

#define SYNC_FLAG_GLFINISH 1
#define SYNC_FLAG_SLEEP 2
#define SYNC_FLAG_FENCE 4

static int g_sync_flags = 0;

static int64_t g_sleep_until_vsync_last_time = 0;

#define VBLANK_COUNT 100
//static fs_emu_stat_queue g_vblank_times;
static int64_t g_vblank_times[VBLANK_COUNT] = {};
static int g_vblank_index = 0;

static fs_mutex *g_vblank_mutex = NULL;
static int64_t g_next_vblank_at = 0;

static int64_t g_epoch = 0;

#if 0
static void fs_ml_lock_buffer_swap() {
    g_mutex_lock(g_buffer_swap_mutex);
}

static void fs_ml_unlock_buffer_swap() {
    g_mutex_unlock(g_buffer_swap_mutex);
}
#endif

#define CHECK_GL_ERROR(msg)
/*
#define CHECK_GL_ERROR(msg) { \
    int error = glGetError(); \
    if (error) { \
        printf("%d (%s)\n", error, msg); \
    } \
} \
*/
//exit(1); \


void fs_ml_frame_update_begin() {
    if (g_fs_ml_video_sync) {
        if (g_fs_ml_video_sync_low_latency) {
            // do nothing here
        }
        else {
            GTimeVal abs_time;
            g_get_current_time(&abs_time);
            g_time_val_add(&abs_time, 40 * 1000);

            g_mutex_lock(g_video_mutex);

#ifdef DEBUG_VIDEO_SYNC
            printf(" 1");
#endif
            //printf("%d\n", fs_ml_is_quitting());
            //if (!fs_ml_is_quitting()) {
                g_cond_timed_wait(g_video_cond, g_video_mutex, &abs_time);
                //}

#ifdef DEBUG_VIDEO_SYNC
            printf(" 3");
#endif
            //g_mutex_unlock(g_video_mutex);
        }
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
        g_cond_signal(g_video_cond);
    }
}

void fs_ml_frame_update_end() {
    if (g_fs_ml_video_sync) {
        if (g_fs_ml_video_sync_low_latency) {

            // with full sync, we will now wait until the rendering thread have
            // rendered one frame, as is ready for a new frame to be generated

            g_mutex_lock(g_video_mutex);
            g_cond_signal(g_video_cond);
            //if (!fs_ml_is_quitting()) {
                g_cond_wait(g_video_cond, g_video_mutex);
                //}
            g_mutex_unlock(g_video_mutex);
        }
        else {
#if 0
            g_mutex_lock(g_buffer_swap_mutex);
            g_cond_signal(g_buffer_swap_cond);
            g_mutex_unlock(g_buffer_swap_mutex);
#endif
            //g_mutex_lock(g_video_mutex);
#ifdef DEBUG_VIDEO_SYNC
            printf(" 4");
#endif
            g_cond_signal(g_video_cond);

            g_mutex_unlock(g_video_mutex);
        }

#if 0
        //GTimeVal abs_time;
        //g_get_current_time(&abs_time);
        //g_time_val_add(&abs_time, 40 * 1000);

        g_mutex_lock(g_video_mutex);

        fs_ml_lock_buffer_swap();
        g_cond_signal(g_buffer_swap_cond);
        fs_ml_unlock_buffer_swap();

        //g_cond_timed_wait(g_video_cond, g_video_mutex, &abs_time);
        g_cond_wait(g_video_cond, g_video_mutex);
        g_mutex_unlock(g_video_mutex);
#endif
    }
    else {
        g_cond_signal(g_video_cond);
    }
}

static void frame_wait() {

    if (g_fs_ml_video_sync_low_latency && g_fs_ml_video_sync) {
        //printf("a0\n");
        g_mutex_lock(g_video_mutex);
        //printf("a1\n");

        //printf("- 2\n");
        g_cond_signal(g_video_cond);

        /*
        if (fs_ml_is_quitting) {
            g_cond_signal(g_video_cond);
            g_mutex_unlock(g_video_mutex);
            return;
        }
        */

        //g_cond_wait(g_video_cond, g_video_mutex);
        //printf("a2\n");

        static GTimeVal abs_time;
        g_get_current_time(&abs_time);
        g_time_val_add(&abs_time, 40 * 1000);
        if (!fs_ml_is_quitting()) {
            g_cond_timed_wait(g_video_cond, g_video_mutex, &abs_time);
        }
        //printf("- 4\n");

        //printf("a3\n");
        g_mutex_unlock(g_video_mutex);
        //printf("a4\n");

#if 0
        // signal that a new frame can be created
        g_cond_signal(g_video_cond);

        // wait for the frame to finish

        fs_ml_lock_buffer_swap();
        g_cond_signal(g_video_cond);

        static GTimeVal abs_time;
        g_get_current_time(&abs_time);
        g_time_val_add(&abs_time, 40 * 1000);
        g_cond_timed_wait(g_buffer_swap_cond, g_buffer_swap_mutex, &abs_time);
        fs_ml_unlock_buffer_swap();

        //fs_ml_lock_buffer_swap();
        //fs_ml_signal_buffer_swap();
        //fs_ml_unlock_buffer_swap();
#endif
    }
}


static void synchronized_buffer_swap() {
    if (g_fs_ml_video_sync) {
        if (g_fs_ml_video_sync_low_latency) {
            // do nothing
        }
        else {
#if 0
            static GTimeVal abs_time;
            fs_ml_lock_buffer_swap();
            g_cond_signal(g_video_cond);
            // give the client max 3 ms to perform the swap / should be almost
            // instant anyway
            g_get_current_time(&abs_time);
            g_time_val_add(&abs_time, 3 * 1000);
            g_cond_timed_wait(g_buffer_swap_cond, g_buffer_swap_mutex,
                    &abs_time);
            fs_ml_unlock_buffer_swap();
#endif

            //printf("a0\n");
            g_mutex_lock(g_video_mutex);
            //printf("a1\n");

#ifdef DEBUG_VIDEO_SYNC
            printf(" 2");
#endif
            g_cond_signal(g_video_cond);


            //g_cond_wait(g_video_cond, g_video_mutex);
            //printf("a2\n");

            static GTimeVal abs_time;
            g_get_current_time(&abs_time);
            g_time_val_add(&abs_time, 100 * 1000);
            if (!fs_ml_is_quitting()) {
                g_cond_timed_wait(g_video_cond, g_video_mutex, &abs_time);
            }
#ifdef DEBUG_VIDEO_SYNC
            printf(" 5\n");
#endif

            //printf("a3\n");
            g_mutex_unlock(g_video_mutex);
            //printf("a4\n");
        }
    }
}

int fs_ml_get_vblank_count() {
    return g_vblank_count;
}

int64_t fs_ml_get_vblank_time() {
    return g_vblank_time;
}

void fs_ml_stop() {
    g_fs_ml_running = 0;
    // signal g_video_cond because video (main) thread may be blocking on
    // this condition
    g_cond_signal(g_video_cond);
}

static int eltime(int64_t t) {
    return (int) ((t - g_epoch));
}

static void update_frame() {
    if (g_fs_ml_video_update_function) {
        g_fs_ml_video_update_function();
    }
}

static void render_frame() {
    if (g_fs_ml_video_render_function) {
        g_fs_ml_video_render_function();
    }
}

static void swap_opengl_buffers() {
    //int64_t t1 = fs_get_monotonic_time();
    SDL_GL_SwapBuffers();
    //int64_t t2 = fs_get_monotonic_time();
}

static void gl_finish() {
    //int64_t t1 = fs_get_monotonic_time();
    fs_gl_finish();
    CHECK_GL_ERROR("render_frame");
    //int64_t t3 = fs_get_monotonic_time();
    //printf("          %lld : %lld\n", t2 - t1, t3 - t2);
}

static void vblank_post_handler() {
    g_vblank_time = fs_ml_monotonic_time();
    g_vblank_count++;

    fs_mutex_lock(g_vblank_mutex);
    g_vblank_times[g_vblank_index] = g_vblank_time;
    g_vblank_index = (g_vblank_index + 1) % VBLANK_COUNT;

    fs_mutex_unlock(g_vblank_mutex);

    g_sleep_until_vsync_last_time = g_vblank_time;
}

//void fs_ml_wait_vblank() {
//}

static void sleep_until_vsync() {
#ifdef NEW_VSYNC
    int64_t t = fs_get_monotonic_time();
    int64_t diff = g_next_vblank_at - t;
    //while (diff < 0) {
    //    diff += 2000;
    //}
    printf("%lld -------> %lld (%lld)\n", t, g_next_vblank_at, diff);
    int64_t t = fs_get_monotonic_time();
    int64_t diff = g_next_vblank_at - t;

    if (diff > 0) {
        diff = diff - 3000;
        if (diff > 0) {
            fs_ml_usleep(diff);
        }
    }
    return;
#else

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
#endif
}

static void full_sleep_until_vsync() {
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

#if 0
static void vblank_linear_regression(int64_t *a, int64_t *b, int *c) {
    int64_t x_sum = 0;
    int64_t y_sum = 0;
    int n = 0;
    for (int i = 0; i < VBLANK_COUNT; i++) {
        int64_t y = g_vblank_times[(g_vblank_index + i) % VBLANK_COUNT];
        if (y == 0) {
            // skip this unset value
            continue;
        }
        y_sum += y;
        x_sum += i;
        n++;
    }

    if (n == 0) {
        *a = 0;
        *b = 0;
        *c = 0;
        return;
    }

    int64_t x_avg = x_sum / n;
    int64_t y_avg = y_sum / n;

    int64_t numerator = 0;
    int64_t denomerator = 0;
    for (int i = 0; i < VBLANK_COUNT; i++) {
        int64_t y = g_vblank_times[(g_vblank_index + i) % VBLANK_COUNT];
        if (y == 0) {
            // skip this unset value
            continue;
        }

        numerator += (i - x_avg) * (y - y_avg);
        denomerator += (i - x_avg) * (i - x_avg);
    }

    if (denomerator == 0) {
        *a = 0;
        *b = 0;
        *c = 0;
        return;
    }
    *b = 1.0 * (numerator / denomerator);
    *a = y_avg - *b * x_avg;
    *c = n;
}

static void *vblank_thread(void *data) {
    printf("vblank_thread\n");

    while (1) {
        fs_mutex_lock(g_vblank_mutex);

        // c is number of data points
        int64_t a, b;
        int c;
        vblank_linear_regression(&a, &b, &c);
        printf("a = %d b = %d\n", a, b);

        fs_mutex_unlock(g_vblank_mutex);

        int64_t t = fs_get_monotonic_time();
        int64_t next = a + b * c;
        g_next_vblank_at = next;
        printf("%lld\n", next);

        int64_t notify = next - b * 0.5;

        int64_t diff = (notify - t);
        printf("%lld - %lld = %lld\n", notify, t, diff);
        while (diff > 1000) {
            printf("sleep_a %d\n", diff);
            fs_ml_usleep(diff);
            t = fs_get_monotonic_time();
            diff = (notify - t);
        }
#ifdef NEW_VSYNC
        g_cond_signal(g_video_cond);
#endif

        t = fs_get_monotonic_time();
        diff = (next - t);
        while (diff > 1000) {
            printf("sleep_b %d\n", diff);
            fs_ml_usleep(diff);
            t = fs_get_monotonic_time();
            diff = (next - t);
        }
    }

    return 0;
}
#endif


typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;

void (APIENTRYP glWaitSync)(GLsync sync, GLbitfield flags,
        GLuint64 timeout) = NULL;
GLenum (APIENTRYP glClientWaitSync)(GLsync sync, GLbitfield flags,
        GLuint64 timeout) = NULL;
GLsync (APIENTRYP glFenceSync)(GLenum condition, GLbitfield flags) = NULL;

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
static GLuint g_fence;
static GLsync g_sync;

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
    if (a && fs_ascii_strcasecmp(a, b) == 0) {
        return 1;
    }
    return 0;
}

static void init_sync_method() {
    fs_log("deciding video sync method\n");
    const char *c = fs_config_get_const_string("video_sync_method");
    if (check_sync_method(c, "swap")) {
        fs_log("- SYNC_SWAP\n");
        g_sync_method = SYNC_SWAP;
    }
    else if (check_sync_method(c, "swap-finish")) {
        fs_log("- SYNC_SWAP_FINISH\n");
        g_sync_method = SYNC_SWAP_FINISH;
    }
    else if (check_sync_method(c, "finish-swap-finish")) {
        fs_log("- SYNC_FINISH_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SWAP_FINISH;
    }
    else if (check_sync_method(c, "sleep-swap-finish")) {
        fs_log("- SYNC_SLEEP_SWAP_FINISH\n");
        g_sync_method = SYNC_SLEEP_SWAP_FINISH;
    }
    else if (check_sync_method(c, "finish-sleep-swap-finish")) {
        fs_log("- SYNC_FINISH_SLEEP_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SLEEP_SWAP_FINISH;
    }
    else if (check_sync_method(c, "swap-fence")) {
        fs_log("- SYNC_SWAP_FENCE\n");
        g_sync_method = SYNC_SWAP_FENCE;
    }
    else if (check_sync_method(c, "swap-sleep-fence")) {
        fs_log("- SYNC_SWAP_SLEEP_FENCE\n");
        g_sync_method = SYNC_SWAP_SLEEP_FENCE;
    }
    else if (c) {
        fs_log("WARNING: unknown sync method specified, using default\n");
        g_sync_method = 0;
    }

    int fence_support = g_has_nv_fence || g_has_apple_fence || g_has_arb_sync;
    if (g_sync_method >= SYNC_SWAP_FENCE_START && !fence_support) {
        fs_log("- no fence support, cannot use this sync method\n");
        g_sync_method = 0;
    }
    if (g_sync_method == 0) {
        fs_log("- using default sync method\n");
#ifdef LINUX
        fs_log("- SYNC_FINISH_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SWAP_FINISH;
#else
        fs_log("- SYNC_FINISH_SLEEP_SWAP_FINISH\n");
        g_sync_method = SYNC_FINISH_SLEEP_SWAP_FINISH;
#endif
    }

#if 0
    const char *sync_method = fs_config_get_const_string("sync_method");
    if (sync_method && fs_ascii_strcasecmp(sync_method, "fence") == 0) {
        if (g_has_arb_sync || g_has_apple_fence || g_has_nv_fence) {
            fs_log("- using fences\n");
            g_sync_flags = SYNC_FLAG_FENCE;
        }
        else {
            fs_log("- no suitable fence extension found\n");
        }
    }
    else if (sync_method && fs_ascii_strcasecmp(sync_method, "finish") == 0) {
        g_sync_flags = SYNC_FLAG_GLFINISH;
    }
    else if (sync_method && fs_ascii_strcasecmp(
            sync_method, "sleep+finish") == 0) {
        g_sync_flags = SYNC_FLAG_SLEEP | SYNC_FLAG_GLFINISH;
    }
    else {
        g_sync_flags = SYNC_FLAG_GLFINISH;
#ifdef LINUX

#else
        g_sync_flags |= SYNC_FLAG_SLEEP;
#endif
    }

    fs_log("- sync flags:\n");
    if (g_sync_flags & SYNC_FLAG_FENCE) {
        fs_log("  SYNC_FLAG_FENCE\n");
    }
    if (g_sync_flags & SYNC_FLAG_GLFINISH) {
        fs_log("  SYNC_FLAG_GLFINISH\n");
    }
    if (g_sync_flags & SYNC_FLAG_SLEEP) {
        fs_log("  SYNC_FLAG_SLEEP\n");
    }
#endif
}

static void check_opengl_caps() {
    g_sync_flags = 0;
    fs_log("checking OpenGL capabilities\n");
    const char *ext = (const char *) glGetString(GL_EXTENSIONS);
    if (ext) {
        if (strstr(ext, "GL_NV_fence") != NULL) {
            g_has_nv_fence = 1;
            //g_sync_flags |= SYNC_FLAG_FENCE;
            fs_log("GL_NV_fence extension found \n");
        }
        if (strstr(ext, "GL_APPLE_fence") != NULL) {
            g_has_apple_fence = 1;
            //g_sync_flags |= SYNC_FLAG_FENCE;
            fs_log("GL_APPLE_fence extension found\n");
        }
        if (strstr(ext, "GL_ARB_sync") != NULL) {
            fs_log("GL_ARB_sync extension found\n");
            glFenceSync = __GLeeGetProcAddress("glFenceSync");
            glWaitSync = __GLeeGetProcAddress("glWaitSync");
            glClientWaitSync = __GLeeGetProcAddress("glClientWaitSync");
            if (glFenceSync && glClientWaitSync) {
                g_has_arb_sync = 1;
                //g_sync_flags |= SYNC_FLAG_FENCE;
            }
            else {
                fs_log("error looking up functions\n");
            }
        }
    }
}

static void opengl_fence(int command) {
    if (command == FENCE_SET) {
        if (g_has_nv_fence) {
            //printf("...\n");
            glSetFenceNV(g_fence, GL_ALL_COMPLETED_NV);
            CHECK_GL_ERROR("glSetFenceNV(g_fence, GL_ALL_COMPLETED_NV)");
        }
        else if (g_has_apple_fence) {
            glSetFenceAPPLE(g_fence);
            CHECK_GL_ERROR("glSetFenceAPPLE(g_fence)");
        }
        else if (g_has_arb_sync) {
            g_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
            CHECK_GL_ERROR("glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)");
        }
    }
    else if (command == FENCE_WAIT) {
        if (g_has_nv_fence) {
            //printf("-- f --\n");
            //glFinishFenceNV(g_fence);
            //int64_t t1 = fs_get_monotonic_time();
            //fs_ml_usleep(10000);
            while (!glTestFenceNV(g_fence)) {
                CHECK_GL_ERROR("glTestFenceNV(g_fence)");
                //printf("-> %lld\n", fs_get_monotonic_time() - t1);
                //printf("%d\n", glGetError());
                fs_ml_usleep(1000);
                //printf("-> %lld\n", fs_get_monotonic_time() - t1);
            }
            CHECK_GL_ERROR("glTestFenceNV(g_fence)");
        }
        else if (g_has_apple_fence) {
            while (!glTestFenceAPPLE(g_fence)) {
                CHECK_GL_ERROR("glTestFenceAPPLE(g_fence)");
                fs_ml_usleep(1000);
            }
            CHECK_GL_ERROR("glTestFenceAPPLE(g_fence)");
        }
        else if (g_has_arb_sync) {
            int flags = GL_SYNC_FLUSH_COMMANDS_BIT;
            while (glClientWaitSync(g_sync, flags, 0)
                    == GL_TIMEOUT_EXPIRED) {
                CHECK_GL_ERROR("glClientWaitSync(g_sync, flags, 0)");
                flags = 0;
                fs_ml_usleep(1000);
            }
            CHECK_GL_ERROR("glClientWaitSync(g_sync, flags, 0)");
        }
    }
}

void fs_ml_render_iteration() {
    static int first = 1;
    if (first) {
        check_opengl_caps();
        init_sync_method();
        if (g_has_nv_fence) {
            glGenFencesNV(1, &g_fence);
            CHECK_GL_ERROR("glGenFencesNV(1, &g_fence)");
            //printf("created fence\n");
            //exit(1);
        }
        else if (g_has_apple_fence) {
            glGenFencesAPPLE(1, &g_fence);
            CHECK_GL_ERROR("glGenFencesAPPLE(1, &g_fence)");
        }
        first = 0;
    }

    g_sync_flags = 0;

    if (g_fs_ml_vblank_sync) {
        frame_wait();
        update_frame();
        CHECK_GL_ERROR("update_frame");

        render_frame();
        CHECK_GL_ERROR("render_frame");

        glFlush();

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
        /*
        //sleep_until_vsync();
        full_sleep_until_vsync();
        int64_t t1 = fs_get_monotonic_time();
        glFlush();
        opengl_fence(FENCE_WAIT);
        int64_t t2 = fs_get_monotonic_time();
        //printf("%lld\n", t2 - t1);
         */

        vblank_post_handler();
        synchronized_buffer_swap();
    }
    else if (g_fs_ml_benchmarking) {
        update_frame();
        render_frame();
        swap_opengl_buffers();
    }
    else {
        // when vsync is off, we wait until a new frame is ready and
        // then we display it immediately
        g_mutex_lock(g_video_mutex);

        // FIXME: TEST THE FOLLOWING...
        // wait max 40 ms to allow the user interface to work even if
        // the emu hangs
        GTimeVal abs_time;
        g_get_current_time(&abs_time);
        g_time_val_add(&abs_time, 40 * 1000);
        g_cond_timed_wait(g_video_cond, g_video_mutex, &abs_time);

        g_mutex_unlock(g_video_mutex);
        update_frame();
        render_frame();
        swap_opengl_buffers();
        //gl_finish();
    }

    if (g_fs_ml_video_post_render_function) {
        g_fs_ml_video_post_render_function();
    }
}

void fs_ml_render_init() {
    g_video_cond = g_cond_new();
    g_video_mutex = g_mutex_new();

    g_buffer_swap_cond = g_cond_new();
    g_buffer_swap_mutex = g_mutex_new();

    g_epoch = fs_get_monotonic_time();
    g_vblank_mutex = fs_mutex_create();
    //fs_emu_stat_queue_init(&g_vblank_times, VBLANK_TIMES_COUNT);

#ifdef NEW_VSYNC
    g_thread_create(vblank_thread, NULL, FALSE, NULL);
#endif

    if (fs_config_get_boolean("low_latency_vsync") == 1) {
        fs_log("using low latency vsync when full vsync is enabled\n");
        g_fs_ml_video_sync_low_latency = 1;
    }
    else if (fs_config_get_boolean("low_latency_vsync") == 0) {
        fs_log("disabling use of low latency vsync\n");
        g_fs_ml_video_sync_low_latency = 0;
    }
}
