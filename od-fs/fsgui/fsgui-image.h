#ifndef FSGUI_IMAGE_H
#define FSGUI_IMAGE_H

#include "fsgui-config.h"

// FIXME: Remove fsemu deps
#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fslib-refable.h"

struct fsgui_image;
typedef struct fsgui_image fsgui_image_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsgui_image_init_module(void);

void fsgui_image_ref(fsgui_image_t* font);

void fsgui_image_unref(fsgui_image_t* font);

fsgui_image_t* fsgui_image_load(const char* name);

#include <SDL3_image/SDL_image.h>

struct fsgui_image {
    FSLIB_REFABLE;

    SDL_Surface* surface;

    int left;
    int top;
    int bottom;
    int right;
};

#ifdef __cplusplus
}
#endif

#endif  // FSGUI_IMAGE_H
