/*
 * Time: sleeping, waiting until a time, and the wall clock. The time is read from SDL's clock,
 * SDL_GetTicksNS(), directly where it is needed; the times here are on it, in nanoseconds.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_TIME_H
#define FSAPP_TIME_H

#include <stdint.h>
#include <time.h>

#include <SDL3/SDL.h>

#include "fsapp-config.h"
#include "fs/log.h"
#include "fsapp-util.h"

// A bit uncertain about the effects of doing _mm_pause here, but
// it isn't likely to do any harm, and could have positive effects
// (lower power usage? yield some more to otherhyper threading
// core?). We haven't got any better thing to do...
#if defined(__x86_64__) || defined(__i386__)
#define FSAPP_TIME_USE_MM_PAUSE
#endif

#ifdef FSAPP_TIME_USE_MM_PAUSE
#include <emmintrin.h>
#define fsapp_time_mm_pause() _mm_pause()
#else
#define fsapp_time_mm_pause()
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t tv_sec;
    int32_t tv_usec;
} fsapp_time_val_t;

void fsapp_time_init(void);

static inline void fsapp_time_sleep_ns(int64_t sleep_ns)
{
    fsapp_assert(sleep_ns > 0);
    // SDL_DelayNS sleeps with nanosleep on Linux and macOS, and with a high-resolution waitable
    // timer on Windows where it has one (otherwise Sleep, in milliseconds).
    // REVIEW: On Windows, g_usleep was Sleep(sleep_us / 1000): a sleep under 1 ms was Sleep(0),
    // only a yield, where SDL_DelayNS now really sleeps, for at least the waitable timer's
    // resolution. Today only fsemu's sleeping frame wait (when busy-waiting is toggled off) and
    // the test aids' simulated display and render delay sleep here.
    SDL_DelayNS((Uint64)sleep_ns);
}

// Sleeps until until_ns (on SDL's clock), and returns the time it woke up (or now, when until_ns
// has already passed). This will always sleep, and is not guaranteed to be accurate. Use the wait
// function instead if you need better accuracy.
int64_t fsapp_time_sleep_until_ns(int64_t until_ns);
// fsapp_time_sleep_until_ns, for a caller that already has the current time (now_ns).
int64_t fsapp_time_sleep_until_with_current_time_ns(int64_t until_ns, int64_t now_ns);

// Waits until until_ns (on SDL's clock), sleeping while more than about a millisecond is left and
// busy-waiting the rest, and returns the time the wait ended (or now, when until_ns has already
// passed).
int64_t fsapp_time_wait_until_ns(int64_t until_ns);

struct tm *fsapp_time_localtime_r(const time_t *timep, struct tm *result);
struct tm *fsapp_time_gmtime_r(const time_t *timep, struct tm *result);
time_t fsapp_time_timegm(struct tm *tm);
int fsapp_time_get_local_offset(time_t time);
void fsapp_time_get_current(fsapp_time_val_t *result);

extern int fsapp_time_log_level;

#define fsapp_time_log(format, ...)                                                                \
    FS_LOG_INFO(fsapp_time_log_level, "[FSE] [TME]", format, ##__VA_ARGS__)

#define fsapp_time_log_debug(format, ...)                                                          \
    FS_LOG_DEBUG(fsapp_time_log_level, "[FSE] [TME]", format, ##__VA_ARGS__)

#define fsapp_time_log_warning(format, ...)                                                        \
    FS_LOG_WARNING(fsapp_time_log_level, "[FSE] [TME]", format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_TIME_H */
