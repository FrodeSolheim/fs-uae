#ifndef UAE_STRING_H
#define UAE_STRING_H

#include "uae/tchar.h"
#include "uae/types.h"

#include <string.h>

#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>

#else

#define _strtoui64 strtoll
#define _vsnprintf vsnprintf

#endif

static inline size_t uae_tcslcpy(TCHAR *dst, const TCHAR *src, size_t size)
{
	if (size == 0) {
		return 0;
	}
	size_t src_len = _tcslen(src);
	size_t cpy_len = src_len;
	if (cpy_len >= size) {
		cpy_len = size - 1;
	}
	memcpy(dst, src, cpy_len * sizeof(TCHAR));
	dst[cpy_len] = _T('\0');
	return src_len;
}

static inline size_t uae_strlcpy(char *dst, const char *src, size_t size)
{
	if (size == 0) {
		return 0;
	}
	size_t src_len = strlen(src);
	size_t cpy_len = src_len;
	if (cpy_len >= size) {
		cpy_len = size - 1;
	}
	memcpy(dst, src, cpy_len);
	dst[cpy_len] = '\0';
	return src_len;
}

#ifdef FSUAE

static inline int uaestrlen(const char* s)
{
	return (int) strlen(s);
}

static inline int uaetcslen(const TCHAR* s)
{
	return (int) _tcslen(s);
}


#endif

#endif /* UAE_STRING_H */
