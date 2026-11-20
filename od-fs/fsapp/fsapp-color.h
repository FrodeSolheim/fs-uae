/*
 * Colours packed in 32-bit values.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_COLOR_H
#define FSAPP_COLOR_H

#include <stdint.h>

#include "fsapp-config.h"
#include "fs/log.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t fsapp_color_t;

#define FSAPP_COLOR_RGB(c)                                                                         \
    ((((c) & 0xff0000) >> 16) | ((c) & 0x00ff00) | (((c) & 0x0000ff) << 16) | 0xff000000)

#define FSAPP_COLOR_RGBA(c)                                                                        \
    ((((c) & 0xff000000) >> 24) | (((c) & 0x00ff0000) >> 8) | (((c) & 0x0000ff00) << 8) |          \
     (((c) & 0x000000ff) << 24))

#define FSAPP_COLOR_RGB_A(c, a)                                                                    \
    ((((c) & 0xff0000) >> 16) | ((c) & 0x00ff00) | (((c) & 0x0000ff) << 16) |                      \
     (((a) & 0x000000ff) << 24))

#define FSAPP_COLOR_BLACK FSAPP_COLOR_RGB(0x000000)
#define FSAPP_COLOR_BLUE FSAPP_COLOR_RGB(0x0000ff)
#define FSAPP_COLOR_GREEN FSAPP_COLOR_RGB(0x00ff00)
#define FSAPP_COLOR_RED FSAPP_COLOR_RGB(0xff0000)
#define FSAPP_COLOR_TRANSPARENT FSAPP_COLOR_RGBA(0x00000000)
#define FSAPP_COLOR_WHITE FSAPP_COLOR_RGB(0xffffff)

static inline void fsapp_color_set_alpha(fsapp_color_t *color, int alpha)
{
    *color = (*color & 0xffffff) | ((alpha & 0x000000ff) << 24);
}

static inline void fsapp_color_decode(fsapp_color_t color, uint8_t *r, uint8_t *g, uint8_t *b,
                                      uint8_t *a)
{
    *r = color & 0xff;
    *g = (color & 0xff00) >> 8;
    *b = (color & 0xff0000) >> 16;
    *a = (color & 0xff000000) >> 24;
}

// ----------------------------------------------------------------------------
// FIXME: Deprecated

#define FSAPP_RGB(c) (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | 0xff000000)

#define FSAPP_RGBA(c)                                                                              \
    (((c & 0xff000000) >> 24) | ((c & 0x00ff0000) >> 8) | ((c & 0x0000ff00) << 8) |                \
     ((c & 0x000000ff) << 24))

#define FSAPP_RGB_A(c, a)                                                                          \
    (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | ((a & 0x000000ff) << 24))

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#define fsapp_color_log(format, ...) SDL_Log("[FSE] [COL] " format, ##__VA_ARGS__)

#endif /* FSAPP_COLOR_H */
