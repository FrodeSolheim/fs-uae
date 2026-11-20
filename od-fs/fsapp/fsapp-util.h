/*
 * Small helpers: environment variables and moving averages.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_UTIL_H
#define FSAPP_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "fsapp-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define fsapp_assert SDL_assert
#define fsapp_assert_release SDL_assert_release

#define FSAPP_UTIL_MALLOC(t) (t *)malloc(sizeof(t));
#define FSAPP_UTIL_MALLOC0(t) (t *)calloc(1, sizeof(t));

/** Always returns a string (empty if not set) */
const char *fsapp_util_get_env(const char *name);

// The moving average of the last `size` values, in int64_t (for durations in nanoseconds, which
// overflow an int after 2.1 seconds, and a sum of them sooner). The caller owns the array of `size`
// values; a zero-initialized fsapp_util_moving_average_t is empty.
typedef struct {
    int size;
    int offset;
    int64_t sum;
} fsapp_util_moving_average_t;

// Adds value to the moving average, and returns the average of the last `size` values (fewer
// before `size` values have been added).
static inline int64_t fsapp_util_moving_average_add(fsapp_util_moving_average_t *average,
                                                    int64_t *values, int size, int64_t value)
{
    if (average->size < size) {
        values[average->size++] = value;
        average->sum += value;
    } else {
        average->sum -= values[average->offset];
        values[average->offset] = value;
        average->sum += value;
        average->offset++;
        if (average->offset >= size) {
            average->offset -= size;
        }
    }
    return average->sum / average->size;
}

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_UTIL_H */
