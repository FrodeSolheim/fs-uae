#ifndef FSEMU_LEDS_H_
#define FSEMU_LEDS_H_

#include "fsemu-config.h"
#include "fsemu-led.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_leds_init(void);

fsemu_error_t fsemu_leds_add_led(fsemu_led_t *led);

enum {
    FSEMU_LEDS_SIDE_LEFT,
    FSEMU_LEDS_SIDE_RIGHT,
};

fsemu_error_t fsemu_leds_add_led_to_side(fsemu_led_t *led, int side);

void fsemu_leds_update(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LEDS_H_
