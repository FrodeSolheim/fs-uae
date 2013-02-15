
#include <fs/base.h>
#include <fs/init.h>
#include <fs/log.h>
#include <fs/string.h>
#include <fs/thread.h>
#include <fs/time.h>

#include <stdlib.h>
#include <time.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WINDOWS

#else
// FIXME: move to autoconf
#define HAVE_LOCALTIME_R
#define HAVE_GMTIME_R
#endif

static fs_mutex *g_mutex;

struct tm *fs_localtime_r(const time_t *timep, struct tm *result) {
#ifdef HAVE_LOCALTIME_R
    return localtime_r(timep, result);
#else
    // on Windows, localtime is thread-safe due to using
    // thread-local storage, using mutexes for other
    // platforms...
    if (g_mutex) {
        fs_mutex_lock(g_mutex);
    }
    struct tm* tm = localtime(timep);
    *result = *tm;
    if (g_mutex) {
        fs_mutex_unlock(g_mutex);
    }
    return result;
#endif
}

struct tm *fs_gmtime_r(const time_t *timep, struct tm *result) {
#ifdef HAVE_GMTIME_R
    return gmtime_r(timep, result);
#else
    // on Windows, gmtime is thread-safe due to using
    // thread-local storage, using mutexes for other
    // platforms...
    if (g_mutex) {
        fs_mutex_lock(g_mutex);
    }
    struct tm* tm = gmtime(timep);
    *result = *tm;
    if (g_mutex) {
        fs_mutex_unlock(g_mutex);
    }
    return result;
#endif
}

#ifdef WINDOWS
int _putenv(const char *envstring);
void _tzset(void);
#endif

time_t fs_timegm(struct tm *tm) {
    if (g_mutex) {
        fs_mutex_lock(g_mutex);
    }
    time_t ret;
    // code adapted from the man page of timegm
    char *tz;
#ifdef WINDOWS
    tz = getenv("TZ");
    if (tz) {
        tz = fs_strdup_printf("TZ=%s", tz);
    }
    else {
        tz = fs_strdup("TZ=");
    }
    _putenv("TZ=GMT");
    _tzset();
    ret = mktime(tm);

    _putenv(tz);
    free(tz);
    _tzset();
#else
    tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm);
    if (tz) {
        setenv("TZ", tz, 1);
    }
    else {
        unsetenv("TZ");
    }
    tzset();
#endif
    if (g_mutex) {
        fs_mutex_unlock(g_mutex);
    }
    return ret;
}

int fs_get_local_time_offset(time_t time) {
    time_t t = time;
    struct tm lt;
    fs_localtime_r(&t, &lt);
    struct tm gt;
    fs_gmtime_r(&t, &gt);
    return mktime(&lt) - mktime(&gt);
}

static int g_initialized = 0;

void fs_time_init(void) {
    if (g_initialized) {
        return;
    }
    g_initialized = 1;
    g_mutex = fs_mutex_create();
#ifdef WINDOWS
    _tzset();
#else
    tzset();
#endif

    time_t t = time(NULL);
    fs_log("current time() is: %d\n", (int) t);
    struct tm tm;
    fs_localtime_r(&t, &tm);
    fs_log("localtime+timegm:  %d\n", fs_timegm(&tm));
    fs_log("localtime+mktime:  %d\n", mktime(&tm));
    fs_time_val tv;
    fs_get_current_time(&tv);
    fs_log("time of day:       %d + (%d / 1000000)\n", tv.tv_sec, tv.tv_usec);
}
