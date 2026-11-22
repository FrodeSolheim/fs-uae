#ifndef UAE_ATOMIC_H
#define UAE_ATOMIC_H

#include "uae/types.h"

typedef int uae_atomic;

// Return values as WinUAE's _Interlocked* functions: and/or return the old
// value, inc/dec the new value.

static inline uae_atomic atomic_and(volatile uae_atomic *p, uae_u32 v)
{
    return __sync_fetch_and_and(p, v);
}

static inline uae_atomic atomic_or(volatile uae_atomic *p, uae_u32 v)
{
    return __sync_fetch_and_or(p, v);
}

static inline uae_atomic atomic_inc(volatile uae_atomic *p)
{
    return __sync_add_and_fetch(p, 1);
}

static inline uae_atomic atomic_dec(volatile uae_atomic *p)
{
    return __sync_sub_and_fetch(p, 1);
}

static inline uae_u32 atomic_bit_test_and_reset(volatile uae_atomic *p, uae_u32 v)
{
    uae_u32 value = __sync_fetch_and_and(p, ~(1 << v));
    return (value >> v) & 1;
}

#endif /* UAE_ATOMIC_H */
