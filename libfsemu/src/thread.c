#include <fs/thread.h>
#include <stdlib.h>

#ifdef USE_GLIB
#include <glib.h>
#endif

#ifdef USE_SDL
#include <SDL.h>
#include <SDL_thread.h>
#endif

#ifdef USE_PTHREADS
#include <pthread.h>
#define USE_PSEM
#endif

#ifdef USE_PSEM
#include <semaphore.h>
#endif

struct fs_thread {
#if defined(USE_PTHREADS)
    pthread_t thread;
    pthread_attr_t attr;
#elif defined(USE_GLIB)
    GThread *thread;
#endif
};

struct fs_mutex {
#if defined(USE_PTHREADS)
    pthread_mutex_t mutex;
#elif defined(USE_GLIB)
    GMutex* mutex;
#elif defined(USE_SDL)
    SDL_mutex* mutex;
#endif
};

struct fs_condition {
#if defined(USE_PTHREADS)
    pthread_cond_t condition;
#elif defined(USE_GLIB)
    GCond* condition;
#elif defined(USE_SDL)
    SDL_cond* condition;
#endif
};

struct fs_semaphore {
#if defined(USE_PSEM)
    sem_t semaphore;
#elif defined(USE_SDL)
    SDL_sem* semaphore;
#endif
};

fs_thread *fs_thread_create(fs_thread_function fn, void *data) {
    fs_thread *thread = (fs_thread *) malloc(sizeof(fs_thread));
#if defined(USE_PTHREADS)
    pthread_attr_init(&thread->attr);
    pthread_attr_setdetachstate(&thread->attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread->thread, &thread->attr, fn, data);
#elif defined(USE_GLIB)
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
    void *result;
#if defined(USE_PTHREADS)
    pthread_join(thread->thread, &result);
    pthread_attr_destroy(&thread->attr);
#elif defined(USE_GLIB)
    // FIXME: can use g_thread_ref if we want to keep a reference,
    // and implemented fs_thread_destroy separately
    //g_thread_ref(thread->thread);
    result = g_thread_join(thread->thread);
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
    free(thread);
}

fs_mutex *fs_mutex_create() {
    fs_mutex *mutex = (fs_mutex *) malloc(sizeof(fs_mutex));
#if defined(USE_PTHREADS)
    pthread_mutex_init(&mutex->mutex, NULL);
//#elif defined(USE_GLIB)
#elif defined(USE_GLIB)
    mutex->mutex = g_mutex_new();
#elif defined(USE_SDL)
    mutex->mutex = SDL_CreateMutex();
#endif
    return mutex;
}

void fs_mutex_destroy(fs_mutex *mutex) {
#if defined(USE_PTHREADS)
    pthread_mutex_destroy(&mutex->mutex);
#elif defined(USE_GLIB)
    g_mutex_free(mutex->mutex);
#elif defined(USE_SDL)
    SDL_DestroyMutex(mutex->mutex);
#endif
    free(mutex);
}

int fs_mutex_lock(fs_mutex *mutex) {
#if defined(USE_PTHREADS)
    return pthread_mutex_lock(&mutex->mutex);
#elif defined(USE_GLIB)
    g_mutex_lock(mutex->mutex);
    return 0;
#elif defined(USE_SDL)
    return SDL_mutexP(mutex->mutex);
#endif
}

int fs_mutex_unlock(fs_mutex *mutex) {
#if defined(USE_PTHREADS)
    return pthread_mutex_unlock(&mutex->mutex);
#elif defined(USE_GLIB)
    g_mutex_unlock(mutex->mutex);
    return 0;
#elif defined(USE_SDL)
    return SDL_mutexV(mutex->mutex);
#endif
}

fs_condition *fs_condition_create(void) {
    fs_condition *condition = (fs_condition *) malloc(sizeof(fs_condition));
#if defined(USE_PTHREADS)
    pthread_cond_init(&condition->condition, NULL);
#elif defined(USE_GLIB)
    condition->condition = g_cond_new();
#elif defined(USE_SDL)
    condition->condition = SDL_CreateCond();
#endif
    return condition;
}

void fs_condition_destroy(fs_condition *condition) {
#if defined(USE_PTHREADS)
    pthread_cond_destroy(&condition->condition);
#elif defined(USE_GLIB)
    g_cond_free(condition->condition);
#elif defined(USE_SDL)
    SDL_DestroyCond(condition->condition);
#endif
    free(condition);
}

int fs_condition_wait (fs_condition *condition, fs_mutex *mutex) {
#if defined(USE_PTHREADS)
    return pthread_cond_wait(&condition->condition, &mutex->mutex);
#elif defined(USE_GLIB)
    g_cond_wait(condition->condition, mutex->mutex);
    return 0;
#elif defined(USE_SDL)
    return SDL_CondWait(condition->condition, mutex->mutex);
#endif
}

int fs_condition_timed_wait (fs_condition *condition, fs_mutex *mutex,
        int64_t real_time) {
#if defined(USE_PTHREADS)
    struct timespec tv;
    tv.tv_sec = real_time / 1000000;
    tv.tv_nsec = (real_time % 1000000) * 1000;
    return pthread_cond_timedwait(&condition->condition, &mutex->mutex, &tv);
#elif defined(USE_GLIB)
    GTimeVal tv;
    tv.tv_sec = real_time / 1000000;
    tv.tv_usec = real_time % 1000000;
    gboolean result = g_cond_timed_wait(condition->condition, mutex->mutex,
            &tv);
    return !result;
#elif defined(USE_SDL)
    // FIXME: no timed wait...
    return SDL_CondWait(condition->condition, mutex->mutex);
#endif
}

int fs_condition_signal(fs_condition *condition) {
#if defined(USE_PTHREADS)
    return pthread_cond_signal(&condition->condition);
#elif defined(USE_GLIB)
    g_cond_signal(condition->condition);
    return 0;
#elif defined(USE_SDL)
    return SDL_CondSignal(condition->condition);
#endif
}

fs_semaphore *fs_semaphore_create(int value) {
    fs_semaphore *semaphore = (fs_semaphore *) malloc(sizeof(fs_semaphore));
#if defined(USE_PSEM)
    sem_init(&semaphore->semaphore, 1, value);
#elif defined(USE_SDL)
    semaphore->semaphore = SDL_CreateSemaphore(value);
#endif
    return semaphore;
}

void fs_semaphore_destroy(fs_semaphore *semaphore) {
#if defined(USE_PSEM)
    sem_destroy(&semaphore->semaphore);
#elif defined(USE_SDL)
    SDL_DestroySemaphore(semaphore->semaphore);
#endif
    free(semaphore);
}

int fs_semaphore_post(fs_semaphore *semaphore) {
#if defined(USE_PSEM)
    return sem_post(&semaphore->semaphore);
#elif defined(USE_SDL)
    return SDL_SemPost(semaphore->semaphore);
#endif
}

int fs_semaphore_wait(fs_semaphore *semaphore) {
#if defined(USE_PSEM)
    return sem_wait(&semaphore->semaphore);
#elif defined(USE_SDL)
    return SDL_SemWait(semaphore->semaphore);
#endif
}

int fs_semaphore_try_wait(fs_semaphore *semaphore) {
#if defined(USE_PSEM)
    return sem_trywait(&semaphore->semaphore);
#elif defined(USE_SDL)
    return SDL_SemTryWait(semaphore->semaphore);
#endif
}
