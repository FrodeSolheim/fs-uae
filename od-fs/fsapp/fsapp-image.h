/*
 * Images for the UI: loading, sizing SVG images and tinting, also from Python (_fsapp_image).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_IMAGE_H
#define FSAPP_IMAGE_H

#include "fsapp-config.h"

#include "fs/log.h"
#include "fsapp-refable.h"

struct fsapp_image;
typedef struct fsapp_image fsapp_image_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_image_init(void);

void fsapp_image_ref(fsapp_image_t *font);

void fsapp_image_unref(fsapp_image_t *font);

fsapp_image_t *fsapp_image_load(const char *name);

fsapp_image_t *fsapp_image_load_sized_svg(const char *name, int width, int height);

void fsapp_image_tint(fsapp_image_t *image, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#include <SDL3_image/SDL_image.h>

struct fsapp_image {
    FSAPP_REFABLE;

    SDL_Surface *surface;

    int left;
    int top;
    int bottom;
    int right;
};

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_IMAGE_H */
