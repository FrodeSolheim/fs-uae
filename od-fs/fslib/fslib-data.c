#include "fslib-data.h"
#include "fsemu-util.h" // fsemu_path_exists
#include "fslib-os.h"
#include "fslib-path.h"

#include <glib.h>
#include <glib/gstdio.h>

#ifdef FSLIB_OS_WINDOWS
#include <Windows.h>
#endif

#ifdef FSLIB_OS_MACOS
#include <mach-o/dyld.h>
#endif

// ----------------------------------------------------------------------------

#define FSLIB_LOG_LEVEL fslib_data_log_level
#define FSLIB_LOG_PREFIX "[LIB] [DAT]"
int fslib_data_log_level = FSEMU_LOG_LEVEL_DEBUG;

static struct {
    char* prefix;
    char* dev_prefix;
    char* dev_test;
} fslib_data;

// ----------------------------------------------------------------------------

// FIXME
bool fslib_data_development_mode(void)
{
    static bool initialized;
    static bool development_mode;
    if (fsemu_unlikely(!initialized)) {
        initialized = true;

        char executable_dir[FSLIB_PATH_MAX];
        fslib_path_executable_dir(executable_dir, FSLIB_PATH_MAX);

        gchar* path;
        if (fslib_data.dev_test) {
            path = g_build_filename(executable_dir, fslib_data.dev_test, NULL);
        } else {
            path = g_build_filename(executable_dir, ".git/config", NULL);
        }
        fslib_log(FSLIB_LOG_DEBUG, "Testing %s\n", path);
        if (fslib_path_exists(path)) {
            development_mode = true;
        }
        g_free(path);
        fslib_log(FSLIB_LOG_INFO, "Development mode: %d\n", development_mode);
    }
    return development_mode;
}

// fslib_data_copy_dir -> fslib_data_dir_copy ?
// fslib_data_copy_dir -> fslib_data_assign_dir ?
size_t fslib_data_copy_dir_path(char* path, size_t size)
{
    // fslib_path_copy_exe_dir_path(path, size);
    fslib_path_executable_dir(path, size);
    if (fslib_data_development_mode()) {
        return g_strlcat(path, "/data", size);
    } else {
#ifdef FSLIB_OS_MACOS
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

static bool fslib_data_path_exists_or_free(char* path)
{
    fslib_log(FSLIB_LOG_DEBUG, "Checking '%s'", path);
    bool result = fslib_path_exists(path);
    if (!result) {
        free(path);
    }
    return result;
}

char* fslib_data_file_path(const char* relative)
{
    static int initialized = 0;
    static char executable_dir[FSLIB_PATH_MAX];
    if (!initialized) {
        fslib_path_executable_dir(executable_dir, FSLIB_PATH_MAX);
        fslib_log(FSLIB_LOG_INFO, "Application exe dir: '%s'", executable_dir);
        initialized = 1;
    }
    fslib_log(FSLIB_LOG_DEBUG, "fslib_data_file_path relative='%s'", relative);
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
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }
    }

    // Check the same directory as the executable first
    path = g_build_filename(executable_dir, relative, NULL);
    if (fslib_data_path_exists_or_free(path)) {
        return path;
    }

#ifdef FSLIB_OS_MACOS
    char buffer[FSLIB_PATH_MAX];
    fslib_path_executable_dir(buffer, FSLIB_PATH_MAX);
    path = g_build_filename(buffer, "..", "Resources", "Data", relative, NULL);
#else
    // FIXME: Replace fs-uae with application name build c
    path = g_build_filename(
        executable_dir, "..", "share", "fs-uae", relative, NULL
    );
#endif
    if (fslib_data_path_exists_or_free(path)) {
        return path;
    }

    if (fslib_data_development_mode()) {
        // Check in the data dir (during development and testing)
        path = g_build_filename(executable_dir, "data", relative, NULL);
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }

        // // Check in the fsemu/data dir (during development and testing)
        // path = g_build_filename(
        //     executable_dir, "fsemu", "data", relative, NULL
        // );
        // if (fslib_data_path_exists_or_free(path)) {
        //     return path;
        // }
        // // Check in the ../fsemu/data dir (during development and testing)
        // path = g_build_filename(
        //     executable_dir, "..", "fsemu", "data", relative, NULL
        // );
        // if (fslib_data_path_exists_or_free(path)) {
        //     return path;
        // }

        // DEPRECATED: Check in the data.fs dir (during development and
        // testing)
        // path = g_build_filename(executable_dir, "data.fs", relative, NULL);
        // if (fslib_data_path_exists_or_free(path)) {
        //     return path;
        // }
    }

    // Check in the plugin data dir
    // FIXME: check plugin mode
    bool plugin_mode = true;
    if (plugin_mode) {
        // FIXME:
        path = g_build_filename(
            executable_dir, "System", "FS-UAE", "Data", relative, NULL
        );
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }

