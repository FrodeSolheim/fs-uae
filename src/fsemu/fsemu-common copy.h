#ifndef FSEMU_COMMON_H_
#define FSEMU_COMMON_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __WIN32__
#define FSEMU_WINDOWS
#endif

#ifdef __APPLE__
#define FSEMU_MACOS
#endif

#ifdef __linux__
#define FSEMU_LINUX
#endif

#define FSEMU_FLAG_NONE 0

#define FSEMU_GLIB
#define FSEMU_SDL

/*
#ifdef FSEMU_LINUX
#ifndef FSEMU_ALSA
#define FSEMU_ALSA
#endif
#endif
*/

#ifdef FSEMU_INTERNAL

#include <stdbool.h>

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

#endif

#endif  // FSEMU_COMMON_H_
