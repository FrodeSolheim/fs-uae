/*
 * Files that SDL's I/O streams do not cover. See file.h.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fs/file.h"

#include "fs/path.h"

#include <SDL3/SDL.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
// The size of the longest mode FS_OpenFile takes, with the terminating zero ("rt, ccs=UTF-16LE"
// needs 17).
#define FILE_MODE_SIZE 32

// The mode (ASCII) as UTF-16 in the buffer, or false with SDL's error set.
static bool ConvertModeToUTF16(const char *mode, wchar_t *buffer)
{
    for (int i = 0; i < FILE_MODE_SIZE; i++) {
        unsigned char c = (unsigned char)mode[i];
        if (c >= 0x80) {
            return SDL_SetError("The file mode %s is not ASCII", mode);
        }
        buffer[i] = c;
        if (c == '\0') {
            return true;
        }
    }
    return SDL_SetError("The file mode %s is too long", mode);
}
#endif

FILE *FS_OpenFile(const char *path, const char *mode)
{
#ifdef _WIN32
    wchar_t wide_mode[FILE_MODE_SIZE];
    if (!ConvertModeToUTF16(mode, wide_mode)) {
        errno = EINVAL;
        return NULL;
    }
    wchar_t *wide_path = FS_ConvertPathToUTF16(path);
    if (wide_path == NULL) {
        errno = EINVAL;
        return NULL;
    }
    FILE *f = _wfopen(wide_path, wide_mode);
    SDL_free(wide_path);
#else
    FILE *f = fopen(path, mode);
#endif
    if (f == NULL) {
        // Setting SDL's error may change errno, which the caller may read.
        int error = errno;
        SDL_SetError("Could not open %s: %s", path, strerror(error));
        errno = error;
    }
    return f;
}
