#ifndef FSEMU_MUTEX_H_
#define FSEMU_MUTEX_H_

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_mutex;
typedef struct fsemu_mutex fsemu_mutex_t;

fsemu_mutex_t *fsemu_mutex_create(void);
// void fsemu_mutex_destroy(fsemu_mutex_t *mutex);
int fsemu_mutex_lock(fsemu_mutex_t *mutex);
int fsemu_mutex_unlock(fsemu_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MUTEX_H_
