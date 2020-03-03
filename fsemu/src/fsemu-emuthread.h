#ifndef FSEMU_EMUTHREAD_H_
#define FSEMU_EMUTHREAD_H_

// This module contains data which can be safely used from the emulation
// thread.

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the videothread subsystem. Can safely be called more than once.
// FIXME:
// Will be called from fsemu_video_init, so no reason to call this from other
// contexts.
void fsemu_emuthread_init(void);

void fsemu_emuthread_lock(void);
void fsemu_emuthread_unlock(void);

bool fsemu_emuthread_paused(void);

// FIXME: fsemu_emuthread_(next_)action

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_EMUTHREAD_H_
