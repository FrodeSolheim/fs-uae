#define FSUAE_INTERNAL
#include "fsuae-path.h"

#include <SDL3/SDL.h>
#include <glib.h>
#include <string.h>

#include "fslib-path.h"

// const char *fsuae_path_plugins_dir(void)
// {
//     return "System";
// }

// const char* fsuae_path_appdata_dir(void) {
//     static const char* path;
//     if (!path) {
//         gchar* temp = g_build_filename(fsuae_path_system_dir(), "FS-UAE", "Data", NULL);
//         path = g_canonicalize_filename(temp, NULL);
//         g_free(temp);
//         SDL_Log("fsuae_path_appdata_dir <- %s", path);
//     }
//     return path;
// }

const char* fsuae_path_base_dir(void) {
    static const char* path;
    if (!path) {
        gchar* temp = g_build_filename(fsuae_path_system_dir(), "..", NULL);
        path = g_canonicalize_filename(temp, NULL);
        g_free(temp);
        SDL_Log("fsuae_path_base_dir <- %s", path);
    }
    return path;
}

// const char* fsuae_path_python_dir(void) {
//     static const char* path;
//     if (!path) {
//         char temp[FSLIB_PATH_MAX];

//         if (fsapp_development_mode) {
//             fslib_path_executable_dir(temp, FSLIB_PATH_MAX);
//             path = g_build_filename(temp, "od-fs", "python", NULL);
//         } else {
//             fslib_path_application_dir(temp, FSLIB_PATH_MAX);
//             printf("temp: %s\n", temp);
//             gchar* temp2 = g_build_filename(temp, "..", "..", "Python", NULL);
//             printf("temp2: %s\n", temp2);
//             path = g_canonicalize_filename(temp2, NULL);
//             printf("path: %s\n", temp2);
//             g_free(temp2);
//         }
//         SDL_Log("fsuae_path_python_dir <- %s", path);
//     }
//     return path;
// }

const char* fsuae_path_python_zip(void) {
    static const char* path;
    if (!path) {
        char temp[FSLIB_PATH_MAX];
#if defined(WINDOWS)
        SDL_strlcpy(temp, SDL_GetBasePath(), FSLIB_PATH_MAX);
        gchar* temp2 = g_build_filename(temp, "..", "..", "Python", "FS-UAE.zip", NULL);
        path = g_canonicalize_filename(temp2, NULL);
        g_free(temp2);
#elif defined(MACOS)
        // This will be (...).app/MacOS/Contents/Resources/
        SDL_strlcpy(temp, SDL_GetBasePath(), FSLIB_PATH_MAX);
        SDL_strlcat(temp, "Python/FS-UAE.zip", FSLIB_PATH_MAX);
        path = strdup(temp);
#else
        fslib_path_executable_dir(temp, FSLIB_PATH_MAX);
        path = g_build_filename(temp, "/Python/FS-UAE.zip", NULL);
#endif
    }
    return path;
}

const char* fsuae_path_fs_uae_dat(void) {
    static const char* path;
    if (!path) {
        char temp[FSLIB_PATH_MAX];
        fslib_path_executable_dir(temp, FSLIB_PATH_MAX);
        path = g_build_filename(temp, "fs-uae.dat", NULL);
    }
    return path;
}

const char* fsuae_path_system_dir(void) {
    static const char* path;
    if (!path) {
        char temp[FSLIB_PATH_MAX];

        if (fsapp_development_mode) {
            fslib_path_executable_dir(temp, FSLIB_PATH_MAX);
            path = g_build_filename(temp, "System", NULL);
        } else {
            fslib_path_application_dir(temp, FSLIB_PATH_MAX);
            gchar* temp2 = g_build_filename(temp, "..", "..", "..", NULL);
            path = g_canonicalize_filename(temp2, NULL);
            g_free(temp2);
        }
        SDL_Log("fsuae_path_system_dir <- %s", path);
    }
    return path;
}

void fsuae_path_init(void) {
    // if (module.initialized) {
    //     return;
    // }
    // module.initialized = true;
}
