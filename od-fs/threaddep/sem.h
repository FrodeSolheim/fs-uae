#ifndef THREADDEP_SEM_H
#define THREADDEP_SEM_H

#include <stddef.h>

#include "fsemu-semaphore.h"

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

// FIXME: ...
extern void uae_sem_unpost(uae_sem_t*);

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

#endif  // THREADDEP_SEM_H
