#ifndef FSEMU_THREAD_H_
#define FSEMU_THREAD_H_

#include <stdbool.h>
#include <stdint.h>

#include "fsemu-config.h"
#include "fsemu-util.h"

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

fsemu_thread_id_t fsemu_thread_id(void);
fsemu_thread_t *fsemu_thread_create(const char *name,
                                    fsemu_thread_function_t fn,
                                    void *data);

void fsemu_thread_set_priority(void);

void fsemu_thread_set_emu(void);

// Main thread is registered automatically when fsemu_thread_init is run, so
// usually no need to call this explicitly.
void fsemu_thread_set_main(void);

void fsemu_thread_set_video(void);

extern fsemu_thread_id_t fsemu_thread_emu_thread_id;
extern fsemu_thread_id_t fsemu_thread_main_thread_id;
extern fsemu_thread_id_t fsemu_thread_video_thread_id;

static inline bool fsemu_thread_is_emu(void)
{
    fsemu_assert(fsemu_thread_emu_thread_id);
    return fsemu_thread_id() == fsemu_thread_emu_thread_id;
}

static inline bool fsemu_thread_is_main(void)
{
    fsemu_assert(fsemu_thread_main_thread_id);
    return fsemu_thread_id() == fsemu_thread_main_thread_id;
}

static inline bool fsemu_thread_is_video(void)
{
    fsemu_assert(fsemu_thread_video_thread_id);
    return fsemu_thread_id() == fsemu_thread_video_thread_id;
}

#ifdef FSEMU_DEBUG
// Using defines to get source file/line location correct in error messages.
#define fsemu_thread_assert_emu() fsemu_assert(fsemu_thread_is_emu());
#define fsemu_thread_assert_main() fsemu_assert(fsemu_thread_is_main());
#define fsemu_thread_assert_video() fsemu_assert(fsemu_thread_is_video());
#else
#define fsemu_thread_assert_emu()
#define fsemu_thread_assert_main()
#define fsemu_thread_assert_video()
#endif

/*
static inline void fsemu_thread_assert_emu(void)
{
#ifdef FSEMU_DEBUG
    fsemu_assert(fsemu_thread_is_emu());
#endif
}

static inline void fsemu_thread_assert_main(void)
{
#ifdef FSEMU_DEBUG
    fsemu_assert(fsemu_thread_is_main());
#endif
}

static inline void fsemu_thread_assert_video(void)
{
#ifdef FSEMU_DEBUG
    fsemu_assert(fsemu_thread_is_video());
#endif
}
*/

// fsemu_mutex_t *fsemu_mutex_create(void);
// // void fsemu_mutex_destroy(fsemu_mutex_t *mutex);
// int fsemu_mutex_lock(fsemu_mutex_t *mutex);
// int fsemu_mutex_unlock(fsemu_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_THREAD_H_
