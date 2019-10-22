#ifndef FSEMU_TIME_H_
#define FSEMU_TIME_H_

#include "fsemu-common.h"
#include "fsemu-config.h"

#include <stdint.h>
#include <time.h>

#ifdef FSEMU_GLIB
#include "fsemu-glib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// int64_t fsemu_time_nanos();

static inline int64_t fsemu_time_us(void)
{
#ifdef FSEMU_GLIB
    return g_get_monotonic_time();
#else
#error Missing implementation of fsemu_time_us
#endif
}

static inline int64_t fsemu_time_millis(void)
{
    return fsemu_time_us() / 1000;
}

static inline void fsemu_sleep_us(int us)
{
#ifdef FSEMU_GLIB
    g_usleep(us);
#else
#error Missing implementation of fsemu_sleep_millis
#endif
}

static inline void fsemu_sleep_millis(int millis)
{
#ifdef FSEMU_GLIB
    g_usleep(millis * 1000);
#else
#error Missing implementation of fsemu_sleep_millis
#endif
}

#ifdef __cplusplus
}
#endif

#ifdef __MACH__

#include <mach/mach_time.h>
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif

static inline int fsemu_clock_gettime(int clk_id, struct timespec *t)
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

#define clock_gettime fsemu_clock_gettime

#endif  // __MACH__

#endif  // FSEMU_TIME_H_
