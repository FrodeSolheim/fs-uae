#define FSEMU_INTERNAL 1
#include "fsemu-videothread.h"

#include "fsemu-glvideo.h"
#include "fsemu-mutex.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-video.h"

struct fsemu_video_thread_data {
    fsemu_size_t window_size;
    fsemu_size_t drawable_size;
    fsemu_rect_t client_area;
    fsemu_rect_t video_area;
    fsemu_rect_t video_rect;
};

static struct {
    // Mutex used to copy consistent data from UI thread over to video thread.
    fsemu_mutex_t *ui_video_mutex;
    // Data from UI thread
    struct fsemu_video_thread_data ui_thread_d;
    // Data (copy) for UI thread
    struct fsemu_video_thread_data thread_d;
} fsemu_videothread;

// ----------------------------------------------------------------------------

void fsemu_videothread_lock(void)
{
    fsemu_mutex_lock(fsemu_videothread.ui_video_mutex);
}

void fsemu_videothread_unlock(void)
{
    fsemu_mutex_unlock(fsemu_videothread.ui_video_mutex);
}

// ----------------------------------------------------------------------------

void fsemu_videothread_set_data_from_ui_thread(fsemu_size_t *window_size,
                                               fsemu_size_t *drawable_size,
                                               fsemu_rect_t *client_area,
                                               fsemu_rect_t *video_area,
                                               fsemu_rect_t *video_rect)
{
    // fsemu_mutex_lock(fsemu_videothread.ui_video_mutex);

    fsemu_videothread.ui_thread_d.window_size = *window_size;
    fsemu_videothread.ui_thread_d.drawable_size = *drawable_size;
    fsemu_videothread.ui_thread_d.client_area = *client_area;
    fsemu_videothread.ui_thread_d.video_area = *video_area;
    fsemu_videothread.ui_thread_d.video_rect = *video_rect;

    // fsemu_mutex_unlock(fsemu_videothread.ui_video_mutex);
}

void fsemu_videothread_copy_thread_data(void)
{
    fsemu_mutex_lock(fsemu_videothread.ui_video_mutex);

    fsemu_videothread.thread_d = fsemu_videothread.ui_thread_d;
    // printf("HEIGHT IS NOW %d\n", fsemu_videothread.thread_d.video_rect.h);

    fsemu_mutex_unlock(fsemu_videothread.ui_video_mutex);
}

#if 0
void fsemu_videothread_window_size(fsemu_size_t *size)
{
    *size = fsemu_videothread.thread_d.window_size;
}
#endif

void fsemu_videothread_drawable_size(fsemu_size_t *size)
{
    *size = fsemu_videothread.thread_d.drawable_size;
}

void fsemu_videothread_client_area(fsemu_rect_t *rect)
{
    *rect = fsemu_videothread.thread_d.client_area;
}

void fsemu_videothread_video_area(fsemu_rect_t *rect)
{
    *rect = fsemu_videothread.thread_d.video_area;
}

void fsemu_videothread_video_rect(fsemu_rect_t *rect)
{
    *rect = fsemu_videothread.thread_d.video_rect;
}

// ----------------------------------------------------------------------------

// FIXME:
#include "fsemu-quit.h"

static void *fsemu_videothread_entry(void *data)
{
    fsemu_video_log("Start of video thread function\n");
    fsemu_thread_set_video();

    // SDL_GL_MakeCurrent(fsemu_sdlwindow_window(),
    // fsemu_sdlwindow_gl_context());
    fsemu_video_log("Creating OpenGL context for video thread\n");
    // FIXME: Move to fsemu-video / fsemu-glvideo
    // For example: fsemu_video_init_video_thread()
    SDL_GL_CreateContext(fsemu_sdlwindow_window());

    fsemu_glvideo_init_gl_state();

    // Forget cached OpenGL state in fsemu-opengl module.
    // FIXME: Maybe call fsemu_opengl_reset()
    fsemu_opengl_forget_state();

#if 1
    fsemu_gui_item_t *old_snapshot = NULL;
#endif
    // int64_t t = fsemu_time_us();

    // FIXME: Use separate running state for thread, and shutdown from
    // the video module.
    while (!fsemu_quit_check()) {
        // glViewport(0, 0, 100, 100);
        // glClearColor(1.0, 0.0, 0.0, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT);

        // FIXME: Use semaphore or similar for this
        // OK, existing fsemu_video_work uses wait/timeout in glib queue
        // implementation, which is OK-ish actually.
        fsemu_video_work(1000 * 1000);
        if (fsemu_video_ready()) {
            printf("videothread - ready\n");
#if 1
            fsemu_gui_item_t *snapshot = fsemu_video_get_gui_snapshot_vt();
            if (fsemu_likely(snapshot)) {
                fsemu_gui_free_snapshot(old_snapshot);
                old_snapshot = snapshot;
            } else {
                snapshot = old_snapshot;
            }
            if (fsemu_likely(snapshot)) {
                fsemu_video_render_gui_early(snapshot);
            }
#endif
            fsemu_video_render();
#if 1
            if (fsemu_likely(snapshot)) {
                fsemu_video_render_gui(snapshot);
            }
#endif
            // int64_t time_us = fsemu_time_us();
            // printf("%d\n", (int) (time_us - t) / 1000);
            // t = time_us;
            // glFinish();
            printf("videothread - display\n");
            fsemu_video_display();
        }
    }

    fsemu_video_log("End of video thread function\n");
    return NULL;
}

// ----------------------------------------------------------------------------

void fsemu_videothread_start(void)
{
    fsemu_video_log("Creating video thread\n");
    // FIXME:
    // fsemu_thread_t *video_thread =
    fsemu_thread_create("fsemu-videothread", fsemu_videothread_entry, NULL);
}

// ----------------------------------------------------------------------------

void fsemu_videothread_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_videothread.ui_video_mutex = fsemu_mutex_create();
}
