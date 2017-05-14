#ifndef LIBFSEMU_ACTIONS_H
#define LIBFSEMU_ACTIONS_H

#define FS_EMU_ACTION_LOCAL 0x8000
#define FS_EMU_ACTION_LOCAL_LIBFSEMU 0xc000

enum {
    FS_EMU_ACTION_FIRST = FS_EMU_ACTION_LOCAL_LIBFSEMU,

    FS_EMU_ACTION_FULLSCREEN,
    FS_EMU_ACTION_GRAB_INPUT,
    FS_EMU_ACTION_MENU_ALT,
    FS_EMU_ACTION_PAUSE,
    FS_EMU_ACTION_QUIT,
    FS_EMU_ACTION_SCREENSHOT,
    FS_EMU_ACTION_SPECIAL,
    FS_EMU_ACTION_SWITCH_WINDOW,
    FS_EMU_ACTION_TAUNT,
    FS_EMU_ACTION_VOLUME_DOWN,
    FS_EMU_ACTION_VOLUME_MUTE,
    FS_EMU_ACTION_VOLUME_UP,
    FS_EMU_ACTION_WARP,
    FS_EMU_ACTION_ZOOM,
    FS_EMU_ACTION_ZOOM_BORDER,
    FS_EMU_ACTION_FULL_KEYBOARD,
    FSE_ACTION_CYCLE_STRETCH_MODE,

    FS_EMU_ACTION_LAST,
};

void fs_emu_handle_libfsemu_action(int action, int state);

typedef void (*fs_emu_action_function)(int state);

#define FS_EMU_ACTION_FLAG_MENU 1

typedef struct fs_emu_action {
    int input_event;
    const char *name;
    int flags;
    fs_emu_action_function *function;
} fs_emu_action;

void fs_emu_set_actions(fs_emu_action *actions);
int fs_emu_input_action_from_string(const char *value);

#endif /* LIBFSEMU_ACTIONS_H */

