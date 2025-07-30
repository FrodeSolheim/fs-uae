#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FSUAE_LEGACY

#define FSEMU_INTERNAL
#include <fs/emu/input.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

fse_input_t fse_input = {};

void fse_init_input(void)
{
    fs_log("[INPUT] Initialize\n");
    // fs_emu_input_action_init();
    fs_emu_input_sdl_init();
    fs_emu_input_init_2();
}

#endif // FSUAE_LEGACY
