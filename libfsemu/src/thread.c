#include <fs/thread.h>

#define USE_GLIB
#define USE_SDL

#ifdef USE_GLIB
#include <glib.h>
#endif

#ifdef USE_SDL
#include <SDL.h>
#include <SDL_thread.h>
#endif

#ifdef USE_GLIB
struct fs_thread {
    GThread *thread;
};
#endif

/*
#ifdef USE_SDL
struct fs_thread {
    SDL_Thread* thread;
};
#endif
*/

#ifdef USE_SDL
struct fs_mutex {
    SDL_mutex* mutex;
};
#endif

#ifdef USE_SDL
struct fs_semaphore {
    SDL_sem* semaphore;
};
#endif

/*
void fs_thread_init() {
    g_thread_init(NULL);
}
*/

fs_thread *fs_thread_create(fs_thread_function fn, void *data) {
    fs_thread *thread = (fs_thread *) malloc(sizeof(fs_thread));
#ifdef USE_GLIB
    thread->thread = g_thread_create(fn, data, TRUE, NULL);
#endif
/*
#ifdef USE_SDL
    thread->thread = SDL_CreateThread(fn, data);
#endif
*/
    return thread;
}

void *fs_thread_wait(fs_thread *thread) {
#ifdef USE_GLIB
    // FIXME: can use g_thread_ref if we want to keep a reference,
    // and implemented fs_thread_destroy separately
    //g_thread_ref(thread->thread);
    void *result = g_thread_join(thread->thread);
#endif
/*
#ifdef USE_SDL
    return SDL_WaitThread(thread->thread, NULL);
#endif
*/
    return result;
}

void fs_thread_destroy(fs_thread *thread) {
#ifdef USE_GLIB
    //g_thread_unref(thread->thread);
#endif
    g_free(thread);
}

fs_mutex *fs_mutex_create() {
    fs_mutex *mutex = (fs_mutex *) malloc(sizeof(fs_mutex));
#ifdef USE_SDL
    mutex->mutex = SDL_CreateMutex();
#endif
    return mutex;
}

void fs_mutex_destroy(fs_mutex *mutex) {
#ifdef USE_SDL
    SDL_DestroyMutex(mutex->mutex);
#endif
    free(mutex);
}

int fs_mutex_lock(fs_mutex *mutex) {
#ifdef USE_SDL
    return SDL_mutexP(mutex->mutex);
#endif
}

int fs_mutex_unlock(fs_mutex *mutex) {
#ifdef USE_SDL
    return SDL_mutexV(mutex->mutex);
#endif
}

fs_semaphore *fs_semaphore_create(int value) {
    fs_semaphore *semaphore = (fs_semaphore *) malloc(sizeof(fs_semaphore));
#ifdef USE_SDL
    semaphore->semaphore = SDL_CreateSemaphore(value);
#endif
    return semaphore;
}

void fs_semaphore_destroy(fs_semaphore *semaphore) {
#ifdef USE_SDL
    SDL_DestroySemaphore(semaphore->semaphore);
#endif
    free(semaphore);
}

int fs_semaphore_post(fs_semaphore *semaphore) {
#ifdef USE_SDL
    return SDL_SemPost(semaphore->semaphore);
#endif
}

int fs_semaphore_wait(fs_semaphore *semaphore) {
#ifdef USE_SDL
    return SDL_SemWait(semaphore->semaphore);
#endif
}

int fs_semaphore_try_wait(fs_semaphore *semaphore) {
#ifdef USE_SDL
    return SDL_SemTryWait(semaphore->semaphore);
#endif
}
