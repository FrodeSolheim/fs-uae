#define FSEMU_INTERNAL
#include "fsemu-emuthread.h"

static struct {
    bool paused;
    // Mutex used to copy consistent data from UI thread over to emu thread.
    SDL_Mutex *ui_emu_mutex;
} fsemu_emuthread;

void fsemu_emuthread_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_emuthread.ui_emu_mutex = SDL_CreateMutex();
}

void fsemu_emuthread_lock(void)
{
    SDL_LockMutex(fsemu_emuthread.ui_emu_mutex);
}

void fsemu_emuthread_unlock(void)
{
    SDL_UnlockMutex(fsemu_emuthread.ui_emu_mutex);
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
