#ifndef FSEMU_TYPES_H_
#define FSEMU_TYPES_H_

#include "fsemu-config.h"

#ifdef FSEMU_SDL
#include "fsemu-sdl.h"
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef FSEMU_SDL
typedef SDL_Point fsemu_point_t;
typedef SDL_Rect fsemu_rect_t;

#else
typedef struct fsemu_point {
    int x;
    int y;
} fsemu_point_t;

#endif

typedef struct fsemu_size {
    int w;
    int h;
} fsemu_size_t;

// typedef struct fsemu_point fsemu_point_t;
// typedef struct fsemu_size fsemu_size_t;

typedef struct fsemu_drect {
    double x;
    double y;
    double w;
    double h;
} fsemu_drect_t;

// Makes it easier to use the %lld format specifier without using the ugly
// PRI64 defines, casts or getting compiler warnings.
static inline long long fsemu_lld(int64_t value)
{
    return value;
}

static inline unsigned long long fsemu_llu(uint64_t value)
{
    return value;
}

#ifdef FSEMU_INTERNAL

#define lld(x) fsemu_lld(x)
#define llu(x) fsemu_llu(x)

#endif  // FSEMU_INTERNAL

#endif  // FSEMU_TYPES_H_
