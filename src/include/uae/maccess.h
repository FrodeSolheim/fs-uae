 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory access functions (for all machdeps)
  *
  * Copyright 1996 Bernd Schmidt
  * Copyright 2019 Frode Solheim
  */

#ifndef UAE_MACCESS_H_
#define UAE_MACCESS_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FSUAE // NL
#include "uae/types.h"
#include "uae/inline.h"
#endif

#ifdef _WIN32
#include <stdlib.h>
#define uae_bswap_16 _byteswap_uint16
#define uae_bswap_32 _byteswap_uint32
#define uae_bswap_64 _byteswap_uint64
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define uae_bswap_16 OSSwapInt16
#define uae_bswap_32 OSSwapInt32
#define uae_bswap_64 OSSwapInt64
#else
#include <byteswap.h>
#define uae_bswap_16 bswap_16
#define uae_bswap_32 bswap_32
#define uae_bswap_64 bswap_64
#endif

// Use builtin byteswap functions where possible. In many cases, the compiler
// may use optimized byteswap builtins anyway, but better to not risk using
// slower function calls.

#ifdef HAVE___BUILTIN_BSWAP16
#undef uae_bswap_16
#define uae_bswap_16 __builtin_bswap16
#endif
#ifdef HAVE___BUILTIN_BSWAP32
#undef uae_bswap_32
#define uae_bswap_32 __builtin_bswap32
#endif
#ifdef HAVE___BUILTIN_BSWAP64
#undef uae_bswap_64
#define uae_bswap_64 __builtin_bswap64
#endif

#define ALIGN_POINTER_TO32(p) ((~(uintptr_t)(p)) & 3)

static inline uae_u64 do_get_mem_quad(uae_u64 *a)
{
#ifdef WORDS_BIGENDIAN
	return *a;
#else
	return uae_bswap_64(*a);
#endif
}

static inline uae_u32 do_get_mem_long(uae_u32 *a)
{
#ifdef WORDS_BIGENDIAN
	return *a;
#else
	return uae_bswap_32(*a);
#endif
}

static inline uae_u16 do_get_mem_word(uae_u16 *a)
{
#ifdef WORDS_BIGENDIAN
	return *a;
#else
	return uae_bswap_16(*a);
#endif
}

#define do_get_mem_byte(a) ((uae_u32)*(uae_u8 *)(a))

static inline void do_put_mem_quad(uae_u64 *a, uae_u64 v)
{
#ifdef WORDS_BIGENDIAN
	*a = v;
#else
	*a = uae_bswap_64(v);
#endif
}

static inline void do_put_mem_long(uae_u32 *a, uae_u32 v)
{
#ifdef WORDS_BIGENDIAN
	*a = v;
#else
	*a = uae_bswap_32(v);
#endif
}

static inline void do_put_mem_word(uae_u16 *a, uae_u16 v)
{
#ifdef WORDS_BIGENDIAN
	*a = v;
#else
	*a = uae_bswap_16(v);
#endif
}

static inline void do_put_mem_byte(uae_u8 *a, uae_u8 v)
{
	*a = v;
}

#define call_mem_get_func(func, addr) ((*func)(addr))
#define call_mem_put_func(func, addr, v) ((*func)(addr, v))

#endif  // UAE_MACCESS_H_
