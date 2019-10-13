#ifndef FSEMU_PERFGUI_H_
#define FSEMU_PERFGUI_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_perfgui_init(void);
void fsemu_perfgui_update(void);
void fsemu_perfgui_cycle(void);

#ifdef FSEMU_INTERNAL

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_PERFGUI_H_
