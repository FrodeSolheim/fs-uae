#ifndef THREADDEP_THREAD_H
#define THREADDEP_THREAD_H

#include "uae/log.h"

#include "fsemu-semaphore.h"

#include <stddef.h>

typedef fsemu_semaphore_t *uae_sem_t;

static inline int uae_sem_init(uae_sem_t *sem, int dummy, int init)
{
    *sem = fsemu_semaphore_create(init);
    return (*sem == 0);
}

static inline void uae_sem_destroy(uae_sem_t *sem)
{
    if (*sem) {
        fsemu_semaphore_destroy(*sem);
        *sem = NULL;
    }
}

static inline int uae_sem_post(uae_sem_t *sem)
{
    return fsemu_semaphore_post(*sem);
}

static inline void uae_sem_unpost(uae_sem_t*)
{
    uae_log_warning("uae_sem_unpost used");
}

static inline int uae_sem_wait(uae_sem_t *sem)
{
    return fsemu_semaphore_wait(*sem);
}

static inline int uae_sem_trywait(uae_sem_t *sem)
{
    return fsemu_semaphore_try_wait(*sem);
}

static inline int uae_sem_trywait_delay(uae_sem_t *sem, int millis)
{
    int result = fsemu_semaphore_wait_timeout_ms(*sem, millis);
    if (result == 0) {
        return 0;
    } else if (result == FSEMU_SEMAPHORE_TIMEOUT) {
        return -1;
    }
    return -3;
}

#include <fs/thread.h>

// FIXME: Possibly only safe for 64-bit systems now
// SDL3: typedef Uint64 SDL_ThreadID;
// Maybe better to use uint64_t also here...
typedef fs_thread *uae_thread_id;

// #define BAD_THREAD NULL
#define BAD_THREAD 0

#define uae_set_thread_priority(thread_id, pri)

// typedef int (*uae_thread_function) (void *);
// typedef fs_thread_function uae_thread_function;
typedef void (*uae_thread_function)(void *);

int uae_start_thread(const char *name,
                     uae_thread_function fn,
                     void *arg,
                     uae_thread_id *tid);
extern int uae_start_thread_fast(void (*f)(void *), void *arg, uae_thread_id *thread);
int uae_wait_thread(uae_thread_id thread);
void uae_end_thread(uae_thread_id *thread);

void uae_register_main_thread(void);
bool uae_is_main_thread(void);

void uae_register_emulation_thread(void);
bool uae_is_emulation_thread(void);

uae_thread_id uae_thread_get_id(void);

/* Do nothing; thread exits if thread function returns.  */
#define UAE_THREAD_EXIT \
    do {                \
    } while (0)

#include "commpipe.h"

#endif // THREADDEP_THREAD_H
