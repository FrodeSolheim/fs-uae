/*
 * UAE - The Un*x Amiga Emulator
 *
 * MC68000 emulation - machine dependent bits
 *
 * Copyright 1996 Bernd Schmidt
 * Copyright 2004-2007 Richard Drummond
 */

#ifndef MACHDEP_M68K_H
#define MACHDEP_M68K_H

/*
 * TODO: Maybe use od-win32/machdep/m68k.h instead. Its x86 (SAHF_SETO_PROFITABLE)
 * and AArch64 flag layouts are the same as here, so layout changes would come
 * with upstream merges. It declares cctrue() out of line (needs
 * od-win32/machdep/m68k.cpp), GET_CZNV() is function-like, and USE_X86_FPUCW
 * would move to FS-UAE's sysconfig. Needs a JIT test.
 */

#include <stdlib.h>

#if defined(CPU_AARCH64)

/*
 * Machine dependent structure for holding the 68k CCR flags
 */
struct flag_struct {
	union {
		uae_u64 cznv;
		uae_u64 nzcv;
	};
	uae_u64 x;
};

extern struct flag_struct regflags;

/*
 * The ARM64 JIT stores the 68k CZNV flags in the host NZCV bit positions
 * (as in od-win32/machdep/m68k.h).
 */

#define FLAGBIT_N   31
#define FLAGBIT_Z   30
#define FLAGBIT_C   29
#define FLAGBIT_V   28
#define FLAGBIT_X   0

#define FLAGVAL_N   (1u << FLAGBIT_N)
#define FLAGVAL_Z   (1u << FLAGBIT_Z)
#define FLAGVAL_C   (1u << FLAGBIT_C)
#define FLAGVAL_V   (1u << FLAGBIT_V)
#define FLAGVAL_X   (1u << FLAGBIT_X)

#define SET_ZFLG(y) (regflags.nzcv = (regflags.nzcv & ~FLAGVAL_Z) | ((uae_u64)((y) ? 1 : 0) << FLAGBIT_Z))
#define SET_CFLG(y) (regflags.nzcv = (regflags.nzcv & ~FLAGVAL_C) | ((uae_u64)((y) ? 1 : 0) << FLAGBIT_C))
#define SET_VFLG(y) (regflags.nzcv = (regflags.nzcv & ~FLAGVAL_V) | ((uae_u64)((y) ? 1 : 0) << FLAGBIT_V))
#define SET_NFLG(y) (regflags.nzcv = (regflags.nzcv & ~FLAGVAL_N) | ((uae_u64)((y) ? 1 : 0) << FLAGBIT_N))
#define SET_XFLG(y) (regflags.x    = ((y) ? 1 : 0))

#define GET_ZFLG()  ((regflags.nzcv >> FLAGBIT_Z) & 1)
#define GET_CFLG()  ((regflags.nzcv >> FLAGBIT_C) & 1)
#define GET_VFLG()  ((regflags.nzcv >> FLAGBIT_V) & 1)
#define GET_NFLG()  ((regflags.nzcv >> FLAGBIT_N) & 1)
#define GET_XFLG()  (regflags.x & 1)

#define CLEAR_CZNV()    (regflags.nzcv  = 0)
#define GET_CZNV    ((uae_u32)regflags.nzcv)
#define IOR_CZNV(X) (regflags.nzcv |= (X))
#define SET_CZNV(X) (regflags.nzcv  = (X))

#define COPY_CARRY() (regflags.x = (regflags.nzcv >> FLAGBIT_C) & 1)

#else

/*
 * Machine dependent structure for holding the 68k CCR flags
 */
struct flag_struct {
	unsigned int cznv;
	unsigned int x;
};

extern struct flag_struct regflags;

/*
 * The bits in the cznv field in the above structure are assigned to
 * allow the easy mirroring of the x86 condition flags. (For example,
 * from the AX register - the x86 overflow flag can be copied to AL
 * with a setto %AL instr and the other flags copied to AH with an
 * lahf instr).
 *
 * The 68k CZNV flags are thus assigned in cznv as:
 *
 * <--AL-->  <--AH-->
 * 76543210  FEDCBA98 --------- ---------
 * xxxxxxxV  NZxxxxxC xxxxxxxxx xxxxxxxxx
 */

#define FLAGBIT_N   15
#define FLAGBIT_Z   14
#define FLAGBIT_C   8
#define FLAGBIT_V   0
#define FLAGBIT_X   0 /* The x86 JIT stores carry with SETcc. */

