#define _GNU_SOURCE 1
#ifdef WINDOWS
#include <Windows.h>
#else
#include <sys/types.h>
#include <utime.h>
#include <sys/time.h>
#endif

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <fs/base.h>
#include <fs/filesys.h>
#include <fs/string.h>

#ifdef USE_GLIB
#include <glib.h>
#include <glib/gstdio.h>
#else

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>

#ifdef WINDOWS
#define FS_IS_DIR_SEPARATOR(x) ((x) == '/' || (x) == '\\')
#define FS_DIR_SEPARATOR '\\'
#else
#define FS_IS_DIR_SEPARATOR(x) ((x) == '/')
#define FS_DIR_SEPARATOR '/'
#endif

#endif

#ifdef WINDOWS

static wchar_t *wide(const char *utf8) {
    if (utf8[0] == '\0') {
        wchar_t *result = (wchar_t *) malloc(sizeof(wchar_t));
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

    int chars = MultiByteToWideChar(CP_UTF8, flags, utf8, size,
            NULL, 0);
    if (chars == 0) {
        // error convering to wide string
        wchar_t *result = (wchar_t *) malloc(sizeof(wchar_t));
        result[0] = L'0';
        return result;
    }

    // convert

    wchar_t* wstr = (wchar_t*) (malloc(sizeof(wchar_t) * (chars + 1)));
    chars = MultiByteToWideChar(CP_UTF8, flags, utf8, size, wstr,
            chars + 1);
    if (chars == 0) {
        free(wstr);
        wchar_t *result = (wchar_t *) malloc(sizeof(wchar_t));
        result[0] = L'0';
        return result;
    }
    return wstr;
}

static void time_val_to_file_time(struct timeval *tv, FILETIME *ft) {
    // Note that LONGLONG is a 64-bit value
    //LONGLONG ll;
    ULARGE_INTEGER uli;

    uli.QuadPart = Int32x32To64(tv->tv_sec, 10000000) + 116444736000000000LL;
    // FILETIME contains 100-nanosecond intervals
    uli.QuadPart += ((LONGLONG) tv->tv_usec) * 10;
    //ft->dwLowDateTime = (DWORD)ll;
    //ft->dwHighDateTime = ll >> 32;
    ft->dwLowDateTime = uli.LowPart;
    ft->dwHighDateTime = uli.HighPart;
    //printf("%d %d %lld, %d, %d\n", (int) tv->tv_sec, (int) tv->tv_usec, uli.QuadPart, ft->dwHighDateTime, ft->dwLowDateTime);
}

static void file_time_to_time_val(FILETIME *ft, struct timeval *tv) {
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

    uli.QuadPart -= ((int64_t) 116444736) * ((int64_t) 1000000000);
    uli.QuadPart /= 10;

    tv->tv_sec = uli.QuadPart / 1000000;
    tv->tv_usec = uli.QuadPart % 1000000;
}

#endif

#if defined(WINDOWS)

#elif defined(MACOSX)
#define HAVE_STAT_TV_NSEC2
#else
#define HAVE_STAT_TV_NSEC
#endif

// some code adapted from glib

int fs_stat(const char *path, struct fs_stat *buf) {
    struct stat st;
#ifdef USE_GLIB
    // g_stat handles unicode file names on Windows
    int result = g_stat(path, &st);
#else
    int result = stat(path, &st);
#endif

    if (result == 0) {
        buf->atime = st.st_atime;
        buf->mtime = st.st_mtime;
        buf->ctime = st.st_ctime;
        buf->size = st.st_size;
        buf->mode = st.st_mode;

#if defined(HAVE_STAT_TV_NSEC)
        buf->atime_nsec = st.st_atim.tv_nsec;
        buf->mtime_nsec = st.st_mtim.tv_nsec;
        buf->ctime_nsec = st.st_ctim.tv_nsec;
#elif defined(HAVE_STAT_TV_NSEC2)
        buf->atime_nsec = st.st_atimespec.tv_nsec;
        buf->mtime_nsec = st.st_mtimespec.tv_nsec;
        buf->ctime_nsec = st.st_ctimespec.tv_nsec;
#elif defined(HAVE_STAT_NSEC)
        buf->atime_nsec = st.st_atime_nsec;
        buf->mtime_nsec = st.st_mtime_nsec;
        buf->ctime_nsec = st.st_ctime_nsec;
#elif defined(WINDOWS)
        HANDLE h;
        DWORD flags;
        DWORD attr;
        FILETIME t1, t2, t3;

        wchar_t *upath = wide(path);
        attr = GetFileAttributesW(upath);
        flags = FILE_ATTRIBUTE_NORMAL;
        if (attr != INVALID_FILE_ATTRIBUTES &&
                (attr & FILE_ATTRIBUTE_DIRECTORY)) {
            // needed to obtain a handle to a directory
            flags = FILE_FLAG_BACKUP_SEMANTICS;
        }
        h = CreateFileW(upath, FILE_READ_ATTRIBUTES, 0, NULL, OPEN_EXISTING,
                flags, NULL);
        if (h != INVALID_HANDLE_VALUE) {
            if (GetFileTime (h, &t1, &t2, &t3)) {
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
            CloseHandle (h);
        }
        free(upath);
#else
#error no sub-second mtime
#endif
    }
    return result;
}

int fs_set_file_time(const char *path, struct timeval *t) {
#ifdef WINDOWS
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
    h = CreateFileW(upath, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING,
            flags, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        if (SetFileTime (h, NULL, NULL, &ft)) {
            result = 0;
        }
        CloseHandle (h);
    }
    free(upath);
    return result;
#else
    struct timeval tv[2];
    tv[0].tv_sec = t->tv_sec;
    tv[1].tv_sec = t->tv_sec;
    tv[0].tv_usec = t->tv_usec;
    tv[1].tv_usec = t->tv_usec;
    //printf("utimes %s %lld %lld\n", path, (int64_t) t->tv_sec,
    //        (int64_t) t->tv_usec);
    return utimes(path, tv);
#endif
}

int64_t fs_path_get_size(const char *path) {
    struct fs_stat buf;
    int result = fs_stat(path, &buf);
    if (result == 0) {
        return buf.size;
    }
    return -1;
}

int fs_fstat(int fd, struct fs_stat *buf) {
    struct stat st;
    int result = fstat(fd, &st);
    if (result == 0) {
        buf->atime = st.st_atime;
        buf->mtime = st.st_mtime;
        buf->ctime = st.st_ctime;
        buf->size = st.st_size;
        buf->mode = st.st_mode;
    }
    return result;
}

//#define USE_GLIB

struct fs_dir {
#ifdef USE_GLIB
    GDir *gdir;
#else
    DIR* dir;
#endif
};

fs_dir* fs_dir_open(const char *path, int flags) {
#ifdef USE_GLIB
    GDir* gdir = g_dir_open(path, flags, NULL);
    if (!gdir) {
        return NULL;
    }
    fs_dir *dir = fs_new(fs_dir, 1);
    dir->gdir = gdir;
    return dir;
#else
    DIR* d = opendir(path);
    if (!d) {
        return NULL;
    }
    fs_dir *dir = fs_new(fs_dir, 1);
    dir->dir = d;
    return dir;
#endif
}

const char *fs_dir_read_name(fs_dir *dir) {
#ifdef USE_GLIB
    return g_dir_read_name(dir->gdir);
#else
    struct dirent *de = readdir(dir->dir);
    while (de != NULL) {
        // skip . and .. names
        if (de->d_name[0] == '.') {
            if (de->d_name[1] == '\0' ||
                    (de->d_name[1] == '.' && de->d_name[2] == '\0')) {
                de = readdir(dir->dir);
                continue;
            }
        }
        //printf(">> %s", de->d_name);
        return fs_strdup(de->d_name);
    }
    return NULL;
#endif
}

void fs_dir_close(fs_dir *dir) {
#ifdef USE_GLIB
    g_dir_close(dir->gdir);
#else
    closedir(dir->dir);
#endif
    free(dir);
}

FILE *fs_fopen(const char *path, const char *mode) {
#ifdef USE_GLIB
    return g_fopen(path, mode);
#else
    return fopen(path, mode);
#endif
}

int fs_open(const char *path, int flags, int mode) {
#ifdef USE_GLIB
    return g_open(path, flags, mode);
#else
    return open(path, flags, mode);
#endif
}

int fs_rmdir(const char *path) {
#ifdef USE_GLIB
    return g_rmdir(path);
#else
    return rmdir(path);
#endif
}

int fs_unlink(const char *path) {
#ifdef USE_GLIB
    return g_unlink(path);
#else
    return unlink(path);
#endif
}

int fs_rename(const char *old_path, const char *new_path) {
#ifdef USE_GLIB
    return g_rename(old_path, new_path);
#else
    return rename(old_path, new_path);
#endif
}

int fs_mkdir(const char *path, int mode) {
#ifdef USE_GLIB
    return g_mkdir(path, mode);
#else
    return mkdir(path, mode);
#endif
}

#ifndef USE_GLIB
static const char *path_skip_root(const char *fn) {
    const char *p = fn;
#ifdef WINDOWS
    if (p[0] != '\0' && p[1] == ':') {
        p += 2;
        while (*p == '/' || *p == '\\') {
            ++p;
        }
    }
#else
    while (*p++ == '/') {}
#endif
    return p;
}
#endif

int fs_mkdir_with_parents(const char *pathname, int mode) {
#ifdef USE_GLIB
    return g_mkdir_with_parents(pathname, mode);
#else
    char *fn, *p;
    if (pathname == NULL || *pathname == '\0') {
        errno = EINVAL;
        return -1;
    }
    fn = fs_strdup(pathname);
    if (fs_path_is_absolute(fn))
        p = (char *) path_skip_root(fn);
    else
        p = fn;
    do {
        while (*p && !FS_IS_DIR_SEPARATOR (*p))
            p++;
        if (!*p)
            p = NULL;
        else
            *p = '\0';
        if (!fs_path_exists(fn)) {
            if (fs_mkdir(fn, mode) == -1 && errno != EEXIST) {
                int errno_save = errno;
                free(fn);
                errno = errno_save;
                return -1;
            }
        }
        else if (!fs_path_is_dir(fn)) {
            free(fn);
            errno = ENOTDIR;
            return -1;
        }
        if (p) {
            *p++ = FS_DIR_SEPARATOR;
            while (*p && FS_IS_DIR_SEPARATOR (*p))
                p++;
        }
    }
    while (p);
    free(fn);
    return 0;
#endif
}

char *fs_path_get_dirname(const char *path) {
#ifdef USE_GLIB
    return g_path_get_dirname(path);
#else
    char *p = fs_strdup(path);
    char *result = fs_strdup(dirname(p));
    free(p);
    return result;
#endif
}

char *fs_path_get_basename(const char *path) {
#ifdef USE_GLIB
    return g_path_get_basename(path);
#else
    char *p = fs_strdup(path);
    char *result = fs_strdup(basename(p));
    free(p);
    return result;
#endif
}

const char *fs_get_home_dir(void) {
#ifdef USE_GLIB
    return g_get_home_dir();
#else
    const char *result = getenv("HOME");
    if (!result) {
        result = "";
    }
    return result;
#endif
}

const char *fs_get_documents_dir(void) {
#ifdef USE_GLIB
    return g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS);
#else
    static const char *dir = NULL;
    // FIXME: not MT safe
    // FIXME: support XDG spec, Windows
    if (dir == NULL) {
        dir = fs_path_join(fs_get_home_dir(), "Documents", NULL);
    }
    return dir;
#endif
}

const char *fs_get_desktop_dir(void) {
#ifdef USE_GLIB
    return g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP);
#else
    static const char *dir = NULL;
    // FIXME: not MT safe
    // FIXME: support XDG spec, Windows
    if (dir == NULL) {
        dir = fs_path_join(fs_get_home_dir(), "Desktop", NULL);
    }
    return dir;
#endif
}

