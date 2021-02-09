// ----------------------------------------------------------------------------
// FSEMU library; Copyright (c) 2011-2021 Frode Solheim. See the file COPYING.
// ----------------------------------------------------------------------------

#ifndef FSEMU_PERFGUI_H_
#define FSEMU_PERFGUI_H_

#include "fsemu-config.h"
#include "fsemu-image.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_perfgui_init(void);

void fsemu_perfgui_update(void);

int fsemu_perfgui_cycle_mode(void);

#ifdef FSEMU_INTERNAL

typedef enum {
    FSEMU_PERFGUI_MODE_OFF,
    FSEMU_PERFGUI_MODE_GRAYSCALE,
    FSEMU_PERFGUI_MODE_COLOR
} fsemu_perfgui_mode_t;

#define FSEMU_PERFGUI_IMAGE_WIDTH 256
#define FSEMU_PERFGUI_IMAGE_HEIGHT 512

fsemu_perfgui_mode_t fsemu_perfgui_mode(void);

void fsemu_perfgui_update_image(void);

fsemu_image_t *fsemu_perfgui_image(void);

#endif  // FSEMU_INTERNAL

extern int fsemu_perfgui_log_level;

#define fsemu_perfgui_log(format, ...)                             \
    if (fsemu_likely(fsemu_perfgui_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSE] [PFG] " format, ##__VA_ARGS__);           \
    }

#define fsemu_perfgui_log_debug(format, ...)                          \
    if (fsemu_unlikely(fsemu_perfgui_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log_debug("[FSE] [PFG] " format, ##__VA_ARGS__);        \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_PERFGUI_H_
