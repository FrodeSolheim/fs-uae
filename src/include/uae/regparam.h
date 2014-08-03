#ifndef UAE_REGPARAM_H
#define UAE_REGPARAM_H

#ifdef __i386__

#ifdef _WIN32

#if defined (_MSC_VER)

#define REGPARAM
#define REGPARAM2 __fastcall
#define REGPARAM3 __fastcall

#elif defined(HAVE_FUNC_ATTRIBUTE_FASTCALL)

#define REGPARAM
#define REGPARAM2 __attribute__((fastcall))
#define REGPARAM3 __attribute__((fastcall))

#endif

#else // _WIN32 not defined

#if defined(HAVE_FUNC_ATTRIBUTE_REGPARM)

#define REGPARAM
#define REGPARAM2 __attribute__((regparm(3)))
#define REGPARAM3 __attribute__((regparm(3)))

#endif

#endif // _WIN32

#else // __i386__ not defined

#define REGPARAM
#define REGPARAM2
#define REGPARAM3

#endif // __i386__

#ifndef REGPARAM
#error REGPARAM is not defined
#endif

#ifndef REGPARAM2
#error REGPARAM2 is not defined
#endif

#ifndef REGPARAM3
#error REGPARAM3 is not defined
#endif

#endif // UAE_REGPARAM_H
