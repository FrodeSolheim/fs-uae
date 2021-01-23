#define FSEMU_INTERNAL 1
#include "fsemu-thread.h"

// #ifdef HAVE_CONFIG_H
// #include "config.h"
// #endif

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fsemu-time.h"
#include "fsemu-util.h"

// #include <fs/base.h>
// #include <fs/log.h>
// #include <fs/thread.h>

#ifdef FSEMU_SDL
#ifndef USE_SDL2
#define USE_SDL2
#endif
#endif

#ifdef FSEMU_GLIB
#ifndef USE_GLIB
#define USE_GLIB
#endif
#endif

#include <stdlib.h>
#ifdef USE_GLIB
#include "fsemu-glib.h"
#endif
#ifdef USE_SDL2
#ifndef USE_SDL
#define USE_SDL
#endif
#endif
#ifdef USE_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#endif
#ifdef USE_PTHREADS
#include <pthread.h>
#define USE_PSEM
#endif
#ifdef USE_PSEM
#include <semaphore.h>
#endif

#ifdef FSEMU_OS_WINDOWS
#include <Windows.h>
#endif

#ifdef FSEMU_OS_LINUX
#include <errno.h>
#endif

struct fsemu_thread {
#if defined(USE_PTHREADS)
    pthread_t thread;
    pthread_attr_t attr;
#elif defined(USE_GLIB)
    GThread *thread;
#endif
};

// struct fsemu_mutex {
// #if defined(USE_PTHREADS)
//     pthread_mutex_t mutex;
// #elif defined(USE_GLIB)
//     GMutex mutex;
// #elif defined(USE_SDL)
//     SDL_mutex *mutex;
// #endif
// };

struct fs_condition {
#if defined(USE_PTHREADS)
    pthread_cond_t condition;
#elif defined(USE_GLIB)
    GCond condition;
#elif defined(USE_SDL)
    SDL_cond *condition;
#endif
};

struct fs_semaphore {
#if defined(USE_PSEM)
    sem_t semaphore;
#elif defined(USE_SDL)
    SDL_sem *semaphore;
#endif
};

// static struct {
// } fsemu_thread;

fsemu_thread_id_t fsemu_thread_emu_thread_id;
fsemu_thread_id_t fsemu_thread_main_thread_id;
fsemu_thread_id_t fsemu_thread_video_thread_id;

void fsemu_thread_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_time_init();

    fsemu_thread_set_main();
}

void fsemu_thread_set_emu(void)
{
    fsemu_thread_emu_thread_id = fsemu_thread_id();
}

void fsemu_thread_set_main(void)
{
    fsemu_thread_main_thread_id = fsemu_thread_id();
}

void fsemu_thread_set_video(void)
{
    fsemu_thread_video_thread_id = fsemu_thread_id();
}

fsemu_thread_id_t fsemu_thread_id(void)
{
    fsemu_thread_id_t thread_id = 0;
#if defined(USE_PTHREADS)
#error pthreads support must be updated
#elif defined(FSEMU_GLIB)
    thread_id = (uintptr_t)(void *) g_thread_self();
#else
#error no thread support
#endif
    return thread_id;
}

fsemu_thread_t *fsemu_thread_create(const char *name,
                                    fsemu_thread_function_t fn,
                                    void *data)
{
    fsemu_thread_t *thread = FSEMU_UTIL_MALLOC0(fsemu_thread_t);
#if defined(USE_PTHREADS)
    pthread_attr_init(&thread->attr);
    pthread_attr_setdetachstate(&thread->attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread->thread, &thread->attr, fn, data);
#elif defined(FSEMU_GLIB)
    thread->thread = g_thread_new(name, fn, data);
#else
#error no thread support
#endif
    return thread;
}

#if 0

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

#endif

