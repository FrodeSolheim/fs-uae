#include "fsemu-internal.h"
#include "fsemu-videothread.h"

#include "fsemu-mutex.h"
#include "fsemu-thread.h"

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

void fsemu_videothread_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_videothread.ui_video_mutex = fsemu_mutex_create();
}

void fsemu_videothread_lock(void)
{
    fsemu_mutex_lock(fsemu_videothread.ui_video_mutex);
}

void fsemu_videothread_unlock(void)
{
    fsemu_mutex_unlock(fsemu_videothread.ui_video_mutex);
}

// ---------------------------------------------------------------------------

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
