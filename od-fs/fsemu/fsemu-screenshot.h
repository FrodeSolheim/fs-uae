#ifndef FSEMU_SCREENSHOT_H
#define FSEMU_SCREENSHOT_H

#include <stdbool.h>

#include "fsemu-video.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_screenshot_init(void);

const char *fsemu_screenshot_dir(void);

void fsemu_screenshot_capture(void);
bool fsemu_screenshot_should_capture(void);
void fsemu_screenshot_confirm_captured(void);

// Maybe
// int fsemu_screenshot_crop(int *x, int *y, int *w, int *h):
// void fsemu_screenshot_set_crop(int x, int y, int w, int h);

const char *fsemu_screenshot_path(void);
const char *fsemu_screenshot_path_for_type(const char *type);

void fsemu_screenshot_capture_video_frame(fsemu_video_frame_t *frame);

#ifdef FSEMU_INTERNAL

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_screenshot_log_level;

#define fsemu_screenshot_log(format, ...)                                                          \
    FS_LOG_INFO(fsemu_screenshot_log_level, "[FSE] [SHT]", format, ##__VA_ARGS__)

#define fsemu_screenshot_log_debug(format, ...)                                                    \
    FS_LOG_DEBUG(fsemu_screenshot_log_level, "[FSE] [SHT]", format, ##__VA_ARGS__)

#define fsemu_screenshot_log_error(format, ...)                                                    \
    FS_LOG_ERROR(fsemu_screenshot_log_level, "[FSE] [SHT]", format, ##__VA_ARGS__)

#define fsemu_screenshot_log_info(format, ...)                                                     \
    FS_LOG_INFO(fsemu_screenshot_log_level, "[FSE] [SHT]", format, ##__VA_ARGS__)

#define fsemu_screenshot_log_trace(format, ...)                                                    \
    FS_LOG_TRACE(fsemu_screenshot_log_level, "[FSE] [SHT]", format, ##__VA_ARGS__)

#define fsemu_screenshot_log_warning(format, ...)                                                  \
    FS_LOG_WARNING(fsemu_screenshot_log_level, "[FSE] [SHT]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_SCREENSHOT_H */