#ifdef FSLIB_OS_MACOS
        // Need to go further up in the hierarchy due to being bundled inside
        // an application bundle.
        path = g_build_filename(
            executable_dir, "..", "..", "..", "..", "..", "Data", relative,
            NULL
        );
#else
        path = g_build_filename(
            executable_dir, "..", "..", "Data", relative, NULL
        );
#endif
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }

#if 0
        path = g_build_filename(executable_dir, "share", relative, NULL);
        fslib_data_log("Checking %s\n", path);
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }
        path = g_build_filename(executable_dir, "..", "share", relative, NULL);
        fslib_data_log("Checking %s\n", path);
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }
#endif
    }

#if 0
#ifdef FSEMU_GLIB
    const char *user_dir = g_get_user_data_dir();
    path = g_build_filename(user_dir, relative, NULL);
    if (fslib_data_path_exists_or_free(path)) {
        return path;
    }

    const char *const *dirs = g_get_system_data_dirs();
    while (*dirs) {
        path = g_build_filename(*dirs, relative, NULL);
        if (fslib_data_path_exists_or_free(path)) {
            return path;
        }
        dirs++;
    }
#endif
#endif

    return NULL;
}

bool fslib_data_load(const char* name, void** data, int* data_size)
{
    fslib_data_log("Load %s\n", name);
    *data = NULL;
    *data_size = 0;
    char* path = fslib_data_file_path(name);
    if (!path) {
        fslib_log(FSLIB_LOG_WARNING, "Could not find data file '%s'", name);
        return false;
    }
    fslib_log(FSLIB_LOG_INFO, "Path: %s", path);

    FILE* f = g_fopen(path, "rb");
    g_free(path);
    if (!f) {
        fslib_log(FSLIB_LOG_INFO, "Could not open file '%s'", path);
        return false;
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
        fslib_log(FSLIB_LOG_INFO, "Did not read expected data");
        return false;
    }
    fclose(f);

    *data = result;
    *data_size = file_size;
    return true;
}

fslib_stream_t* fslib_data_stream(const char* name)
{
    char* path = fslib_data_file_path(name);
    if (!path) {
        fslib_data_log("No path!\n");
        return fslib_stream_null();
    }

    fslib_stream_t* stream = fslib_stream_from_path(path, "rw");
    return stream;
}

void fslib_data_init_prefix(const char* prefix)
{
    if (fslib_data.prefix != NULL) {
        g_free(fslib_data.prefix);
    }
    SDL_assert(prefix != NULL);
    fslib_data.prefix = g_strdup(prefix);
}

void fslib_data_init_dev_prefix(const char* dev_prefix)
{
    if (fslib_data.dev_prefix != NULL) {
        g_free(fslib_data.dev_prefix);
    }
    SDL_assert(dev_prefix != NULL);
    fslib_data.dev_prefix = g_strdup(dev_prefix);
}

void fslib_data_init_dev_test(const char* dev_test)
{
    if (fslib_data.dev_test != NULL) {
        g_free(fslib_data.dev_test);
    }
    if (dev_test) {
        fslib_data.dev_test = g_strdup(dev_test);
    } else {
        fslib_data.dev_test = NULL;
    }
}

void fslib_data_init(void)
{
    // FIXME: init only once

    // Pre-check development mode (not strictly necessary)
    fslib_data_development_mode();
}
