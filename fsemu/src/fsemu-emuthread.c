#define FSEMU_INTERNAL
#include "fsemu-emuthread.h"

#include "fsemu-mutex.h"
#include "fsemu-thread.h"

#if 0
struct fsemu_video_thread_data {
    fsemu_size_t window_size;
    fsemu_size_t drawable_size;
    fsemu_rect_t client_area;
    fsemu_rect_t video_area;
    fsemu_rect_t video_rect;
};
#endif

static struct {
    bool paused;
    // Mutex used to copy consistent data from UI thread over to emu thread.
    fsemu_mutex_t *ui_emu_mutex;
    // Data from UI thread
    // struct fsemu_video_thread_data ui_thread_d;
    // Data (copy) for UI thread
    // struct fsemu_video_thread_data thread_d;
} fsemu_emuthread;

void fsemu_emuthread_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_emuthread.ui_emu_mutex = fsemu_mutex_create();
}

void fsemu_emuthread_lock(void)
{
    fsemu_mutex_lock(fsemu_emuthread.ui_emu_mutex);
}

void fsemu_emuthread_unlock(void)
{
    fsemu_mutex_unlock(fsemu_emuthread.ui_emu_mutex);
}

void fsemu_emuthread_set_from_current(void)
{
    fsemu_thread_set_emu();
}

bool fsemu_emuthread_paused(void)
{
    fsemu_thread_assert_emu();
    return fsemu_emuthread.paused;
}

void fsemu_emuthread_check_load_state(int *slot)
{
    fsemu_thread_assert_emu();
}

void fsemu_emuthread_check_save_state(int *slot)
{
    fsemu_thread_assert_emu();
}
