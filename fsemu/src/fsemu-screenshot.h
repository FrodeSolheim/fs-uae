#ifndef FSEMU_SCREENSHOT_H_
#define FSEMU_SCREENSHOT_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_screenshot_capture(void);
bool fsemu_screenshot_should_capture(void);
void fsemu_screenshot_confirm_captured(void);

// Maybe
// int fsemu_screenshot_crop(int *x, int *y, int *w, int *h):
// void fsemu_screenshot_set_crop(int x, int y, int w, int h);

const char *fsemu_screenshot_path(void);
const char *fsemu_screenshot_path_for_type(const char *type);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SCREENSHOT_H_
