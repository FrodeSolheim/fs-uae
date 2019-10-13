#ifndef FSEMU_SDL_H_
#define FSEMU_SDL_H_

#include "fsemu-common.h"

#ifdef FSEMU_SDL

#ifdef FSEMU_OPENGL
// Must make sure glew.h is included before gl.h
#include "fsemu-opengl.h"
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
// #include <SDL.h>

#if !SDL_VERSION_ATLEAST(2, 0, 5)
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_PIXELFORMAT_BGRA32 SDL_PIXELFORMAT_BGRA8888
#else
#define SDL_PIXELFORMAT_BGRA32 SDL_PIXELFORMAT_ARGB8888
#endif
#endif

#endif

#endif  // FSEMU_SDL_H_
