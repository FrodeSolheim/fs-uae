#ifndef FSEMU_COMMON_H_
#define FSEMU_COMMON_H_

#include "fsemu-config.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** This must be called before init functions to be guaranteed to have an
 * effect. */
void fsemu_set_emulator_name(const char *emulator_name);

const char *fsemu_emulator_name(void);

#ifdef __cplusplus
}
#endif

#ifdef FSEMU_INTERNAL

// FIXME: Temporarily disabled translations
#define _(x) x

#define fsemu_return_if_already_initialized() \
    static bool initialized;                  \
    if (initialized) {                        \
        return;                               \
    }                                         \
    initialized = true;

#define fsemu_init_once(var) \
    if (*(var)) {            \
        return;              \
    }                        \
    *(var) = true;

#endif  // FSEMU_INTERNAL

#endif  // FSEMU_COMMON_H_
