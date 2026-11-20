/*
 * File status and times (stat, fstat, setting a file's time).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-file.h"

#define _GNU_SOURCE 1
// #include <fs/filesys.h>

#ifdef FSAPP_OS_LINUX
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#define HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC 1
#endif

#ifdef FSAPP_OS_MACOS
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#define HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC 1
#endif

#ifdef FSAPP_OS_WINDOWS
#include <windows.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_UTIME_H
#include <utime.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <glib.h>
#include <glib/gstdio.h>

#ifdef FSAPP_OS_WINDOWS
typedef struct _stati64 stat_type;
#define wstat_function _wstati64
#define fstat_function _fstati64

#else
typedef struct stat stat_type;
#define stat_function stat
#define fstat_function fstat
#endif

#ifdef FSAPP_OS_WINDOWS

static wchar_t *wide(const char *utf8)
{
    if (utf8[0] == '\0') {
        wchar_t *result = (wchar_t *)malloc(sizeof(wchar_t));
        result[0] = L'0';
        return result;
    }
    int strict = 1;
    int size = -1;
    DWORD flags = 0;
    if (strict) {
        flags = MB_ERR_INVALID_CHARS;
    } else {
        // FIXME: non-strict mode does not seem to work on Vista/7
    }

    // count number of elements to convert, do not use MB_PRECOMPOSED
    // with CP_UTF8 -- will fail with error INVALID_FLAGS

    int chars = MultiByteToWideChar(CP_UTF8, flags, utf8, size, NULL, 0);
    if (chars == 0) {
        // error convering to wide string
        wchar_t *result = (wchar_t *)malloc(sizeof(wchar_t));
        result[0] = L'0';
        return result;
    }

    // convert

    wchar_t *wstr = (wchar_t *)(malloc(sizeof(wchar_t) * (chars + 1)));
    chars = MultiByteToWideChar(CP_UTF8, flags, utf8, size, wstr, chars + 1);
    if (chars == 0) {
        free(wstr);
        wchar_t *result = (wchar_t *)malloc(sizeof(wchar_t));
        result[0] = L'0';
        return result;
    }
    return wstr;
}

static void time_val_to_file_time(struct timeval *tv, FILETIME *ft)
{
    // Note that LONGLONG is a 64-bit value
    // LONGLONG ll;
    ULARGE_INTEGER uli;

    uli.QuadPart = Int32x32To64(tv->tv_sec, 10000000) + 116444736000000000LL;
    // FILETIME contains 100-nanosecond intervals
    uli.QuadPart += ((LONGLONG)tv->tv_usec) * 10;
    // ft->dwLowDateTime = (DWORD)ll;
    // ft->dwHighDateTime = ll >> 32;
    ft->dwLowDateTime = uli.LowPart;
    ft->dwHighDateTime = uli.HighPart;
    // printf("%d %d %lld, %d, %d\n", (int) tv->tv_sec, (int) tv->tv_usec,
    // uli.QuadPart, ft->dwHighDateTime, ft->dwLowDateTime);
}

static void file_time_to_time_val(FILETIME *ft, struct timeval *tv)
{
#if 0
    static int test = 1;
    if (test == 1) {
        test = 0;
        FILETIME ft;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        time_val_to_file_time(&tv, &ft);
        file_time_to_time_val(&ft, &tv);
        printf("%d %d\n", (int) tv.tv_sec, (int) tv.tv_usec);
        exit(1);
    }
#endif

    ULARGE_INTEGER uli;
    uli.LowPart = ft->dwLowDateTime;
    uli.HighPart = ft->dwHighDateTime;

    // Convert from 100s of nanoseconds since 1601-01-01
    // to Unix epoch. Yes, this is Y2038 unsafe.

    uli.QuadPart -= ((int64_t)116444736) * ((int64_t)1000000000);
    uli.QuadPart /= 10;

    tv->tv_sec = uli.QuadPart / 1000000;
    tv->tv_usec = uli.QuadPart % 1000000;
}

#endif

// Some code adapted from glib

// This function is copied from Glib because of an bug(?) in Mingw-w32 4.6.3
// which causes _wstat to not be defined as _wstati64 (while stat is defined
// as _stati64). This causes g_stat from glib to "fail" (garbage in stat
// buffer due to mistmatch of stat struct).
static int g_stat_reimpl(const char *filename, stat_type *buf)
{
#if defined(G_OS_WIN32)
    wchar_t *wfilename = (wchar_t *)g_utf8_to_utf16(filename, -1, NULL, NULL, NULL);
    int retval;
    int save_errno;
    int len;

    if (wfilename == NULL) {
        errno = EINVAL;
        return -1;
    }

    len = wcslen(wfilename);
    while (len > 0 && G_IS_DIR_SEPARATOR(wfilename[len - 1])) {
        len--;
    }
    if (len > 0 && (!g_path_is_absolute(filename) || len > g_path_skip_root(filename) - filename)) {
        wfilename[len] = '\0';
    }

    // use explicit stat function (see define further up) to work around bug
    retval = wstat_function(wfilename, buf);
    save_errno = errno;

    g_free(wfilename);

    errno = save_errno;
    return retval;
#else
    return stat_function(filename, buf);
#endif
}

int fsapp_file_stat(const char *path, fsapp_file_stat_t *buf)
{
    stat_type st;
    // g_stat handles unicode file names on Windows
    int result = g_stat_reimpl(path, &st);

    if (result == 0) {
        buf->atime = st.st_atime;
        buf->mtime = st.st_mtime;
        buf->ctime = st.st_ctime;
        buf->size = st.st_size;
        buf->mode = st.st_mode;

#ifdef HAVE_STRUCT_STAT_ST_BLOCKS
        buf->blocks = st.st_blocks;
#endif

#if defined(HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC)
        buf->atime_nsec = st.st_atim.tv_nsec;
        buf->mtime_nsec = st.st_mtim.tv_nsec;
        buf->ctime_nsec = st.st_ctim.tv_nsec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC)
        buf->atime_nsec = st.st_atimespec.tv_nsec;
        buf->mtime_nsec = st.st_mtimespec.tv_nsec;
        buf->ctime_nsec = st.st_ctimespec.tv_nsec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIME_NSEC)
        buf->atime_nsec = st.st_atime_nsec;
        buf->mtime_nsec = st.st_mtime_nsec;
        buf->ctime_nsec = st.st_ctime_nsec;
#elif defined(FSAPP_OS_WINDOWS)
        HANDLE h;
        DWORD flags;
        DWORD attr;
        FILETIME t1, t2, t3;

        wchar_t *upath = wide(path);
        attr = GetFileAttributesW(upath);
        flags = FILE_ATTRIBUTE_NORMAL;
        if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
            // needed to obtain a handle to a directory
            flags = FILE_FLAG_BACKUP_SEMANTICS;
        }
        h = CreateFileW(upath, FILE_READ_ATTRIBUTES, 0, NULL, OPEN_EXISTING, flags, NULL);
        if (h != INVALID_HANDLE_VALUE) {
            if (GetFileTime(h, &t1, &t2, &t3)) {
                struct timeval tv;
                file_time_to_time_val(&t1, &tv);
                buf->ctime = tv.tv_sec;
                buf->ctime_nsec = tv.tv_usec * 1000;

                file_time_to_time_val(&t2, &tv);
                buf->atime = tv.tv_sec;
                buf->atime_nsec = tv.tv_usec * 1000;

                file_time_to_time_val(&t3, &tv);
                buf->mtime = tv.tv_sec;
                buf->mtime_nsec = tv.tv_usec * 1000;
            }
            CloseHandle(h);
        }
        free(upath);
#else
#error no sub-second mtime found in struct stat
#endif
    }
    return result;
}

int fsapp_file_set_time(const char *path, struct timeval *t)
{
#ifdef FSAPP_OS_WINDOWS
    HANDLE h;
    DWORD flags;
    DWORD attr;
    FILETIME ft;
    time_val_to_file_time(t, &ft);
    int result = -1;

    wchar_t *upath = wide(path);
    attr = GetFileAttributesW(upath);
    flags = FILE_ATTRIBUTE_NORMAL;
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
        // needed to obtain a handle to a directory
        flags = FILE_FLAG_BACKUP_SEMANTICS;
    }
    h = CreateFileW(upath, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, flags, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        if (SetFileTime(h, NULL, NULL, &ft)) {
            result = 0;
        }
        CloseHandle(h);
    }
    free(upath);
    return result;
#else
    struct timeval tv[2];
    tv[0].tv_sec = t->tv_sec;
    tv[1].tv_sec = t->tv_sec;
    tv[0].tv_usec = t->tv_usec;
    tv[1].tv_usec = t->tv_usec;
    return utimes(path, tv);
#endif
}

int fsapp_file_fstat(int fd, fsapp_file_stat_t *buf)
{
    stat_type st;
    int result = fstat_function(fd, &st);
    if (result == 0) {
        buf->atime = st.st_atime;
        buf->mtime = st.st_mtime;
        buf->ctime = st.st_ctime;
        buf->size = st.st_size;
        buf->mode = st.st_mode;
#ifdef HAVE_STRUCT_STAT_ST_BLOCKS
        buf->blocks = st.st_blocks;
#endif
    }
    return result;
}
