#ifndef FSGUI_FONT_H
#define FSGUI_FONT_H

#include "fsgui-config.h"

struct fsgui_font;
typedef struct fsgui_font fsgui_font_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsgui_font_init_module(void);

void fsgui_font_ref(fsgui_font_t* font);

void fsgui_font_unref(fsgui_font_t* font);

fsgui_font_t* fsgui_font_load(const char* name, int size);

#ifdef __cplusplus
}
#endif

#endif  // FSGUI_FONT_H
