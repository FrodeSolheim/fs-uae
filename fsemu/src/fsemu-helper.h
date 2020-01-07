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

// Returns the latest GUI snapshot pointer after helper is done.
// FIXME: Remove: deprecated, use fsemu_helper_gui instead
fsemu_gui_item_t *fsemu_helper_startup_gui(void);

fsemu_gui_item_t *fsemu_helper_gui(void);
void fsemu_helper_set_gui(fsemu_gui_item_t *gui);

void fsemu_helper_sleep_display_end_start(double hz);

void fsemu_helper_render_sleep_display_end_start(double hz);

// Conveniently updates all the standard FSEMU models as initialized by
// fsemu_helper_init_emulator.
void fsemu_helper_update(void);

// Calls fsemu_helper_update and then proceeds to render the early GUI.
// Do not call both this and fsemu_helper_update.
void fsemu_helper_update_render_early(void);

void fsemu_helper_framewait(void);

// Called by fsemu_helper_init_emulator; usually not called manually.
void fsemu_helper_startup_loop(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HELPER_H_
