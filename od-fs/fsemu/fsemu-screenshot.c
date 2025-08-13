#define FSEMU_INTERNAL
#include "fsemu-screenshot.h"

#include <time.h>

#include "fsemu-application.h"
#include "fsemu-config.h"
#include "fsemu-glib.h"
#include "fsemu-hud.h"
#include "fsemu-log.h"
#include "fsemu-module.h"
#include "fsemu-mutex.h"
#include "fsemu-option.h"
#include "fsemu-util.h"

int fsemu_screenshot_log_level = FSEMU_LOG_LEVEL_INFO;

static struct {
    bool initialized;
    bool capture;
    // int crop[4];
    char* prefix;
    char* screenshots_dir;
    char* time_str;
    int last_counter;
    fsemu_mutex_t* mutex;
} fsemu_screenshot;

static void fsemu_screenshot_lock(void) {
    fsemu_assert(fsemu_screenshot.initialized);
    fsemu_mutex_lock(fsemu_screenshot.mutex);
}

static void fsemu_screenshot_unlock(void) {
    fsemu_mutex_unlock(fsemu_screenshot.mutex);
}

void fsemu_screenshot_capture(void) {
    fsemu_screenshot_log("Scheduling screenshot capture\n");
    fsemu_screenshot_lock();

    fsemu_screenshot.capture = true;
    time_t tm1;
    time(&tm1);
    struct tm* tm2 = localtime(&tm1);
    static char time_buffer[64];
    strftime(time_buffer, 64 - 1, "%y%m%d-%H%M", tm2);
    time_buffer[64 - 1] = '\0';

    if (fsemu_screenshot.time_str) {
        free(fsemu_screenshot.time_str);
    }
    fsemu_screenshot.time_str = strdup(time_buffer);

    fsemu_screenshot_unlock();
}

bool fsemu_screenshot_should_capture(void) {
    return fsemu_screenshot.capture;
}

