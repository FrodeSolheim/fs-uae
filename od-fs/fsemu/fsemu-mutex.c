#define FSEMU_INTERNAL
#include "fsemu-mutex.h"

#include <stdlib.h>

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fsemu-util.h"

#ifdef FSEMU_GLIB
#include "fsemu-glib.h"
#endif
#ifdef FSEMU_SDL
#include "fsemu-sdl.h"
#endif
#ifdef USE_PTHREADS
#include <pthread.h>
#endif

struct fsemu_mutex {
#if defined(FSEMU_PTHREADS)
    pthread_mutex_t mutex;
#elif defined(FSEMU_GLIB)
    GMutex mutex;
#elif defined(FSEMU_SDL)
    SDL_mutex* mutex;
#endif
};

void fsemu_mutex_init(fsemu_mutex_t* mutex) {
    SDL_assert(mutex != NULL);
#if defined(FSEMU_PTHREADS)
    pthread_mutex_init(&mutex->mutex, NULL);
#elif defined(FSEMU_GLIB)
    g_mutex_init(&mutex->mutex);
#elif defined(FSEMU_SDL)
    mutex->mutex = SDL_CreateMutex();
#else
#error no thread support
#endif
}

fsemu_mutex_t* fsemu_mutex_create(void) {
    fsemu_mutex_t* mutex = FSEMU_UTIL_MALLOC(fsemu_mutex_t);
    fsemu_mutex_init(mutex);
    return mutex;
}

void fsemu_mutex_destroy(fsemu_mutex_t* mutex) {
    SDL_assert(mutex != NULL);
#if defined(FSEMU_PTHREADS)
    pthread_mutex_destroy(&mutex->mutex);
#elif defined(FSEMU_GLIB)
    g_mutex_clear(&mutex->mutex);
#elif defined(FSEMU_SDL)
    SDL_DestroyMutex(mutex->mutex);
#else
#error no thread support
#endif
    free(mutex);
}

int fsemu_mutex_lock(fsemu_mutex_t* mutex) {
    SDL_assert(mutex != NULL);
#if defined(FSEMU_PTHREADS)
    return pthread_mutex_lock(&mutex->mutex);
#elif defined(FSEMU_GLIB)
    g_mutex_lock(&mutex->mutex);
    return 0;
#elif defined(FSEMU_SDL)
    return SDL_mutexP(mutex->mutex);
#else
#error no thread support
#endif
}

int fsemu_mutex_unlock(fsemu_mutex_t* mutex) {
    SDL_assert(mutex != NULL);
#if defined(FSEMU_PTHREADS)
    return pthread_mutex_unlock(&mutex->mutex);
#elif defined(FSEMU_GLIB)
    g_mutex_unlock(&mutex->mutex);
    return 0;
#elif defined(FSEMU_SDL)
    return SDL_mutexV(mutex->mutex);
#else
#error no thread support
#endif
}
