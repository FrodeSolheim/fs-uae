#ifndef FSLIB_STREAM_H
#define FSLIB_STREAM_H

#include "fsemu-common.h"
#include "fsemu-sdl.h"
#include "fslib-config.h"
#include "fslib-refable.h"

#ifdef __cplusplus
extern "C" {
#endif

// Almost 1:1 mapping to SDL_RWops. Stream objects are refable.

typedef struct fslib_stream {
    FSLIB_REFABLE;
    SDL_IOStream* rwops;
} fslib_stream_t;

fslib_stream_t* fslib_stream_null(void);

fslib_stream_t* fslib_stream_new(void);

void fslib_stream_unref(fslib_stream_t* stream);

fslib_stream_t* fslib_stream_from_const_data(const void* mem, int size);

fslib_stream_t* fslib_stream_from_file(FILE* fp, bool autoclose);

fslib_stream_t* fslib_stream_from_path(const char* file, const char* mode);

fslib_stream_t* fslib_stream_from_data(void* mem, int size);

int fslib_stream_close(fslib_stream_t* stream);

int64_t fslib_stream_size(fslib_stream_t* stream);

size_t fslib_stream_read(fslib_stream_t* stream, void* ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // FSLIB_STREAM_H
