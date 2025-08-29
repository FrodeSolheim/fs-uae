#ifndef FSAPP_SURFACE_H
#define FSAPP_SURFACE_H

#include <Python.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glib.h>

#include "fsgui-config.h"
#include "fslib-log.h"
#include "fslib-refable.h"

struct fsapp_surface;
typedef struct fsapp_surface fsapp_surface_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_surface_init_module(void);

void fsapp_surface_ref(fsapp_surface_t* font);

void fsapp_surface_unref(fsapp_surface_t* font);

struct fsapp_surface_pos {
    // float x;
    // float y;
    int x;
    int y;
    int width;
    int height;
    // float z;
    int z;
};

struct fsapp_surface {
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

    // FIXME: use struct fsapp_surface_pos
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
    struct fsapp_surface_pos render;
};

GList* fsapp_surface_get_list();
GList* fsapp_surface_get_opengl_free_list();

#ifdef __cplusplus
}
#endif

#endif  // FSAPP_SURFACE_H
