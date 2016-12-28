#ifndef THREADDEP_SEM_H
#define THREADDEP_SEM_H

#include <stddef.h>
#include <fs/thread.h>

typedef fs_semaphore *uae_sem_t;

static inline int uae_sem_init(uae_sem_t *sem, int dummy, int init)
{
	*sem = fs_semaphore_create(init);
	return (*sem == 0);
}

static inline void uae_sem_destroy(uae_sem_t *sem)
{
	if (*sem) {
		fs_semaphore_destroy(*sem);
		*sem = NULL;
	}
}

static inline int uae_sem_post(uae_sem_t *sem)
{
	return fs_semaphore_post(*sem);
}

static inline int uae_sem_wait(uae_sem_t *sem)
{
	return fs_semaphore_wait(*sem);
}

static inline int uae_sem_trywait(uae_sem_t *sem)
{
	return fs_semaphore_try_wait(*sem);
}

static inline int uae_sem_trywait_delay(uae_sem_t *sem, int delay)
{
#warning delay for uae_sem_trywait_delay not implemented
	return fs_semaphore_try_wait(*sem);
}

#endif // THREADDEP_SEM_H
