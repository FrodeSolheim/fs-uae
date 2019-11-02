#ifndef FSEMU_HELPER_H_
#define FSEMU_HELPER_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_helper_init_emulator(const char *emulator_name,
                                bool external_events,
                                int renderer,
                                int fullscreen,
                                int vsync);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HELPER_H_
