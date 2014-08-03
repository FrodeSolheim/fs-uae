#ifndef UAE_TYPES_H
#define UAE_TYPES_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

/* Define uae_ integer types. Prefer long long int for uae_x64 since we can
 * then use the %lld format specifier for both 32-bit and 64-bit instead of
 * the ugly PRIx64 macros. */

#ifdef HAVE_STDINT_H

#include <stdint.h>

typedef int8_t uae_s8;
typedef uint8_t uae_u8;

typedef int16_t uae_s16;
typedef uint16_t uae_u16;

typedef int32_t uae_s32;
typedef uint32_t uae_u32;

#if SIZEOF_LONG_LONG_INT == 8
typedef long long int uae_s64;
typedef unsigned long long int uae_u64;
#else
typedef int64_t uae_s64;
typedef uint64_t uae_u64;
#endif

#else

#error uae integer types not defined

#endif

typedef uae_u32 uaecptr;

typedef char uae_char;

typedef char TCHAR;

// FIXME: remove these (currently needed by bsdsocket)
typedef unsigned int WPARAM;
typedef long LPARAM;
typedef int BOOL;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif // UAE_TYPES_H
