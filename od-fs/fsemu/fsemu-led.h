#ifndef FSEMU_LED_H_
#define FSEMU_LED_H_

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_LED_MAX_STATES 4

typedef enum {
    FSEMU_LED_STATE_OFF,
    FSEMU_LED_STATE_ON,
    FSEMU_LED_STATE_ALT,
} fsemu_led_state_t;

struct fsemu_led_t;
typedef struct fsemu_led_t fsemu_led_t;

void fsemu_led_init(fsemu_led_t* led);

fsemu_led_t* fsemu_led_create(void);

const char* fsemu_led_id(fsemu_led_t* led);

void fsemu_led_set_id(fsemu_led_t* led, const char* id);

const char* fsemu_led_label(fsemu_led_t* led);

void fsemu_led_set_label(fsemu_led_t* led, const char* label);

fsemu_led_state_t fsemu_led_state(fsemu_led_t* led);

void fsemu_led_set_state(fsemu_led_t* led, fsemu_led_state_t state);

// Brightness in percentage

int fsemu_led_brightness(fsemu_led_t* led);

void fsemu_led_set_brightness(fsemu_led_t* led, int brightness);

// Check if the LED has changed since the last time this funciton was called.
// Useful when rendering the LEDs.

bool fsemu_led_check_and_reset_changed(fsemu_led_t* led);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_led_log_level;

#define fsemu_led_log(format, ...) FSEMU_LOG(led, "[FSE] [LED]", format, ##__VA_ARGS__)

#define fsemu_led_log_debug(format, ...) FSEMU_LOG_DEBUG(led, "[FSE] [LED]", format, ##__VA_ARGS__)

#define fsemu_led_log_error(format, ...) FSEMU_LOG_ERROR(led, "[FSE] [LED]", format, ##__VA_ARGS__)

#define fsemu_led_log_info(format, ...) FSEMU_LOG_INFO(led, "[FSE] [LED]", format, ##__VA_ARGS__)

#define fsemu_led_log_trace(format, ...) FSEMU_LOG_TRACE(led, "[FSE] [LED]", format, ##__VA_ARGS__)

#define fsemu_led_log_warning(format, ...) \
    FSEMU_LOG_WARNING(led, "[FSE] [LED]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LED_H_
