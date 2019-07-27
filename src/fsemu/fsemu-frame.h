#ifndef FSEMU_FRAME_H_
#define FSEMU_FRAME_H_

#include "fsemu/fsemu-common.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_frame_init(void);

void fsemu_frame_end(void);

int fsemu_frame_counter(void);

int fsemu_frame_counter_mod(int modulus);

void fsemu_frame_update_timing(double hz, bool turbo);

int fsemu_frame_emutime_avg_us(void);

extern int64_t fsemu_frame_begin_at;
extern int64_t fsemu_frame_end_at;
extern int64_t fsemu_frame_wait_duration;
extern int64_t fsemu_frame_emu_duration;
extern int64_t fsemu_frame_sleep_duration;
extern int64_t fsemu_frame_extra_duration;

#ifdef __cplusplus
}
#endif

#define fsemu_frame_log(format, ...) \
    fsemu_log("[FSEMU] [FRAME] " format, ##__VA_ARGS__)

#endif  // FSEMU_FRAME_H_
