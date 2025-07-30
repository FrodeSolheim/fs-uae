 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory access functions
  *
  * Copyright 1996 Bernd Schmidt
  */

#ifndef MACHDEP_X86_MACCESS_H
#define MACHDEP_X86_MACCESS_H

#ifdef FSUAE // NL
#include "uae/types.h"
#include "uae/inline.h"
#endif

#ifdef _WIN32
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#else
#include <byteswap.h>
#endif

STATIC_INLINE uae_u32 do_get_mem_long (uae_u32 *a)
{
    uae_u32 retval;

    __asm__ ("bswap %0" : "=r" (retval) : "0" (*a) : "cc");
    return retval;
}

STATIC_INLINE uae_u32 do_get_mem_word (uae_u16 *a)
{
    uae_u32 retval;

#ifdef X86_PPRO_OPT
    __asm__ ("movzwl %w1,%k0\n\tshll $16,%k0\n\tbswap %k0\n" : "=&r" (retval) : "m" (*a) : "cc");
#else
    __asm__ ("xorl %k0,%k0\n\tmovw %w1,%w0\n\trolw $8,%w0" : "=&r" (retval) : "m" (*a) : "cc");
#endif
    return retval;
}

#define do_get_mem_byte(a) ((uae_u32)*((uae_u8 *)a))

STATIC_INLINE void do_put_mem_quad(uae_u64 *a, uae_u64 v)
{
#ifdef _WIN32
	*a = _byteswap_uint64(v);
#elif defined(__APPLE__)
	*a = OSSwapInt64(v);
#else
	*a = bswap_64(v);
#endif
}

STATIC_INLINE void do_put_mem_long (uae_u32 *a, uae_u32 v)
{
    __asm__ ("bswap %0" : "=r" (v) : "0" (v) : "cc");
    *a = v;
}

STATIC_INLINE void do_put_mem_word (uae_u16 *a, uae_u32 v)
{
#ifdef X86_PPRO_OPT
    __asm__ ("bswap %0" : "=&r" (v) : "0" (v << 16) : "cc");
#else
    __asm__ ("rolw $8,%w0" : "=r" (v) : "0" (v) : "cc");
#endif
    *a = v;
}

#define do_put_mem_byte(a,v) (*(uae_u8 *)(a) = (v))

#define call_mem_get_func(func,addr) ((*func)(addr))
#define call_mem_put_func(func,addr,v) ((*func)(addr,v))

#undef NO_INLINE_MEMORY_ACCESS
#undef MD_HAVE_MEM_1_FUNCS

#ifdef MD_HAVE_MEM_1_FUNCS
STATIC_INLINE uae_u32 longget_1 (uae_cptr addr)
{
    uae_u32 result;

    __asm__ ("andl $0x00FFFFFF,%1\n"
         "\tcmpb $0,(%1,%3)\n"
         "\tleal 1f,%%ecx\n"
         "\tje longget_stub\n"
         "\taddl address_space,%1\n"
         "\tmovl (%1),%0\n"
         "\tbswap %0\n"
         "\t1:"
         : "=c" (result), "=d" (addr) : "1" (addr), "r" (good_address_map) : "cc");
    return result;
}
STATIC_INLINE uae_u32 wordget_1 (uae_cptr addr)
{
    uae_u32 result;

    __asm__ ("andl $0x00FFFFFF,%1\n"
         "\tcmpb $0,(%1,%3)\n"
         "\tleal 1f,%%ecx\n"
         "\tje wordget_stub\n"
         "\taddl address_space,%1\n"
         "\tmovzwl (%1),%0\n"
         "\trolw $8,%w0\n"
         "\t1:"
         : "=c" (result), "=d" (addr) : "1" (addr), "r" (good_address_map) : "cc");
    return result;
}
STATIC_INLINE uae_u32 byteget_1 (uae_cptr addr)
{
    uae_u32 result;

    __asm__ ("andl $0x00FFFFFF,%1\n"
         "\tcmpb $0,(%1,%3)\n"
         "\tleal 1f,%%ecx\n"
         "\tje byteget_stub\n"
         "\taddl address_space,%1\n"
         "\tmovzbl (%1),%0\n"
         "\t1:"
         : "=c" (result), "=d" (addr) : "1" (addr), "r" (good_address_map) : "cc");
    return result;
}
STATIC_INLINE void longput_1 (uae_cptr addr, uae_u32 l)
{
    __asm__ __volatile__("andl $0x00FFFFFF,%0\n"
         "\tcmpb $0,(%0,%3)\n"
         "\tleal 1f,%%ecx\n"
         "\tje longput_stub\n"
         "\taddl address_space,%0\n"
         "\tbswap %1\n"
         "\tmovl %1,(%0)\n"
         "\t1:"
         : "=d" (addr), "=b" (l) : "0" (addr), "r" (good_address_map), "1" (l) : "cc", "memory", "ecx");
}
STATIC_INLINE void wordput_1 (uae_cptr addr, uae_u32 w)
{
    __asm__ __volatile__("andl $0x00FFFFFF,%0\n"
         "\tcmpb $0,(%0,%3)\n"
         "\tleal 1f,%%ecx\n"
         "\tje wordput_stub\n"
         "\taddl address_space,%0\n"
         "\trolw $8,%1\n"
         "\tmovw %w1,(%0)\n"
         "\t1:"
         : "=d" (addr), "=b" (w) : "0" (addr), "r" (good_address_map), "1" (w) : "cc", "memory", "ecx");
}
STATIC_INLINE void byteput_1 (uae_cptr addr, uae_u32 b)
{
    __asm__ __volatile__("andl $0x00FFFFFF,%0\n"
         "\tcmpb $0,(%0,%3)\n"
         "\tleal 1f,%%ecx\n"
         "\tje byteput_stub\n"
         "\taddl address_space,%0\n"
         "\tmovb %b1,(%0)\n"
         "\t1:"
         : "=d" (addr), "=b" (b) : "0" (addr), "r" (good_address_map), "1" (b) : "cc", "memory", "ecx");
}

#endif

#define ALIGN_POINTER_TO32(p) ((~(uintptr_t)(p)) & 3)

/* Not the best place for this, but then there's no good place for a kludge
 * like this... */
#define HAVE_UAE_U24
typedef struct {
    unsigned char a, b, c;
} __attribute__ ((packed)) uae_u24;

STATIC_INLINE uae_u24 uae24_convert (uae_u32 v)
{
    return *(uae_u24 *)&v;
}

#endif // MACHDEP_X86_MACCESS_H
