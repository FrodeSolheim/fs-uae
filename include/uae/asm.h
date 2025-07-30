#ifndef UAE_ASM_H
#define UAE_ASM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef X86_ASSEMBLY
#define ASM_SYM_FOR_FUNC(a) __asm__(a)
#else
#define ASM_SYM_FOR_FUNC(a)
#endif

#endif /* UAE_ASM_H */
