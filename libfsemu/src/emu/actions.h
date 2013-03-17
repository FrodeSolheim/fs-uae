#ifndef LIBFSEMU_ACTIONS_H_
#define LIBFSEMU_ACTIONS_H_

#define FS_EMU_ACTION_TAUNT 0xc001
#define FS_EMU_ACTION_SCREENSHOT 0xc002

void fs_emu_handle_libfsemu_action(int action, int state);

#endif // LIBFSEMU_ACTIONS_H_