char *fs_get_current_dir(void) {
#ifdef USE_GLIB
    return g_get_current_dir();
#else
    //return get_current_dir_name();
    char *buf = malloc(PATH_MAX);
    char *result = getcwd(buf, PATH_MAX);
    if (result == NULL) {
        free(buf);
        return fs_strdup("");
    }
    return buf;
#endif
}

int fs_path_exists(const char *path) {
#ifdef USE_GLIB
    return g_file_test(path, G_FILE_TEST_EXISTS);
#else
    struct stat buf;
    if (stat(path, &buf) != 0) {
        return 0;
    }
    return 1;
#endif
}

int fs_path_is_file(const char *path) {
#ifdef USE_GLIB
    return g_file_test(path, G_FILE_TEST_IS_REGULAR);
#else
    struct stat buf;
    if (stat(path, &buf) != 0) {
        return 0;
    }
    return S_ISREG(buf.st_mode);
#endif
}

int fs_path_is_dir(const char *path) {
#ifdef USE_GLIB
    return g_file_test(path, G_FILE_TEST_IS_DIR);
#else
    struct stat buf;
    if (stat(path, &buf) != 0) {
        return 0;
    }
    return S_ISDIR(buf.st_mode);
#endif
}

int fs_path_is_absolute(const char *path) {
#ifdef USE_GLIB
    return g_path_is_absolute(path);
#else
#ifdef WINDOWS
    // simple check, only checking that it starts with x:
    return path[0] != '\0' && path[1] == ':';
#else
    return path && path[0] == '/';
#endif
#endif
}

