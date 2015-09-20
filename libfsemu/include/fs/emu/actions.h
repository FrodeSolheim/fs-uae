#ifndef LIBFSEMU_ACTIONS_H
#define LIBFSEMU_ACTIONS_H

#define FS_EMU_ACTION_TAUNT      0xc001
#define FS_EMU_ACTION_SCREENSHOT 0xc002
#define FS_EMU_ACTION_PAUSE      0xc003
#define FS_EMU_ACTION_MENU_ALT   0xc004

void fs_emu_handle_libfsemu_action(int action, int state);

#endif /* LIBFSEMU_ACTIONS_H */

