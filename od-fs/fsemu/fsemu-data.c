#define FSEMU_INTERNAL
#include "fsemu-data.h"

#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

#ifdef FSEMU_OS_WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

// ----------------------------------------------------------------------------

int fsemu_data_log_level = FSEMU_LOG_LEVEL_DEBUG;

/*
static struct fsemu_titlebar {

} fsemu_data;
*/

// ----------------------------------------------------------------------------

void fsemu_data_init(void) {
    fsemu_return_if_already_initialized();
}

#ifdef FSEMU_OS_WINDOWS
#elif defined(FSEMU_OS_MACOS)
#else

static char* find_program_in_path(const char* prog) {
#ifdef FSEMU_GLIB
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

static int fs_get_application_exe_path(char* buffer, int size) {
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
    if (size < 1) {
        return 0;
    }
    buffer[0] = '\0';

#if defined(FSEMU_OS_WINDOWS)

    wchar_t* temp_buf = (wchar_t*)g_malloc(sizeof(wchar_t) * FSEMU_PATH_MAX);
    /* len is the number of characters NOT including the terminating null
     * character. */
    int len = GetModuleFileNameW(NULL, temp_buf, FSEMU_PATH_MAX);
    /* Specify size - 1 to reserve space for a null-terminating byte. */
    int result = WideCharToMultiByte(CP_UTF8, 0, temp_buf, len, buffer, size - 1, NULL, NULL);
    g_free(temp_buf);
    if (result == 0) {
        return 0;
    }
    /* Since len does not include the null-terminator, WideCharToMultiByte
     * does not null-terminate the buffer. */
    buffer[result] = '\0';
    return 1;

#elif defined(FSEMU_OS_MACOS)

    unsigned int usize = size;
    int result = _NSGetExecutablePath(buffer, &usize);
    if (result == 0) {
        return 1;
    } else {
        fsemu_data_log("_NSGetExecutablePath failed with result %d\n", result);
        buffer[0] = '\0';
        return 0;
    }

#else

    FILE* f = fopen("/proc/self/cmdline", "rb");
    char cmdline[1024];
    if (fread(cmdline, 1, 1024, f) == 0) {
        fsemu_data_log("Could not read from /proc/self/cmdline\n");
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
    result = find_program_in_path(cmdline);
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

    if ((int)strlen(result) > size - 1) {
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

int fsemu_application_exe_path(char* buffer, int size) {
    return fs_get_application_exe_path(buffer, size);
}

static int fs_get_application_exe_dir(char* buffer, int size) {
    // FIXME: Cache result?
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

bool fsemu_data_development_mode(void) {
    static bool initialized;
    static bool development_mode;
    if (fsemu_unlikely(!initialized)) {
        initialized = true;
        char executable_dir[FSEMU_PATH_MAX];
        fs_get_application_exe_dir(executable_dir, FSEMU_PATH_MAX);
        char* path = g_build_filename(executable_dir, "fsemu", "src", "fsemu-all.c", NULL);
        printf("%s\n", path);
        if (fsemu_path_exists(path)) {
            development_mode = true;
        }
        free(path);
        fsemu_data_log("Development mode: %d\n", development_mode);
    }
    return development_mode;
}

size_t fsemu_application_exe_dir_strlcpy(char* path, size_t size) {
    fs_get_application_exe_dir(path, size);
    // Return value is not entirely compatible with strlcpy if there was not
    // enough room.
    // FIXME: Allocate exe dir path in init, and g_strlcpy here
    return strlen(path);
}

size_t fsemu_data_dir_strlcpy(char* path, size_t size) {
    fs_get_application_exe_dir(path, size);
    if (fsemu_data_development_mode()) {
        return g_strlcat(path, "/data", size);
    } else {
#ifdef FSEMU_OS_MACOS
        return g_strlcat(path, "/../Resources/Data", size);
#else
        // Pass
#endif
    }

    // Return value is not entirely compatible with strlcpy if there was not
    // enough room.
    // FIXME: Allocate exe dir path in init, and g_strlcpy here
    return strlen(path);
}

static bool fsemu_data_path_exists_or_free(char* path) {
    fsemu_data_log("Checking %s\n", path);
    bool result = fsemu_path_exists(path);
    if (!result) {
        free(path);
    }
    return result;
}

char* fsemu_data_file_path(const char* relative) {
    static int initialized = 0;
    static char executable_dir[FSEMU_PATH_MAX];
    if (!initialized) {
        fs_get_application_exe_dir(executable_dir, FSEMU_PATH_MAX);
        fsemu_data_log("Application exe dir: %s\n", executable_dir);
        initialized = 1;
    }
    fsemu_data_log_debug("fsemu_data_file_path relative=%s\n", relative);
    char* path;

    // FIXME: Can remove basename checks now that dirs have been stripped from
    // from data paths.
    const char* basename = relative;
    const char* p = basename;
    while (*p) {
        if (*p == '/') {
            basename = p + 1;
        }
        p++;
    }
    // Check the same directory as the executable first, basename only
    if (basename != relative) {
        path = g_build_filename(executable_dir, basename, NULL);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
    }

    // Check the same directory as the executable first
    path = g_build_filename(executable_dir, relative, NULL);
    if (fsemu_data_path_exists_or_free(path)) {
        return path;
    }

#ifdef FSEMU_OS_MACOS
    char buffer[FSEMU_PATH_MAX];
    fs_get_application_exe_dir(buffer, FSEMU_PATH_MAX);
    path = g_build_filename(buffer, "..", "Resources", "Data", relative, NULL);
#else
    // FIXME: Replace fs-uae with application name build c
    path = g_build_filename(executable_dir, "..", "share", "fs-uae", relative, NULL);
#endif
    if (fsemu_data_path_exists_or_free(path)) {
        return path;
    }

    if (fsemu_data_development_mode()) {
        // Check in the data dir (during development and testing)
        path = g_build_filename(executable_dir, "data", relative, NULL);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
        // Check in the fsemu/data dir (during development and testing)
        path = g_build_filename(executable_dir, "fsemu", "data", relative, NULL);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
        // Check in the ../fsemu/data dir (during development and testing)
        path = g_build_filename(executable_dir, "..", "fsemu", "data", relative, NULL);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
        // DEPRECATED: Check in the data.fs dir (during development and
        // testing)
        path = g_build_filename(executable_dir, "data.fs", relative, NULL);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
    }

    // Check in the plugin data dir
    // FIXME: check plugin mode
    bool plugin_mode = true;
    if (plugin_mode) {
#ifdef FSEMU_OS_MACOS
        // Need to go further up in the hierarchy due to being bundled inside
        // an application bundle.
        path =
            g_build_filename(executable_dir, "..", "..", "..", "..", "..", "Data", relative, NULL);
#else
        path = g_build_filename(executable_dir, "..", "..", "Data", relative, NULL);
#endif
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }

#if 0
        path = g_build_filename(executable_dir, "share", relative, NULL);
        fsemu_data_log("Checking %s\n", path);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
        path = g_build_filename(executable_dir, "..", "share", relative, NULL);
        fsemu_data_log("Checking %s\n", path);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
#endif
    }

#if 0
#ifdef FSEMU_GLIB
    const char *user_dir = g_get_user_data_dir();
    path = g_build_filename(user_dir, relative, NULL);
    if (fsemu_data_path_exists_or_free(path)) {
        return path;
    }

    const char *const *dirs = g_get_system_data_dirs();
    while (*dirs) {
        path = g_build_filename(*dirs, relative, NULL);
        if (fsemu_data_path_exists_or_free(path)) {
            return path;
        }
        dirs++;
    }
#endif
#endif

    return NULL;
}

void fsemu_data_load(const char* name, void** data, int* data_size) {
    fsemu_data_log("Load %s\n", name);
    *data = NULL;
    *data_size = 0;
    char* path = fsemu_data_file_path(name);
    if (!path) {
        fsemu_data_log_warning("Could not find data file '%s'\n", name);
        return;
    }
    fsemu_data_log("Path: %s\n", path);

    FILE* f = g_fopen(path, "rb");
    g_free(path);
    if (!f) {
        fsemu_data_log("Could not open file!\n");
        return;
    }
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    void* result = malloc(file_size);
    /*
    void *p = *data;
    while(file_size > 0) {
        fread();
    }
    */
    if (fread(result, file_size, 1, f) != 1) {
        free(result);
        fclose(f);
        fsemu_data_log("Did not read expected data\n");
        return;
    }
    fclose(f);

    *data = result;
    *data_size = file_size;
}

fsemu_stream_t* fsemu_data_stream(const char* name) {
    char* path = fsemu_data_file_path(name);
    if (!path) {
        fsemu_data_log("No path!\n");
        return fsemu_stream_null();
    }

    fsemu_stream_t* stream = fsemu_stream_from_path(path, "rw");
    return stream;
}
