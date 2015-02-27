#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/input.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

void fs_emu_input_init(void)
{
    fs_log("fs_emu_input_init\n");
    fs_emu_input_action_init();
    fs_emu_input_sdl_init();
}

#endif /* FS_EMU_DRIVERS */
