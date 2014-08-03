#ifndef UAE_INLINE_H
#define UAE_INLINE_H

// FIXME: could be named funcattr.h or something, perhaps
// FIXME: move regparams here?

#if defined(HAVE_FUNC_ATTRIBUTE_ALWAYS_INLINE)
#define STATIC_INLINE static __inline__ __attribute__ ((always_inline))
#elif defined(_MSC_VER)
#define STATIC_INLINE static __forceinline
#else
#define STATIC_INLINE static inline
#endif

#if defined(HAVE_FUNC_ATTRIBUTE_NOINLINE)
#define NOINLINE __attribute__ ((noinline))
#elif defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE
#endif

#if defined(HAVE_FUNC_ATTRIBUTE_NORETURN)
#define NORETURN __attribute__ ((noreturn))
#elif defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

#endif // UAE_INLINE_H
