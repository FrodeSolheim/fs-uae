#define FSEMU_INTERNAL 1
#include "fsemu-led.h"

#include <stddef.h>
#include <string.h>

#include "fsemu-util.h"

struct fsemu_led_t {
    char *id;
    char *label;
    fsemu_led_state_t state;
};

void fsemu_led_init(fsemu_led_t *led)
{
    fsemu_led_set_id(led, "");
    fsemu_led_set_label(led, "");
}

fsemu_led_t *fsemu_led_create(void)
{
    fsemu_led_t *led = FSEMU_UTIL_MALLOC(fsemu_led_t);
    fsemu_led_init(led);
    return led;
}

const char *fsemu_led_id(fsemu_led_t *led)
{
    return led->id;
}

void fsemu_led_set_id(fsemu_led_t *led, const char *id)
{
    fsemu_assert(id != NULL);
    if (led->id) {
        free(led->id);
    }
    led->id = strdup(id);
}

const char *fsemu_led_label(fsemu_led_t *led)
{
    return led->label;
}

void fsemu_led_set_label(fsemu_led_t *led, const char *label)
{
    fsemu_assert(label != NULL);
    if (led->label) {
        free(led->label);
    }
    led->label = strdup(label);
}

fsemu_led_state_t fsemu_led_state(fsemu_led_t *led)
{
    return led->state;
}

void fsemu_led_set_state(fsemu_led_t *led, fsemu_led_state_t state)
{
    led->state = state;
}
