#ifndef FSEMU_RECORDING_H_
#define FSEMU_RECORDING_H_

#include <stdint.h>

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_recording_init(void);

void fsemu_recording_begin_frame();

void fsemu_recording_end_frame();

void fsemu_recording_on_load_state_finished(int slot, const char* path);

void fsemu_recording_on_save_state_finished(int slot, const char* path);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_RECORDING_H_
