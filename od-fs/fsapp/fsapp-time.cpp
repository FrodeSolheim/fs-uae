/*
 * Time: sleeping, waiting until a time, and the wall clock (see fsapp-time.h).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-time.h"

#include <string>

#include "fsapp-mutex.h"
#include "fsapp-os.h"
#include "fsapp-util.h"

int fsapp_time_log_level = SDL_LOG_PRIORITY_INFO;

static struct {
    bool initialized;
    fsapp_mutex_t *mutex;
} fsapp_time;

int64_t fsapp_time_sleep_until_ns(int64_t until_ns)
{
    return fsapp_time_sleep_until_with_current_time_ns(until_ns, (int64_t)SDL_GetTicksNS());
}

int64_t fsapp_time_sleep_until_with_current_time_ns(int64_t until_ns, int64_t now_ns)
{
    if (now_ns >= until_ns) {
        return now_ns;
    }
#if 0
    // Warning, SDL_DelayNS may sleep with Sleep (1 ms precision) on Windows, and
    // nanosleep elsewhere.
    // FIXME: Try clock_nanosleep instead?

    now_ns = (int64_t)SDL_GetTicksNS();
#if 1
    while (now_ns < until_ns - (int64_t)SDL_MS_TO_NS(2)) {
        fsapp_time_sleep_ns((int64_t)SDL_MS_TO_NS(1));

        int64_t t = (int64_t)SDL_GetTicksNS();
        int64_t overslept_ms = SDL_NS_TO_MS(t - (now_ns + (int64_t)SDL_MS_TO_NS(1)));
        if (overslept_ms >= 1) {
            fsapp_time_log_warning("Overslept 1 ms sleep in loop by %lld ms\n", (long long)overslept_ms);
        }
        now_ns = t;
    }
#endif
    while (now_ns < until_ns) {
        fsapp_time_mm_pause();
        now_ns = (int64_t)SDL_GetTicksNS();
    }

#else
    fsapp_time_sleep_ns(until_ns - now_ns);
#endif
    int64_t t = (int64_t)SDL_GetTicksNS();
    if (t > until_ns + (int64_t)SDL_MS_TO_NS(1)) {
        fsapp_time_log_warning("Overslept by %lld ms\n", (long long)SDL_NS_TO_MS(t - until_ns));
    }
    return t;
}

int64_t fsapp_time_wait_until_ns(int64_t until_ns)
{
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    if (now_ns >= until_ns) {
        // Already over sleep target...
        return now_ns;
    }
    // Minimum wait time before using sleep instead of just busy waiting.
    const int64_t busywait_threshold_ns = SDL_MS_TO_NS(1);
    // Busywait period once threshold is reached (can be less than threshold).
    const int64_t busywait_period_ns = SDL_MS_TO_NS(1);
    if (until_ns - now_ns > busywait_threshold_ns) {
        fsapp_time_sleep_ns(until_ns - now_ns - busywait_period_ns);
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    while (now_ns < until_ns) {
        // FIXME: busy looping right now
        fsapp_time_mm_pause();
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    if (now_ns - until_ns > (int64_t)SDL_MS_TO_NS(1)) {
        fsapp_time_log_warning("Overslept with %lld us\n",
                               (long long)SDL_NS_TO_US(now_ns - until_ns));
    }
    return now_ns;
}

struct tm *fsapp_time_localtime_r(const time_t *timep, struct tm *result)
{
#ifdef HAVE_LOCALTIME_R
    return localtime_r(timep, result);
#else
    // On Windows, localtime is thread-safe due to using thread-local
    // storage, using mutexes for other platforms (and hopefully no-one
    // else calls localtime simultaneously...).
    if (fsapp_time.mutex) {
        fsapp_mutex_lock(fsapp_time.mutex);
    }
    struct tm *tm = localtime(timep);
    if (tm == NULL) {
        fsapp_time_log_warning("localtime - invalid time_t (%ld)\n", (long)*timep);
    } else {
        *result = *tm;
    }
    if (fsapp_time.mutex) {
        fsapp_mutex_unlock(fsapp_time.mutex);
    }
    if (tm == NULL) {
        return NULL;
    }
    return result;
#endif
}

struct tm *fsapp_time_gmtime_r(const time_t *timep, struct tm *result)
{
#ifdef HAVE_GMTIME_R
    return gmtime_r(timep, result);
#else
    /* on Windows, gmtime is thread-safe due to using thread-local
     * storage, using mutexes for other platforms (and hopefully no-one
     * else calls gmtime simultaneously...) */
    if (fsapp_time.mutex) {
        fsapp_mutex_lock(fsapp_time.mutex);
    }
    struct tm *tm = gmtime(timep);
    if (tm == NULL) {
        fsapp_time_log_warning("gmtime - invalid time_t (%ld)\n", (long)*timep);
    } else {
        *result = *tm;
    }
    if (fsapp_time.mutex) {
        fsapp_mutex_unlock(fsapp_time.mutex);
    }
    if (tm == NULL) {
        return NULL;
    }
    return result;
