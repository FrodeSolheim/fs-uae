#ifndef LIBFSEMU_DIALOG_H_
#define LIBFSEMU_DIALOG_H_

#include <fs/emu.h>

// void fs_emu_dialog_init();
void fs_emu_dialog_render();
void fs_emu_dialog_handle_action(int action, int state);

#endif // LIBFSEMU_DIALOG_H_
