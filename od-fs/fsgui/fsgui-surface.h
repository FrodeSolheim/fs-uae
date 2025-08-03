#ifndef FSGUI_SURFACE_H
#define FSGUI_SURFACE_H

#include "fsgui-config.h"

#include "fslib-log.h"
#include "fslib-refable.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

struct fsgui_surface;
typedef struct fsgui_surface fsgui_surface_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsgui_surface_init_module(void);

void fsgui_surface_ref(fsgui_surface_t* font);

void fsgui_surface_unref(fsgui_surface_t* font);

struct fsgui_surface {
    FSLIB_REFABLE;

    // TTF_Font* font;
    // TTF_TextEngine* engine;
    // TTF_Text* text;
    // SDL_IOStream *rwops;
    // void *data;

    SDL_Surface* sdl;

    void* data;
    int x;
    int y;
    int width;
    int height;
    Py_buffer buffer;
    SDL_Rect rect;
    int buffer_width;
    int buffer_height;
    // SDL_Texture* texture;
    GLuint texture;
    // bool initialized; // Has this surface been drawn to at least once?

    int special;
};

#ifdef __cplusplus
}
#endif

#endif // FSGUI_SURFACE_H
