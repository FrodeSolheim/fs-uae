#define FSEMU_INTERNAL 1
#include "fsemu-leds.h"

#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layer.h"
#include "fsemu-module.h"
#include "fsemu-mouse.h"
#include "fsemu-refable.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-util.h"
#include "fsemu-widget.h"

#define FSEMU_LEDS_MAX_LEDS 16

static struct {
    bool initialized;
    fsemu_widget_t *container_w;
    fsemu_led_t *leds[FSEMU_LEDS_MAX_LEDS];
    int side[FSEMU_LEDS_MAX_LEDS];
    fsemu_image_t *images[FSEMU_LEDS_MAX_LEDS][FSEMU_LED_MAX_STATES];
    // LED container widget
    fsemu_widget_t *led_w[FSEMU_LEDS_MAX_LEDS];
    fsemu_widget_t *label_w[FSEMU_LEDS_MAX_LEDS];
    fsemu_widget_t *base_w[FSEMU_LEDS_MAX_LEDS];
    fsemu_widget_t *overlay_w[FSEMU_LEDS_MAX_LEDS];
    int num_leds;
    int num_leds_side[2];
    // Built-in LEDs
    fsemu_led_t *vsync_led;
    fsemu_led_t *mlock_led;
    fsemu_led_t *klock_led;
} fsemu_leds;

static void fsemu_leds_init_led(int index)
{
    fsemu_widget_t *w;
    fsemu_led_t *led = fsemu_leds.leds[index];
    int side = fsemu_leds.side[index];

    w = fsemu_leds.led_w[index] = fsemu_widget_new_with_name("led-container");
    fsemu_widget_set_color(w, FSEMU_COLOR_TRANSPARENT);
    fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_leds.container_w, w);

    w = fsemu_leds.label_w[index] = fsemu_widget_new_with_name("led-label");
    // fsemu_widget_set_font_name()
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0x777777));
    fsemu_widget_set_font_size(w, 24);
    if (side == FSEMU_LEDS_SIDE_LEFT) {
        fsemu_widget_set_right(w, -60);
        fsemu_widget_set_text_halign(w, 1.0);
    } else {
        fsemu_widget_set_left(w, 60);
    }
    fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_leds.led_w[index], w);

    // fsemu_image_t *image = fsemu_image_load_png
    char *image_name = g_strdup_printf("Images/%s.png", fsemu_led_id(led));
    fsemu_image_t *image = fsemu_image_load(image_name);
    free(image_name);

    w = fsemu_leds.base_w[index] = fsemu_widget_new_with_name("led-base");
    fsemu_widget_set_image(w, image, FSEMU_REF_ASSIGN);
    // fsemu_widget_set_left(w, 100);
    if (side == FSEMU_LEDS_SIDE_LEFT) {
        fsemu_widget_set_left_2(w, -60, FSEMU_WIDGET_PARENT_RIGHT);
    } else {
        fsemu_widget_set_right_2(w, 60, FSEMU_WIDGET_PARENT_LEFT);
    }
    fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_leds.led_w[index], w);

    image_name = g_strdup_printf("Images/%s_On.png", fsemu_led_id(led));
    image = fsemu_image_load(image_name);
    free(image_name);

    fsemu_leds.images[index][1] = image;

    image_name = g_strdup_printf("Images/%s_Alt.png", fsemu_led_id(led));
    image = fsemu_image_load(image_name);
    free(image_name);
    fsemu_leds.images[index][2] = image;

    w = fsemu_leds.overlay_w[index] =
        fsemu_widget_new_with_name("led-overlay");
    // fsemu_widget_set_font_name()
    // fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0xCCCCCC));
    // fsemu_widget_set_font_size(w, 24);

    // FIXME: Add to images instead, and do not assign ref
    // fsemu_widget_set_image(w, image, FSEMU_REF_ASSIGN);

    // fsemu_widget_set_left(w, 100);
    // fsemu_widget_set_right_2(w, 60, FSEMU_WIDGET_PARENT_LEFT);
    if (side == FSEMU_LEDS_SIDE_LEFT) {
        fsemu_widget_set_left_2(w, -60, FSEMU_WIDGET_PARENT_RIGHT);
    } else {
        fsemu_widget_set_right_2(w, 60, FSEMU_WIDGET_PARENT_LEFT);
    }
    // fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_leds.led_w[index], w);
}

static void fsemu_leds_update_led(int index)
{
    fsemu_widget_t *w;
    fsemu_led_t *led = fsemu_leds.leds[index];
    if (fsemu_leds.led_w[index] == NULL) {
        fsemu_leds_init_led(index);
    }
    w = fsemu_leds.label_w[index];
    fsemu_widget_set_text(w, fsemu_led_label(led));
    // printf("LED TEXT -> %s\n", fsemu_led_label(led));
    int state = fsemu_led_state(led);
    w = fsemu_leds.overlay_w[index];
    fsemu_widget_set_visible(w, state > 0);

    if (state == 1) {
        fsemu_widget_set_image(w, fsemu_leds.images[index][1], false);
    } else if (state == 2) {
        fsemu_widget_set_image(w, fsemu_leds.images[index][2], false);
    }

    // fsemu_widget_set_opacity(w, fsemu_led_brightness(led));
    fsemu_widget_set_color(
        w, FSEMU_COLOR_RGB_A(0xFFFFFF, 255 * fsemu_led_brightness(led) / 100));
}

// ----------------------------------------------------------------------------

