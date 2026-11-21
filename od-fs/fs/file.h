/*
 * Files that SDL's I/O streams do not cover.
 *
 * This module uses fs/path (on Windows).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FS_FILE_H
#define FS_FILE_H

#include <SDL3/SDL.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Open a file as a C stream, with a path in UTF-8 on every platform.
 *
 * The counterpart of fopen() for code that needs a `FILE *`; SDL_IOFromFile() takes a path in
 * UTF-8 too, but gives an SDL_IOStream. On Windows, the path and the mode are converted to UTF-16
 * and the file is opened with _wfopen(), since fopen() reads the path in the ANSI code page there.
 * Elsewhere it is fopen().
 *
 * \param path the path of the file, in UTF-8.
 * \param mode the mode, as for fopen() (in ASCII).
 * \returns the stream, to be closed with fclose(), on success or NULL on failure; call
 *          SDL_GetError() for more information. errno is set as by fopen(), or to EINVAL when the
 *          path or the mode cannot be converted.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
FILE *FS_OpenFile(const char *path, const char *mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FS_FILE_H */
