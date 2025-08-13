#ifndef FSEMU_FRAME_H_
#define FSEMU_FRAME_H_

#include <stdint.h>

#include "fsemu-config.h"
#include "fsemu-time.h"

// FIXME: Check / define order of
// - fsemu_frame_end
// - fsemu_frame_update_timing
// - and the actual posting of frames
// (making sure frame numbers and stats are consistent)

#ifdef __cplusplus
extern "C" {
#endif

int64_t fsemu_frame_epoch(void);
void fsemu_frame_reset_epoch(void);

double fsemu_frame_rate_multiplier(void);

bool fsemu_frame_paused(void);
bool fsemu_frame_warping(void);

bool fsemu_frame_check_load_state(int* slot);
bool fsemu_frame_check_save_state(int* slot);

// Affects when the frame starts emulating.
// FIXME: In use?
void fsemu_frame_set_emulation_duration_us(int emulation_duration_us);

// Affects when the frame starts rendering before vsync.
// FIXME: In use?
void fsemu_frame_set_render_duration_us(int render_duration_us);

void fsemu_frame_wait_until(int64_t until_us);

// Registers the currently emulated frame as done.
void fsemu_frame_end(void);

// Returns the current frame number.
// FIXME: Rename to fsemu_frame_no?
int fsemu_frame_counter(void);

int fsemu_frame_counter_mod(int modulus);

// It is important to use as close to real (emulated) Hz as possible here, so
// that the audio system knows how much data to expect and can correct if
// video output rate deviates from the emulated frame rate.
void fsemu_frame_start(double hz);

// Returns the frame rate as given to fsemu_frame_start. Should probably only
// be called from the same thread as fsemu_frame_start.
double fsemu_frame_rate_hz(void);

// Deprecated; use fsemu_frame_start instead
// void fsemu_frame_update_timing(double hz, bool turbo);

int fsemu_frame_emutime_avg_us(void);

void fsemu_frame_reset_timer(int64_t t);

void fsemu_frame_toggle_sleep_busywait(void);
void fsemu_frame_wait_until(int64_t until_us);
void fsemu_frame_wait_until_frame_end(void);

void fsemu_frame_add_overshoot_time(int64_t t);
void fsemu_frame_add_framewait_time(int64_t t);
void fsemu_frame_add_gui_time(int64_t t);
void fsemu_frame_add_emulation_time(int64_t t);
void fsemu_frame_add_render_time(int64_t t);
void fsemu_frame_add_sleep_time(int64_t t);
void fsemu_frame_add_extra_time(int64_t t);

extern double fsemu_frame_hz;
// extern bool fsemu_frame_warp;

extern int64_t fsemu_frame_epoch_at;
extern int64_t fsemu_frame_origin_at;

extern int64_t fsemu_frame_begin_at;
extern int64_t fsemu_frame_end_at;

extern int64_t fsemu_frame_overshoot_duration;
extern int64_t fsemu_frame_wait_duration;
extern int64_t fsemu_frame_gui_duration;
extern int64_t fsemu_frame_emu_duration;
extern int64_t fsemu_frame_render_duration;
extern int64_t fsemu_frame_sleep_duration;
extern int64_t fsemu_frame_extra_duration;

// void fsemu_frame_set_began_at(int frame, int64_t began_at);
// void fsemu_frame_set_rendered_at(int frame, int64_t rendered_at);
// void fsemu_frame_set_swapped_at(int frame, int64_t swapped_at);

#ifdef FSEMU_INTERNAL

// Initializes the frame module (not a single frame).
void fsemu_frame_init(void);

extern volatile int fsemu_frame_number_began;
extern volatile int fsemu_frame_number_ended;
extern volatile int fsemu_frame_number_posted;
extern volatile int fsemu_frame_number_rendering;
extern volatile int fsemu_frame_number_rendered;
extern volatile int fsemu_frame_number_displaying;
extern volatile int fsemu_frame_number_displayed;
extern volatile int fsemu_frame_number_swapped;

#endif  // FSEMU_INTERNAL

extern int fsemu_frame_log_level;

#define fsemu_frame_log(format, ...)                     \
    if (fsemu_frame_log_level >= FSEMU_LOG_LEVEL_INFO) { \
        fsemu_log("[FSE] [FRM] " format, ##__VA_ARGS__); \
    }

#define fsemu_frame_log_trace(format, ...)                \
    if (fsemu_frame_log_level >= FSEMU_LOG_LEVEL_TRACE) { \
        fsemu_log("[FSE] [FRM] " format, ##__VA_ARGS__);  \
    }

#define fsemu_frame_log_epoch(format, ...)                                              \
    if (fsemu_frame_log_level >= FSEMU_LOG_LEVEL_TRACE) {                               \
        fsemu_log("[FSE] [%5d] " format, (int)(fsemu_time_us() - fsemu_frame_epoch_at), \
                  ##__VA_ARGS__);                                                       \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_FRAME_H_
