#ifndef FSEMU_AUDIO_H
#define FSEMU_AUDIO_H

#include "fs/log.h"
#include "fsemu-common.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_AUDIO_DRIVER_NULL 0
#define FSEMU_AUDIO_DRIVER_SDL 1
#define FSEMU_AUDIO_DRIVER_ALSA 2

// The target audio buffer level (counting the audio in flight), in nanoseconds: one PAL frame
// period (20 ms). The ceiling of the audio ring buffer is a multiple of it
// (FSEMU_AUDIORING_CEILING_FACTOR).
#define FSEMU_AUDIO_TARGET_LEVEL_NS ((int64_t)SDL_MS_TO_NS(20))

typedef struct {
    // The target audio buffer level (ns).
    int64_t target_latency_ns;
    int buffer_bytes;
    int recent_bytes;
    // The audio the emulator had not written to the audio ring buffer yet at the frame's end
    // (fsemu_audio_set_pending_bytes).
    int pending_bytes;
    // The audio in flight (0 or more).
    int in_flight_bytes;
    // The audio underruns, audio overruns and resets of the audio ring buffer in the frame.
    int underruns;
    int overruns;
    int resets;
    // The audio buffer level's average over recent frames (ns).
    int64_t avg_latency_ns;
    int min_buffer_bytes;
    // The audio rate adjustment's parts for the frame (fsemu-audiorate.h), in ppb, and whether the
    // controller was frozen.
    int rate_feed_forward_ppb;
    int drift_correction_ppb;
    bool rate_adjustment_frozen;
} fsemu_audio_frame_stats_t;

void fsemu_audio_init(void);

// The emulator's sound has stopped (sound off, or automatic sound switching after a while without
// sound), not the emulation (see fsemu_audio_set_paused): the audio device keeps running, and the
// controller is frozen while nothing is written. Emulation thread.
void fsemu_audio_on_emulator_sound_stopped(void);

// The emulator's sound starts again: the audio ring buffer, run empty meanwhile, is reset to the
// target. Emulation thread.
void fsemu_audio_on_emulator_sound_started(void);

// Holds the audio rate adjustment's controller (frozen, its learnt drift kept) while the emulator
// does something that says nothing about the clocks, such as loading or saving a savestate. Set
// by the emulation thread before fsemu_frame_end.
void fsemu_audio_set_rate_adjustment_held(bool held);

bool fsemu_audio_muted(void);

void fsemu_audio_set_muted(bool muted);

int fsemu_audio_volume(void);

void fsemu_audio_set_volume(int volume);

int fsemu_audio_driver(void);

int fsemu_audio_frequency(void);

// The rate the audio ring buffer is fed and read at (Hz): the audio device's own rate, set by the
// audio driver when fsemu_audio_init opens the device (44100 without a driver). The emulator's
// sound frequency must be set from fsemu_audio_frequency after fsemu_audio_init.
void fsemu_audio_set_frequency(int frequency);

// From main or video thread

// These functions can be called from other threads (i.e. emulation threads)

/* This should be called from the emulation thread when starting the
   emulation of a new frame. The idea is to get his function as reguarly as
   possible and a similar emulation state to be able to do something useful
   about the audio buffer sizes. */
// void fsemu_audio_start_frame(int number);

// The audio buffer level measured at the last emulated frame's end (ns), 0 before.
int64_t fsemu_audio_get_latency_ns(void);

// The audio the emulator has produced but not yet written to the audio ring buffer (bytes), such
// as a partly filled buffer of its own: counted in the audio buffer level measured at the end of
// the emulated frame. Set by the emulation thread before fsemu_frame_end.
void fsemu_audio_set_pending_bytes(int bytes);

// The audio rate adjustment for the current emulated frame (fsemu-audiorate.h): the factor to
// apply to the emulator's sample clock, whose sample interval is the nominal one divided by
// (1 + adjustment). Computed at the frame's start from the audio buffer level measured at the end
// of the frame before. Emulation thread.
double fsemu_audio_get_rate_adjustment(void);

// From the audio callback: the audio in flight it estimates after handing over audio at when_ns
// (a time on the clock, SDL_GetTicksNS), and the audio ring buffer's read position after it and the
// write position it saw.
void fsemu_audio_register_data_sent(int size, int64_t when_ns, int64_t read_position,
                                    int64_t write_position);

int fsemu_audio_log_buffer_stats(void);

void fsemu_audio_frame_stats(int frame, fsemu_audio_frame_stats_t *stats);
// How long audio of this size plays at the audio frequency, in milliseconds (for the log) and in
// nanoseconds.
int fsemu_audio_bytes_to_ms(int bytes);
int64_t fsemu_audio_bytes_to_ns(int bytes);

#ifdef FSEMU_INTERNAL

// At the emulated frame's start: the controller's step (fsemu_audio_get_rate_adjustment).
void fsemu_audio_begin_frame(void);

// At the emulated frame's end: measures the audio buffer level.
void fsemu_audio_end_frame(void);

// The audio in flight as estimated now (bytes): what the last audio callback handed over
// (including the silence of an audio underrun), less what the device has played since at its
// nominal rate, never below 0.
int fsemu_audio_get_in_flight_bytes(void);

// The audio buffer level outside the audio ring buffer as estimated now (bytes): the audio in
// flight and the audio the emulator has not written yet.
int fsemu_audio_get_outside_bytes(void);

// A timeline reset (after too many late frames): the audio ring buffer is reset to the target.
void fsemu_audio_on_timeline_reset(void);

// The emulation is paused (not only the emulator's sound, see
// fsemu_audio_on_emulator_sound_stopped): the audio device is paused and its stream cleared. It
// resumes: the audio ring buffer is cleared and filled to the target with silence, and the device
// resumed.
void fsemu_audio_set_paused(bool paused);

// From the audio callback: what is left in the audio ring buffer after a read.
void fsemu_audio_update_min_fill(int ring_bytes);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_audio_log_level;

#define fsemu_audio_log(format, ...)                                                               \
    FS_LOG_INFO(fsemu_audio_log_level, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_debug(format, ...)                                                         \
    FS_LOG_DEBUG(fsemu_audio_log_level, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_error(format, ...)                                                         \
    FS_LOG_ERROR(fsemu_audio_log_level, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_info(format, ...)                                                          \
    FS_LOG_INFO(fsemu_audio_log_level, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_trace(format, ...)                                                         \
    FS_LOG_TRACE(fsemu_audio_log_level, "[FSE] [AUD]", format, ##__VA_ARGS__)

#define fsemu_audio_log_warning(format, ...)                                                       \
    FS_LOG_WARNING(fsemu_audio_log_level, "[FSE] [AUD]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif // FSEMU_INTERNAL

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_AUDIO_H */
