#ifdef WINDOWS
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlobj.h>
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif
#ifdef MACOSX
#include <mach-o/dyld.h>
#include <copyfile.h>
#endif
#include <stdio.h>
#include <fs/log.h>
#include <fs/base.h>
#include <fs/filesys.h>

#if defined(WINDOWS)

#elif defined(MACOSX)

//#define HAVE_CLOCK_GETTIME
//#define CLOCK_MONOTONIC
#include <sys/time.h>

#else

#define HAVE_CLOCK_GETTIME
#define HAVE_CLOCK_MONOTONIC
#include <sys/time.h>

#endif

// FIXME g_atomic_int_get / g_atomic_int_set
#include <glib.h>

// some function adapted from glib

char *fs_get_data_file(const char *relative) {
    //printf("fs_get_data_file_path %s\n", relative);
    char *path;
    path = fs_path_join("share", relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    free(path);

#ifdef MACOSX
    char buffer[FS_PATH_MAX];
    fs_get_application_exe_dir(buffer, FS_PATH_MAX);
    gchar *test;
    path = g_build_filename(buffer, "..", "Resources", relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    free(path);
#endif

    const char *user_dir = g_get_user_data_dir();
    path = fs_path_join(user_dir, relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    free(path);

    const char * const *dirs = g_get_system_data_dirs();
    while(*dirs) {
        path = fs_path_join(*dirs, relative, NULL);
        if (fs_path_exists(path)) {
            return path;
        }
        free(path);
        dirs++;
    }
    return NULL;
}

char *fs_get_program_data_file(const char *relative) {
    char *relative2 = fs_path_join(g_get_prgname(), relative, NULL);
    char *result = fs_get_data_file(relative2);
    free(relative2);
    return result;
}

void fs_get_current_time(fs_time_val *result) {
#ifndef WINDOWS
    struct timeval r;

    if (result == NULL) {
        return;
    }

    /*this is required on alpha, there the timeval structs are int's
     not longs and a cast only would fail horribly*/
    gettimeofday(&r, NULL);
    result->tv_sec = r.tv_sec;
    result->tv_usec = r.tv_usec;
#else
    FILETIME ft;
    uint64_t time64;

    if (result == NULL) {
        return;
    }

    GetSystemTimeAsFileTime(&ft);
    memmove(&time64, &ft, sizeof(FILETIME));

    /* Convert from 100s of nanoseconds since 1601-01-01
     * to Unix epoch. Yes, this is Y2038 unsafe.
     */
    //time64 -= G_GINT64_CONSTANT (116444736000000000);
    time64 -= ((int64_t) 116444736) * ((int64_t) 1000000000);
    time64 /= 10;

    result->tv_sec = time64 / 1000000;
    result->tv_usec = time64 % 1000000;
#endif
}

int64_t fs_get_real_time(void) {
    fs_time_val tv;

    fs_get_current_time(&tv);

    return (((int64_t) tv.tv_sec) * 1000000) + tv.tv_usec;
}

#ifdef WINDOWS
static ULONGLONG (*g_GetTickCount64)(void) = NULL;
static uint32_t g_win32_tick_epoch = 0;

//G_GNUC_INTERNAL
void fs_clock_win32_init(void) {
    HMODULE kernel32;

    g_GetTickCount64 = NULL;
    kernel32 = GetModuleHandle("KERNEL32.DLL");
    if (kernel32 != NULL) g_GetTickCount64 = (void *) GetProcAddress(kernel32,
            "GetTickCount64");
    g_win32_tick_epoch = ((uint32_t) GetTickCount()) >> 31;
}
#endif

/**
 * g_get_monotonic_time:
 *
 * Queries the system monotonic time, if available.
 *
 * On POSIX systems with clock_gettime() and <literal>CLOCK_MONOTONIC</literal> this call
 * is a very shallow wrapper for that.  Otherwise, we make a best effort
 * that probably involves returning the wall clock time (with at least
 * microsecond accuracy, subject to the limitations of the OS kernel).
 *
 * It's important to note that POSIX <literal>CLOCK_MONOTONIC</literal> does
 * not count time spent while the machine is suspended.
 *
 * On Windows, "limitations of the OS kernel" is a rather substantial
 * statement.  Depending on the configuration of the system, the wall
 * clock time is updated as infrequently as 64 times a second (which
 * is approximately every 16ms). Also, on XP (but not on Vista or later)
 * the monotonic clock is locally monotonic, but may differ in exact
 * value between processes due to timer wrap handling.
 *
 * Returns: the monotonic time, in microseconds
 *
 * Since: 2.28
 **/
int64_t fs_get_monotonic_time(void) {
    //return g_get_monotonic_time();

#ifdef HAVE_CLOCK_GETTIME
    /* librt clock_gettime() is our first choice */
    struct timespec ts;

#ifdef HAVE_CLOCK_MONOTONIC
    clock_gettime (CLOCK_MONOTONIC, &ts);
#else
    clock_gettime (CLOCK_REALTIME, &ts);
#endif

    /* In theory monotonic time can have any epoch.
     *
     * glib presently assumes the following:
     *
     *   1) The epoch comes some time after the birth of Jesus of Nazareth, but
     *      not more than 10000 years later.
     *
     *   2) The current time also falls sometime within this range.
     *
     * These two reasonable assumptions leave us with a maximum deviation from
     * the epoch of 10000 years, or 315569520000000000 seconds.
     *
     * If we restrict ourselves to this range then the number of microseconds
     * will always fit well inside the constraints of a int64 (by a factor of
     * about 29).
     *
     * If you actually hit the following assertion, probably you should file a
     * bug against your operating system for being excessively silly.
     **/
#if 0
    g_assert (G_GINT64_CONSTANT (-315569520000000000) < ts.tv_sec &&
            ts.tv_sec < G_GINT64_CONSTANT (315569520000000000));
#endif
    return (((int64_t) ts.tv_sec) * 1000000) + (ts.tv_nsec / 1000);

#elif defined (WINDOWS)
    uint64_t ticks;
    uint32_t ticks32;

    /* There are four sources for the monotonic time on Windows:
     *
     * Three are based on a (1 msec accuracy, but only read periodically) clock chip:
     * - GetTickCount (GTC)
     *    32bit msec counter, updated each ~15msec, wraps in ~50 days
     * - GetTickCount64 (GTC64)
     *    Same as GetTickCount, but extended to 64bit, so no wrap
     *    Only availible in Vista or later
     * - timeGetTime (TGT)
     *    similar to GetTickCount by default: 15msec, 50 day wrap.
     *    available in winmm.dll (thus known as the multimedia timers)
     *    However apps can raise the system timer clock frequency using timeBeginPeriod()
     *    increasing the accuracy up to 1 msec, at a cost in general system performance
     *    and battery use.
     *
     * One is based on high precision clocks:
     * - QueryPrecisionCounter (QPC)
     *    This has much higher accuracy, but is not guaranteed monotonic, and
     *    has lots of complications like clock jumps and different times on different
     *    CPUs. It also has lower long term accuracy (i.e. it will drift compared to
     *    the low precision clocks.
     *
     * Additionally, the precision available in the timer-based wakeup such as
     * MsgWaitForMultipleObjectsEx (which is what the mainloop is based on) is based
     * on the TGT resolution, so by default it is ~15msec, but can be increased by apps.
     *
     * The QPC timer has too many issues to be used as is. The only way it could be used
     * is to use it to interpolate the lower precision clocks. Firefox does something like
     * this:
     *   https://bugzilla.mozilla.org/show_bug.cgi?id=363258
     *
     * However this seems quite complicated, so we're not doing this right now.
     *
     * The approach we take instead is to use the TGT timer, extending it to 64bit
     * either by using the GTC64 value, or if that is not availible, a process local
     * time epoch that we increment when we detect a timer wrap (assumes that we read
     * the time at least once every 50 days).
     *
     * This means that:
     *  - We have a globally consistent monotonic clock on Vista and later
     *  - We have a locally monotonic clock on XP
     *  - Apps that need higher precision in timeouts and clock reads can call
     *    timeBeginPeriod() to increase it as much as they want
     */

    if (g_GetTickCount64 != NULL) {
        uint32_t ticks_as_32bit;

        ticks = g_GetTickCount64();
        ticks32 = timeGetTime();

        /* GTC64 and TGT are sampled at different times, however they
         * have the same base and source (msecs since system boot).
         * They can differ by as much as -16 to +16 msecs.
         * We can't just inject the low bits into the 64bit counter
         * as one of the counters can have wrapped in 32bit space and
         * the other not. Instead we calculate the signed difference
         * in 32bit space and apply that difference to the 64bit counter.
         */
        ticks_as_32bit = (uint32_t) ticks;

        /* We could do some 2's complement hack, but we play it safe */
        if (ticks32 - ticks_as_32bit <= INT32_MAX) ticks += ticks32
                - ticks_as_32bit;
        else ticks -= ticks_as_32bit - ticks32;
    }
    else {
        uint32_t epoch;

        epoch = g_atomic_int_get(&g_win32_tick_epoch);

        /* Must read ticks after the epoch. Then we're guaranteed
         * that the ticks value we read is higher or equal to any
         * previous ones that lead to the writing of the epoch.
         */
        ticks32 = timeGetTime();

        /* We store the MSB of the current time as the LSB
         * of the epoch. Comparing these bits lets us detect when
         * the 32bit counter has wrapped so we can increase the
         * epoch.
         *
         * This will work as long as this function is called at
         * least once every ~24 days, which is half the wrap time
         * of a 32bit msec counter. I think this is pretty likely.
         *
         * Note that g_win32_tick_epoch is a process local state,
         * so the monotonic clock will not be the same between
         * processes.
         */
        if ((ticks32 >> 31) != (epoch & 1)) {
            epoch++;
            g_atomic_int_set(&g_win32_tick_epoch, epoch);
        }

        ticks = (uint64_t) ticks32 | ((uint64_t) epoch) << 31;
    }

    return ticks * 1000;

#else /* !HAVE_CLOCK_GETTIME && ! G_OS_WIN32*/

    fs_time_val tv;

    fs_get_current_time(&tv);

    return (((int64_t) tv.tv_sec) * 1000000) + tv.tv_usec;
#endif
}

int fs_get_application_exe_path(char *buffer, int size) {
#if defined(WINDOWS)
    // FIXME: SHOULD HANDLE UTF8 <--> MBCS..
    size = GetModuleFileName(NULL, buffer, size);
    //return utf8(path_buffer);
    return 1;
#elif defined(MACOSX)
    //fs_log("fs_get_application_exe_path for Mac OS X\n");
    unsigned int usize = size;
    int result = _NSGetExecutablePath(buffer, &usize);
    if (result == 0) {
        return 1;
    }
    else {
        fs_log("_NSGetExecutablePath failed with result %d\n", result);
        buffer[0] = '\0';
        return 0;
    }
#elif defined(LINUX)
    fs_log("WARNING: fs_get_application_exe_path not implemented\n");
#else
    // Mac OS X: _NSGetExecutablePath() (man 3 dyld)
    // Linux: readlink /proc/self/exe
    // Solaris: getexecname()
    // FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
    // BSD with procfs: readlink /proc/curproc/file
    // Windows: GetModuleFileName() with hModule = NULL
    fs_log("WARNING: fs_get_application_exe_path not implemented\n");
#endif
    fs_log("WARNING: fs_get_application_exe_path failed\n");
    buffer[0] = '\0';
    return 0;
}

int fs_get_application_exe_dir(char *buffer, int size) {
    int result = fs_get_application_exe_path(buffer, size);
    if (result == 0) {
        return 0;
    }
    int pos = strlen(buffer) - 1;
    while (pos >= 0) {
        if (buffer[pos] == '\\' || buffer[pos] == '/') {
            buffer[pos] = '\0';
            return 1;
        }
        pos -= 1;
    }
    return 0;
}

void *fs_malloc0(size_t n_bytes) {
    void *data = malloc(n_bytes);
    if (data) {
        memset(data, 0, n_bytes);
    }
    return data;
}
