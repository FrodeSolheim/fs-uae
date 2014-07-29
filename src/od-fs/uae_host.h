#ifndef UAE_OD_FS_UAE_HOST_H
#define UAE_OD_FS_UAE_HOST_H

#include <stdio.h>
#include <stdint.h>

int64_t uae_ftello64(FILE *stream);
int uae_fseeko64(FILE *stream, int64_t offset, int whence);

char *uae_expand_path(const char *path);
FILE *uae_fopen(const char *path, const char *mode);

#endif // UAE_OD_FS_UAE_HOST_H