#define MAX_JOINED_PATH 1024

char *fs_path_join(const char *first_element, ...) {
    // MAX_JOINED_PATH 1024 + 1 terminating null character + 1 to
    // be able to skip some checks
    char *result = malloc(MAX_JOINED_PATH + 1 + 1);
    int index = 0;
    result[index] = '\0';
    va_list args;
    va_start(args, first_element);

    const char *next_element = first_element;
    while (1) {
        const char *element;
        //const gchar *start;
        //const gchar *end;

        if (!next_element) {
            break;
        }
        element = next_element;
        next_element = va_arg(args, const char*);

        if (element[0] == '\0') {
            // skip blank component
            continue;
        }

        if (index > 0 && (result[index - 1] != '/'
#ifdef WINDOWS
                && result[index - 1] != '\\'
#endif
            )) {
#ifdef WINDOWS
            result[index] = '\\';
#else
            result[index] = '/';
#endif
            ++index;
        }

        //printf("%p %s\n", element, element);
        int len = strlen(element);
        if (index + len > MAX_JOINED_PATH) {
            strcpy(result, "ERROR_MAX_JOINED_PATH");
            break;
        }

        strcpy(result + index, element);
        index += len;
    }

    va_end(args);
    //printf(">>>>%s<\n", result);
    return result;

#if 0
    gchar *str;
    va_list args;
    va_start(args, first_element);
#ifndef WINDOWS
    str = g_build_path_va("/", first_element, &args, NULL);
#else
    str = g_build_pathname_va(first_element, &args, NULL);
#endif
    va_end(args);
    return str;
#endif
}
