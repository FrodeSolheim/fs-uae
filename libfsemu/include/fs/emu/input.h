#ifndef FS_EMU_INPUT_H
#define FS_EMU_INPUT_H

#include <fs/ml.h>

bool fs_emu_full_keyboard_emulation(void);
void fs_emu_set_full_keyboard_emulation(bool full, bool notification);

#ifdef FS_EMU_DRIVERS

typedef struct fs_emu_action_id {
    int value;
} fs_emu_action_id;

typedef void (*fs_emu_input_action_function)(int state, void *data);

fs_emu_action_id fs_emu_input_action_register(
    const char *name, fs_emu_input_action_function function, void *data);

fs_emu_action_id fs_emu_input_action_lookup(const char *name);

void fs_emu_input_action_execute(fs_emu_action_id action, int state);

#endif

#ifdef FS_EMU_INTERNAL

void fs_emu_input_init(void);

void fs_emu_input_init_2(void);

void fs_emu_input_dummy_init(void);

void fs_emu_input_sdl_init(void);

void fs_emu_input_action_init(void);

extern int (*fs_emu_input_handler)(void);

int fs_ml_event_loop(void);

#endif /* FS_EMU_INTERNAL */

#endif /* FS_EMU_INPUT_H */
