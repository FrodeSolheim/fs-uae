#define FSEMU_INTERNAL
#include "fsemu/fsemu-font.h"

#include <SDL_ttf.h>

void fsemu_font_init(void)
{
    fsemu_return_if_already_initialized();

    if (!TTF_WasInit() && TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    fsemu_font_log("Initialize\n");
}
