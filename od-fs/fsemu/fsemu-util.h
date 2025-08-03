#ifndef FSEMU_UTIL_H_
#define FSEMU_UTIL_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define fsemu_log SDL_Log
#ifdef FSEMU_SDL
#include "fsemu-sdl.h"
#define fsemu_assert SDL_assert
#define fsemu_assert_release SDL_assert_release
#endif

#define FSEMU_UTIL_MALLOC(t) (t *) malloc(sizeof(t));
#define FSEMU_UTIL_MALLOC0(t) (t *) calloc(1, sizeof(t));

int fsemu_util_copy_file_with_size_limit(const char *src,
                                         const char *dst,
                                         int64_t size_limit);
int fsemu_util_copy_file(const char *src, const char *dst);

int fsemu_util_delete_file(const char *state_recording_path);
int fsemu_util_delete_file_if_exists(const char *state_recording_path);

/** Always returns a string (empty if not set) */
const char *fsemu_getenv(const char *name);

const char *fsemu_read_env_option(const char *name);

typedef struct {
    int size;
    int offset;
    int sum;
} fsemu_mavgi_t;

bool fsemu_path_exists(const char *path);

static inline void fsemu_mavgi_clear(fsemu_mavgi_t *mavg)
{
    mavg->size = 0;
    mavg->offset = 0;
    mavg->sum = 0;
}

static inline int fsemu_mavgi(fsemu_mavgi_t *mavg,
                              int *values,
                              int size,
                              int value)
{
    if (mavg->size < size) {
        values[mavg->size++] = value;
        mavg->sum += value;
        // printf("mavg + %d : %d %d = %d\n", value, mavg->mavg, mavg->size,
        // mavg->mavg / mavg->size);
    } else {
        mavg->sum -= values[mavg->offset];
        values[mavg->offset] = value;
        mavg->sum += value;
        mavg->offset++;
        if (mavg->offset >= size)
            mavg->offset -= size;
    }
    return mavg->sum / mavg->size;
}

#if 0
#define FSEMU_MAVGI(size, value)                                 \
    {                                                            \
        static fsemu_mavgi_t unnamed_mavg;                       \
        static int unnamed_values[8];                            \
        fsemu_mavgi(&unnamed_mavg, unnamed_values, size, value); \
    };
#endif

// ----------------------------------------------------------------------------

typedef struct fsemu_util_spring {
    double current;
    double friction;
    double from;
    int64_t last_us;
    double mass;
    bool stopped;
    double target;
    double tension;
    double velocity;
} fsemu_util_spring_t;

void fsemu_util_spring_init(fsemu_util_spring_t *spring);
// void fsemu_util_spring_init_with_params(fsemu_util_spring_t *spring, double
// ...);
void fsemu_util_spring_update(fsemu_util_spring_t *spring);
void fsemu_util_spring_update_with_time(fsemu_util_spring_t *spring,
                                        int64_t time_us);

// Instantly finish current animation
void fsemu_util_spring_finish(fsemu_util_spring_t *spring);

void fsemu_util_spring_set_target(fsemu_util_spring_t *spring, double target);
void fsemu_util_spring_set_tension(fsemu_util_spring_t *spring,
                                   double tension);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_UTIL_H_
