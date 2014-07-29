#ifndef UAE_JITCONFIG_H
#define UAE_JITCONFIG_H

// #define JIT_DEBUG

#if defined(__i386__)
// set up correct REGPARAMS for JIT
// using __fastcall / __attribute__((fastcall)) on non-Windows as well
#ifdef WINDOWS
#define REGPARAM
#define REGPARAM2 __attribute__((fastcall))
#define REGPARAM3 __attribute__((fastcall))
#else
#define REGPARAM
#define REGPARAM2 __attribute__((regparm(3)))
#define REGPARAM3 __attribute__((regparm(3)))
#endif
#else
#define REGPARAM
#define REGPARAM2
#define REGPARAM3
#endif

#endif // UAE_JITCONFIG_H
