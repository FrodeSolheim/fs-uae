#ifndef UAE_ATOMIC_H
#define UAE_ATOMIC_H

#include "uae/types.h"

typedef int uae_atomic;

static inline uae_atomic atomic_and(volatile uae_atomic *p, uae_u32 v)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_and(p, v);
}

static inline uae_atomic atomic_or(volatile uae_atomic *p, uae_u32 v)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_or(p, v);
}

static inline uae_atomic atomic_inc(volatile uae_atomic *p)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_add(p, 1);
}

static inline uae_atomic atomic_dec(volatile uae_atomic *p)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_sub(p, 1);
}

static inline uae_u32 atomic_bit_test_and_reset(volatile uae_atomic *p, uae_u32 v)
{
    uae_u32 value = __sync_fetch_and_and(p, ~(1 << v));
    // FIXME: Doublecheck that this should return 0 or 1, and not 0 or 1 << v)
    return (value >> v) & 1;
}

#endif /* UAE_ATOMIC_H */
