#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/input.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

fse_input_t fse_input = {};

void fs_emu_input_init(void)
{
    fs_log("fs_emu_input_init\n");
    // fs_emu_input_action_init();
    fs_emu_input_sdl_init();
}
