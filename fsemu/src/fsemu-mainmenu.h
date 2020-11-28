#ifndef FSEMU_MAINMENU_H_
#define FSEMU_MAINMENU_H_

#include <stdint.h>

#include "fsemu-action.h"
#include "fsemu-config.h"
#include "fsemu-menu.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_mainmenu_init(void);

void fsemu_mainmenu_update(void);

fsemu_menu_t *fsemu_mainmenu_get_menu(void);

// typedef void (*fsemu_mainmenu_reset_function_t)(void);

// void fsemu_mainmenu_set_soft_reset_function(
//     fsemu_mainmenu_reset_function_t function);
// void fsemu_mainmenu_set_hard_reset_function(
//     fsemu_mainmenu_reset_function_t function);

#if 0
bool fsemu_osmenu_open(void);

void fsemu_osmenu_set_open(bool open);

void fsemu_osmenu_toggle_open(void);

void fsemu_osmenu_navigate(int navigate, fsemu_action_state_t state);
#endif

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MAINMENU_H_
