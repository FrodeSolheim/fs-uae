#ifndef FSE_HUD_H
#define FSE_HUD_H

#include <fs/emu.h>

#define FSE_MAX_CHAT_STRING_LENGTH 128

void fse_init_hud_early(void);
void fse_init_hud_after_config(void);

void fs_emu_hud_enable_chat_mode(void);
void fse_render_hud_chat(void);
int fs_emu_hud_in_chat_mode(void);
int fs_emu_hud_handle_chat_input(fs_emu_event *event);

extern char g_fs_emu_chat_string[];
extern int g_fs_emu_hud_mode;

#endif /* FSE_HUD_H */
