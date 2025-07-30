#ifndef FSEMU_MAIN_H_
#define FSEMU_MAIN_H_

#include "fsemu-action.h"
#include "fsemu-config.h"
#include "fsemu-menu.h"

#ifdef __cplusplus
extern "C" {
#endif

// New name for !fsemu_quit_check()
bool fsemu_main_is_running(void);

void fsemu_main_update(void);

void fsemu_main_update_and_snapshot_gui(void);

void fsemu_main_handle_events_until_next_frame(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MAIN_H_
