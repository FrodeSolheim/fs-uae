#define FSEMU_INTERNAL
#include "fsemu-time.h"

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

int64_t fsemu_time_wait_until_us(int64_t until_us)
{
    int64_t now_us = fsemu_time_us();
    return fsemu_time_wait_until_us_2(until_us, now_us);
}

int64_t fsemu_time_wait_until_us_2(int64_t until_us, int64_t now_us)
{
    if (now_us == 0) {
        now_us = fsemu_time_us();
    }
    if (now_us >= until_us) {
        // printf("WARNNING: Already over sleep target with %d us\n", (int)
        // (now - until_us));

        // Already over sleep target...
        return now_us;
    }
#if 1

    // Minimum wait time before using sleep instead of just busy waiting.
    int64_t busywait_threshold = 1000;
    // Busywait period once threshold is reached (can be less than threshold).
    int64_t busywait_period = 1000;
#if 1
    if (until_us - now_us > busywait_threshold) {
        // int64_t sleep_start = now;
        // fsemu_sleep_us(until_us - now);
        fsemu_sleep_us(until_us - now_us - busywait_period);
        now_us = fsemu_time_us();
    }
#endif

    while (now_us < until_us) {
        // FIXME: busy looping right now
        fsemu_time_mm_pause();
        now_us = fsemu_time_us();
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
    if (now_us - until_us > 1000) {
        printf("WARNNING: Overslept with %d us\n", (int) (now_us - until_us));
    }
    return now_us;
}