// fsemu_mutex_t *fsemu_mutex_create(void)
// {
//     fsemu_mutex_t *mutex = (fsemu_mutex_t *)
//     g_malloc(sizeof(fsemu_mutex_t));
// #if defined(USE_PTHREADS)
//     pthread_mutex_init(&mutex->mutex, NULL);
// #elif defined(USE_GLIB)
//     g_mutex_init(&mutex->mutex);
// #elif defined(USE_SDL)
//     mutex->mutex = SDL_CreateMutex();
// #else
// #error no thread support
// #endif
//     return mutex;
// }

// #if 0
// void fsemu_mutex_destroy(fsemu_mutex_t *mutex)
// {
// #if defined(USE_PTHREADS)
//     pthread_mutex_destroy(&mutex->mutex);
// #elif defined(USE_GLIB)
//     g_mutex_clear(&mutex->mutex);
// #elif defined(USE_SDL)
//     SDL_DestroyMutex(mutex->mutex);
// #else
// #error no thread support
// #endif
//     g_free(mutex);
// }
// #endif

// int fsemu_mutex_lock(fsemu_mutex_t *mutex)
// {
// #if defined(USE_PTHREADS)
//     return pthread_mutex_lock(&mutex->mutex);
// #elif defined(USE_GLIB)
//     g_mutex_lock(&mutex->mutex);
//     return 0;
// #elif defined(USE_SDL)
//     return SDL_mutexP(mutex->mutex);
// #else
// #error no thread support
// #endif
// }

// int fsemu_mutex_unlock(fsemu_mutex_t *mutex)
// {
// #if defined(USE_PTHREADS)
//     return pthread_mutex_unlock(&mutex->mutex);
// #elif defined(USE_GLIB)
//     g_mutex_unlock(&mutex->mutex);
//     return 0;
// #elif defined(USE_SDL)
//     return SDL_mutexV(mutex->mutex);
// #else
// #error no thread support
// #endif
// }

#if 0
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
	fsemu_log("WARNING: uae_sem_trywait_delay failed\n");
	return -3;
#else
#error no thread support
#endif
}

#endif

#ifdef FSEMU_OS_LINUX
#include <sched.h>
#include <sys/resource.h>
#include <sys/time.h>
#endif

void fsemu_thread_set_priority(void)
{
#ifdef FSEMU_OS_WINDOWS
#ifndef TIMERR_NOERROR
#define TIMERR_NOERROR 0
#endif
#if 0
    if (timeBeginPeriod(1) == TIMERR_NOERROR) {
        fsemu_log("successfully set timeBeginPeriod(1)\n");
    } else {
        fsemu_log("error setting timeBeginPeriod(1)\n");
    }
#endif
#if 0
    if (SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS)) {
        fsemu_log(
            "set process priority class to ABOVE_NORMAL_PRIORITY_CLASS\n");
    } else {
        int dwError = GetLastError();
        fsemu_log("Failed to set process priority class (%d)\n", dwError);
    }
#endif
#endif

#ifdef FSEMU_OS_LINUX
    struct rlimit rlim;
    getrlimit(RLIMIT_RTPRIO, &rlim);
    fsemu_log("[FSE] Thread priority current %d max %d\n",
              (int) rlim.rlim_cur,
              (int) rlim.rlim_max);
    // rlim.rlim_cur = 10;
    // rlim.rlim_max = 10;
    // setrlimit(RLIMIT_RTPRIO, &rlim);
    // getrlimit(RLIMIT_RTPRIO, &rlim);
    // printf("%d %d\n", rlim.rlim_cur, rlim.rlim_max);

    int policy;
    struct sched_param params;
    pthread_getschedparam(pthread_self(), &policy, &params);
    // params.sched_priority = sched_get_priority_min(SCHED_FIFO);
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);
    // params.sched_priority = 50;
    fsemu_log("[FSE] Trying to set priority to %d\n", params.sched_priority);
    int result = sched_setscheduler(0, SCHED_FIFO, &params);
    if (result == 0) {
        fsemu_log("[FSE] Has set real time priority\n");
    } else {
        fsemu_log("[FSE] Could not set real time priority, errno = %d\n",
                  errno);
    }
#endif
}
