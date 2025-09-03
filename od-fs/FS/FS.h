#ifndef FS_FS_H
#define FS_FS_H

#include <SDL3/SDL.h>

#ifdef _WIN32

#include <windows.h>
#define FS_MAX_PATH MAX_PATH

#else

#ifdef __linux__
// Not really needed, but Visual Studio Code does not seem to see PATH_MAX without it...
#include <linux/limits.h>
#endif
#include <limits.h>
#define FS_MAX_PATH PATH_MAX

#endif

static inline void FS_CopyPath(char* dst, const char* src)
{
    SDL_utf8strlcpy(dst, src, FS_MAX_PATH);
}

static inline bool isdelim(char c)
{
#if 1 // #ifdef _WIN32
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}

/**
 * @brief FS_AppendDirName appends a name and a trailing directory slash
 */
void FS_AppendDirNameOrFileName(char* dst, const char* src, bool dir);

static inline void FS_AppendFileName(char* dst, const char* src)
{
    return FS_AppendDirNameOrFileName(dst, src, false);
}

static inline void FS_AppendDirName(char* dst, const char* src)
{
    return FS_AppendDirNameOrFileName(dst, src, true);
}

/**
 * Appends without considering directory slashes.
 */
static inline void FS_AppendPath(char* dst, const char* src)
{
    SDL_strlcat(dst, src, FS_MAX_PATH);
}

bool FS_ParentDir(char* path);

static inline bool FS_PathExists(const char* path)
{
    return SDL_GetPathInfo(path, NULL);
}

const char *FS_GetApplicationDir(void);

#endif // FS_FS_H
