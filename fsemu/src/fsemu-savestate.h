#ifndef FSEMU_SAVESTATE_H_
#define FSEMU_SAVESTATE_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char *(*fsuae_savestate_path_function_t)(int slot);

// ----------------------------------------------------------------------------
// Early functions

void fsemu_savestate_set_path_function(
    fsuae_savestate_path_function_t function);

// ----------------------------------------------------------------------------

void fsemu_savestate_init(void);

bool fsemu_savestate_has_state(int slot);

void fsemu_savestate_update_slot(int slot);

char *fsemu_savestate_description(int slot);

int fsemu_savestate_slots(void);

void fsemu_savestate_load(int slot);

void fsemu_savestate_save(int slot);

// void fsemu_savestate_handle_action(int action);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SAVESTATE_H_
