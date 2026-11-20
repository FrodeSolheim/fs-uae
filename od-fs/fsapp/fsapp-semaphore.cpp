/*
 * Semaphores.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-semaphore.h"

#include <SDL3/SDL.h>

#include "fs/log.h"
#include "fsapp-util.h"

struct fsapp_semaphore_t {
    SDL_Semaphore *semaphore;
};

void fsapp_semaphore_init(fsapp_semaphore_t *semaphore, int value)
{
    semaphore->semaphore = SDL_CreateSemaphore(value);
}

fsapp_semaphore_t *fsapp_semaphore_create(int value)
{
    fsapp_semaphore_t *semaphore = FSAPP_UTIL_MALLOC(fsapp_semaphore_t);
    fsapp_semaphore_init(semaphore, value);
    return semaphore;
}

void fsapp_semaphore_destroy(fsapp_semaphore_t *semaphore)
{
    SDL_DestroySemaphore(semaphore->semaphore);
    free(semaphore);
}

int fsapp_semaphore_post(fsapp_semaphore_t *semaphore)
{
    SDL_SignalSemaphore(semaphore->semaphore);
    return 0;
}

int fsapp_semaphore_wait(fsapp_semaphore_t *semaphore)
{
    SDL_WaitSemaphore(semaphore->semaphore);
    return 0;
}

int fsapp_semaphore_try_wait(fsapp_semaphore_t *semaphore)
{
    // FIXME: Check return values here, the semantics don't look the same
    return SDL_TryWaitSemaphore(semaphore->semaphore);
}

int fsapp_semaphore_wait_timeout_ns(fsapp_semaphore_t *semaphore, int64_t timeout_ns)
{
    Sint32 timeout_ms = -1;
    if (timeout_ns >= 0) {
        timeout_ms =
            (Sint32)SDL_min(SDL_NS_TO_MS(timeout_ns + SDL_NS_PER_MS - 1), (int64_t)SDL_MAX_SINT32);
    }
    bool result = SDL_WaitSemaphoreTimeout(semaphore->semaphore, timeout_ms);
    if (result) {
        return 0;
    } else {
        return FSAPP_SEMAPHORE_TIMEOUT;
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "fsapp_semaphore_wait_timeout_ns failed\n");
    return -1;
}
