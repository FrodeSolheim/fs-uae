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

#endif  // FSEMU_TYPES_H_
