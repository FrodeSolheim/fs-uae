#include "fslib-path.h"
#include "fslib-os.h"

#include <glib.h>
#include <stdio.h>

int fslib_path_log_level = FSEMU_LOG_LEVEL_DEBUG;

bool fslib_path_exists(const char* path)
{
    return g_file_test(path, G_FILE_TEST_EXISTS);
}

#ifdef _WIN32
#elif defined(FSEMU_OS_MACOS)
#else

#define USE_GLIB

static char* fslib_path_find_program_in_path(const char* prog)
{
#ifdef USE_GLIB
    return g_find_program_in_path(prog);
#else
    const char* c = prog;
    while (*c) {
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
    char* result = NULL;
    char** dirs = g_strsplit(env_path, ":", 0);
    char** dir = dirs;
    while (*dir) {
        if (*dir) {
            char* abs = g_build_filename(*dir, prog, NULL);
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

#endif

bool fslib_path_executable_path(char* buffer, int buffer_size)
{
    // FIXME: CACHE THIS

    /* Possible sources:
     * Mac OS X: _NSGetExecutablePath() (man 3 dyld)
     * Linux: readlink /proc/self/exe
     * Solaris: getexecname()
     * FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
     * BSD with procfs: readlink /proc/curproc/file
     * Windows: GetModuleFileName() with hModule = NULL
     * or guess using argv[0] and PATH
     */
    if (buffer_size < 1) {
        return false;
    }
    buffer[0] = '\0';

#if defined(FSLIB_OS_WINDOWS)

    wchar_t* temp_buf = (wchar_t*)g_malloc(sizeof(wchar_t) * FSEMU_PATH_MAX);
    /* len is the number of characters NOT including the terminating null
     * character. */
    int len = GetModuleFileNameW(NULL, temp_buf, FSEMU_PATH_MAX);
    /* Specify size - 1 to reserve space for a null-terminating byte. */
    int result = WideCharToMultiByte(
        CP_UTF8, 0, temp_buf, len, buffer, buffer_size - 1, NULL, NULL
    );
    g_free(temp_buf);
    if (result == 0) {
        return false;
    }
    /* Since len does not include the null-terminator, WideCharToMultiByte
     * does not null-terminate the buffer. */
    buffer[result] = '\0';
    return true;

#elif defined(FSLIB_OS_MACOS)

    unsigned int usize = buffer_size;
    int result = _NSGetExecutablePath(buffer, &usize);
    if (result == 0) {
        return 1;
    } else {
        fslib_path_log("_NSGetExecutablePath failed with result %d\n", result);
        buffer[0] = '\0';
        return false;
    }

#else

    FILE* f = fopen("/proc/self/cmdline", "rb");
    char cmdline[1024];
    if (fread(cmdline, 1, 1024, f) == 0) {
        fslib_path_log("Could not read from /proc/self/cmdline\n");
        cmdline[0] = '\0';
    }
    cmdline[1023] = '\0';

#if 0
    if (g_argc == 0 || g_argv == NULL) {
        buffer[0] = '\0';
        return 0;
    }
#endif

    char* result;
    result = fslib_path_find_program_in_path(cmdline);
    if (result == NULL) {
        buffer[0] = '\0';
        return false;
    }

    if (result[0] != '/') {
        char* old_result = result;
        char* current_dir = g_get_current_dir();
        result = g_build_filename(current_dir, old_result, NULL);
        g_free(old_result);
        g_free(current_dir);
    }

    if ((int)strlen(result) > buffer_size - 1) {
        buffer[0] = '\0';
        g_free(result);
        return false;
    }

    // We have already checked that the buffer is big enough
    strcpy(buffer, result);
    g_free(result);
    return true;
#endif
}

// FIXME: Have an executable_dir which (on macOS) refers to the
// app bundle and not the actual executable dir
// fslib_path_application_dir for example?

bool fslib_path_executable_dir(char* buffer, int buffer_size)
{
    static bool initialized;
    static char path[FSLIB_PATH_MAX];
    if (!initialized) {
        if (!fslib_path_executable_path(path, FSLIB_PATH_MAX)) {
            return false;
        }
        int pos = strlen(path) - 1;
        while (pos >= 0) {
            if (path[pos] == '\\' || path[pos] == '/') {
                path[pos] = '\0';
                break;
            }
            pos -= 1;
        }
        if (pos < 0) {
            return false;
        }
        initialized = true;
    }
    g_strlcpy(buffer, path, buffer_size);
    return true;
}

// FIXME: macOS - directory containing the .app bundle

bool fslib_path_application_dir(char* buffer, int buffer_size)
{
    static bool initialized;
    static char path[FSLIB_PATH_MAX];
    if (!initialized) {
        if (!fslib_path_executable_dir(path, FSLIB_PATH_MAX)) {
            return false;
        }
#ifdef FSLIB_OS_MACOS
        int pos = strlen(path) - 1;
        int count = 0;
        while (pos >= 0) {
            if (path[pos] == '\\' || path[pos] == '/') {
                count += 1;
                if (count == 3) {
                    path[pos] = '\0';
                    break;
                }
            }
            pos -= 1;
        }
        if (pos < 0) {
            return false;
        }
#endif
        initialized = true;
    }
    g_strlcpy(buffer, path, buffer_size);
    return true;
}
