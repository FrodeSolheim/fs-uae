#ifndef UAE_IO_H
#define UAE_IO_H

#include "uae/tchar.h"

#include <stdio.h>
#include <stdint.h>

#ifdef WINUAE
#define uae_tfopen _tfopen
#else
FILE *uae_tfopen(const TCHAR *path, const TCHAR *mode);
#endif

int64_t uae_ftello64(FILE *stream);
int uae_fseeko64(FILE *stream, int64_t offset, int whence);

#endif /* UAE_IO_H */
