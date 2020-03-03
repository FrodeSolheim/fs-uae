#ifndef FSEMU_FONT_H_
#define FSEMU_FONT_H_

#include "fsemu-common.h"
#include "fsemu-image.h"
#include "fsemu-log.h"

struct fsemu_font;
typedef struct fsemu_font fsemu_font_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_font_init(void);

void fsemu_font_ref(fsemu_font_t *font);

void fsemu_font_unref(fsemu_font_t *font);

fsemu_font_t *fsemu_font_load(const char *name, int size);

fsemu_image_t *fsemu_font_render_text_to_image(fsemu_font_t *font,
                                               const char *text,
                                               uint32_t color);

#ifdef __cplusplus
}
#endif

#define fsemu_font_log(format, ...) \
    fsemu_log("[FSEMU] [FONT ] " format, ##__VA_ARGS__)

#endif  // FSEMU_FONT_H_
