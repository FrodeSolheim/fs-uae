#define FSEMU_INTERNAL
#include "fsemu-helpbar.h"

#include "fsemu-layer.h"
#include "fsemu-module.h"
#include "fsemu-widget.h"

static struct {
    bool initialized;
    fsemu_widget_t* container_w;
} fsemu_helpbar;

// ----------------------------------------------------------------------------

void fsemu_helpbar_update(void) {
}

// ----------------------------------------------------------------------------

static void fsemu_helpbar_quit(void) {
}

// ----------------------------------------------------------------------------

void fsemu_helpbar_init(void) {
    fsemu_widget_t* w;

    if (FSEMU_MODULE_INIT(helpbar)) {
        return;
    }

    w = fsemu_helpbar.container_w = fsemu_widget_new_with_name("leds-container");
    // FIXME: Make sure that this is optimized and not attempted rendering.
    fsemu_widget_set_color(w, FSEMU_COLOR_TRANSPARENT);
    // FIXME: Check what z-index to use (below video)

    fsemu_widget_set_left_2(w, 0, FSEMU_WIDGET_SCREEN_LEFT);
    // fsemu_widget_set_top_2(w, 0, FSEMU_WIDGET_SCREEN_TOP);
    fsemu_widget_set_right_2(w, 0, FSEMU_WIDGET_SCREEN_RIGHT);
    // fsemu_widget_set_bottom_2(w, 0, FSEMU_WIDGET_SCREEN_BOTTOM);
    fsemu_widget_set_z_index(w, FSEMU_LAYER_HELPBAR);
    fsemu_widget_set_visible(w, true);
    // fsemu_widget_set_right(w,
    fsemu_gui_add_item(w);

    // fsemu_leds.vsync_led = fsemu_led_create();
    // fsemu_led_set_id(fsemu_leds.vsync_led, "VSyncLed");
    // fsemu_led_set_label(fsemu_leds.vsync_led, "VSYNC");
    // fsemu_leds_add_led_to_side(fsemu_leds.vsync_led, FSEMU_LEDS_SIDE_LEFT);

    // fsemu_leds.mlock_led = fsemu_led_create();
    // fsemu_led_set_id(fsemu_leds.mlock_led, "MLockLed");
    // fsemu_led_set_label(fsemu_leds.mlock_led, "M.LOCK");
    // fsemu_leds_add_led_to_side(fsemu_leds.mlock_led, FSEMU_LEDS_SIDE_LEFT);

    // fsemu_leds.klock_led = fsemu_led_create();
    // fsemu_led_set_id(fsemu_leds.klock_led, "KLockLed");
    // fsemu_led_set_label(fsemu_leds.klock_led, "K.LOCK");
    // fsemu_leds_add_led_to_side(fsemu_leds.klock_led, FSEMU_LEDS_SIDE_LEFT);
}
