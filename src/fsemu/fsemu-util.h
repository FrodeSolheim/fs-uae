#ifndef FSEMU_UTIL_H_
#define FSEMU_UTIL_H_

#include "fsemu/fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define fsemu_log SDL_Log
#ifdef FSEMU_SDL
#define fsemu_assert SDL_assert
#endif

const char *fsemu_getenv(const char *name);
const char *fsemu_read_env(const char *name);

typedef struct {
    int size;
    int offset;
    int mavg;
} fsemu_mavgi_t;

static void fsemu_mavgi_clear(fsemu_mavgi_t *mavg)
{
    mavg->size = 0;
    mavg->offset = 0;
    mavg->mavg = 0;
}

static int fsemu_mavgi(fsemu_mavgi_t *mavg, int *values, int size, int value)
{
    if (mavg->size < size) {
        values[mavg->size++] = value;
        mavg->mavg += value;
    } else {
        mavg->mavg -= values[mavg->offset];
        values[mavg->offset] = value;
        mavg->mavg += value;
        mavg->offset++;
        if (mavg->offset >= size)
            mavg->offset -= size;
    }
    return mavg->mavg / mavg->size;
}

#if 0
#define FSEMU_MAVGI(size, value)                                 \
    {                                                            \
        static fsemu_mavgi_t unnamed_mavg;                       \
        static int unnamed_values[8];                            \
        fsemu_mavgi(&unnamed_mavg, unnamed_values, size, value); \
    };
#endif

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_UTIL_H_
