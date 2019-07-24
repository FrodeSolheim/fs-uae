#ifndef FSEMU_SDLINPUT_H_
#define FSEMU_SDLINPUT_H_

#include "fsemu/fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlinput_init(void);
void fsemu_sdlinput_work(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDLINPUT_H_
