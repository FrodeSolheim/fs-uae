#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/thread.h>
#include <fs/base.h>
#include <stdlib.h>
#ifdef USE_GLIB
#include <glib.h>
#endif
#ifdef USE_SDL2
#define USE_SDL
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
    GMutex mutex;
#elif defined(USE_SDL)
    SDL_mutex* mutex;
#endif
};

struct fs_condition {
#if defined(USE_PTHREADS)
    pthread_cond_t condition;
#elif defined(USE_GLIB)
    GCond condition;
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

fs_thread_id_t fs_thread_id(void)
{
    fs_thread_id_t thread_id = 0;
#if defined(USE_GLIB)
    thread_id = (uintptr_t) g_thread_self();
#else
#error no thread support
#endif
    return thread_id;
}

fs_thread *fs_thread_create(
        const char *name, fs_thread_function fn, void *data)
{
    fs_thread *thread = (fs_thread *) g_malloc(sizeof(fs_thread));
#if defined(USE_PTHREADS)
    pthread_attr_init(&thread->attr);
    pthread_attr_setdetachstate(&thread->attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread->thread, &thread->attr, fn, data);
#elif defined(USE_GLIB)
    thread->thread = g_thread_new(name, fn, data);
#else
#error no thread support
#endif
    return thread;
}

#if 0
fs_thread *fs_thread_create_detached(
        const char *name, fs_thread_function fn, void *data)
{
    fs_thread *thread = (fs_thread *) g_malloc(sizeof(fs_thread));
#if defined(USE_PTHREADS)
    pthread_attr_init(&thread->attr);
    pthread_attr_setdetachstate(&thread->attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread->thread, &thread->attr, fn, data);
#elif defined(USE_GLIB)
    thread->thread = g_thread_create(fn, data, FALSE, NULL);
#else
#error no thread support
#endif
    return thread;
}
#endif

void *fs_thread_wait(fs_thread *thread)
{
    void *result;
#if defined(USE_PTHREADS)
    pthread_join(thread->thread, &result);
    pthread_attr_destroy(&thread->attr);
#elif defined(USE_GLIB)
    /* FIXME: can use g_thread_ref if we want to keep a reference, and
     * implement fs_thread_destroy separately */
    result = g_thread_join(thread->thread);
#else
#error no thread support
#endif
    return result;
}

void fs_thread_free(fs_thread *thread)
{
    g_free(thread);
}

fs_mutex *fs_mutex_create()
{
    fs_mutex *mutex = (fs_mutex *) g_malloc(sizeof(fs_mutex));
#if defined(USE_PTHREADS)
    pthread_mutex_init(&mutex->mutex, NULL);
#elif defined(USE_GLIB)
    g_mutex_init(&mutex->mutex);
#elif defined(USE_SDL)
    mutex->mutex = SDL_CreateMutex();
#else
#error no thread support
#endif
    return mutex;
}

void fs_mutex_destroy(fs_mutex *mutex)
{
#if defined(USE_PTHREADS)
    pthread_mutex_destroy(&mutex->mutex);
#elif defined(USE_GLIB)
    g_mutex_clear(&mutex->mutex);
#elif defined(USE_SDL)
    SDL_DestroyMutex(mutex->mutex);
#else
#error no thread support
#endif
    g_free(mutex);
}

int fs_mutex_lock(fs_mutex *mutex)
{
#if defined(USE_PTHREADS)
    return pthread_mutex_lock(&mutex->mutex);
#elif defined(USE_GLIB)
    g_mutex_lock(&mutex->mutex);
    return 0;
#elif defined(USE_SDL)
    return SDL_mutexP(mutex->mutex);
#else
#error no thread support
#endif
}

int fs_mutex_unlock(fs_mutex *mutex)
{
#if defined(USE_PTHREADS)
    return pthread_mutex_unlock(&mutex->mutex);
#elif defined(USE_GLIB)
    g_mutex_unlock(&mutex->mutex);
    return 0;
#elif defined(USE_SDL)
    return SDL_mutexV(mutex->mutex);
#else
#error no thread support
#endif
}

fs_condition *fs_condition_create(void)
{
    fs_condition *condition = (fs_condition *) g_malloc(sizeof(fs_condition));
#if defined(USE_PTHREADS)
    pthread_cond_init(&condition->condition, NULL);
#elif defined(USE_GLIB)
    g_cond_init(&condition->condition);
#elif defined(USE_SDL)
    condition->condition = SDL_CreateCond();
#else
#error no thread support
#endif
    return condition;
}

void fs_condition_destroy(fs_condition *condition)
{
#if defined(USE_PTHREADS)
    pthread_cond_destroy(&condition->condition);
#elif defined(USE_GLIB)
    g_cond_clear(&condition->condition);
#elif defined(USE_SDL)
    SDL_DestroyCond(condition->condition);
#else
#error no thread support
#endif
    g_free(condition);
}

int fs_condition_wait (fs_condition *condition, fs_mutex *mutex)
{
#if defined(USE_PTHREADS)
    return pthread_cond_wait(&condition->condition, &mutex->mutex);
#elif defined(USE_GLIB)
    g_cond_wait(&condition->condition, &mutex->mutex);
    return 0;
#elif defined(USE_SDL)
    return SDL_CondWait(condition->condition, mutex->mutex);
#else
#error no thread support
#endif
}

int64_t fs_condition_get_wait_end_time(int period)
{
#if defined(USE_PTHREADS)
#error FIXME: implement with clock_gettime(CLOCK_REALTIME, &ts);
#elif defined(USE_GLIB)
    return g_get_monotonic_time() + period;
#else
    return fs_get_current_time() + period;
#endif
}

int fs_condition_wait_until(
        fs_condition *condition, fs_mutex *mutex, int64_t end_time)
{
#if defined(USE_PTHREADS)
    struct timespec tv;
    tv.tv_sec = end_time / 1000000;
    tv.tv_nsec = (end_time % 1000000) * 1000;
    return pthread_cond_timedwait(&condition->condition, &mutex->mutex, &tv);
#elif defined(USE_GLIB)
    gboolean result = g_cond_wait_until(
             &condition->condition, &mutex->mutex, end_time);
    return !result;
#elif defined(USE_SDL2)
#error FIXME: implement with SDL_CondWaitTimeout
#elif defined(USE_SDL)
#error no support for timed condition wait
#else
#error no thread support
#endif
}

int fs_condition_signal(fs_condition *condition)
{
#if defined(USE_PTHREADS)
    return pthread_cond_signal(&condition->condition);
#elif defined(USE_GLIB)
    g_cond_signal(&condition->condition);
    return 0;
#elif defined(USE_SDL)
    return SDL_CondSignal(condition->condition);
#else
#error no thread support
#endif
}

fs_semaphore *fs_semaphore_create(int value)
{
    fs_semaphore *semaphore = (fs_semaphore *) g_malloc(sizeof(fs_semaphore));
#if defined(USE_PSEM)
    sem_init(&semaphore->semaphore, 1, value);
#elif defined(USE_SDL)
    semaphore->semaphore = SDL_CreateSemaphore(value);
#else
#error no thread support
#endif
    return semaphore;
}

void fs_semaphore_destroy(fs_semaphore *semaphore)
{
#if defined(USE_PSEM)
    sem_destroy(&semaphore->semaphore);
#elif defined(USE_SDL)
    SDL_DestroySemaphore(semaphore->semaphore);
#else
#error no thread support
#endif
    g_free(semaphore);
}

int fs_semaphore_post(fs_semaphore *semaphore)
{
#if defined(USE_PSEM)
    return sem_post(&semaphore->semaphore);
#elif defined(USE_SDL)
    return SDL_SemPost(semaphore->semaphore);
#else
#error no thread support
#endif
}

int fs_semaphore_wait(fs_semaphore *semaphore)
{
#if defined(USE_PSEM)
    return sem_wait(&semaphore->semaphore);
#elif defined(USE_SDL)
    return SDL_SemWait(semaphore->semaphore);
#else
#error no thread support
#endif
}

int fs_semaphore_try_wait(fs_semaphore *semaphore)
{
#if defined(USE_PSEM)
    return sem_trywait(&semaphore->semaphore);
#elif defined(USE_SDL)
    return SDL_SemTryWait(semaphore->semaphore);
#else
#error no thread support
#endif
}

int fs_semaphore_wait_timeout_ms(fs_semaphore *semaphore, int timeout)
{
#if defined(USE_PSEM)
#error not implemented
#elif defined(USE_SDL)
	int result = SDL_SemWaitTimeout(semaphore->semaphore, timeout);
	if (result == 0) {
		return 0;
	}
	if (result == SDL_MUTEX_TIMEDOUT) {
		return FS_SEMAPHORE_TIMEOUT;
	}
	fs_log("WARNING: uae_sem_trywait_delay failed\n");
	return -3;
#else
#error no thread support
#endif
}
