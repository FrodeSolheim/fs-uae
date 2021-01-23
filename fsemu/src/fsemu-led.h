#ifndef FSEMU_LED_H_
#define FSEMU_LED_H_

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FSEMU_LED_STATE_OFF,
    FSEMU_LED_STATE_ON,
} fsemu_led_state_t;

struct fsemu_led_t;
typedef struct fsemu_led_t fsemu_led_t;

void fsemu_led_init(fsemu_led_t *led);

fsemu_led_t *fsemu_led_create(void);

const char *fsemu_led_id(fsemu_led_t *led);

void fsemu_led_set_id(fsemu_led_t *led, const char *id);

const char *fsemu_led_label(fsemu_led_t *led);

void fsemu_led_set_label(fsemu_led_t *led, const char *label);

fsemu_led_state_t fsemu_led_state(fsemu_led_t *led);

void fsemu_led_set_state(fsemu_led_t *led, fsemu_led_state_t state);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LED_H_
