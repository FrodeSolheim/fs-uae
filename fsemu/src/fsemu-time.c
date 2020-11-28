#include "fsemu-internal.h"
#include "fsemu-time.h"

#include "fsemu-util.h"

#include <stdio.h>

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

#include "fsemu-mutex.h"

static struct {
    bool initialized;
    fsemu_mutex_t *mutex;
} fsemu_time;

int64_t fsemu_time_sleep_until_us(int64_t until_us)
{
    return fsemu_time_sleep_until_us_2(until_us, fsemu_time_us());
}

int64_t fsemu_time_sleep_until_us_2(int64_t until_us, int64_t now_us)
{
    if (until_us > now_us) {
        fsemu_sleep_us(until_us - now_us);
    }
    return fsemu_time_us();
}

int64_t fsemu_time_wait_until_us(int64_t until_us)
{
    return fsemu_time_wait_until_us_2(until_us, fsemu_time_us());
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

struct tm *fsemu_time_localtime_r(const time_t *timep, struct tm *result)
{
#ifdef HAVE_LOCALTIME_R
    return localtime_r(timep, result);
#else
    // On Windows, localtime is thread-safe due to using thread-local
    // storage, using mutexes for other platforms (and hopefully no-one
    // else calls localtime simultaneously...).
    if (fsemu_time.mutex) {
        fsemu_mutex_lock(fsemu_time.mutex);
    }
    struct tm *tm = localtime(timep);
    if (tm == NULL) {
        fsemu_log("WARNING: localtime - invalid time_t (%ld)\n",
                  (long) *timep);
    } else {
        *result = *tm;
    }
    if (fsemu_time.mutex) {
        fsemu_mutex_unlock(fsemu_time.mutex);
    }
    if (tm == NULL) {
        return NULL;
    }
    return result;
#endif
}

struct tm *fsemu_time_gmtime_r(const time_t *timep, struct tm *result)
{
#ifdef HAVE_GMTIME_R
    return gmtime_r(timep, result);
#else
    /* on Windows, gmtime is thread-safe due to using thread-local
     * storage, using mutexes for other platforms (and hopefully no-one
     * else calls gmtime simultaneously...) */
    if (fsemu_time.mutex) {
        fsemu_mutex_lock(fsemu_time.mutex);
    }
    struct tm *tm = gmtime(timep);
    if (tm == NULL) {
        fsemu_log("WARNING: gmtime - invalid time_t (%ld)\n", (long) *timep);
    } else {
        *result = *tm;
    }
    if (fsemu_time.mutex) {
        fsemu_mutex_unlock(fsemu_time.mutex);
    }
    if (tm == NULL) {
        return NULL;
    }
    return result;
#endif
}

time_t fsemu_time_timegm(struct tm *tm)
{
    if (fsemu_time.mutex) {
        fsemu_mutex_lock(fsemu_time.mutex);
    }
    time_t ret;
    // Code adapted from the man page of timegm.
    char *tz;
#ifdef FSEMU_WINDOWS
    tz = getenv("TZ");
    if (tz) {
        tz = g_strdup_printf("TZ=%s", tz);
    } else {
        tz = g_strdup("TZ=");
    }
    _putenv("TZ=GMT");
    _tzset();
    ret = mktime(tm);

    _putenv(tz);
    g_free(tz);
    _tzset();
#else
    tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm);
    if (tz) {
        setenv("TZ", tz, 1);
    } else {
        unsetenv("TZ");
    }
    tzset();
#endif
    if (fsemu_time.mutex) {
        fsemu_mutex_unlock(fsemu_time.mutex);
    }
    return ret;
}

int fsemu_time_local_offset(time_t time)
{
    time_t t = time;
    struct tm lt;
    void *result1 = fsemu_time_localtime_r(&t, &lt);
    struct tm gt;
    void *result2 = fsemu_time_gmtime_r(&t, &gt);
    gt.tm_isdst = lt.tm_isdst;
    if (result1 == NULL || result2 == NULL) {
        return 0;
    }
    return t - mktime(&gt);
}

int64_t fsemu_time_real(void)
{
#ifdef FSEMU_GLIB
    return g_get_real_time();
#else
#error not implemented
#endif
}

void fsemu_time_current(fsemu_time_val_t *result)
{
    if (result == NULL) {
        return;
    }
    int64_t real = fsemu_time_real();
    result->tv_sec = real / 1000000;
    result->tv_usec = real % 1000000;
}

void fsemu_time_init(void)
{
    if (fsemu_time.initialized) {
        return;
    }
    fsemu_time.initialized = true;

    fsemu_time.mutex = fsemu_mutex_create();
#ifdef WINDOWS
    _tzset();
#else
    tzset();
#endif

    time_t t = time(NULL);
    fsemu_log("[TIME] current time() is: %d\n", (int) t);
    struct tm tm;
    fsemu_time_localtime_r(&t, &tm);
    fsemu_log("[TIME] localtime + timegm:  %ld\n",
              (long) fsemu_time_timegm(&tm));
    fsemu_log("[TIME] localtime + mktime:  %ld\n", (long) mktime(&tm));
    fsemu_time_val_t tv;
    fsemu_time_current(&tv);
    fsemu_log("[TIME] time of day:       %d + (%d / 1000000)\n",
              tv.tv_sec,
              tv.tv_usec);
    fsemu_log("[TIME] localtime offset:  %d\n", fsemu_time_local_offset(t));
}
