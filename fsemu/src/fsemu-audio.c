#define FSEMU_INTERNAL
#include "fsemu-audio.h"

#ifdef FSEMU_ALSA
#include "fsemu-audio-alsa.h"
#endif
#include "fsemu-audio-buffer.h"
#include "fsemu-frame.h"
#include "fsemu-log.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#ifdef FSEMU_SDL
#include "fsemu-sdlaudio.h"
#endif
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-util.h"

// int fsemu_audio_buffer.size;
// volatile uint8_t *fsemu_audio_buffer;
// volatile uint8_t *fsemu_audio_buffer.end;
// volatile uint8_t *volatile fsemu_audio_buffer.read;
// volatile uint8_t *volatile fsemu_audio_buffer.write;

#define FSEMU_AUDIO_MAX_FRAME_STATS (1 << 8)  // 256

static struct {
    int frequency;
    fsemu_mutex_t *mutex;
    int sent_size;
    int64_t sent_when;
    int64_t sent_when_prev;
    uint8_t *sent_read;
    uint8_t *sent_write;
    int underruns;
    int64_t latency_us;
    fsemu_audio_frame_stats_t stats[FSEMU_AUDIO_MAX_FRAME_STATS];
} fsemu_audio;

int fsemu_audio_log_level = 1;

static void fsemu_audio_lock(void)
{
    if (fsemu_audio.mutex) {
        fsemu_mutex_lock(fsemu_audio.mutex);
    }
}

static void fsemu_audio_unlock(void)
{
    if (fsemu_audio.mutex) {
        fsemu_mutex_unlock(fsemu_audio.mutex);
    }
}

static void fsemu_audio_init_driver(void)
{
    fsemu_audio_log("Initialize driver...\n");

    // FIXME: Bug in SDL or in macOS? When requesting 48000 Hz, the callback
    // rate is 44100, but "have" struct return from SDL_OpenAudio says
    // 48000...

    fsemu_audio.frequency = 44100;

    // const char *driver = fsemu_config_string("audio_driver");
    const char *driver = "sdl";
    if (0) {
#ifdef FSEMU_OPENAL
    } else if (!driver || strcmp(driver, "openal") == 0) {
        fse_init_openal_audio();
#endif
#ifdef FSEMU_ALSA
    } else if (!driver || strcmp(driver, "alsa") == 0) {
        fsemu_audio_alsa_init();
#endif
#ifdef FSEMU_PULSE
    } else if (!driver || strcmp(driver, "pulse") == 0) {
        fsemu_audio_pulse_init();
#endif
#ifdef FSEMU_SDL
    } else if (!driver || strcmp(driver, "sdl") == 0) {
        fsemu_sdlaudio_init();
#endif
    } else {
        // fse_init_dummy_audio();
    }
    /*
    for (int i = 0; i < FSE_MAX_AUDIO_STREAMS; i++) {
        fse_audio.want_volume[i] = 1.0;
    }
    int volume = fs_config_get_int_clamped("volume", 0, 100);
    if (volume != FS_CONFIG_NONE) {
        if (volume == 0) {
            fse_set_audio_muted(FS_EMU_AUDIO_MASTER, 1);
        } else {
            fse_set_set_audio_volume(FS_EMU_AUDIO_MASTER, volume);
        }
    }
    */
}

void fsemu_audio_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_frame_init();

    fsemu_audio_log("Init\n");
    fsemu_option_read_int(FSEMU_OPTION_LOG_AUDIO, &fsemu_audio_log_level);

    fsemu_audio.mutex = fsemu_mutex_create();
    fsemu_audio_buffer_init();
    fsemu_audio_init_driver();
}

int fsemu_audio_frequency(void)
{
    return fsemu_audio.frequency;
}

void fsemu_audio_set_frequency(int frequency)
{
    fsemu_audio_log("Frequency is now %d Hz\n", frequency);
    fsemu_audio.frequency = frequency;
}

void fsemu_audio_pause(void)
{
    fsemu_audio_log("Pause audio (FIXME!)\n");
    SDL_PauseAudio(1);
}

void fsemu_audio_resume(void)
{
    fsemu_audio_log("Resume audio (FIXME!)\n");
    SDL_PauseAudio(0);
}

