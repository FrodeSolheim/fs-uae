#ifndef FSEMU_TYPES_H_
#define FSEMU_TYPES_H_

#define FSEMU_SDL

#ifdef FSEMU_SDL
#include <SDL2/SDL.h>
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef FSEMU_SDL
typedef SDL_Point fsemu_point;
typedef SDL_Rect fsemu_rect;
typedef SDL_Rect fsemu_rect_t;

#else
typedef struct fsemu_point {
    int x;
    int y;
} fsemu_point;

#endif

typedef struct fsemu_size {
    int w;
    int h;
} fsemu_size;

typedef struct fsemu_size fsemu_size_t;

#ifdef FSEMU_INTERNAL

/** Makes it easier to use the %lld format specifier without using the ugly
 * PRI64 defines, casts or getting compiler warnings. */
static inline long long lld(int64_t value)
{
    return value;
}

#endif  // FSEMU_INTERNAL

#endif  // FSEMU_TYPES_H_
