#ifndef FS_UNICODE_H_
#define FS_UNICODE_H_

#include <fs/defines.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

char *fs_utf8_strdown(const char *str, ssize_t len) FS_MALLOC;

#ifdef __cplusplus
}
#endif

#endif // FS_UNICODE_H_
