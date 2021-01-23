#ifndef FSEMU_LEDS_H_
#define FSEMU_LEDS_H_

#include "fsemu-config.h"

#include "fsemu-led.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_leds_init(void);

fsemu_error_t fsemu_leds_add_led(fsemu_led_t *led);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LEDS_H_
