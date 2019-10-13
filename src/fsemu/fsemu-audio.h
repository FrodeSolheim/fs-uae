#ifndef FSEMU_AUDIO_H_
#define FSEMU_AUDIO_H_

#include "fsemu-common.h"
#include "fsemu-types.h"

typedef struct {
    int buffer_bytes;
    int recent_bytes;
    int inflight_bytes;
    int dt;
    int underruns;
} fsemu_audio_frame_stats_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_audio_init(void);

int fsemu_audio_frequency(void);

/** If you want to set the frequency, you need to do it before calling
 * fsemu_audio_init. */
void fsemu_audio_set_frequency(int frequency);

void fsemu_audio_pause(void);
void fsemu_audio_resume(void);

// void fsemu_audio_work(int timeout_us);

// From main or video thread

// These functions can be called from other threads (i.e. emulation threads)

/* This should be called from the emulation thread when starting the
   emulation of a new frame. The idea is to get his function as reguarly as
   possible and a similar emulation state to be able to do something useful
   about the audio buffer sizes. */
// void fsemu_audio_start_frame(int number);

/* The latency information is updated after each fsemu_audio_start_frame */
int64_t fsemu_audio_latency_us(void);

void fsemu_audio_register_data_sent(int size,
                                    int64_t when,
                                    uint8_t *read,
                                    uint8_t *write);

int fsemu_audio_log_buffer_stats(void);
void fsemu_audio_log_inflight_estimate(void);

void fsemu_audio_frame_stats(int frame, fsemu_audio_frame_stats_t *stats);
int fsemu_audio_bytes_to_ms(int bytes);
int64_t fsemu_audio_bytes_to_us(int bytes);

void fsemu_audio_register_underrun(void);

#ifdef FSEMU_INTERNAL

void fsemu_audio_end_frame(void);

#endif

#ifdef __cplusplus
}
#endif

#define fsemu_audio_log(format, ...) \
    fsemu_log_null("[FSEMU] [AUDIO] " format, ##__VA_ARGS__)

#endif  // FSEMU_AUDIO_H_
