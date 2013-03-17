#ifndef FS_UNICODE_H_
#define FS_UNICODE_H_

#include <fs/defines.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

char *fs_utf8_strdown(const char *str, ssize_t len) FS_MALLOC;
char *fs_utf8_strup(const char *str, ssize_t len) FS_MALLOC;

char *fs_utf8_to_latin1(const char *src, int len);
char *fs_utf8_from_latin1(const char *src, int len);

#ifdef __cplusplus
}
#endif

#endif // FS_UNICODE_H_
