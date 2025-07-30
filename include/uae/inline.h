#ifndef UAE_INLINE_H_
#define UAE_INLINE_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// FIXME: could be named funcattr.h or something, perhaps
// FIXME: move regparams here?

#if 1

// It turns out (by benchmarking) that FS-UAE runs the same or slightly faster
// when the compiler decides whether to inline or not. The biggest difference
// is on ARM architectures (tested on Raspberry Pi 4), where using the simple
// `static inline` gave an additinal frame per second.
#define UAE_STATIC_INLINE static inline

#else

#if defined(HAVE_FUNC_ATTRIBUTE_ALWAYS_INLINE)
#define UAE_STATIC_INLINE static __inline__ __attribute__ ((always_inline))
#elif defined(_MSC_VER)
#define UAE_STATIC_INLINE static __forceinline
#else
#define UAE_STATIC_INLINE static inline
#endif

#endif

#if defined(HAVE_FUNC_ATTRIBUTE_NOINLINE)
#define UAE_NOINLINE __attribute__ ((noinline))
#elif defined(_MSC_VER)
#define UAE_NOINLINE __declspec(noinline)
#else
#define UAE_NOINLINE
#endif

#if defined(HAVE_FUNC_ATTRIBUTE_NORETURN)
#define UAE_NORETURN __attribute__ ((noreturn))
#elif defined(_MSC_VER)
#define UAE_NORETURN __declspec(noreturn)
#else
#define UAE_NORETURN
#endif

#ifndef STATIC_INLINE
#define STATIC_INLINE UAE_STATIC_INLINE
#endif

#ifndef NOINLINE
#define NOINLINE UAE_NOINLINE
#endif

#ifndef NORETURN
#define NORETURN UAE_NORETURN
#endif

#endif  // UAE_INLINE_H_
