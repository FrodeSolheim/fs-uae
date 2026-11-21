/*
 * Path utilities that complement SDL's filesystem API.
 *
 * This module has no dependencies on other FS modules.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FS_PATH_H
#define FS_PATH_H

#include <SDL3/SDL.h>

#if defined(__GNUC__) || defined(__clang__)
#define FS_PATH_SENTINEL __attribute__((sentinel))
#else
#define FS_PATH_SENTINEL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The directory separator that paths are made with: '\\' on Windows, '/' elsewhere. On Windows,
 * '/' is a separator too (see FS_IsPathSeparator()).
 */
#ifdef _WIN32
#define FS_PATH_SEPARATOR '\\'
#else
#define FS_PATH_SEPARATOR '/'
#endif

/**
 * Check whether a character is a directory separator.
 *
 * \param c the character to check.
 * \returns true for '/', and on Windows also for '\\'; false otherwise.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
bool FS_IsPathSeparator(char c);

/**
 * Join path elements with one separator between them.
 *
 * This is a macro that adds the NULL that ends the list of elements for
 * FS_JoinPathNullTerminated(): `FS_JoinPath(directory, "Plugins", name)`. Call it rather than
 * FS_JoinPathNullTerminated() with a plain NULL, which is an integer and not a pointer in C++ on
 * some systems (MinGW).
 *
 * As g_build_filename() does, it skips empty elements and makes the separators where two elements
 * meet into one, while the separators at the start of the first element and at the end of the
 * last are kept. A separator at the end of an element is reused ('/' or '\\' on Windows);
 * otherwise FS_PATH_SEPARATOR is added. A root (`/`, `C:\`) keeps its separator. The elements are
 * joined as text: `.` and `..` are kept, and nothing is looked up on disk.
 *
 * Free the returned string with SDL_free() when it is no longer needed.
 *
 * \returns a newly allocated path in UTF-8 (empty when every element is empty) on success, or NULL
 *          on failure; call SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
#define FS_JoinPath(...) FS_JoinPathNullTerminated(__VA_ARGS__, (const char *)NULL)

/**
 * Join path elements, the list ending with NULL. Call FS_JoinPath() instead, which adds the NULL.
 *
 * \param first the first element, in UTF-8.
 * \returns a newly allocated path, as FS_JoinPath() returns it.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
char *FS_JoinPathNullTerminated(const char *first, ...) FS_PATH_SENTINEL;

/**
 * Get the directory that contains a path.
 *
 * Separators at the end of the path are ignored, and the parent has no separator at its end,
 * except a root (`/`, `C:\`). The path is taken as text: nothing is looked up on disk, and a path
 * that ends with `..` has the path without it as its parent.
 *
 * Free the returned string with SDL_free() when it is no longer needed.
 *
 * \param path the path, in UTF-8.
 * \returns a newly allocated path in UTF-8, empty when the path is a root or has no directory part,
 *          or NULL on failure; call SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
char *FS_GetParentPath(const char *path);

/**
 * Check whether a path is a directory, following symbolic links.
 *
 * To check whether a path exists at all, call `SDL_GetPathInfo(path, NULL)`.
 *
 * \param path the path, in UTF-8.
 * \returns true if the path is a directory, false if it is something else, does not exist or
 *          cannot be checked. SDL's error is set when SDL_GetPathInfo() fails (the path does not
 *          exist, for example).
 *
 * \threadsafety It is safe to call this function from any thread.
 */
bool FS_IsDirectory(const char *path);

/**
 * Check whether a path is a regular file, following symbolic links.
 *
 * \param path the path, in UTF-8.
 * \returns true if the path is a regular file, false if it is something else (a directory, a
 *          device), does not exist or cannot be checked. SDL's error is set when SDL_GetPathInfo()
 *          fails.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
bool FS_IsFile(const char *path);

/**
 * Convert a path to an absolute path.
 *
 * Absolute paths are copied unchanged. Ordinary relative paths are joined to the current working
 * directory reported by SDL_GetCurrentDirectory(), with `.` and `..` components preserved. The path
 * need not exist.
 *
 * On Windows, both slash and backslash are recognized as separators. Fully qualified paths such as
 * `C:\foo` and `\\server\share\foo` are copied unchanged. Paths such as `\foo` (rooted on the
 * current drive) and `C:foo` (relative to drive C's current directory) are resolved by
 * GetFullPathNameW(), which collapses `.` and `..` components.
 *
 * Free the returned string with SDL_free() when it is no longer needed.
 *
 * \param path the path to convert, in UTF-8.
 * \returns a newly allocated absolute path in UTF-8 on success or NULL on failure; call
 *          SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread. The working directory belongs to
 *               the process, so if another thread changes it, the same relative path can resolve
 *               differently.
 */
char *FS_GetAbsolutePath(const char *path);

#ifdef _WIN32
/**
 * Convert a path in UTF-8 to UTF-16, for the wide (`W`) functions of Windows. Only on Windows.
 *
 * Text that is not valid UTF-8 is an error, not replaced, so that a path is never changed.
 *
 * Free the returned string with SDL_free() when it is no longer needed.
 *
 * \param path the path, in UTF-8.
 * \returns a newly allocated path in UTF-16 on success or NULL on failure; call SDL_GetError() for
 *          more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
wchar_t *FS_ConvertPathToUTF16(const char *path);

/**
 * Convert a path in UTF-16, from the wide (`W`) functions of Windows, to UTF-8. Only on Windows.
 *
 * Text that is not valid UTF-16 (an unpaired surrogate) is an error, not replaced, so that a path
 * is never changed.
 *
 * Free the returned string with SDL_free() when it is no longer needed.
 *
 * \param path the path, in UTF-16.
 * \returns a newly allocated path in UTF-8 on success or NULL on failure; call SDL_GetError() for
 *          more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 */
char *FS_ConvertPathToUTF8(const wchar_t *path);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FS_PATH_H */
