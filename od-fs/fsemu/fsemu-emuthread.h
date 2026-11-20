#ifndef FSEMU_EMUTHREAD_H
#define FSEMU_EMUTHREAD_H

// This module contains data which can be safely used from the emulation
// thread.

#include <SDL3/SDL.h>

#include "fsemu-common.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the emuthread module. Can safely be called more than once.
void fsemu_emuthread_init(void);

void fsemu_emuthread_lock(void);
void fsemu_emuthread_unlock(void);

bool fsemu_emuthread_paused(void);

// Registers the current thread as the emulation thread.
void fsemu_emuthread_set_from_current(void);

extern SDL_ThreadID fsemu_emuthread_thread_id;

static inline bool fsemu_emuthread_is_current(void)
{
    SDL_assert(fsemu_emuthread_thread_id);
    return SDL_GetCurrentThreadID() == fsemu_emuthread_thread_id;
}

// Using a define to get source file/line location correct in error messages.
#define fsemu_emuthread_assert_current() SDL_assert(fsemu_emuthread_is_current());

void fsemu_emuthread_check_load_state(int *slot);

void fsemu_emuthread_check_save_state(int *slot);

// FIXME: fsemu_emuthread_(next_)action

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_EMUTHREAD_H */
