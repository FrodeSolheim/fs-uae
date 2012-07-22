#ifndef LIBAMIGA_UAE_HOST_H_
#define LIBAMIGA_UAE_HOST_H_

#include <stdio.h>
#include <stdint.h>

#if 0
int uae_random();
#endif

int64_t uae_ftello64(FILE *stream);
int uae_fseeko64(FILE *stream, int64_t offset, int whence);

char *uae_expand_path(const char *path);
FILE *uae_fopen(const char *path, const char *mode);

#endif //  LIBAMIGA_UAE_HOST_H_
