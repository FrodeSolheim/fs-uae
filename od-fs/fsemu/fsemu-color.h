#ifndef FSEMU_COLOR_H_
#define FSEMU_COLOR_H_

#include <stdint.h>

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t fsemu_color_t;

#define FSEMU_COLOR_RGB(c)                                                    \
    ((((c) & 0xff0000) >> 16) | ((c) & 0x00ff00) | (((c) & 0x0000ff) << 16) | \
     0xff000000)

#define FSEMU_COLOR_RGBA(c)                                   \
    ((((c) & 0xff000000) >> 24) | (((c) & 0x00ff0000) >> 8) | \
     (((c) & 0x0000ff00) << 8) | (((c) & 0x000000ff) << 24))

#define FSEMU_COLOR_RGB_A(c, a)                                               \
    ((((c) & 0xff0000) >> 16) | ((c) & 0x00ff00) | (((c) & 0x0000ff) << 16) | \
     (((a) & 0x000000ff) << 24))

#define FSEMU_COLOR_BLACK FSEMU_COLOR_RGB(0x000000)
#define FSEMU_COLOR_BLUE FSEMU_COLOR_RGB(0x0000ff)
#define FSEMU_COLOR_GREEN FSEMU_COLOR_RGB(0x00ff00)
#define FSEMU_COLOR_RED FSEMU_COLOR_RGB(0xff0000)
#define FSEMU_COLOR_TRANSPARENT FSEMU_COLOR_RGBA(0x00000000)
#define FSEMU_COLOR_WHITE FSEMU_COLOR_RGB(0xffffff)

static inline void fsemu_color_set_alpha(fsemu_color_t *color, int alpha)
{
    *color = (*color & 0xffffff) | ((alpha & 0x000000ff) << 24);
}

static inline void fsemu_color_decode(
    fsemu_color_t color, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a)
{
    *r = color & 0xff;
    *g = (color & 0xff00) >> 8;
    *b = (color & 0xff0000) >> 16;
    *a = (color & 0xff000000) >> 24;
}

// ----------------------------------------------------------------------------
// FIXME: Deprecated

#define FSEMU_RGB(c)                                                    \
    (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | \
     0xff000000)

#define FSEMU_RGBA(c)                                     \
    (((c & 0xff000000) >> 24) | ((c & 0x00ff0000) >> 8) | \
     ((c & 0x0000ff00) << 8) | ((c & 0x000000ff) << 24))

#define FSEMU_RGB_A(c, a)                                               \
    (((c & 0xff0000) >> 16) | (c & 0x00ff00) | ((c & 0x0000ff) << 16) | \
     ((a & 0x000000ff) << 24))

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#define fsemu_color_log(format, ...) \
    fsemu_log("[FSE] [COL] " format, ##__VA_ARGS__)

#endif  // FSEMU_COLOR_H_
