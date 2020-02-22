#ifndef FSEMU_STREAM_H_
#define FSEMU_STREAM_H_

#include <stdint.h>

#include "fsemu-common.h"
#include "fsemu-refable.h"
#include "fsemu-sdl.h"

#ifdef __cplusplus
extern "C" {
#endif

// Almost 1:1 mapping to SDL_RWops. Stream objects are refable.

typedef struct fsemu_stream {
    FSEMU_REFABLE;
    SDL_RWops *rwops;
} fsemu_stream_t;

fsemu_stream_t *fsemu_stream_null(void);

fsemu_stream_t *fsemu_stream_new(void);

void fsemu_stream_unref(fsemu_stream_t *stream);

fsemu_stream_t *fsemu_stream_from_const_data(const void *mem, int size);

fsemu_stream_t *fsemu_stream_from_file(FILE *fp, SDL_bool autoclose);

fsemu_stream_t *fsemu_stream_from_path(const char *file, const char *mode);

fsemu_stream_t *fsemu_stream_from_data(void *mem, int size);

int fsemu_stream_close(fsemu_stream_t *stream);

int64_t fsemu_stream_size(fsemu_stream_t *stream);
size_t fsemu_stream_read(fsemu_stream_t *stream,
                         void *ptr,
                         size_t size,
                         size_t maxnum);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_STREAM_H_
