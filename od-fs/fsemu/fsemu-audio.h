#ifndef FSEMU_AUDIO_H_
#define FSEMU_AUDIO_H_

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_AUDIO_DRIVER_NULL 0
#define FSEMU_AUDIO_DRIVER_SDL 1
#define FSEMU_AUDIO_DRIVER_ALSA 2

typedef struct {
    int target_latency_us;
    int buffer_bytes;
    int recent_bytes;
    int inflight_bytes;
    int dt;
    int underruns;
    int avg_latency_us;
    int min_buffer_bytes;
} fsemu_audio_frame_stats_t;

void fsemu_audio_init(void);

void fsemu_audio_pause(void);

void fsemu_audio_resume(void);

bool fsemu_audio_muted(void);

void fsemu_audio_set_muted(bool muted);

int fsemu_audio_volume(void);

void fsemu_audio_set_volume(int volume);

int fsemu_audio_driver(void);

int fsemu_audio_frequency(void);

/** If you want to set the frequency, you need to do it before calling
 * fsemu_audio_init. */
void fsemu_audio_set_frequency(int frequency);

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
                                    uintptr_t read,
                                    uintptr_t write);

int fsemu_audio_log_buffer_stats(void);
void fsemu_audio_log_inflight_estimate(void);

void fsemu_audio_frame_stats(int frame, fsemu_audio_frame_stats_t *stats);
int fsemu_audio_bytes_to_ms(int bytes);
int64_t fsemu_audio_bytes_to_us(int bytes);

void fsemu_audio_register_underrun(void);

#ifdef FSEMU_INTERNAL

void fsemu_audio_end_frame(void);

void fsemu_audio_update_min_fill(uint8_t volatile *read,
                                 uint8_t volatile *write);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_audio_log_level;

#define fsemu_audio_log(format, ...) \
    FSEMU_LOG(audio, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_debug(format, ...) \
    FSEMU_LOG_DEBUG(audio, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_error(format, ...) \
    FSEMU_LOG_ERROR(audio, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_info(format, ...) \
    FSEMU_LOG_INFO(audio, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_trace(format, ...) \
    FSEMU_LOG_TRACE(audio, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_warning(format, ...) \
    FSEMU_LOG_WARNING(audio, "[FSE] [AUD]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_AUDIO_H_