fsemu_error_t fsemu_leds_add_led_to_side(fsemu_led_t *led, int side)
{
    if (fsemu_leds.num_leds == FSEMU_LEDS_MAX_LEDS) {
        return -1;
    }
    fsemu_assert(side == FSEMU_LEDS_SIDE_LEFT ||
                 side == FSEMU_LEDS_SIDE_RIGHT);
    int led_index = fsemu_leds.num_leds;
    fsemu_leds.leds[led_index] = led;
    fsemu_leds.side[led_index] = side;
    fsemu_leds.num_leds++;
    fsemu_leds.num_leds_side[side]++;

    fsemu_leds_init_led(led_index);
    return 0;
}

fsemu_error_t fsemu_leds_add_led(fsemu_led_t *led)
{
    return fsemu_leds_add_led_to_side(led, FSEMU_LEDS_SIDE_RIGHT);
}

// ----------------------------------------------------------------------------

void fsemu_leds_update(void)
{
    // Update the state of default lets

    if (fsemu_led_state(fsemu_leds.vsync_led) == 1) {
        fsemu_led_set_state(fsemu_leds.vsync_led, 2);
    } else {
        fsemu_led_set_state(fsemu_leds.vsync_led, 1);
    }

    fsemu_led_set_state(fsemu_leds.mlock_led, fsemu_mouse_captured());
    // FIXME: Replace fsemu_sdlwindow_full_keyboard_emulation()
    fsemu_led_set_state(fsemu_leds.klock_led,
                        fsemu_sdlwindow_full_keyboard_emulation());

    // double left = 1920 - 200;
    // FIXME: There is some bug with calculating image position relative to
    // "screen"; try with top = 0 and see.

    // Vertically center LEDs (calculate top position of first LED).
    double height[2];
    height[0] = fsemu_leds.num_leds_side[0] * 60;
    height[1] = fsemu_leds.num_leds_side[1] * 60;
    double top[2];
    top[0] = (1080 - height[0]) / 2;
    top[1] = (1080 - height[1]) / 2;

    for (int i = 0; i < fsemu_leds.num_leds; i++) {
        fsemu_led_t *led = fsemu_leds.leds[i];
        if (fsemu_led_check_and_reset_changed(led)) {
            fsemu_leds_update_led(i);
        }
        int side = fsemu_leds.side[i];

        // FIXME: For later; we want to anchor this to the right of the video
        // rectangle.
        if (side == FSEMU_LEDS_SIDE_LEFT) {
            fsemu_widget_set_right_2(
                fsemu_leds.led_w[i], 200, FSEMU_WIDGET_PARENT_LEFT);
        } else {
            fsemu_widget_set_left_2(
                fsemu_leds.led_w[i], -200, FSEMU_WIDGET_PARENT_RIGHT);
        }
        // fsemu_widget_set_right(fsemu_leds.led_w[i], left + 200);
        fsemu_widget_set_top(fsemu_leds.led_w[i], top[side]);
        fsemu_widget_set_bottom(fsemu_leds.led_w[i], top[side] + 60);
        top[side] += 60;
    }
}

// ----------------------------------------------------------------------------

static void fsemu_leds_quit(void)
{
}

// ----------------------------------------------------------------------------

#define FSEMU_OPTION_PERFORMANCE_GUI_DEFAULT 0

void fsemu_leds_init(void)
{
    fsemu_widget_t *w;

    if (FSEMU_MODULE_INIT(leds)) {
        return;
    }

    w = fsemu_leds.container_w = fsemu_widget_new_with_name("leds-container");
    // FIXME: Make sure that this is optimized and not attempted rendering.
    fsemu_widget_set_color(w, FSEMU_COLOR_TRANSPARENT);
    // FIXME: Check what z-index to use (below video)

    fsemu_widget_set_left_2(w, 0, FSEMU_WIDGET_SCREEN_LEFT);
    // fsemu_widget_set_top_2(w, 0, FSEMU_WIDGET_SCREEN_TOP);
    fsemu_widget_set_right_2(w, 0, FSEMU_WIDGET_SCREEN_RIGHT);
    // fsemu_widget_set_bottom_2(w, 0, FSEMU_WIDGET_SCREEN_BOTTOM);
    fsemu_widget_set_z_index(w, FSEMU_LAYER_LEDS);
    fsemu_widget_set_visible(w, true);
    // fsemu_widget_set_right(w,
    fsemu_gui_add_item(w);

    fsemu_leds.vsync_led = fsemu_led_create();
    fsemu_led_set_id(fsemu_leds.vsync_led, "VSyncLed");
    fsemu_led_set_label(fsemu_leds.vsync_led, "VSYNC");
    fsemu_leds_add_led_to_side(fsemu_leds.vsync_led, FSEMU_LEDS_SIDE_LEFT);

    fsemu_leds.mlock_led = fsemu_led_create();
    fsemu_led_set_id(fsemu_leds.mlock_led, "MLockLed");
    // fsemu_led_set_label(fsemu_leds.mlock_led, "M.LOCK");
    fsemu_led_set_label(fsemu_leds.mlock_led, "MOUSE");
    fsemu_leds_add_led_to_side(fsemu_leds.mlock_led, FSEMU_LEDS_SIDE_LEFT);

    fsemu_leds.klock_led = fsemu_led_create();
    fsemu_led_set_id(fsemu_leds.klock_led, "KLockLed");
    fsemu_led_set_label(fsemu_leds.klock_led, "K.LOCK");
    // fsemu_led_set_label(fsemu_leds.klock_led, "KEYLOCK");
    fsemu_leds_add_led_to_side(fsemu_leds.klock_led, FSEMU_LEDS_SIDE_LEFT);
}
