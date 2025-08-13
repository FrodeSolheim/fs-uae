#ifndef FSGUI_SURFACE_H
#define FSGUI_SURFACE_H

#include <Python.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glib.h>

#include "fsgui-config.h"
#include "fslib-log.h"
#include "fslib-refable.h"

struct fsgui_surface;
typedef struct fsgui_surface fsgui_surface_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsgui_surface_init_module(void);

void fsgui_surface_ref(fsgui_surface_t* font);

void fsgui_surface_unref(fsgui_surface_t* font);

struct fsgui_surface_pos {
    // float x;
    // float y;
    int x;
    int y;
    int width;
    int height;
    // float z;
    int z;
};

struct fsgui_surface {
    FSLIB_REFABLE;

    // TTF_Font* font;
    // TTF_TextEngine* engine;
    // TTF_Text* text;
    // SDL_IOStream *rwops;
    // void *data;

    SDL_Surface* sdl;
    Py_buffer buffer;
    int buffer_width;
    int buffer_height;

    SDL_Rect rect;

    void* data;

    bool deleted;

    // FIXME: use struct fsgui_surface_pos
    bool visible;
    // float x;
    // float y;
    int x;
    int y;
    int width;
    int height;
    int z;
    // SDL_Texture* texture;
    // bool initialized; // Has this surface been drawn to at least once?

    int special;

    // render information

    GLuint render_texture;
    struct fsgui_surface_pos render;
};

GList* fsapp_surface_get_list();
GList* fsapp_surface_get_opengl_free_list();

#ifdef __cplusplus
}
#endif

#endif  // FSGUI_SURFACE_H
