#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/input.h>
#include <fs/log.h>
#include <SDL.h>

static int sdl_input_handler(void)
{
    // printf("sdl_input_handler\n");
#if 0
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            printf("SDL_QUIT\n");
            fs_ml_quit();
            return 1;
        }
    }
    return 0;
#endif
    return fs_ml_event_loop();
    // return 0;
}

static void register_functions(void)
{
    fse_input.input_handler = sdl_input_handler;
}

void fs_emu_input_sdl_init(void)
{
    fs_log("fs_emu_input_sdl_init\n");
    register_functions();
}
