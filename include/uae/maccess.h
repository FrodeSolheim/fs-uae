 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory access functions (for all machdeps)
  *
  * Copyright 1996 Bernd Schmidt
  * Copyright 2019 Frode Solheim
  */

#ifndef UAE_MACCESS_H
#define UAE_MACCESS_H

#include "uae/byteswap.h"

#define ALIGN_POINTER_TO32(p) ((~(uintptr_t)(p)) & 3)

static inline uae_u64 do_get_mem_quad(uae_u64 *a)
{
#ifdef WORDS_BIGENDIAN
	return *a;
#else
	return uae_bswap64(*a);
#endif
}

static inline uae_u32 do_get_mem_long(uae_u32 *a)
{
#ifdef WORDS_BIGENDIAN
	return *a;
#else
	return uae_bswap32(*a);
#endif
}

static inline uae_u16 do_get_mem_word(uae_u16 *a)
{
#ifdef WORDS_BIGENDIAN
	return *a;
#else
	return uae_bswap16(*a);
#endif
}

#define do_get_mem_byte(a) ((uae_u32)*(uae_u8 *)(a))

static inline void do_put_mem_quad(uae_u64 *a, uae_u64 v)
{
#ifdef WORDS_BIGENDIAN
	*a = v;
#else
	*a = uae_bswap64(v);
#endif
}

static inline void do_put_mem_long(uae_u32 *a, uae_u32 v)
{
#ifdef WORDS_BIGENDIAN
	*a = v;
#else
	*a = uae_bswap32(v);
#endif
}

static inline void do_put_mem_word(uae_u16 *a, uae_u16 v)
{
#ifdef WORDS_BIGENDIAN
	*a = v;
#else
	*a = uae_bswap16(v);
#endif
}

static inline void do_put_mem_byte(uae_u8 *a, uae_u8 v)
{
	*a = v;
}

#define call_mem_get_func(func, addr) ((*func)(addr))
#define call_mem_put_func(func, addr, v) ((*func)(addr, v))

static inline uae_u64 do_byteswap_64(uae_u64 v)
{
	return uae_bswap64(v);
}

static inline uae_u32 do_byteswap_32(uae_u32 v)
{
	return uae_bswap32(v);
}

static inline uae_u16 do_byteswap_16(uae_u16 v)
{
	return uae_bswap16(v);
}

#endif /* UAE_MACCESS_H */
