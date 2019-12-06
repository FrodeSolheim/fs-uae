#ifndef FSEMU_GAMEMODE_H_
#define FSEMU_GAMEMODE_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_gamemode_init(void);
void fsemu_gamemode_suspend(void);
void fsemu_gamemode_resume(void);
void fsemu_gamemode_check_linux_cpu_governor(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_GAMEMODE_H_
