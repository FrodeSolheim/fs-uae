/*
 * Path utilities that complement SDL's filesystem API. See path.h.
 *
 * This module has no dependencies on other FS modules.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fs/path.h"

#include <SDL3/SDL.h>
#include <stdarg.h>

#include <string>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#endif

bool FS_IsPathSeparator(char c)
{
#ifdef _WIN32
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}

// The length of the root at the start of the path (`/`, `C:\` or `C:/`; also `//` and, on Windows,
// `\\`, the start of a UNC path), or 0 if it has none.
static size_t GetRootLength(std::string_view path)
{
#ifdef _WIN32
    if (path.size() >= 3 && path[1] == ':' && FS_IsPathSeparator(path[2])) {
        return 3;
    }
#endif
    size_t length = 0;
    while (length < path.size() && FS_IsPathSeparator(path[length])) {
        length++;
    }
    return length;
}

// A copy of the text (a new string), or NULL with SDL's error set.
static char *CopyString(std::string_view text)
{
    char *result = SDL_strndup(text.data(), text.size());
    if (result == NULL) {
        SDL_OutOfMemory();
    }
    return result;
}

char *FS_JoinPathNullTerminated(const char *first, ...)
{
    std::string result;
    va_list ap;
    va_start(ap, first);
    for (const char *name = first; name != NULL; name = va_arg(ap, const char *)) {
        if (name[0] == '\0') {
            continue;
        }
        if (result.empty()) {
            result = name;
            continue;
        }
        // One separator where the elements meet (but not into the root of the first one), reusing
        // a separator that was there ('/' or '\\' on Windows).
        char separator = FS_PATH_SEPARATOR;
        size_t root = GetRootLength(result);
        while (result.size() > root && FS_IsPathSeparator(result.back())) {
            separator = result.back();
            result.pop_back();
        }
        while (FS_IsPathSeparator(name[0])) {
            name++;
        }
        if (!FS_IsPathSeparator(result.back())) {
            result += separator;
        }
        result += name;
    }
    va_end(ap);
    return CopyString(result);
}

char *FS_GetParentPath(const char *path_text)
{
    std::string_view path(path_text);
    size_t root = GetRootLength(path);
    size_t end = path.size();
    // Trailing separators, then the last name, then the separators before it.
    while (end > root && FS_IsPathSeparator(path[end - 1])) {
        end--;
    }
    if (end == root) {
        // A root (or an empty path) has no parent.
        return CopyString("");
    }
    while (end > root && !FS_IsPathSeparator(path[end - 1])) {
        end--;
    }
    while (end > root && FS_IsPathSeparator(path[end - 1])) {
        end--;
    }
    return CopyString(path.substr(0, end));
}

bool FS_IsDirectory(const char *path)
{
    SDL_PathInfo info;
    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY;
}

bool FS_IsFile(const char *path)
{
    SDL_PathInfo info;
    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE;
}

#ifdef _WIN32
// Whether the path starts with a drive letter and a colon.
static bool HasDrivePrefix(const char *path)
{
    return SDL_isalpha((unsigned char)path[0]) && path[1] == ':';
}
#endif

// Whether the path is absolute: on Windows, fully qualified (C:\foo, or \\server\share\foo and
// other paths that start with two separators).
static bool IsAbsolutePath(const char *path)
{
#ifdef _WIN32
    if (HasDrivePrefix(path)) {
        return FS_IsPathSeparator(path[2]);
    }
    return FS_IsPathSeparator(path[0]) && FS_IsPathSeparator(path[1]);
#else
    return FS_IsPathSeparator(path[0]);
#endif
}

#ifdef _WIN32
// How often GetFullPathNameW is asked again when the working directory changes between the call
// that gets the size and the one that writes the path (another thread changed it), and the path
// no longer fits.
#define PATH_FULL_PATH_TRIES 4

wchar_t *FS_ConvertPathToUTF16(const char *path)
{
    int size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, NULL, 0);
    if (size <= 0) {
        SDL_SetError("Could not convert %s to UTF-16 (error %lu)", path, GetLastError());
        return NULL;
    }
    wchar_t *result = (wchar_t *)SDL_malloc(size * sizeof(wchar_t));
    if (result == NULL) {
        SDL_OutOfMemory();
        return NULL;
    }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, result, size) != size) {
        SDL_SetError("Could not convert %s to UTF-16 (error %lu)", path, GetLastError());
        SDL_free(result);
        return NULL;
    }
    return result;
}

char *FS_ConvertPathToUTF8(const wchar_t *path)
{
    int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, path, -1, NULL, 0, NULL, NULL);
    if (size <= 0) {
        SDL_SetError("Could not convert a path to UTF-8 (error %lu)", GetLastError());
        return NULL;
    }
    char *result = (char *)SDL_malloc(size);
    if (result == NULL) {
        SDL_OutOfMemory();
        return NULL;
    }
    if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, path, -1, result, size, NULL, NULL) !=
        size) {
        SDL_SetError("Could not convert a path to UTF-8 (error %lu)", GetLastError());
        SDL_free(result);
        return NULL;
    }
    return result;
}

// The full path (a new string) that GetFullPathNameW makes of the path, or NULL with SDL's error
// set.
static wchar_t *GetFullPathNameUTF16(const wchar_t *path, const char *path_utf8)
{
    // The size with the terminating zero.
    DWORD size = GetFullPathNameW(path, 0, NULL, NULL);
    for (int i = 0; i < PATH_FULL_PATH_TRIES; i++) {
        if (size == 0) {
            SDL_SetError("GetFullPathNameW failed for %s (error %lu)", path_utf8, GetLastError());
            return NULL;
        }
        wchar_t *full = (wchar_t *)SDL_malloc(size * sizeof(wchar_t));
        if (full == NULL) {
            SDL_OutOfMemory();
            return NULL;
        }
        // The length without the terminating zero when it fits, otherwise the size it needs (or
        // 0 when it fails).
        DWORD length = GetFullPathNameW(path, size, full, NULL);
        if (length == 0) {
            SDL_SetError("GetFullPathNameW failed for %s (error %lu)", path_utf8, GetLastError());
            SDL_free(full);
            return NULL;
        }
        if (length < size) {
            return full;
        }
        SDL_free(full);
        size = length;
    }
    SDL_SetError("GetFullPathNameW failed for %s: the working directory kept changing", path_utf8);
    return NULL;
}

// A path that starts with one separator (\foo, on the current drive) or with a drive prefix but no
// separator (C:foo, in drive C's current directory), resolved by GetFullPathNameW. Returns a new
// string, or NULL with SDL's error set.
static char *GetFullPathNameUTF8(const char *path)
{
    wchar_t *wide = FS_ConvertPathToUTF16(path);
    if (wide == NULL) {
        return NULL;
    }
    wchar_t *full = GetFullPathNameUTF16(wide, path);
    SDL_free(wide);
    if (full == NULL) {
        return NULL;
    }
    char *result = FS_ConvertPathToUTF8(full);
    SDL_free(full);
    return result;
}
#endif // _WIN32

char *FS_GetAbsolutePath(const char *path)
{
    if (IsAbsolutePath(path)) {
        char *result = SDL_strdup(path);
        if (result == NULL) {
            SDL_OutOfMemory();
        }
        return result;
    }
#ifdef _WIN32
    if (FS_IsPathSeparator(path[0]) || HasDrivePrefix(path)) {
        return GetFullPathNameUTF8(path);
    }
#endif
    // Sets SDL's error when it fails.
    char *directory = SDL_GetCurrentDirectory();
    if (directory == NULL) {
        return NULL;
    }
    // SDL's current directory ends with a path separator.
    char *result = NULL;
    if (SDL_asprintf(&result, "%s%s", directory, path) < 0) {
        SDL_OutOfMemory();
        result = NULL;
    }
    SDL_free(directory);
    return result;
}
