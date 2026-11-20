#define FSEMU_INTERNAL
#include "fsemu-emuthread.h"

#include "fsapp-mutex.h"

static struct {
    bool paused;
    // Mutex used to copy consistent data from UI thread over to emu thread.
    fsapp_mutex_t *ui_emu_mutex;
} fsemu_emuthread;

void fsemu_emuthread_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_emuthread.ui_emu_mutex = fsapp_mutex_create();
}

void fsemu_emuthread_lock(void)
{
    fsapp_mutex_lock(fsemu_emuthread.ui_emu_mutex);
}

void fsemu_emuthread_unlock(void)
{
    fsapp_mutex_unlock(fsemu_emuthread.ui_emu_mutex);
}

SDL_ThreadID fsemu_emuthread_thread_id;

void fsemu_emuthread_set_from_current(void)
{
    fsemu_emuthread_thread_id = SDL_GetCurrentThreadID();
}

bool fsemu_emuthread_paused(void)
{
    fsemu_emuthread_assert_current();
    return fsemu_emuthread.paused;
}

void fsemu_emuthread_check_load_state(int *slot)
{
    fsemu_emuthread_assert_current();
}

void fsemu_emuthread_check_save_state(int *slot)
{
    fsemu_emuthread_assert_current();
}
