#ifndef LIBFSEMU_HUD_H_
#define LIBFSEMU_HUD_H_

#include <fs/emu.h>

#define FS_EMU_MAX_CHAT_STRING_SIZE 128

void fs_emu_hud_init(void);
void fs_emu_hud_init_after_config(void);

void fs_emu_hud_enable_chat_mode(void);
void fs_emu_hud_render_chat(void);
int fs_emu_hud_in_chat_mode(void);
int fs_emu_hud_handle_chat_input(fs_emu_event *event);

extern char g_fs_emu_chat_string[];
//extern int g_fs_emu_chat_mode;
extern int g_fs_emu_hud_mode;

#endif // LIBFSEMU_HUD_H_