void fsemu_audio_register_data_sent(int size,
                                    int64_t when,
                                    uint8_t *read,
                                    uint8_t *write)
{
    fsemu_audio_lock();
    // FIXME: Average sent_when / sent bytes per time unit
    fsemu_audio.sent_when_prev = fsemu_audio.sent_when;
    fsemu_audio.sent_size = size;
    fsemu_audio.sent_when = when;
    fsemu_audio.sent_read = read;
    fsemu_audio.sent_write = write;
    fsemu_audio_unlock();
}

void fsemu_audio_log_inflight_estimate(void)
{
    int64_t now = fsemu_time_us();

    fsemu_audio_lock();
    int sent_size = fsemu_audio.sent_size;
    int64_t sent_when = fsemu_audio.sent_when;
    // int64_t sent_when_prev = fsemu_audio.sent_when_prev;
    // uint8_t *sent_read = fsemu_audio.sent_read;
    // uint8_t *sent_write = fsemu_audio.sent_write;
    fsemu_audio_unlock();

    int frequency = fsemu_audio_frequency();
    double consumption = frequency * 4.0 / 1000000;
    int64_t dt = now - sent_when;
    int inflight = sent_size - dt * consumption;
    fsemu_audio_log_trace("+ %2d (sent, dt %5d cons %0.1f))\n",
                          inflight / 4 * 1000 / frequency,
                          (int) dt,
                          consumption);
}

// static int fsemu_audio_frame_number;

static void fsemu_audio_update_stats(void)
{
    // fsemu_audio_log("Audio: Frame %d\n", number);
    // int buffered = fsemu_audio_buffer_fill();

    int64_t now = fsemu_time_us();

    fsemu_audio_lock();
    int sent_size = fsemu_audio.sent_size;
    int64_t sent_when = fsemu_audio.sent_when;
    // int64_t sent_when_prev = fsemu_audio.sent_when_prev;
    uint8_t *sent_read = fsemu_audio.sent_read;
    uint8_t *sent_write = fsemu_audio.sent_write;
    int underruns = fsemu_audio.underruns;
    fsemu_audio.underruns = 0;
    fsemu_audio_unlock();

    // uint8_t volatile *read = fsemu_audio_buffer.read;
    uint8_t volatile *write = fsemu_audio_buffer.write;

    intptr_t buffer_fill;
    if (sent_write >= sent_read) {
        buffer_fill = sent_write - sent_read;
    } else {
        buffer_fill = (fsemu_audio_buffer.end - sent_read) +
                      (sent_write - fsemu_audio_buffer.data);
    }

    intptr_t newly_written = write - sent_write;
    if (newly_written < 0) {
        newly_written += fsemu_audio_buffer.size;
    }
    // Mint local_buffer_fill = buffer_fill;

    // FIXME: Not accurate enough yet
    // bytes per micro second

    int frequency = fsemu_audio_frequency();
#if 1
    double consumption = frequency * 4.0 / 1000000;
#else
    double consumption = (double) sent_size / (sent_when - sent_when_prev);
#endif
    int64_t dt = now - sent_when;
    // fsemu_audio_log("Now %ld sent when %ld dt %ld\n", now, sent_when, dt);

    int inflight = sent_size - dt * consumption;
    if (inflight < 0) {
        fsemu_audio_log_trace(
            "Inflight %d < 0 (consumes %0.1f B/us)\n", inflight, consumption);
        // When using SDL with Pulseaudio for example, inflight can be
        // negative. We do not know how big the internal buffer in Pulseaudio
        // is, and SDL/PA can choose to delay asking for more data.
        // inflight = 0;
    }

    int total = buffer_fill + newly_written + inflight;

    fsemu_audio_frame_stats_t *stats =
        &fsemu_audio
             .stats[fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS)];

    stats->buffer_bytes = buffer_fill + newly_written;
    stats->recent_bytes = newly_written;
    stats->inflight_bytes = inflight;
    stats->dt = dt;
    stats->underruns = underruns;

    if (frequency) {
        fsemu_audio.latency_us = (int64_t) total / 4 * 1000000LL / frequency;
    } else {
        // For when audio subsystem is not initialized.
        fsemu_audio.latency_us = 0;
    }

    static fsemu_mavgi_t latency_mavg;
