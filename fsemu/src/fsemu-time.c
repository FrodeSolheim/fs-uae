#define FSEMU_INTERNAL
#include "fsemu-time.h"

// A bit uncertain about the effects of doing _mm_pause here, but
// it isn't likely to do any harm, and could have positive effects
// (lower power usage? yield some more to otherhyper threading
// core?). We haven't got any better thing to do...
#define USE_MM_PAUSE

#ifdef USE_MM_PAUSE
#include <emmintrin.h>
#endif

int64_t fsemu_time_wait_until_us(int64_t until_us)
{
    int64_t now = fsemu_time_us();
    return fsemu_time_wait_until_us_2(until_us, now);
}

int64_t fsemu_time_wait_until_us_2(int64_t until_us, int64_t now)
{
    if (now >= until_us) {
        // printf("WARNNING: Already over sleep target with %d us\n", (int)
        // (now - until_us));
        return now;
    }
#if 1

#if 0
    if (until_us - now > 1000) {
        // int64_t sleep_start = now;
        // fsemu_sleep_us(until_us - now);
        fsemu_sleep_us(until_us - now - 1000);
        now = fsemu_time_us();
    }
#endif

    while (now < until_us) {
        // FIXME: busy looping right now
#ifdef USE_MM_PAUSE
        _mm_pause();
#endif
        now = fsemu_time_us();
    }
#else
#if 1
    // FIXME: use busywait option?
    if (now < until) {
        int64_t sleep_start = now;
        while (now < until) {
            // A bit uncertain about the effects of doing _mm_pause here, but
            // it isn't likely to do any harm, and could have positive effects
            // (lower power usage? yield some more to otherhyper threading
            // core?). We haven't got any better thing to do...
            _mm_pause();
            now = fsemu_time_us();
        }
    }
#else
    if (until - now > 1000) {
        int64_t sleep_start = now;
        fsemu_sleep_us(until - now);
        now = fsemu_time_us();
        fsemu_frame_sleep_duration += now - sleep_start;
    }
#endif
#endif
    if (now - until_us > 1000) {
        printf("WARNNING: Overslept with %d us\n", (int) (now - until_us));
    }
    return now;
}
