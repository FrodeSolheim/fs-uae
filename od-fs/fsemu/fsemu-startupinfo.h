#ifndef FSEMU_STARTUPINFO_H_
#define FSEMU_STARTUPINFO_H_

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** This must be called before fsemu_startupinfo_init. */
void fsemu_startupinfo_set_emulator_name(const char* emulator_name);

/** This must be called before fsemu_startupinfo_init. The string
 *  must remain valid after the call. */
void fsemu_startupinfo_set_emulator_fork_info(const char* fork_info);

void fsemu_startupinfo_init(void);

void fsemu_startupinfo_update(void);

void fsemu_startupinfo_toggle(void);

// void fsemu_titlebar_set_size(int w, int h);
// void fsemu_startupinfo_set_width(int w);

#ifdef __cplusplus
}
#endif

#ifdef FSEMU_INTERNAL

#define fsemu_startupinfo_log(format, ...) fsemu_log("[FSE] [STI] " format, ##__VA_ARGS__)

#endif  // FSEMU_INTERNAL

#endif  // FSEMU_STARTUPINFO_H_
