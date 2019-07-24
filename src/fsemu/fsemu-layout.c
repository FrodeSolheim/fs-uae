#define FSEMU_INTERNAL
#include "fsemu/fsemu-layout.h"

// FIXME: Maybe we want to remove this dependency and have the
// window depend on/update the layout when it changes size?
#include "fsemu/fsemu-window.h"

static struct {
    int width;
    int height;
    int video_width;
    int video_height;
    // double video_aspect;
    double pixel_aspect;
} fsemu_layout;

static int g_fsemu_client_margins[4];

void fsemu_layout_set_size(int width, int height)
{
    fsemu_layout.width = width;
    fsemu_layout.height = height;
}

void fsemu_layout_set_client_margins(int margins[4])
{
    for (int i = 0; i < 4; i++) {
        g_fsemu_client_margins[0] = margins[0];
    }
}

void fsemu_layout_set_video_size(int width, int height)
{
    fsemu_layout.video_width = width;
    fsemu_layout.video_height = height;
}

#if 0
void fsemu_layout_set_video_aspect(double aspect)
{
    fsemu_layout.video_aspect = aspect;
}
#endif

void fsemu_layout_set_pixel_aspect(double aspect)
{
    fsemu_layout.pixel_aspect = aspect;
}

double fsemu_layout_pixel_aspect(void)
{
    if (fsemu_layout.pixel_aspect == 0) {
        return 1.0;
    }
    return fsemu_layout.pixel_aspect;
}

void fsemu_layout_client_rect(fsemu_rect *rect)
{
    fsemu_size size;
    // fsemu_window_size(&size);
    rect->x = 0;
    rect->y = 0;
    rect->w = fsemu_layout.width;
    rect->h = fsemu_layout.height;
}

void fsemu_layout_video_rect(fsemu_rect *rect)
{
    fsemu_rect client_rect;
    fsemu_layout_client_rect(&client_rect);

    double scale_x = 1.0;
    double scale_y = 1.0;
    // if (fsgs_stretch() == FSGS_STRETCH_FILL_SCREEN) {
    // } else {
    double pixel_aspect = 1.0;
    // if (fsgs_stretch() == FSGS_STRETCH_ASPECT) {
    pixel_aspect = fsemu_layout_pixel_aspect();
    // }
    double initial_aspect = (double) client_rect.w / client_rect.h;
    double aspect =
        ((double) fsemu_layout.video_width / fsemu_layout.video_height) /
        pixel_aspect;
    if (aspect < initial_aspect) {
        scale_x = aspect / initial_aspect;
    } else {
        scale_y = initial_aspect / aspect;
    }
    // }

    rect->w = client_rect.w * scale_x;
    rect->h = client_rect.h * scale_y;
    rect->x = (client_rect.w - rect->w) / 2;
    rect->y = (client_rect.h - rect->h) / 2;

    /*
    fsemu_size size;
    fsemu_window_size(&size);
    rect->x = 0;
    rect->y = 0;
    // rect->w = size.w;
    // rect->h = size.h;
    rect->w = 752;
    rect->h = 572;
    */
}

int fsemu_scale_mode(void)
{
    return FSEMU_MAX_SCALING;
}

bool fsemu_custom_frame(void)
{
    static bool custom_frame = false;
    static int initialized = 0;
    if (!initialized) {
        if (getenv("FSE_CUSTOM_FRAME")) {
            if (getenv("FSE_CUSTOM_FRAME")[0] == '1') {
                custom_frame = 1;
            }
        }
    }
    return custom_frame;
}

int fsemu_stretch_clamp(void)
{
    return 10;
}
