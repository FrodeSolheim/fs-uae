#ifndef FSEMU_HELPER_H_
#define FSEMU_HELPER_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_helper_init_emulator(const char *emulator_name,
                                bool external_events,
                                int renderer,
                                int fullscreen,
                                int vsync);
void fsemu_helper_startup_loop(void);

// Returns the latest GUI snapshot pointer after helper is done.
// FIXME: Remove: deprecated, use fsemu_helper_gui instead
fsemu_gui_item_t *fsemu_helper_startup_gui(void);

fsemu_gui_item_t *fsemu_helper_gui(void);
void fsemu_helper_set_gui(fsemu_gui_item_t *gui);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HELPER_H_