#define FLAGVAL_N   (1 << FLAGBIT_N)
#define FLAGVAL_Z   (1 << FLAGBIT_Z)
#define FLAGVAL_C   (1 << FLAGBIT_C)
#define FLAGVAL_V   (1 << FLAGBIT_V)
#define FLAGVAL_X   (1 << FLAGBIT_X)

#define SET_ZFLG(y) (regflags.cznv = (regflags.cznv & ~FLAGVAL_Z) | (((y) ? 1 : 0) << FLAGBIT_Z))
#define SET_CFLG(y) (regflags.cznv = (regflags.cznv & ~FLAGVAL_C) | (((y) ? 1 : 0) << FLAGBIT_C))
#define SET_VFLG(y) (regflags.cznv = (regflags.cznv & ~FLAGVAL_V) | (((y) ? 1 : 0) << FLAGBIT_V))
#define SET_NFLG(y) (regflags.cznv = (regflags.cznv & ~FLAGVAL_N) | (((y) ? 1 : 0) << FLAGBIT_N))
#define SET_XFLG(y) (regflags.x    = ((y) ? 1 : 0) << FLAGBIT_X)

#define GET_ZFLG()  ((regflags.cznv >> FLAGBIT_Z) & 1)
#define GET_CFLG()  ((regflags.cznv >> FLAGBIT_C) & 1)
#define GET_VFLG()  ((regflags.cznv >> FLAGBIT_V) & 1)
#define GET_NFLG()  ((regflags.cznv >> FLAGBIT_N) & 1)
#define GET_XFLG()  ((regflags.x    >> FLAGBIT_X) & 1)

#define CLEAR_CZNV()    (regflags.cznv  = 0)
#define GET_CZNV    (regflags.cznv)
#define IOR_CZNV(X) (regflags.cznv |= (X))
#define SET_CZNV(X) (regflags.cznv  = (X))

#define COPY_CARRY() (regflags.x = regflags.cznv >> (FLAGBIT_C - FLAGBIT_X))

#endif


/*
 * Test CCR condition
 */
STATIC_INLINE int cctrue (int cc)
{
    uae_u32 cznv = regflags.cznv;

    switch (cc) {
    case 0:  return 1;                              /*              T  */
    case 1:  return 0;                              /*              F  */
    case 2:  return (cznv & (FLAGVAL_C | FLAGVAL_Z)) == 0;              /* !CFLG && !ZFLG       HI */
    case 3:  return (cznv & (FLAGVAL_C | FLAGVAL_Z)) != 0;              /*  CFLG || ZFLG        LS */
    case 4:  return (cznv & FLAGVAL_C) == 0;                    /* !CFLG            CC */
    case 5:  return (cznv & FLAGVAL_C) != 0;                    /*  CFLG            CS */
    case 6:  return (cznv & FLAGVAL_Z) == 0;                    /* !ZFLG            NE */
    case 7:  return (cznv & FLAGVAL_Z) != 0;                    /*  ZFLG            EQ */
    case 8:  return (cznv & FLAGVAL_V) == 0;                    /* !VFLG            VC */
    case 9:  return (cznv & FLAGVAL_V) != 0;                    /*  VFLG            VS */
    case 10: return (cznv & FLAGVAL_N) == 0;                    /* !NFLG            PL */
    case 11: return (cznv & FLAGVAL_N) != 0;                    /*  NFLG            MI */
    case 12: return (((cznv << (FLAGBIT_N - FLAGBIT_V)) ^ cznv) & FLAGVAL_N) == 0;  /*  NFLG == VFLG        GE */
    case 13: return (((cznv << (FLAGBIT_N - FLAGBIT_V)) ^ cznv) & FLAGVAL_N) != 0;  /*  NFLG != VFLG        LT */
    case 14: cznv &= (FLAGVAL_N | FLAGVAL_Z | FLAGVAL_V);               /* ZFLG && (NFLG == VFLG)   GT */
         return (((cznv << (FLAGBIT_N - FLAGBIT_V)) ^ cznv) & (FLAGVAL_N | FLAGVAL_Z)) == 0;
    case 15: cznv &= (FLAGVAL_N | FLAGVAL_Z | FLAGVAL_V);               /* ZFLG && (NFLG != VFLG)   LE */
         return (((cznv << (FLAGBIT_N - FLAGBIT_V)) ^ cznv) & (FLAGVAL_N | FLAGVAL_Z)) != 0;
    }
    abort ();
    return 0;
}

#define USE_X86_FPUCW 1

#endif // MACHDEP_M68K_H
