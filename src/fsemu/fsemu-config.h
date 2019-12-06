#ifndef FSEMU_CONFIG_H_
#define FSEMU_CONFIG_H_

#include "fsemu-common.h"

// #ifdef HAVE_CONFIG_H
// #include "config.h"
// #endif

#ifdef _WIN32
#define FSEMU_WINDOWS 1
#endif

#ifdef __APPLE__
#define FSEMU_MACOS 1
#endif

#ifdef __linux__
#define FSEMU_LINUX 1
#endif

#define FSEMU_FLAG_NONE 0

#define FSEMU_GLIB 1
#define FSEMU_OPENGL 1
#define FSEMU_PNG 1
#define FSEMU_SDL 1

#ifdef FSUAE
// FS-UAE adjusts audio frequency internally
#else
#define FSEMU_SAMPLERATE 1
#endif

/*
#ifdef FSEMU_LINUX
#ifndef FSEMU_ALSA
#define FSEMU_ALSA
#endif
#endif
*/

#define FSEMU_PATH_MAX 4096

// ---------------------------------------------------------------------------
// FIXME: REMOVE START
// ---------------------------------------------------------------------------

#include <stdbool.h>

#define FSEMU_CONFIG_NONE 0x12345678

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

static inline int fsemu_config_boolean(const char *name)
{
    return FSEMU_CONFIG_NONE;
}

int fsemu_config_read_bool_default(const char *name,
                                   bool *result,
                                   bool default_value);

// ---------------------------------------------------------------------------
// FIXME: REMOVE END
// ---------------------------------------------------------------------------

#endif  // FSEMU_CONFIG_H_
