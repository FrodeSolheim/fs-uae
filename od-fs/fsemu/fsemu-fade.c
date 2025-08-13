#define FSEMU_INTERNAL
#include "fsemu-fade.h"

#include "fsemu-gui.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-video.h"

static struct {
    bool force;
    fsemu_gui_item_t startup_fade_item;
} fsemu_fade;

void fsemu_fade_init(void) {
    fsemu_thread_assert_main();
    fsemu_return_if_already_initialized();

    fsemu_gui_rectangle(&fsemu_fade.startup_fade_item, 0, 0, 1920, 1080, FSEMU_RGB(0x000000));
    fsemu_gui_item_set_visible(&fsemu_fade.startup_fade_item, true);
    fsemu_fade.startup_fade_item.coordinates = FSEMU_COORD_1080P;
    fsemu_fade.startup_fade_item.z_index = 8999;
    fsemu_gui_add_item(&fsemu_fade.startup_fade_item);
}

void fsemu_fade_set_color(uint32_t color) {
    fsemu_fade.startup_fade_item.color = color;
}

#if 0
void fsemu_fade_force(bool force)
{
    fsemu_fade.force = force;
}
#endif

void fsemu_fade_update(void) {
    if (fsemu_fade.force) {
        return;
    }
    // Render fade item over the screen for the first X number of frames.
    // This is to avoid getting flickering (first frames from emulated
    // machine).
#if 1
    static int64_t frame_counter;
    if (frame_counter < 30) {
        frame_counter++;
        return;
    }
#endif
    // Now we gradually fade in
    static int64_t first_update_at;
    int64_t now = fsemu_time_us();
    if (first_update_at == 0) {
        first_update_at = now;
    }
    if (fsemu_fade.startup_fade_item.visible) {
        int64_t dt = now - first_update_at;
        // int alpha = 255 * (1.0 - dt / (1.0 * 1000 * 1000));
        int alpha = 255 * (1.0 - dt / (1.0 * 500 * 1000));
        // printf("alpha %d\n", alpha);
        if (alpha <= 0) {
            fsemu_fade.startup_fade_item.visible = false;
        } else {
            fsemu_color_set_alpha(&fsemu_fade.startup_fade_item.color, alpha);
        }
    }
}
