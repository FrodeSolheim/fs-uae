/*
 * Bitmaps in memory: loading PNG images, and saving them.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_BITMAP_H
#define FSAPP_BITMAP_H

#include <stdint.h>

#include "fsapp-config.h"
#include "fs/log.h"
#include "fsapp-refable.h"
#include "fsapp-stream.h"

typedef struct fsapp_bitmap {
    FSAPP_REFABLE;
    int error;
    uint8_t *data;
    int width;
    int height;
    int depth;
    int bpp;
    int stride;
    int format;
    void (*free_function)(void *data);
    void *free_param;
} fsapp_bitmap_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_bitmap_init(void);

fsapp_bitmap_t *fsapp_bitmap_create(void);

static inline void fsapp_bitmap_ref(fsapp_bitmap_t *image)
{
    return fsapp_refable_ref(&image->refable);
}

static inline void fsapp_bitmap_unref(fsapp_bitmap_t *image)
{
    return fsapp_refable_unref(&image->refable);
}

fsapp_bitmap_t *fsapp_bitmap_load(const char *name);
fsapp_bitmap_t *fsapp_bitmap_create_from_png_data(void *data, int data_size);

fsapp_bitmap_t *fsapp_bitmap_create_from_stream(fsapp_stream_t *stream, bool owner);
fsapp_bitmap_t *fsapp_bitmap_create_with_size(int width, int height);

int fsapp_bitmap_save_png_file_from_rgba_data(const char *path, void *buffer, int width,
                                              int height);

extern int fsapp_bitmap_log_level;

#define fsapp_bitmap_log(format, ...)                                                              \
    FS_LOG_INFO(fsapp_bitmap_log_level, "[FSE] [IMG]", format, ##__VA_ARGS__)

#define fsapp_bitmap_log_debug(format, ...)                                                        \
    FS_LOG_DEBUG(fsapp_bitmap_log_level, "[FSE] [IMG]", format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_BITMAP_H */
