#ifndef FSEMU_THEME_H_
#define FSEMU_THEME_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_theme_module_init(void);

void fsemu_theme_margins(int* top, int* right, int* bottom, int* left);

void fsemu_theme_update(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_THEME_H_
