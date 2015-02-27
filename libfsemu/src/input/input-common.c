#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/input.h>

int (*fs_emu_input_handler)(void) = 0;

#endif /* FS_EMU_DRIVERS */
