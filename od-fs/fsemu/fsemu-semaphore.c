#define FSEMU_INTERNAL
#include "fsemu-semaphore.h"

#include "fsemu-util.h"

#ifdef FSEMU_SDL
#include "fsemu-sdl.h"
#endif

#ifdef FSEMU_PSEM
#include <semaphore.h>
#endif

struct fsemu_semaphore_t {
#if defined(FSEMU_PSEM)
    sem_t semaphore;
#elif defined(FSEMU_SDL)
    SDL_Semaphore *semaphore;
#endif
};

void fsemu_semaphore_init(fsemu_semaphore_t *semaphore, int value)
{
#if defined(FSEMU_PSEM)
    sem_init(&semaphore->semaphore, 1, value);
#elif defined(FSEMU_SDL)
    semaphore->semaphore = SDL_CreateSemaphore(value);
#else
#error no thread support
#endif
}

fsemu_semaphore_t *fsemu_semaphore_create(int value)
{
    fsemu_semaphore_t *semaphore = FSEMU_UTIL_MALLOC(fsemu_semaphore_t);
    fsemu_semaphore_init(semaphore, value);
    return semaphore;
}

void fsemu_semaphore_destroy(fsemu_semaphore_t *semaphore)
{
#if defined(FSEMU_PSEM)
    sem_destroy(&semaphore->semaphore);
#elif defined(FSEMU_SDL)
    SDL_DestroySemaphore(semaphore->semaphore);
#else
#error no thread support
#endif
    free(semaphore);
}

int fsemu_semaphore_post(fsemu_semaphore_t *semaphore)
{
#if defined(FSEMU_PSEM)
    return sem_post(&semaphore->semaphore);
#elif defined(FSEMU_SDL)
    SDL_SignalSemaphore(semaphore->semaphore);
    return 0;
#else
#error no thread support
#endif
}

int fsemu_semaphore_wait(fsemu_semaphore_t *semaphore)
{
#if defined(FSEMU_PSEM)
    return sem_wait(&semaphore->semaphore);
#elif defined(FSEMU_SDL)
    SDL_WaitSemaphore(semaphore->semaphore);
    return 0;
#else
#error no thread support
#endif
}

int fsemu_semaphore_try_wait(fsemu_semaphore_t *semaphore)
{
    // FIXME: Check return values here, the semantics don't look the same
#if defined(FSEMU_PSEM)
    return sem_trywait(&semaphore->semaphore);
#elif defined(FSEMU_SDL)
    return SDL_TryWaitSemaphore(semaphore->semaphore);
#else
#error no thread support
#endif
}

int fsemu_semaphore_wait_timeout_ms(fsemu_semaphore_t *semaphore,
                                    int timeout_ms)
{
#if defined(FSEMU_PSEM)
#error not implemented
#elif defined(FSEMU_SDL)
    bool result = SDL_WaitSemaphoreTimeout(semaphore->semaphore, timeout_ms);
    if (result) {
        return 0;
    } else {
        return FSEMU_SEMAPHORE_TIMEOUT;
    }
    fsemu_log_warning("fsemu_semaphore_wait_timeout_ms failed\n");
    return -1;
#else
#error no thread support
#endif
}
