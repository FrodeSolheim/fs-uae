/*
 * Fonts: loading them and drawing text, also from Python (_fsapp_font).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_FONT_H
#define FSAPP_FONT_H

#include "fsapp-config.h"

struct fsapp_font;
typedef struct fsapp_font fsapp_font_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_font_init(void);

void fsapp_font_ref(fsapp_font_t *font);

void fsapp_font_unref(fsapp_font_t *font);

fsapp_font_t *fsapp_font_load(const char *name, int size);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_FONT_H */
