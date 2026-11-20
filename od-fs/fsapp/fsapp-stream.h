/*
 * Streams: reading and writing files and memory through one interface.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_STREAM_H
#define FSAPP_STREAM_H

#include <SDL3/SDL.h>
#include <stdio.h>

#include "fsapp-config.h"
#include "fsapp-refable.h"

#ifdef __cplusplus
extern "C" {
#endif

// Almost 1:1 mapping to SDL_RWops. Stream objects are refable.

typedef struct fsapp_stream {
    FSAPP_REFABLE;
    SDL_IOStream *rwops;
} fsapp_stream_t;

fsapp_stream_t *fsapp_stream_create_null(void);

fsapp_stream_t *fsapp_stream_create(void);

void fsapp_stream_unref(fsapp_stream_t *stream);

fsapp_stream_t *fsapp_stream_create_from_const_data(const void *mem, int size);

fsapp_stream_t *fsapp_stream_create_from_file(FILE *fp, bool autoclose);

fsapp_stream_t *fsapp_stream_create_from_path(const char *file, const char *mode);

fsapp_stream_t *fsapp_stream_create_from_data(void *mem, int size);

int fsapp_stream_close(fsapp_stream_t *stream);

int64_t fsapp_stream_get_size(fsapp_stream_t *stream);

size_t fsapp_stream_read(fsapp_stream_t *stream, void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_STREAM_H */
