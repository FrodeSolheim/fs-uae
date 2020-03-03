#ifndef FSEMU_SAVESTATE_H_
#define FSEMU_SAVESTATE_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool fsemu_savestate_has_state(int slot);

int fsemu_savestate_slots(void);

void fsemu_savestate_load(int slot);

void fsemu_savestate_save(int slot);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SAVESTATE_H_
