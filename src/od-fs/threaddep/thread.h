#ifndef THREADDEP_THREAD_H
#define THREADDEP_THREAD_H

#include <fs/thread.h>

typedef fs_semaphore *uae_sem_t;
typedef fs_thread *uae_thread_id;

static inline int uae_sem_init(uae_sem_t *sem, int dummy, int init) {
   *sem = fs_semaphore_create(init);
   return (*sem == 0);
}

static inline void uae_sem_destroy(uae_sem_t *sem) {
    return fs_semaphore_destroy(*sem);
}

static inline int uae_sem_post(uae_sem_t *sem) {
    return fs_semaphore_post(*sem);
}

static inline int uae_sem_wait(uae_sem_t *sem) {
    return fs_semaphore_wait(*sem);
}

static inline int uae_sem_trywait(uae_sem_t *sem) {
    return fs_semaphore_try_wait(*sem);
}

//#define uae_sem_post(PSEM)     SDL_SemPost (*PSEM)
//#define uae_sem_wait(PSEM)     SDL_SemWait (*PSEM)
//#define uae_sem_trywait(PSEM)  SDL_SemTryWait (*PSEM)
//#define uae_sem_getvalue(PSEM) SDL_SemValue (*PSEM)

#include "commpipe.h"

#define BAD_THREAD NULL

#define uae_set_thread_priority(thread_id, pri)

//typedef int (*uae_thread_function) (void *);
typedef fs_thread_function uae_thread_function;
int uae_start_thread (char *name, uae_thread_function fn, void *arg,
        uae_thread_id *tid);
int uae_wait_thread (uae_thread_id thread);
void uae_end_thread (uae_thread_id *thread);

/* Do nothing; thread exits if thread function returns.  */
#define UAE_THREAD_EXIT do {} while (0)

#endif // THREADDEP_THREAD_H
