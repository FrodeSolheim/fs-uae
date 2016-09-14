#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _GNU_SOURCE 1
#include <fs/filesys.h>
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
// #include <fs/log.h>
#include <fs/base.h>
#include <fs/data.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef USE_GLIB
// FIXME g_atomic_int_get / g_atomic_int_set
#include <glib.h>
#endif

const char *fs_get_user_config_dir(void)
{
#ifdef USE_GLIB
#ifdef MACOSX
    static char *path = NULL;
    if (path == NULL) {
        path = g_build_filename(g_get_home_dir(), "Library", "Preferences",
                NULL);
    }
    return path;
#else
    return g_get_user_config_dir();
#endif
#else
#error not implemented
#endif
}

const char *fs_get_user_data_dir(void)
{
#ifdef USE_GLIB
    return g_get_user_data_dir();
#else
#error not implemented
#endif
}

char *fs_get_data_file(const char *relative)
{
    static int initialized = 0;
    static char executable_dir[PATH_MAX];
    if (!initialized) {
        fs_get_application_exe_dir(executable_dir, PATH_MAX);
        initialized = 1;
    }
    char *path;
    path = g_build_filename(executable_dir, "share", relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    g_free(path);
    path = g_build_filename(executable_dir, "..", "share", relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    g_free(path);

#ifdef MACOSX
    char buffer[FS_PATH_MAX];
    fs_get_application_exe_dir(buffer, FS_PATH_MAX);
    path = g_build_filename(buffer, "..", "Resources", relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    g_free(path);
#endif

#ifdef USE_GLIB
    const char *user_dir = g_get_user_data_dir();
    path = g_build_filename(user_dir, relative, NULL);
    if (fs_path_exists(path)) {
        return path;
    }
    g_free(path);

    const char * const *dirs = g_get_system_data_dirs();
    while(*dirs) {
        path = g_build_filename(*dirs, relative, NULL);
        if (fs_path_exists(path)) {
            return path;
        }
        g_free(path);
        dirs++;
    }
#endif

    return NULL;
}

static const char *g_data_dir;

void fs_set_data_dir(const char *path)
{
    g_data_dir = path;
}

const char *fs_data_dir(void)
{
    g_assert(g_data_dir != NULL);
    return g_data_dir;
}

char *fs_get_program_data_file(const char *relative)
{
    char *relative2 = g_build_filename(fs_get_prgname(), relative, NULL);
    char *result = fs_get_data_file(relative2);
    g_free(relative2);
    return result;
}

int fs_get_program_data(const char *relative, char **data, int *size)
{
    char *name = g_build_filename("share", fs_get_prgname(), relative, NULL);
    int error = fs_data_file_content(name, data, size);
    g_free(name);
    return error;
}

void fs_get_current_time(fs_time_val *result)
{
    if (result == NULL) {
        return;
    }
#ifdef USE_GLIB
    GTimeVal result2;
    g_get_current_time(&result2);
    result->tv_sec = result2.tv_sec;
    result->tv_usec = result2.tv_usec;
#else
#error not implemented
#endif
}

int64_t fs_get_real_time(void)
{
    fs_time_val tv;
    fs_get_current_time(&tv);
    return (((int64_t) tv.tv_sec) * 1000000) + tv.tv_usec;
}

int64_t fs_get_monotonic_time(void)
{
#ifdef USE_GLIB
    return g_get_monotonic_time();
#else
#error not implemented
#endif
}

static char** g_argv = NULL;
static int g_argc = 0;

void fs_set_argv(int argc, char* argv[])
{
    g_argc = argc;
    g_argv = argv;
}

static char *find_program_in_path(const char *prog)
{
#ifdef USE_GLIB
    return g_find_program_in_path(prog);
#else
    const char* c = prog;
    while(*c) {
        if (*c++ == '/') {
            /* path contains / - not started via PATH, it's either
             * already an absolute path - or a relative path. */
            return g_strdup(prog);
        }
    }

    const char* env_path = getenv("PATH");
    if (env_path == NULL) {
        return NULL;
    }
    char *result = NULL;
    char **dirs = g_strsplit(env_path, ":", 0);
    char **dir = dirs;
    while(*dir) {
        if (*dir) {
            char *abs = g_build_filename(*dir, prog, NULL);
            if (fs_path_is_file(abs)) {
                result = abs;
                break;
            }
            g_free(abs);
        }
        dir++;
    }
    g_strfreev(dirs);
    return result;
#endif
}

int fs_get_application_exe_path(char *buffer, int size)
{
    /* Possible sources:
     * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
     * Linux: readlink /proc/self/exe
     * Solaris: getexecname()
     * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
     * BSD with procfs: readlink /proc/curproc/file
     * Windows: GetModuleFileName() with hModule = NULL
     * or guess using argv[0] and PATH
     */
    if (size < 1) {
        return 0;
    }
    buffer[0] = '\0';

#if defined(WINDOWS)

    wchar_t * temp_buf = g_malloc(sizeof(wchar_t) * PATH_MAX);
    /* len is the number of characters NOT including the terminating null
     * character. */
    int len = GetModuleFileNameW(NULL, temp_buf, PATH_MAX);
    /* Specify size - 1 to reserve space for a null-terminating byte. */
    int result = WideCharToMultiByte(CP_UTF8, 0, temp_buf, len,
            buffer, size - 1, NULL, NULL);
    g_free(temp_buf);
    if (result == 0) {
        return 0;
    }
    /* Since len does not include the null-terminator, WideCharToMultiByte
     * does not null-terminate the buffer. */
    buffer[result] = '\0';
    return 1;

#elif defined(MACOSX)

    unsigned int usize = size;
    int result = _NSGetExecutablePath(buffer, &usize);
    if (result == 0) {
        return 1;
    } else {
        fs_log("_NSGetExecutablePath failed with result %d\n", result);
        buffer[0] = '\0';
        return 0;
    }

#else

    if (g_argc == 0) {
        buffer[0] = '\0';
        return 0;
    }

    char* result;
    result = find_program_in_path(g_argv[0]);
    if (result == NULL) {
        buffer[0] = '\0';
        return 0;
    }

    if (result[0] != '/') {
        char* old_result = result;
        char* current_dir = g_get_current_dir();
        result = g_build_filename(current_dir, old_result, NULL);
        g_free(old_result);
        g_free(current_dir);
    }

    if (strlen(result) > size - 1) {
        buffer[0] = '\0';
        g_free(result);
        return 0;
    }

    /* We have already checked that the buffer is big enough */
    strcpy(buffer, result);
    g_free(result);
    return 1;

#endif
}

int fs_get_application_exe_dir(char *buffer, int size)
{
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

#ifndef USE_GLIB
static char *g_prgname = "unnamed-program";
static char *g_application_name = "Unnamed Program";
#endif

const char *fs_get_application_name(void)
{
#ifdef USE_GLIB
    return g_get_application_name();
#else
    return g_application_name;
#endif
}

void fs_set_application_name(const char *application_name)
{
#ifdef USE_GLIB
    g_set_application_name(application_name);
#else
    g_application_name = fs_strdup(application_name);
#endif
}

const char *fs_get_prgname(void)
{
#ifdef USE_GLIB
    return g_get_prgname();
#else
    return g_prgname;
#endif
}

void fs_set_prgname(const char *prgname)
{
#ifdef USE_GLIB
    g_set_prgname(prgname);
#else
    g_prgname = fs_strdup(prgname);
#endif
}
