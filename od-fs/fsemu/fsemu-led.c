#define FSEMU_INTERNAL
#include "fsemu-led.h"

#include <stddef.h>
#include <string.h>

#include "fsemu-util.h"

// FIXME: DEBUG -> INFO;
int fsemu_led_log_level = FSEMU_LOG_LEVEL_DEBUG;

struct fsemu_led_t {
    char *id;
    char *label;
    fsemu_led_state_t state;
    int brightness;
    bool changed;
};

void fsemu_led_init(fsemu_led_t *led)
{
    // Memory might not be initialized to zeros, all fields must be
    // initialized here.
    led->id = strdup("");
    led->label = strdup("");
    led->state = 0;
    led->brightness = 100;
    // Set changed to one initially, useful for check in renderer.
    led->changed = 1;
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
    if (strcmp(id, led->id) == 0) {
        return;
    }
    // if (led->id) {
    free(led->id);
    // }
    led->id = strdup(id);
    led->changed = true;
}

const char *fsemu_led_label(fsemu_led_t *led)
{
    return led->label;
}

void fsemu_led_set_label(fsemu_led_t *led, const char *label)
{
    fsemu_assert(label != NULL);
    if (strcmp(label, led->label) == 0) {
        return;
    }
    // if (led->label) {
    free(led->label);
    // }
    led->label = strdup(label);
    led->changed = true;
}

fsemu_led_state_t fsemu_led_state(fsemu_led_t *led)
{
    return led->state;
}

void fsemu_led_set_state(fsemu_led_t *led, fsemu_led_state_t state)
{
    if (state == led->state) {
        return;
    }
    fsemu_assert(state >= 0 && state < FSEMU_LED_MAX_STATES);
    led->state = state;
    // printf("LED \"%s\" -> state %d\n", led->label, state);
    led->changed = true;
}

int fsemu_led_brightness(fsemu_led_t *led)
{
    return led->brightness;
}

void fsemu_led_set_brightness(fsemu_led_t *led, int brightness)
{
    if (brightness == led->brightness) {
        return;
    }
    led->brightness = brightness;
    // printf("LED \"%s\" -> brightness %d\n", led->label, brightness);
    led->changed = true;
}

bool fsemu_led_check_and_reset_changed(fsemu_led_t *led)
{
    if (led->changed) {
        led->changed = false;
        return true;
    }
    return false;
}
