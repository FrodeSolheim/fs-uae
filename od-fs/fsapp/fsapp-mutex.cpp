/*
 * Mutexes.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-mutex.h"

#include <glib.h>
#include <stdlib.h>

#include "fs/log.h"
#include "fsapp-tsan.h"
#include "fsapp-util.h"

struct fsapp_mutex {
    GMutex mutex;
};

void fsapp_mutex_init(fsapp_mutex_t *mutex)
{
    SDL_assert(mutex != NULL);
    g_mutex_init(&mutex->mutex);
}

fsapp_mutex_t *fsapp_mutex_create(void)
{
    fsapp_mutex_t *mutex = FSAPP_UTIL_MALLOC(fsapp_mutex_t);
    fsapp_mutex_init(mutex);
    return mutex;
}

void fsapp_mutex_destroy(fsapp_mutex_t *mutex)
{
    SDL_assert(mutex != NULL);
    g_mutex_clear(&mutex->mutex);
    free(mutex);
}

int fsapp_mutex_lock(fsapp_mutex_t *mutex)
{
    SDL_assert(mutex != NULL);
    g_mutex_lock(&mutex->mutex);
    // GMutex is not seen by ThreadSanitizer (fsapp-tsan.h).
    FSAPP_TSAN_ACQUIRE(mutex);
    return 0;
}

int fsapp_mutex_unlock(fsapp_mutex_t *mutex)
{
    SDL_assert(mutex != NULL);
    FSAPP_TSAN_RELEASE(mutex);
    g_mutex_unlock(&mutex->mutex);
    return 0;
}
