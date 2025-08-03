#ifndef FSEMU_SDL_H
#define FSEMU_SDL_H

#include "fsemu-common.h"
#include "fsemu-config.h"

#ifdef FSEMU_SDL

#ifdef FSEMU_OPENGL
// Must make sure glew.h is included before gl.h
#include "fsemu-opengl.h"
#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#endif

#endif  // FSEMU_SDL_H