#endif
}

time_t fsapp_time_timegm(struct tm *tm)
{
    if (fsapp_time.mutex) {
        fsapp_mutex_lock(fsapp_time.mutex);
    }
    time_t ret;
    // Code adapted from the man page of timegm.
    char *tz;
#ifdef FSAPP_OS_WINDOWS
    tz = getenv("TZ");
    // Copied before _putenv changes the environment that tz points into.
    std::string old_tz = std::string("TZ=") + (tz ? tz : "");
    _putenv("TZ=GMT");
    _tzset();
    ret = mktime(tm);

    _putenv(old_tz.c_str());
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
    if (fsapp_time.mutex) {
        fsapp_mutex_unlock(fsapp_time.mutex);
    }
    return ret;
}

int fsapp_time_get_local_offset(time_t time)
{
    time_t t = time;
    struct tm lt;
    void *result1 = fsapp_time_localtime_r(&t, &lt);
    struct tm gt;
    void *result2 = fsapp_time_gmtime_r(&t, &gt);
    gt.tm_isdst = lt.tm_isdst;
    if (result1 == NULL || result2 == NULL) {
        return 0;
    }
    return t - mktime(&gt);
}

void fsapp_time_get_current(fsapp_time_val_t *result)
{
    if (result == NULL) {
        return;
    }
    // The wall clock, in nanoseconds since the Unix epoch.
    // REVIEW: SDL_GetCurrentTime instead of GLib's g_get_real_time, so that fsapp's time does not
    // use GLib; the sdl-clock spec leaves the wall clock out of scope otherwise.
    SDL_Time real_ns = 0;
    if (!SDL_GetCurrentTime(&real_ns)) {
        fsapp_time_log_warning("SDL_GetCurrentTime failed: %s\n", SDL_GetError());
    }
    result->tv_sec = (int32_t)(real_ns / SDL_NS_PER_SECOND);
    result->tv_usec = (int32_t)SDL_NS_TO_US(real_ns % SDL_NS_PER_SECOND);
}

void fsapp_time_init(void)
{
    if (fsapp_time.initialized) {
        return;
    }
    fsapp_time.initialized = true;

    fsapp_time.mutex = fsapp_mutex_create();
#ifdef WINDOWS
    _tzset();
#else
    tzset();
#endif

    time_t t = time(NULL);
    fsapp_time_log("current time() is: %d\n", (int)t);
    struct tm tm;
    fsapp_time_localtime_r(&t, &tm);
    fsapp_time_log("localtime + timegm:  %ld\n", (long)fsapp_time_timegm(&tm));
    fsapp_time_log("localtime + mktime:  %ld\n", (long)mktime(&tm));
    fsapp_time_val_t tv;
    fsapp_time_get_current(&tv);
    fsapp_time_log("time of day:       %d + (%d / 1000000)\n", tv.tv_sec, tv.tv_usec);
    fsapp_time_log("localtime offset:  %d\n", fsapp_time_get_local_offset(t));
}
