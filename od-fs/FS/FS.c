#include "FS/FS.h"

void FS_AppendDirNameOrFileName(char* dst, const char* src, bool dir)
{
    int len = strlen(dst);
#ifdef _WIN32
    const char* delim = "\\";
#else
    const char* delim = "/";
#endif
    if (!isdelim(dst[len - 1])) {
        SDL_strlcat(dst, delim, FS_MAX_PATH);
    }
    SDL_strlcat(dst, src, FS_MAX_PATH);
    if (dir) {
        SDL_strlcat(dst, delim, FS_MAX_PATH);
    }
}

bool FS_ParentDir(char* path)
{
    // char* r = path;
    char* p = path;
    int len = 0;
    // while (*++r) p = r;
    while (*(p++))
        len++;

    // First look at trailing slashes (and remove it/them)
    while (len > 0 && isdelim(path[len - 1])) {
        if (len == 1) {
            // Unix root directory
            return false;
        }
#if 1 // def _WIN32
        if (len == 3 && path[1] == ':') {
            // Windows drive root (e.g., "C:\")
            return false;
        }
#endif
        len--;
    }
    // Then remove the rightmost part of the path
    while (len > 0 && !isdelim(path[len - 1])) {
        len--;
    }
    SDL_assert(len >= 0);
    path[len] = '\0';
    return true;
}

static const char* application_dir = NULL;

const char* FS_GetApplicationDir(void)
{
    if (application_dir == NULL) {
        char path[FS_MAX_PATH];
        FS_CopyPath(path, SDL_GetBasePath());
#ifdef __DARWIN__
        SDL_Log("Check if \"%s\" is inside an .app bundle", path);
        // strlen("/Contents/Resources/") = 20
        int len = strlen(path);
        if (len > 20 && strcmp(path + len - 20, "/Contents/Resources/" == 0)) {
            FS_ParentDir(path);
            FS_ParentDir(path);
            FS_ParentDir(path);
        }
#endif
        SDL_Log("application_dir = %s", path);
        application_dir = strdup(path);
    }
    return application_dir;
}