void fsemu_screenshot_confirm_captured(void) {
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

/*
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
*/
static const char* fsemu_screenshot_prefix(void) {
    fsemu_assert(fsemu_screenshot.initialized);

    if (fsemu_screenshot.prefix == NULL) {
        if (fsemu_getenv("FSEMU_SCREENSHOTS_PREFIX")[0]) {
            fsemu_screenshot.prefix = strdup(fsemu_getenv("FSEMU_SCREENSHOTS_PREFIX"));
        } else if (fsemu_application_name()) {
            fsemu_screenshot.prefix = strdup(fsemu_application_name());
        } else {
            fsemu_screenshot.prefix = strdup("FS-Emu");
        }
    }
    //     return screenshots_base;
    // }
    // if (fsemu_getenv("FSEMU_SCREENSHOTS_prefix")[0]) {
    //     screenshots_base = fsemu_getenv("FSEMU_SCREENSHOTS_prefix");
    // } else {
    //     screenshots_base = "screenshot";
    // }

    return fsemu_screenshot.prefix;
}

const char* fsemu_screenshot_path(void) {
    return fsemu_screenshot_path_for_type("");
}

const char* fsemu_screenshot_dir(void) {
    fsemu_assert(fsemu_screenshot.initialized);

    if (fsemu_screenshot.screenshots_dir == NULL) {
        if (fsemu_getenv("FSEMU_SCREENSHOTS_DIR")[0]) {
            fsemu_screenshot.screenshots_dir = strdup(fsemu_getenv("FSEMU_SCREENSHOTS_DIR"));
        } else {
            const char* app_base_dir = fsemu_application_base_dir();
            // const char *app_data_dir = fsemu_application_data_dir();
            fsemu_screenshot.screenshots_dir = g_build_filename(app_base_dir, "Screenshots", NULL);
        }
        fsemu_screenshot_log("Using screenshots directory: %s\n", fsemu_screenshot.screenshots_dir);

        if (!g_file_test(fsemu_screenshot.screenshots_dir, G_FILE_TEST_IS_DIR)) {
            int error = g_mkdir_with_parents(fsemu_screenshot.screenshots_dir, 0755);
            if (error) {
                fsemu_application_log_warning("Could not create screenshots directory %s\n",
                                              fsemu_screenshot.screenshots_dir);
            }
        }
    }

    return fsemu_screenshot.screenshots_dir;
}

// FIXME: Deprecated
const char* fsemu_screenshot_path_for_type(const char* type) {
    int counter = 0;
    static char buffer[FSEMU_PATH_MAX];
    const char* base = fsemu_screenshot_prefix();
    const char* dir = fsemu_screenshot_dir();
    // time_t tm1;
    // time(&tm1);
    // struct tm *tm2 = localtime(&tm1);
    // static char time_buffer[64];
    // strftime(time_buffer, 64 - 1, "%y%m%d%H%M", tm2);
    // time_buffer[64 - 1] = '\0';
    while (true) {
        if (type && type[0]) {
            snprintf(buffer, FSEMU_PATH_MAX - 1, "%s/%s_%s_%s_%02d.png", dir, base, type,
                     fsemu_screenshot.time_str, counter);
        } else {
            snprintf(buffer, FSEMU_PATH_MAX - 1, "%s/%s_%s_%02d.png", dir, base,
                     fsemu_screenshot.time_str, counter);
        }
        buffer[FSEMU_PATH_MAX - 1] = '\0';
        if (g_file_test(buffer, G_FILE_TEST_EXISTS)) {
            counter += 1;
        } else {
            break;
        }
    }
    fsemu_screenshot.last_counter = counter;
    fsemu_screenshot_log("Screenshot: %s\n", buffer);
    return buffer;
}

// FIXME: Move to fsemu-video
static SDL_Surface* fsemu_video_surface_from_frame(fsemu_video_frame_t* frame) {
    printf("depth: %d\n", frame->depth);
    fsemu_assert(frame->depth == 16 || frame->depth == 32);
    // FIXME: Get pitch from frame?
    int pitch = frame->width * frame->depth / 8;
    // FIXME: We should definitively have frame->format!
    Uint32 format;
    if (frame->depth == 16) {
        format = SDL_PIXELFORMAT_RGB565;
    } else if (frame->depth == 32) {
#ifdef FSEMU_CPU_BIGENDIAN
        uint32_t set_alpha = 0x000000ff;
#else
        uint32_t set_alpha = 0xff000000;
#endif
        if (fsemu_video_format() == FSEMU_VIDEO_FORMAT_BGRA) {
            format = SDL_PIXELFORMAT_BGRA32;
        } else {
            format = SDL_PIXELFORMAT_RGBA32;
        }
        // We're really using RGBx, not RGBA, so make sure to set alpha value
        // to full intensity.
        uint32_t* pixels = (uint32_t*)frame->buffer;
        for (int y = 0; y < frame->height; y++) {
            for (int x = 0; x < frame->width; x++) {
                pixels[x] |= set_alpha;
            }
            pixels += pitch / 4;
        }
    } else {
        fsemu_assert(false);
        // Should not happen, depth should be either 16 or 32
        return NULL;
    }

    printf("width %d height %d depth %d pitch %d format %d\n", frame->width, frame->height,
           frame->depth, pitch, format);
    SDL_Surface* surface =
        SDL_CreateSurfaceFrom(frame->width, frame->height, format, frame->buffer, pitch);
    return surface;
}

// ----------------------------------------------------------------------

void fsemu_screenshot_capture_video_frame(fsemu_video_frame_t* frame) {
    fsemu_screenshot_lock();

    // For simplicity (?) we always generate 32-bit PNG screenshots
    SDL_Surface* dst = SDL_CreateSurface(frame->width, frame->height, SDL_PIXELFORMAT_RGBA32);
    // SDL_Surface *dst = SDL_CreateRGBSurfaceWithFormat(
    //      0, frame->width, frame->height, 32, SDL_PIXELFORMAT_RGBX8888);
    SDL_Surface* src = fsemu_video_surface_from_frame(frame);
    printf("src %p\n", src);
    SDL_Rect rect = {0, 0, frame->width, frame->height};
    SDL_BlitSurface(src, &rect, dst, &rect);

    const char* path = fsemu_screenshot_path_for_type("Full");
    fsemu_image_save_png_file_from_rgba_data(path, dst->pixels, frame->width, frame->height);
    SDL_DestroySurface(src);
    SDL_DestroySurface(dst);

    char buffer[32];
    g_snprintf(buffer, 32, "%s_%02d", fsemu_screenshot.time_str, fsemu_screenshot.last_counter);
    fsemu_hud_notify(FSEMU_HUD_ID("SSHOTTED"), "camera", "Screenshot captured", buffer);

    fsemu_screenshot.capture = false;
    free(fsemu_screenshot.time_str);
    fsemu_screenshot.time_str = NULL;
    fsemu_screenshot_unlock();
}

// ----------------------------------------------------------------------

static void fsemu_screenshot_quit(void) {
}

// ----------------------------------------------------------------------------

void fsemu_screenshot_init(void) {
    if (FSEMU_MODULE_INIT(screenshot)) {
        return;
    }
    fsemu_application_init();

    fsemu_screenshot.mutex = fsemu_mutex_create();

    const char* prefix = fsemu_option_const_string(FSEMU_OPTION_SCREENSHOTS_OUTPUT_PREFIX);
    if (prefix) {
        fsemu_screenshot.prefix = g_strdup(prefix);
    } else {
#ifdef FSUAE
        fsemu_screenshot.prefix = strdup("FS-UAE");
#else
        fsemu_screenshot.prefix = strdup("FSEMU");
#endif
    }

    // if (fsemu_getenv("FSEMU_SCREENSHOTS_DIR")[0]) {
    //     fsemu_screenshot.screenshots_dir =
    //         strdup(fsemu_getenv("FSEMU_SCREENSHOTS_DIR"));
    // } else {
    //     const char *app_base_dir = fsemu_application_base_dir();
    //     // const char *app_data_dir = fsemu_application_data_dir();
    //     fsemu_screenshot.screenshots_dir =
    //         g_build_filename(app_base_dir, "Screenshots", NULL);
    // }
    // fsemu_screenshot_log("Using screenshots directory: %s\n",
    //                      fsemu_screenshot.screenshots_dir);
}
