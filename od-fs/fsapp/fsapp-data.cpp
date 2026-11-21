/*
 * The program's data files: finding them, and loading and streaming them.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-data.h"

#include <SDL3/SDL.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "fsapp-os.h"
#include "fs/file.h"
#include "fs/path.h"

// ----------------------------------------------------------------------------

int fsapp_data_log_level = SDL_LOG_PRIORITY_DEBUG;

void fsapp_data_init(void)
{
}

static bool fsapp_data_path_exists_or_free(char *path)
{
    fsapp_data_log("Checking %s\n", path);
    bool result = SDL_GetPathInfo(path, NULL);
    if (!result) {
        SDL_free(path);
    }
    return result;
}

char *fsapp_data_find_path(const char *relative)
{
    // The executable's folder, and in a macOS app bundle Contents/Resources/ (a sibling of
    // Contents/MacOS, so the paths below that go up from it reach the same places).
    const char *base_path = SDL_GetBasePath();
    if (base_path == NULL) {
        base_path = "";
    }
    fsapp_data_log_debug("fsapp_data_find_path relative=%s\n", relative);
    char *path;

    // FIXME: Can remove basename checks now that dirs have been stripped from
    // from data paths.
    const char *basename = relative;
    const char *p = basename;
    while (*p) {
        if (*p == '/') {
            basename = p + 1;
        }
        p++;
    }
    // Check the base path first, basename only
    if (basename != relative) {
        path = FS_JoinPath(base_path, basename);
        if (fsapp_data_path_exists_or_free(path)) {
            return path;
        }
    }

    // Then the relative path in it
    path = FS_JoinPath(base_path, relative);
    if (fsapp_data_path_exists_or_free(path)) {
        return path;
    }

#ifdef FSAPP_OS_MACOS
    path = FS_JoinPath(base_path, "..", "Resources", "Data", relative);
#else
    // FIXME: Replace fs-uae with application name build c
    path = FS_JoinPath(base_path, "..", "share", "fs-uae", relative);
#endif
    if (fsapp_data_path_exists_or_free(path)) {
        return path;
    }

    // Check in the plugin data dir
    // FIXME: check plugin mode
    bool plugin_mode = true;
    if (plugin_mode) {
#ifdef FSAPP_OS_MACOS
        // Need to go further up in the hierarchy due to being bundled inside
        // an application bundle.
        path = FS_JoinPath(base_path, "..", "..", "..", "..", "..", "Data", relative);
#else
        path = FS_JoinPath(base_path, "..", "..", "Data", relative);
#endif
        if (fsapp_data_path_exists_or_free(path)) {
            return path;
        }

#if 0
        path = FS_JoinPath(base_path, "share", relative);
        fsapp_data_log("Checking %s\n", path);
        if (fsapp_data_path_exists_or_free(path)) {
            return path;
        }
        path = FS_JoinPath(base_path, "..", "share", relative);
        fsapp_data_log("Checking %s\n", path);
        if (fsapp_data_path_exists_or_free(path)) {
            return path;
        }
#endif
    }

#if 0
    const char *user_dir = g_get_user_data_dir();
    path = FS_JoinPath(user_dir, relative);
    if (fsapp_data_path_exists_or_free(path)) {
        return path;
    }

    const char *const *dirs = g_get_system_data_dirs();
    while (*dirs) {
        path = FS_JoinPath(*dirs, relative);
        if (fsapp_data_path_exists_or_free(path)) {
            return path;
        }
        dirs++;
    }
#endif

    return NULL;
}

void fsapp_data_load(const char *name, void **data, int *data_size)
{
    fsapp_data_log("Load %s\n", name);
    *data = NULL;
    *data_size = 0;
    char *path = fsapp_data_find_path(name);
    if (!path) {
        fsapp_data_log_warning("Could not find data file '%s'\n", name);
        return;
    }
    fsapp_data_log("Path: %s\n", path);

    FILE *f = FS_OpenFile(path, "rb");
    SDL_free(path);
    if (!f) {
        fsapp_data_log("Could not open file!\n");
        return;
    }
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    void *result = malloc(file_size);
    /*
    void *p = *data;
    while(file_size > 0) {
        fread();
    }
    */
    if (fread(result, file_size, 1, f) != 1) {
        free(result);
        fclose(f);
        fsapp_data_log("Did not read expected data\n");
        return;
    }
    fclose(f);

    *data = result;
    *data_size = file_size;
}

fsapp_stream_t *fsapp_data_open_stream(const char *name)
{
    char *path = fsapp_data_find_path(name);
    if (!path) {
        fsapp_data_log("No path!\n");
        return fsapp_stream_create_null();
    }

    fsapp_stream_t *stream = fsapp_stream_create_from_path(path, "rw");
    SDL_free(path);
    return stream;
}
