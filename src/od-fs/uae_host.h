#ifndef _UAE_OD_FS_UAE_HOST_H_
#define _UAE_OD_FS_UAE_HOST_H_

#include <stdio.h>
#include <stdint.h>

int64_t uae_ftello64(FILE *stream);
int uae_fseeko64(FILE *stream, int64_t offset, int whence);

char *uae_expand_path(const char *path);
FILE *uae_fopen(const char *path, const char *mode);

#endif // _UAE_OD_FS_UAE_HOST_H_
