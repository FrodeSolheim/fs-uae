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

fsemu_menu_t* fsemu_mainmenu_get_menu(void);

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

#ifdef FSEMU_INTERNAL

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_mainmenu_log_level;

#define fsemu_mainmenu_log(format, ...) FSEMU_LOG(mainmenu, "[FSE] [MMU]", format, ##__VA_ARGS__)

#define fsemu_mainmenu_log_debug(format, ...) \
    FSEMU_LOG_DEBUG(mainmenu, "[FSE] [MMU]", format, ##__VA_ARGS__)

#define fsemu_mainmenu_log_error(format, ...) \
    FSEMU_LOG_ERROR(mainmenu, "[FSE] [MMU]", format, ##__VA_ARGS__)

#define fsemu_mainmenu_log_info(format, ...) \
    FSEMU_LOG_INFO(mainmenu, "[FSE] [MMU]", format, ##__VA_ARGS__)

#define fsemu_mainmenu_log_trace(format, ...) \
    FSEMU_LOG_TRACE(mainmenu, "[FSE] [MMU]", format, ##__VA_ARGS__)

#define fsemu_mainmenu_log_warning(format, ...) \
    FSEMU_LOG_WARNING(mainmenu, "[FSE] [MMU]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MAINMENU_H_
