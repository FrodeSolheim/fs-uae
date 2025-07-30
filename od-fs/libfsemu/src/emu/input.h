#ifndef LIBFSEMU_INPUT_H
#define LIBFSEMU_INPUT_H

#include <fs/ml.h>
#include <fs/emu.h>

void fse_init_input(void);

extern bool g_fs_emu_grab_input_on_mod_release;

#endif /* LIBFSEMU_INPUT_H */
