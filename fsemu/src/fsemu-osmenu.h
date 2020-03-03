#ifndef FSEMU_OSMENU_H_
#define FSEMU_OSMENU_H_

#include <stdint.h>

#include "fsemu-action.h"
#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_osmenu_init(void);

void fsemu_osmenu_update(void);

bool fsemu_osmenu_open(void);

int fsemu_osmenu_open_offset(void);

void fsemu_osmenu_set_open(bool open);

void fsemu_osmenu_toggle_open(void);

void fsemu_osmenu_navigate(int navigate, fsemu_action_state_t state);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_OSMENU_H_
