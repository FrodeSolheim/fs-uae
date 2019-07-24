#ifndef FSEMU_INPUT_H_
#define FSEMU_INPUT_H_

#include "fsemu/fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_input_init(int flags);
void fsemu_input_work(int timeout);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_INPUT_H_