#define PID_LATENCY_VALUES 64
    static int latency_values[PID_LATENCY_VALUES];
    int latency_avg = fsemu_mavgi(&latency_mavg,
                                  latency_values,
                                  PID_LATENCY_VALUES,
                                  fsemu_audio.latency_us);

    stats->avg_latency_us = latency_avg;
}

void fsemu_audio_end_frame(void)
{
    fsemu_audio_update_stats();
    fsemu_audio_log_buffer_stats();
#if 0
    // fsemu_audio_frame_number = number;
    if (fsemu_audio_frame_number % 1 == 0) {
    }
#endif

    // Clear data for the next frame *before* incrementing the frame counter.
    // This is important since threads may fetch the frame counter and then
    // start updating frame statistics, in some cases adding to stat counters
    // which should already be cleared.

    int next_frame = (fsemu_frame_counter() + 1) % FSEMU_AUDIO_MAX_FRAME_STATS;
    fsemu_audio.stats[next_frame].min_buffer_bytes = 0;
}

#if 0
void fsemu_audio_start_frame(int number)
{
#if 1
    fsemu_audio_frame_number = number;
    // if (fsemu_audio_frame_number % 1 == 0) {
    //     fsemu_audio_log_buffer_stats();
    // }
#endif
}
#endif

int fsemu_audio_log_buffer_stats(void)
{
    fsemu_audio_frame_stats_t *stats =
        &fsemu_audio
             .stats[fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS)];
    int total = stats->buffer_bytes + stats->inflight_bytes;
    int frequency = fsemu_audio_frequency();
    // FIXME: Perhaps easier to make fsemu_audio_frequency always return
    // something sane even if audio is not enabled
    if (!frequency) {
        return 0;
    }
    double consumption = frequency * 4.0 / 1000000;
    fsemu_audio_log_trace(
        "[%6d] Buffer %2d ms (%2d (+%2d) +%2d (est. dt %5d cons "
        "%0.1f))\n",
        (int) fsemu_frame_counter(),
        fsemu_audio_bytes_to_ms(total),
        fsemu_audio_bytes_to_ms(stats->buffer_bytes),
        fsemu_audio_bytes_to_ms(stats->recent_bytes),
        fsemu_audio_bytes_to_ms(stats->inflight_bytes),
        (int) stats->dt,
        consumption);
    return total / 4 * 1000 / frequency;
}

int64_t fsemu_audio_latency_us(void)
{
    return fsemu_audio.latency_us;
}

void fsemu_audio_frame_stats(int frame, fsemu_audio_frame_stats_t *stats)
{
    memcpy(stats,
           &fsemu_audio.stats[frame % FSEMU_AUDIO_MAX_FRAME_STATS],
           sizeof(fsemu_audio_frame_stats_t));
}

int fsemu_audio_bytes_to_ms(int bytes)
{
    return bytes * 1000 / fsemu_audio_frequency() / 4;
}

int64_t fsemu_audio_bytes_to_us(int bytes)
{
    return (int64_t) bytes * 1000000 / fsemu_audio_frequency() / 4;
}

void fsemu_audio_register_underrun(void)
{
    fsemu_audio_lock();
    fsemu_audio.underruns += 1;
    fsemu_audio_unlock();
}

void fsemu_audio_update_min_fill(uint8_t volatile *read,
                                 uint8_t volatile *write)
{
    int bytes;
    if (write >= read) {
        bytes = write - read;
    } else {
        bytes = (fsemu_audio_buffer.end - read) +
                (write - fsemu_audio_buffer.data);
    }
    int frame = fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS);
    // There is a small chance that the min level is registered on the "wrong"
    // frame due to race conditions, and therefore might not show up properly
    // in the graphs. Minor problem, might fix with locking.
    int existing_min = fsemu_audio.stats[frame].min_buffer_bytes;
    if (existing_min == 0 || existing_min > bytes) {
        fsemu_audio.stats[frame].min_buffer_bytes = bytes;
    }
}
