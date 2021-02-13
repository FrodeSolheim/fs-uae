#ifndef FSEMU_TIME_H_
#define FSEMU_TIME_H_

#include <stdint.h>
#include <time.h>

#include "fsemu-common.h"
#include "fsemu-config.h"
#ifdef FSEMU_GLIB
#include "fsemu-glib.h"
#endif
#include "fsemu-sdl.h"
#include "fsemu-util.h"

#ifdef __MACH__
#include <mach/mach_time.h>
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t tv_sec;
    int32_t tv_usec;
} fsemu_time_val_t;

void fsemu_time_init(void);

static inline int64_t fsemu_time_us(void)
{
#ifdef FSEMU_GLIB
    return g_get_monotonic_time();
#else
#error Missing implementation of fsemu_time_us
#endif
}

static inline int64_t fsemu_time_ms(void)
{
    return fsemu_time_us() / 1000;
}

// FIXME: Rename fsemu_time_sleep_us
static inline void fsemu_sleep_us(int64_t sleep_us)
{
    fsemu_assert(sleep_us > 0);
    // FIXME: Consider using nanosleep (or check if g_usleep is good enough).
    // Or even better: clock_nanosleep with CLOCK_MONOTONIC
#ifdef FSEMU_GLIB
    g_usleep((gulong) sleep_us);
#else
#error Missing implementation of fsemu_sleep_us
#endif
}

static inline void fsemu_time_sleep_ms(int sleep_ms)
{
    return fsemu_sleep_us(sleep_ms * 1000);
}

FSEMU_DEPRECATED
static inline void fsemu_sleep_ms(int sleep_ms)
{
    return fsemu_sleep_us(sleep_ms * 1000);
}

// This will always sleep, and is not guaranteed to be accurate. Use the
// wait function instead if you need better accuracy.
int64_t fsemu_time_sleep_until_us(int64_t until_us);
int64_t fsemu_time_sleep_until_us_2(int64_t until_us, int64_t now_us);

int64_t fsemu_time_wait_until_us(int64_t until_us);
int64_t fsemu_time_wait_until_us_2(int64_t until_us, int64_t now_us);

struct tm *fsemu_time_localtime_r(const time_t *timep, struct tm *result);
struct tm *fsemu_time_gmtime_r(const time_t *timep, struct tm *result);
time_t fsemu_time_timegm(struct tm *tm);
int fsemu_time_local_offset(time_t time);
void fsemu_time_current(fsemu_time_val_t *result);
int64_t fsemu_time_real(void);

#ifdef __MACH__

static inline int fsemu_time_clock_gettime_mach(int clk_id, struct timespec *t)
{
    SDL_assert(clk_id == CLOCK_MONOTONIC);
    static mach_timebase_info_data_t timebase;
    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }
    uint64_t time;
    time = mach_absolute_time();
    // FIXME: Get rid of the doubles?
    double nseconds =
        ((double) time * (double) timebase.numer) / ((double) timebase.denom);
    double seconds = ((double) time * (double) timebase.numer) /
                     ((double) timebase.denom * 1e9);
    t->tv_sec = seconds;
    t->tv_nsec = nseconds;
    return 0;
}

#define fsemu_time_clock_gettime fsemu_time_clock_gettime_mach
#define clock_gettime fsemu_time_clock_gettime_mach

#endif  // __MACH__

extern int fsemu_time_log_level;

#define fsemu_time_log(format, ...)                                   \
    if (fsemu_likely(fsemu_time_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSE] [TME] " format, ##__VA_ARGS__);              \
    }

#define fsemu_time_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsemu_time_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log_debug("[FSE] [TME] " format, ##__VA_ARGS__);           \
    }

#define fsemu_time_log_warning(format, ...)                              \
    if (fsemu_likely(fsemu_time_log_level >= FSEMU_LOG_LEVEL_WARNING)) { \
        fsemu_log_debug("[FSE] [TME] " format, ##__VA_ARGS__);           \
    }

#ifdef FSEMU_INTERNAL

// A bit uncertain about the effects of doing _mm_pause here, but
// it isn't likely to do any harm, and could have positive effects
// (lower power usage? yield some more to otherhyper threading
// core?). We haven't got any better thing to do...
#ifdef FSEMU_CPU_X86
#define FSEMU_USE_MM_PAUSE
#endif

#ifdef FSEMU_USE_MM_PAUSE
#include <emmintrin.h>
#define fsemu_time_mm_pause() _mm_pause()
#else
#define fsemu_time_mm_pause()
#endif

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_TIME_H_
