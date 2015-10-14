#ifndef LIBFSEMU_MENU_H_
#define LIBFSEMU_MENU_H_

enum {
    ACTION_MENU_NONE,
    ACTION_MENU_LEFT,
    ACTION_MENU_RIGHT,
    ACTION_MENU_UP,
    ACTION_MENU_DOWN,
    ACTION_MENU_PRIMARY,
    ACTION_MENU_BACK,
    ACTION_MENU_MENU,
    ACTION_MENU_START,
    ACTION_MENU_SELECT,
    ACTION_MENU_ESCAPE,
};

int fs_emu_menu_or_dialog_is_active(void);
void fs_emu_menu_init_opengl(void);

void fs_emu_menu_render(double transition);
void fs_emu_menu_function(int action, int state);

extern int g_fs_emu_menu_mode;
extern const char* g_fs_emu_key_names[];

#endif // LIBFSEMU_MENU_H_
