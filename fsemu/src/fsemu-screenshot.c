#include "fsemu-internal.h"
#include "fsemu-screenshot.h"

#include <time.h>

#include "fsemu-config.h"
#include "fsemu-log.h"
#include "fsemu-util.h"

static struct {
    bool capture;
    // int crop[4];
} fsemu_screenshot;

void fsemu_screenshot_capture(void)
{
    fsemu_screenshot.capture = true;
}

bool fsemu_screenshot_should_capture(void)
{
    return fsemu_screenshot.capture;
}

void fsemu_screenshot_confirm_captured(void)
{
    fsemu_screenshot.capture = false;
}

/*
int fsemu_screenshot_crop(int *x, int *y, int *w, int *h)
{
    *x = fsemu_screenshot.crop[0];
    *y = fsemu_screenshot.crop[1];
    *w = fsemu_screenshot.crop[2];
    *h = fsemu_screenshot.crop[3];
    return (fsemu_screenshot.crop[2] && fsemu_screenshot.crop[3]);
}

void fsemu_screenshot_set_crop(int x, int y, int w, int h)
{
    fsemu_screenshot.crop[0] = x;
    fsemu_screenshot.crop[1] = y;
    fsemu_screenshot.crop[2] = w;
    fsemu_screenshot.crop[3] = h;
}
*/

static const char *fsemu_screenshots_dir(void)
{
    const char *screenshots_dir = NULL;
    if (screenshots_dir != NULL) {
        return screenshots_dir;
    }
    if (fsemu_getenv("FSEMU_SCREENSHOTS_DIR")[0]) {
        screenshots_dir = fsemu_getenv("FSEMU_SCREENSHOTS_DIR");
    } else {
        screenshots_dir = "/tmp";
    }
    return screenshots_dir;
}

static const char *fsemu_screenshots_base(void)
{
    const char *screenshots_base = NULL;
    if (screenshots_base != NULL) {
        return screenshots_base;
    }
    if (fsemu_getenv("FSEMU_SCREENSHOTS_BASE")[0]) {
        screenshots_base = fsemu_getenv("FSEMU_SCREENSHOTS_BASE");
    } else {
        screenshots_base = "screenshot";
    }
    return screenshots_base;
}

const char *fsemu_screenshot_path(void)
{
    return fsemu_screenshot_path_for_type("");
}

const char *fsemu_screenshot_path_for_type(const char *type)
{
    static int counter = 1;
    static char buffer[FSEMU_PATH_MAX];
    const char *base = fsemu_screenshots_base();
    const char *dir = fsemu_screenshots_dir();
    time_t tm1;
    time(&tm1);
    struct tm *tm2 = localtime(&tm1);
    static char time_buffer[64];
    strftime(time_buffer, 64 - 1, "%y%m%d%H%M", tm2);
    time_buffer[64 - 1] = '\0';
    while (1) {
        if (type && type[0]) {
            snprintf(buffer,
                     FSEMU_PATH_MAX - 1,
                     "%s/%s-%s-%s-%02d.png",
                     dir,
                     base,
                     type,
                     time_buffer,
                     counter);
        } else {
            snprintf(buffer,
                     FSEMU_PATH_MAX - 1,
                     "%s/%s-%s-%02d.png",
                     dir,
                     base,
                     time_buffer,
                     counter);
        }
        buffer[FSEMU_PATH_MAX - 1] = '\0';
        // FIXME: Ideally, check if the file exists and repeat, not
        // using static counter
        counter += 1;
        break;
    }
    printf("[FSEMU] [SSHOT] Screenshot: %s\n", buffer);
    return buffer;
}
