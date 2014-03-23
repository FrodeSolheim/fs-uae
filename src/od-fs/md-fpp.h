 /*
  * UAE - The Un*x Amiga Emulator
  *
  * MC68881 emulation
  * Support functions for IEEE 754-compatible host CPUs.
  * These functions use a GCC extension (type punning through unions) and
  * should only be compiled with compilers that support this.
  *
  * Copyright 1999 Sam Jordan
  * Copyright 2007 Richard Drummond
  */

#define FPCR_ROUND_NEAR         0x00000000
#define FPCR_ROUND_ZERO         0x00000010
#define FPCR_ROUND_MINF         0x00000020
#define FPCR_ROUND_PINF         0x00000030

#ifndef HAVE_to_single
#define HAVE_to_single
STATIC_INLINE double to_single (uae_u32 value)
{
    union {
    float f;
    uae_u32 u;
    } val;

    val.u = value;
    return val.f;
}
#endif

#ifndef HAVE_from_single
#define HAVE_from_single
STATIC_INLINE uae_u32 from_single (double src)
{
    union {
    float f;
    uae_u32 u;
    } val;

    val.f = src;
    return val.u;
}
#endif

#ifndef HAVE_to_double
#define HAVE_to_double
STATIC_INLINE double to_double (uae_u32 wrd1, uae_u32 wrd2)
{
    union {
    double d;
    uae_u32 u[2];
    } val;

#ifdef WORDS_BIGENDIAN
    val.u[0] = wrd1;
    val.u[1] = wrd2;
#else
    val.u[1] = wrd1;
    val.u[0] = wrd2;
#endif
    return val.d;
}
#endif

#ifndef HAVE_from_double
#define HAVE_from_double
STATIC_INLINE void from_double (double src, uae_u32 * wrd1, uae_u32 * wrd2)
{
    union {
    double d;
    uae_u32 u[2];
    } val;

    val.d = src;
#ifdef WORDS_BIGENDIAN
    *wrd1 = val.u[0];
    *wrd2 = val.u[1];
#else
    *wrd1 = val.u[1];
    *wrd2 = val.u[0];
#endif
}
#endif

/* Get the rest of the conversion functions defined.  */
#include "fpp-unknown.h"

STATIC_INLINE void to_exten(fpdata *fpd, uae_u32 wrd1, uae_u32 wrd2, uae_u32 wrd3)
{
    // FIXME: hack to compile with WinUAE 2.7.1b8, must review fpp code
	fpd->fp = to_exten(wrd1, wrd2, wrd3);
#ifdef USE_SOFT_LONG_DOUBLE
    // FIXME
#endif
}

STATIC_INLINE void from_exten(fpdata *fpd, uae_u32 * wrd1, uae_u32 * wrd2, uae_u32 * wrd3)
{
#ifdef USE_SOFT_LONG_DOUBLE
	if (fpd->fpx) {
	    // FIXME
	} else
#endif
	{
        return from_exten(fpd->fp, wrd1, wrd2, wrd3);
	}
}
