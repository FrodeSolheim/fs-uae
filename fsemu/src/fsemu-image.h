#ifndef FSEMU_IMAGE_H_
#define FSEMU_IMAGE_H_

#include <stdint.h>

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fsemu-refable.h"
#include "fsemu-stream.h"

typedef struct fsemu_image {
    FSEMU_REFABLE;
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
} fsemu_image_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_image_module_init(void);

fsemu_image_t *fsemu_image_new(void);

static inline void fsemu_image_ref(fsemu_image_t *image)
{
    return fsemu_refable_ref(image);
}

static inline void fsemu_image_unref(fsemu_image_t *image)
{
    return fsemu_refable_unref(image);
}

fsemu_image_t *fsemu_image_load(const char *name);
fsemu_image_t *fsemu_image_load_png_file(const char *path);
fsemu_image_t *fsemu_image_load_png_from_data(void *data, int data_size);

fsemu_image_t *fsemu_image_from_stream(fsemu_stream_t *stream, bool owner);
fsemu_image_t *fsemu_image_from_size(int width, int height);

extern int fsemu_image_log_level;

#define fsemu_image_log(format, ...)                             \
    if (FSEMU_LIKELY(fsemu_image_log_level >= FSEMU_LOG_INFO)) { \
        fsemu_log("[FSEMU] [IMAGE] " format, ##__VA_ARGS__);     \
    }

#define fsemu_image_log_debug(format, ...)                          \
    if (FSEMU_UNLIKELY(fsemu_image_log_level >= FSEMU_LOG_DEBUG)) { \
        fsemu_log("[FSEMU] [IMAGE] " format, ##__VA_ARGS__);        \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_IMAGE_H_
