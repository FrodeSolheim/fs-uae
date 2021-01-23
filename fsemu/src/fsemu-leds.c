#define FSEMU_INTERNAL 1
#include "fsemu-leds.h"

#include "fsemu-module.h"

#define FSEMU_LEDS_MAX_LEDS 16

static struct {
    bool initialized;
    fsemu_led_t *leds[FSEMU_LEDS_MAX_LEDS];
    int num_leds;
} fsemu_leds;

// ----------------------------------------------------------------------------

fsemu_error_t fsemu_leds_add_led(fsemu_led_t *led)
{
    if (fsemu_leds.num_leds == FSEMU_LEDS_MAX_LEDS) {
        return -1;
    }
    fsemu_leds.leds[fsemu_leds.num_leds] = led;
    fsemu_leds.num_leds ++;
    return 0;
}

// ----------------------------------------------------------------------------

static void fsemu_leds_quit(void)
{
}

// ----------------------------------------------------------------------------

#define FSEMU_OPTION_PERFORMANCE_GUI_DEFAULT 0

void fsemu_leds_init(void)
{
    if (FSEMU_MODULE_INIT(leds)) {
        return;
    }
}
