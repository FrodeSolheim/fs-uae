#ifndef FSEMU_SEMAPHORE_H_
#define FSEMU_SEMAPHORE_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FSEMU_PTHREADS
#define FSEMU_PSEM 1
#endif

const int FSEMU_SEMAPHORE_TIMEOUT = 1;

struct fsemu_semaphore_t;
typedef struct fsemu_semaphore_t fsemu_semaphore_t;

void fsemu_semaphore_init(fsemu_semaphore_t* semaphore, int value);

fsemu_semaphore_t* fsemu_semaphore_create(int value);

void fsemu_semaphore_destroy(fsemu_semaphore_t* semaphore);

int fsemu_semaphore_post(fsemu_semaphore_t* semaphore);

int fsemu_semaphore_wait(fsemu_semaphore_t* semaphore);

int fsemu_semaphore_try_wait(fsemu_semaphore_t* semaphore);

int fsemu_semaphore_wait_timeout_ms(fsemu_semaphore_t* semaphore, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SEMAPHORE_H_
