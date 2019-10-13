#ifndef FSEMU_IMAGE_H_
#define FSEMU_IMAGE_H_

#include "fsemu-common.h"
#include "fsemu-log.h"

#include <stdint.h>

typedef struct {
    uint8_t *data;
    int width;
    int height;
    int depth;
    int stride;
    int format;
} fsemu_image_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_image_init(void);

fsemu_image_t *fsemu_image_load_png_file(const char *path);

#ifdef __cplusplus
}
#endif

#define fsemu_image_log(format, ...) \
    fsemu_log("[FSEMU] [IMAGE] " format, ##__VA_ARGS__)

#endif  // FSEMU_IMAGE_H_
