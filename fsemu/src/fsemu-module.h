#ifndef FSEMU_MODULE_H_
#define FSEMU_MODULE_H_

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_module_init(void);

typedef void (*fsemu_module_on_quit_f)(void);

void fsemu_module_on_quit(fsemu_module_on_quit_f function);

// Call this to let all initalized FSEMU modules clean up after themselves. It
// is safe (but pointless) to call this more than once. You should not assume
// that any other FSEMU functions are safe to call after this unless otherwise
// noted.
void fsemu_module_quit(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MODULE_H_
