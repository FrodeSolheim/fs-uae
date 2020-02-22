#ifndef FSEMU_THREAD_H_
#define FSEMU_THREAD_H_

#include <stdbool.h>
#include <stdint.h>

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_thread;
typedef struct fsemu_thread fsemu_thread_t;
typedef uintptr_t fsemu_thread_id_t;
typedef void *(*fsemu_thread_function_t)(void *);

// struct fsemu_mutex;
// typedef struct fsemu_mutex fsemu_mutex_t;

void fsemu_thread_init(void);
bool fsemu_thread_is_main(void);
void fsemu_thread_assert_main(void);

fsemu_thread_id_t fsemu_thread_id(void);
fsemu_thread_t *fsemu_thread_create(const char *name,
                                    fsemu_thread_function_t fn,
                                    void *data);

void fsemu_thread_set_priority(void);

// fsemu_mutex_t *fsemu_mutex_create(void);
// // void fsemu_mutex_destroy(fsemu_mutex_t *mutex);
// int fsemu_mutex_lock(fsemu_mutex_t *mutex);
// int fsemu_mutex_unlock(fsemu_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_THREAD_H_
