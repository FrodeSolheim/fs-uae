#ifndef FSEMU_RECORDING_H_
#define FSEMU_RECORDING_H_

#include <stdint.h>

#include "fsemu-config.h"
#include "fsemu-log.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t (*fsemu_recording_checksum_function_t)(void);
typedef uint32_t (*fsemu_recording_rand_function_t)(void);

void fsemu_recording_init(void);
void fsemu_recording_update(void);
void fsemu_recording_shutdown(void);

bool fsemu_recording_is_writable(void);
void fsemu_recording_toggle_writable(void);

bool fsemu_recording_is_playing(void);
bool fsemu_recording_is_recording(void);
bool fsemu_recording_is_stopped(void);
bool fsemu_recording_is_sync(void);
bool fsemu_recording_is_desync(void);
int fsemu_recording_frame_number(void);

bool fsemu_recording_enabled(void);

void fsemu_recording_set_next_frame_number(int frame_number);

// void fsemu_recording_begin_frame(int frame_number);
// void fsemu_recording_end_frame(int frame_number);
void fsemu_recording_begin_frame();
void fsemu_recording_end_frame();

bool fsemu_recording_next_action(int line, uint16_t *action, int16_t *state);

void fsemu_recording_set_checksum_function(
    fsemu_recording_checksum_function_t function);

void fsemu_recording_set_rand_function(
    fsemu_recording_rand_function_t function);

#ifdef FSEMU_INTERNAL

void fsemu_recording_on_load_state_finished(int slot, const char *path);

void fsemu_recording_on_save_state_finished(int slot, const char *path);

#endif  // FSEMU_INTERNAL

extern int fsemu_recording_log_level;

#define fsemu_recording_log(format, ...)                             \
    if (fsemu_likely(fsemu_recording_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSEMU] [ REC ] " format, ##__VA_ARGS__);         \
    }

#define fsemu_recording_log_debug(format, ...)                          \
    if (fsemu_unlikely(fsemu_recording_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[FSEMU] [ REC ] " format, ##__VA_ARGS__);            \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_RECORDING_H_
