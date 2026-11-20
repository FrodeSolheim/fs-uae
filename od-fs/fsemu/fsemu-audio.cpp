#define FSEMU_INTERNAL
#include "fsemu-audio.h"

#ifdef FSEMU_ALSA
#include "fsemu-alsaaudio.h"
#endif
#include "fs/log.h"
#include "fsemu-audiobuffer.h"
#include "fsemu-audiorate.h"
#include "fsemu-frame.h"
// #include "fsemu-nullaudio.h"
#include "fsemu-option.h"
#ifdef FSEMU_SDL
#include "fsemu-sdlaudio.h"
#endif
#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsapp-util.h"

#include <math.h>

#define FSEMU_AUDIO_MAX_FRAME_STATS (1 << 8) // 256

// A frame that ends more than this after its planned end is late (still catching up after a
// stall), as the timing trace's `late end`.
#define FSEMU_AUDIO_LATE_END_NS SDL_MS_TO_NS(1)

static struct {
    int driver;
    int frequency;
    SDL_Mutex *mutex;
    // From the audio callback: what it handed over last, when (0 before the first callback), and
    // the audio ring buffer's read position after it and the write position it saw.
    int sent_size;
    int64_t sent_when_ns;
    int64_t sent_read;
    int64_t sent_write;
    // The audio ring buffer's counts at the end of the last frame (the emulation thread's own).
    int64_t underruns;
    int64_t overruns;
    int64_t resets;
    int64_t latency_ns;
    fsemu_audio_frame_stats_t stats[FSEMU_AUDIO_MAX_FRAME_STATS];

    // The emulation thread's own: whether the controller is held by the emulator
    // (fsemu_audio_set_rate_adjustment_held), whether the last frame warped, the audio the
    // emulator has produced but not yet written (fsemu_audio_set_pending_bytes), the audio buffer
    // level measured at the last frame's end, whether it says something about the clocks, the
    // write position then, the controller with the time of its last step and its last adjustment,
    // and the excess rule.
    bool held;
    bool warped;
    int pending_bytes;
    bool level_measured;
    double level_ns;
    bool level_frozen;
    int64_t level_write_position;
    fsemu_audiorate_t controller;
    int64_t step_ns;
    fsemu_audiorate_adjustment_t adjustment;
    fsemu_audiorate_excess_rule_t excess_rule;
} fsemu_audio;

int fsemu_audio_log_level = SDL_LOG_PRIORITY_INFO;

// ----------------------------------------------------------------------------

// static void fsemu_nullaudio_init(void)
// {
// }

// ----------------------------------------------------------------------------

static void fsemu_audio_lock(void)
{
    SDL_LockMutex(fsemu_audio.mutex);
}

static void fsemu_audio_unlock(void)
{
    SDL_UnlockMutex(fsemu_audio.mutex);
}

int fsemu_audio_driver(void)
{
    return fsemu_audio.driver;
}

static void fsemu_audio_init_driver(void)
{
    fsemu_audio_log_info("Initialize driver...\n");

    // Without an audio driver; the driver sets the audio device's own rate when it opens it.
    fsemu_audio.frequency = 44100;

    // const char *driver = fsemu_config_string("audio_driver");
    // const char *driver = "sdl";
    const char *driver = fsemu_option_const_string(FSEMU_OPTION_AUDIO_DRIVER);
    if (driver) {
        fsemu_audio_log_info("Want audio driver: %s\n", driver);
    }
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
        fsemu_audio.driver = FSEMU_AUDIO_DRIVER_SDL;
        fsemu_sdlaudio_init();
#endif
    } else {
        // fsemu_nullaudio_init();
        // fse_init_dummy_audio();
    }
}

int fsemu_audio_frequency(void)
{
    return fsemu_audio.frequency;
}

void fsemu_audio_set_frequency(int frequency)
{
    fsemu_audio_log_info("Frequency is now %d Hz\n", frequency);
    fsemu_audio.frequency = frequency;
    // Another device or sample rate: the drift correction learnt so far is not for it.
    fsemu_audiorate_init(&fsemu_audio.controller);
    fsemu_audiorate_init_excess_rule(&fsemu_audio.excess_rule);
}

// The frame period on fsemu's timeline (ns), 0 before the first frame.
static double fsemu_audio_get_frame_period_ns(void)
{
    double timeline_hz = fsemu_frame_get_timeline_rate_hz();
    return timeline_hz > 0 ? (double)SDL_NS_PER_SECOND / timeline_hz : 0.0;
}

// Clears the audio device's stream of what the device has not taken yet.
static void fsemu_audio_clear_device_stream(void)
{
#ifdef FSEMU_SDL
    if (fsemu_audio.driver == FSEMU_AUDIO_DRIVER_SDL) {
        fsemu_sdlaudio_clear();
    }
#endif
}

// Resets the audio ring buffer to the target, counting the level outside it as estimated now
// (dropping the oldest audio, or adding silence faded out from the newest), and clears the audio
// device's stream.
static void fsemu_audio_reset_to_target(void)
{
    fsemu_audiobuffer_set_outside_bytes(fsemu_audio_get_outside_bytes());
    fsemu_audiobuffer_reset_to_target();
    fsemu_audio_clear_device_stream();
}

// Clears the audio ring buffer and fills it to the target with silence, counting the level
// outside it as estimated now, and clears the audio device's stream.
static void fsemu_audio_clear_to_target(int outside_bytes)
{
    fsemu_audiobuffer_set_outside_bytes(outside_bytes);
    fsemu_audiobuffer_clear_to_target();
    fsemu_audio_clear_device_stream();
}

void fsemu_audio_on_emulator_sound_stopped(void)
{
    // Nothing to do: the audio device keeps running (reopening it can take a second), and the
    // controller is frozen while nothing is written.
    fsemu_audio_log_info("The emulator's sound stopped\n");
}

void fsemu_audio_on_emulator_sound_started(void)
{
    fsemu_audio_log_info("The emulator's sound started again\n");
    // The audio ring buffer has run empty meanwhile (the audio device played silence): the audio
    // starts at the target, as at the first write.
    fsemu_audio_reset_to_target();
}

void fsemu_audio_set_paused(bool paused)
{
    if (paused) {
        fsemu_audio_log_info("Pause audio\n");
#ifdef FSEMU_SDL
        if (fsemu_audio.driver == FSEMU_AUDIO_DRIVER_SDL) {
            fsemu_sdlaudio_pause();
        }
#endif
        return;
    }
    fsemu_audio_log_info("Resume audio\n");
    // No stale audio: the audio ring buffer starts again at the target, with silence (the audio
    // device's stream was cleared on pause, and its audio in flight has played).
    fsemu_audio_clear_to_target(fsemu_audio_get_outside_bytes());
#ifdef FSEMU_SDL
    if (fsemu_audio.driver == FSEMU_AUDIO_DRIVER_SDL) {
        fsemu_sdlaudio_resume();
    }
#endif
}

void fsemu_audio_on_timeline_reset(void)
{
    fsemu_audio_reset_to_target();
}

void fsemu_audio_set_rate_adjustment_held(bool held)
{
    fsemu_audio.held = held;
}

void fsemu_audio_register_data_sent(int size, int64_t when_ns, int64_t read_position,
                                    int64_t write_position)
{
    fsemu_audio_lock();
    fsemu_audio.sent_size = size;
    fsemu_audio.sent_when_ns = when_ns;
    fsemu_audio.sent_read = read_position;
    fsemu_audio.sent_write = write_position;
    fsemu_audio_unlock();
}

// What the last audio callback handed over less what the device has played since at its nominal
// rate, at now_ns (bytes): below 0 when the device is overdue, by that much; 0 before the first
// callback.
static int fsemu_audio_get_unplayed_bytes(int64_t now_ns)
{
    fsemu_audio_lock();
    int sent_size = fsemu_audio.sent_size;
    int64_t sent_when_ns = fsemu_audio.sent_when_ns;
    fsemu_audio_unlock();
    if (sent_when_ns == 0) {
        // No callback yet.
        return 0;
    }
    // At the device's nominal rate (bytes per ns), and no further below 0 than an int holds.
    double consumption =
        fsemu_audio_frequency() * (double)FSEMU_AUDIORING_FRAME_BYTES / (double)SDL_NS_PER_SECOND;
    double unplayed = sent_size - (now_ns - sent_when_ns) * consumption;
    return unplayed > -1e9 ? (int)unplayed : -1000000000;
}

int fsemu_audio_get_in_flight_bytes(void)
{
    int unplayed = fsemu_audio_get_unplayed_bytes((int64_t)SDL_GetTicksNS());
    return unplayed > 0 ? unplayed : 0;
}

int fsemu_audio_get_outside_bytes(void)
{
    return fsemu_audio_get_in_flight_bytes() + fsemu_audio.pending_bytes;
}

void fsemu_audio_set_pending_bytes(int bytes)
{
    fsemu_audio.pending_bytes = bytes;
}

double fsemu_audio_get_rate_adjustment(void)
{
    return fsemu_audio.adjustment.adjustment;
}

void fsemu_audio_begin_frame(void)
{
    if (fsemu_frame_warping() && !fsemu_audio.warped) {
        // Warp begins: muted at once, the audio ring buffer held at the target with silence (and
        // then at each frame's end while warping). The ring buffer's own level is held at the
        // target, not counting the audio in flight, so that the audio callback does not run out
        // between the frames' ends (the silence costs no latency); warp's end resets the level.
        fsemu_audio_clear_to_target(0);
    }

    // The rate feed-forward against the timeline the frame runs on, and a step of the controller,
    // frozen when the level measured at the last frame's end says nothing about the clocks. The
    // drift correction's rate of change is limited per real time since the last step, which the
    // trace's counters are timed with too.
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    double elapsed_ns = fsemu_audio.step_ns > 0 ? (double)(now_ns - fsemu_audio.step_ns) : 0.0;
    fsemu_audio.step_ns = now_ns;
    double emulated_hz = fsemu_frame_rate_hz();
    double timeline_hz = fsemu_frame_get_timeline_rate_hz();
    double rate_feed_forward = fsemu_audiorate_get_rate_feed_forward(emulated_hz, timeline_hz);
    bool frozen = !fsemu_audio.level_measured || fsemu_audio.level_frozen;
    fsemu_audio.adjustment = fsemu_audiorate_update(
        &fsemu_audio.controller, fsemu_audio.level_ns, FSEMU_AUDIO_TARGET_LEVEL_NS,
        rate_feed_forward, fsemu_audio_get_frame_period_ns(), elapsed_ns, frozen);

    int64_t rate_feed_forward_ppb = llround(fsemu_audio.adjustment.rate_feed_forward * 1e9);
    int64_t drift_correction_ppb = llround(fsemu_audio.adjustment.drift_correction * 1e9);
    fsemu_audio_frame_stats_t *stats =
        &fsemu_audio.stats[fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS)];
    stats->rate_feed_forward_ppb = (int)rate_feed_forward_ppb;
    stats->drift_correction_ppb = (int)drift_correction_ppb;
    stats->rate_adjustment_frozen = frozen;
    if (FS_IsTraceEnabled()) {
        FS_TraceValue ff[] = {{"rate_feed_forward_ppb", rate_feed_forward_ppb}};
        FS_AddTraceCounter("rate feed-forward", now_ns, ff, 1);
        FS_TraceValue dc[] = {{"drift_correction_ppb", drift_correction_ppb}};
        FS_AddTraceCounter("drift correction", now_ns, dc, 1);
    }
}

// The audio buffer level at an emulated frame's end, and its parts (bytes).
typedef struct {
    int64_t now_ns;
    // The audio ring buffer's part as of the last audio callback, what was newly written since,
    // the audio the emulator has not written yet (such as Paula's partly filled buffer) and the
    // audio in flight.
    int ring_buffer_bytes;
    int newly_written_bytes;
    int pending_bytes;
    int in_flight_bytes;
    // Whether the audio device has called back yet, and whether it is overdue by more than a frame
    // period or has not called back yet (it has stopped calling back, as at its start and while
    // paused).
    bool device_called_back;
    bool device_overdue;
    // The level (the sum of the parts), and the write position then.
    int total_bytes;
    double level_ns;
    int64_t write_position;
} fsemu_audio_level_t;

// Measures the audio buffer level now.
static void fsemu_audio_measure_level(fsemu_audio_level_t *m)
{
    m->now_ns = (int64_t)SDL_GetTicksNS();
    fsemu_audio_lock();
    int64_t sent_write = fsemu_audio.sent_write;
    m->device_called_back = fsemu_audio.sent_when_ns > 0;
    fsemu_audio_unlock();

    // The read position the reader goes on from is the last callback's, or the cut of an audio
    // overrun or a reset since then.
    int64_t read = fsemu_audiobuffer_get_read_position();
    m->write_position = fsemu_audiobuffer_get_write_position();
    m->ring_buffer_bytes = sent_write > read ? (int)(sent_write - read) : 0;
    m->newly_written_bytes = (int)(m->write_position - (sent_write > read ? sent_write : read));
    m->pending_bytes = fsemu_audio.pending_bytes;
    int unplayed = fsemu_audio_get_unplayed_bytes(m->now_ns);
    m->in_flight_bytes = unplayed > 0 ? unplayed : 0;
    int frequency = fsemu_audio_frequency();
    m->device_overdue =
        frequency > 0 && (!m->device_called_back ||
                          fsemu_audio_bytes_to_ns(-unplayed) > fsemu_audio_get_frame_period_ns());
    m->total_bytes =
        m->ring_buffer_bytes + m->newly_written_bytes + m->pending_bytes + m->in_flight_bytes;
    m->level_ns = frequency > 0 ? (double)fsemu_audio_bytes_to_ns(m->total_bytes) : 0.0;
}

// Whether the level says nothing about the clocks, so that the controller is frozen for it: after
// a stall (the frame ended more than a frame period after its planned end), when nothing was
// written during the frame (the emulator produced no audio for longer than a frame period, as
// with sound off), when the audio device is overdue, while warping, and while the emulator holds
// it (loading or saving a savestate).
static bool fsemu_audio_is_level_frozen(const fsemu_audio_level_t *m)
{
    bool stalled = m->now_ns - fsemu_frame_planned_end_ns > fsemu_audio_get_frame_period_ns();
    bool nothing_written = m->write_position == fsemu_audio.level_write_position;
    return stalled || nothing_written || m->device_overdue || fsemu_frame_warping() ||
           fsemu_audio.held;
}

// The audio ring buffer at the frame's end: held at the target while warping (the audio is
// muted), reset to the target when warp has ended, and the excess rule's drop (the audio device
// calling back again after being overdue, or the silence of audio underruns raising the level
// once caught up). Returns whether the controller is to be held for the level.
static bool fsemu_audio_manage_ring_buffer(const fsemu_audio_level_t *m, int underruns)
{
    bool warping = fsemu_frame_warping();
    bool warp_ended = fsemu_audio.warped && !warping;
    fsemu_audio.warped = warping;
    if (warping) {
        fsemu_audiobuffer_set_outside_bytes(0);
        fsemu_audiobuffer_fill_to_target();
        return true;
    }
    fsemu_audiobuffer_set_outside_bytes(m->in_flight_bytes + m->pending_bytes);
    if (warp_ended) {
        fsemu_audiobuffer_reset_to_target();
        fsemu_audio_clear_device_stream();
        return true;
    }
    if (fsemu_audio_frequency() <= 0) {
        return false;
    }
    bool on_time = m->now_ns - fsemu_frame_planned_end_ns <= FSEMU_AUDIO_LATE_END_NS;
    fsemu_audiorate_excess_t excess = fsemu_audiorate_check_excess(
        &fsemu_audio.excess_rule, m->level_ns, FSEMU_AUDIO_TARGET_LEVEL_NS, underruns > 0,
        m->device_overdue, on_time);
    if (excess.drop) {
        fsemu_audiobuffer_drop_to_target();
        fsemu_audio_clear_device_stream();
    }
    return excess.hold || excess.drop;
}

// Records the frame's audio statistics (for the performance graph) and its trace counter.
static void fsemu_audio_record_frame_stats(const fsemu_audio_level_t *m,
                                           fsemu_audio_frame_stats_t *stats)
{
    stats->target_latency_ns = FSEMU_AUDIO_TARGET_LEVEL_NS;
    stats->buffer_bytes = m->ring_buffer_bytes + m->newly_written_bytes;
    stats->recent_bytes = m->newly_written_bytes;
    stats->pending_bytes = m->pending_bytes;
    stats->in_flight_bytes = m->in_flight_bytes;

    if (FS_IsTraceEnabled() && fsemu_audio_frequency()) {
        // The audio buffer level's parts at the emulated frame's end.
        FS_TraceValue parts[] = {
            {"ring_buffer_us", fsemu_audio_bytes_to_ns(m->ring_buffer_bytes)},
            {"newly_written_us", fsemu_audio_bytes_to_ns(m->newly_written_bytes)},
            {"in_flight_us", fsemu_audio_bytes_to_ns(m->in_flight_bytes)},
            {"pending_us", fsemu_audio_bytes_to_ns(m->pending_bytes)},
        };
        FS_AddTraceCounter("audio buffer level at frame end", m->now_ns, parts, 4);
    }

    // 0 when the audio subsystem is not initialized.
    fsemu_audio.latency_ns = (int64_t)m->level_ns;
    static fsapp_util_moving_average_t latency_average;
#define PID_LATENCY_VALUES 64
    static int64_t latency_values[PID_LATENCY_VALUES];
    stats->avg_latency_ns = fsapp_util_moving_average_add(
        &latency_average, latency_values, PID_LATENCY_VALUES, fsemu_audio.latency_ns);
}

// Counts the audio underruns, audio overruns and resets of the audio ring buffer in the frame.
static void fsemu_audio_count_frame_events(fsemu_audio_frame_stats_t *stats)
{
    int64_t underruns = fsemu_audiobuffer_get_underrun_count();
    int64_t overruns = fsemu_audiobuffer_get_overrun_count();
    int64_t resets = fsemu_audiobuffer_get_reset_count();
    stats->underruns = (int)(underruns - fsemu_audio.underruns);
    stats->overruns = (int)(overruns - fsemu_audio.overruns);
    stats->resets = (int)(resets - fsemu_audio.resets);
    fsemu_audio.underruns = underruns;
    fsemu_audio.overruns = overruns;
    fsemu_audio.resets = resets;
}

void fsemu_audio_end_frame(void)
{
    fsemu_audio_frame_stats_t *stats =
        &fsemu_audio.stats[fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS)];
    fsemu_audio_level_t m;
    fsemu_audio_measure_level(&m);
    fsemu_audio_record_frame_stats(&m, stats);

    // For the controller's next step: the level, measured once the audio device has called back,
    // frozen when it says nothing about the clocks, when the ring buffer was reset or dropped,
    // and while the excess rule holds it.
    fsemu_audio.level_measured = fsemu_audio_frequency() > 0 && m.device_called_back;
    fsemu_audio.level_ns = m.level_ns;
    bool frozen = fsemu_audio_is_level_frozen(&m);
    int64_t underruns_before = fsemu_audio.underruns;
    fsemu_audio_count_frame_events(stats);
    bool hold = fsemu_audio_manage_ring_buffer(&m, (int)(fsemu_audio.underruns - underruns_before));
    fsemu_audio.level_frozen = frozen || hold;
    fsemu_audio.level_write_position = fsemu_audiobuffer_get_write_position();

    fsemu_audio_log_buffer_stats();

    // Clear data for the next frame *before* incrementing the frame counter.
    // This is important since threads may fetch the frame counter and then
    // start updating frame statistics, in some cases adding to stat counters
    // which should already be cleared.
    int next_frame = (fsemu_frame_counter() + 1) % FSEMU_AUDIO_MAX_FRAME_STATS;
    fsemu_audio_lock();
    fsemu_audio.stats[next_frame].min_buffer_bytes = 0;
    fsemu_audio_unlock();
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
        &fsemu_audio.stats[fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS)];
    int total = stats->buffer_bytes + stats->in_flight_bytes;
    int frequency = fsemu_audio_frequency();
    // FIXME: Perhaps easier to make fsemu_audio_frequency always return
    // something sane even if audio is not enabled
    if (!frequency) {
        return 0;
    }
    double consumption = frequency * (double)FSEMU_AUDIORING_FRAME_BYTES / 1000000;
    fsemu_audio_log_trace("[%6d] Buffer %2d ms (%2d (+%2d) +%2d (cons %0.1f))\n",
                          (int)fsemu_frame_counter(), fsemu_audio_bytes_to_ms(total),
                          fsemu_audio_bytes_to_ms(stats->buffer_bytes),
                          fsemu_audio_bytes_to_ms(stats->recent_bytes),
                          fsemu_audio_bytes_to_ms(stats->in_flight_bytes), consumption);
    return total / FSEMU_AUDIORING_FRAME_BYTES * 1000 / frequency;
}

int64_t fsemu_audio_get_latency_ns(void)
{
    return fsemu_audio.latency_ns;
}

void fsemu_audio_frame_stats(int frame, fsemu_audio_frame_stats_t *stats)
{
    memcpy(stats, &fsemu_audio.stats[frame % FSEMU_AUDIO_MAX_FRAME_STATS],
           sizeof(fsemu_audio_frame_stats_t));
}

int fsemu_audio_bytes_to_ms(int bytes)
{
    return bytes * 1000 / fsemu_audio_frequency() / FSEMU_AUDIORING_FRAME_BYTES;
}

int64_t fsemu_audio_bytes_to_ns(int bytes)
{
    return (int64_t)bytes * SDL_NS_PER_SECOND / fsemu_audio_frequency() /
           FSEMU_AUDIORING_FRAME_BYTES;
}

void fsemu_audio_update_min_fill(int ring_bytes)
{
    int bytes = ring_bytes;
    int frame = fsemu_frame_counter_mod(FSEMU_AUDIO_MAX_FRAME_STATS);
    // There is a small chance that the min level is registered on the "wrong"
    // frame due to race conditions, and therefore might not show up properly
    // in the graphs. Minor problem. The audio callback writes it, and the emulation thread clears
    // and reads it, under the lock.
    fsemu_audio_lock();
    int existing_min = fsemu_audio.stats[frame].min_buffer_bytes;
    if (existing_min == 0 || existing_min > bytes) {
        fsemu_audio.stats[frame].min_buffer_bytes = bytes;
    }
    fsemu_audio_unlock();
}

bool fsemu_audio_muted(void)
{
    // FIXME
    return false;
}

void fsemu_audio_set_muted(bool muted)
{
    SDL_Log("FIXME: Set volume not implemented yet\n");
}

int fsemu_audio_volume(void)
{
    // FIXME
    return 100;
}

void fsemu_audio_set_volume(int volume)
{
    SDL_Log("FIXME: Set volume not implemented yet\n");
}

void fsemu_audio_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_frame_init();

    fsemu_audio_log_info("Init\n");
    fsemu_audio_log_level = fsemu_option_int_default(FSEMU_OPTION_LOG_AUDIO, SDL_LOG_PRIORITY_INFO);

    fsemu_audio.mutex = SDL_CreateMutex();
    fsemu_audiorate_init(&fsemu_audio.controller);
    fsemu_audiorate_init_excess_rule(&fsemu_audio.excess_rule);
    fsemu_audiobuffer_init();
    fsemu_audio_init_driver();

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

    int volume = fsemu_option_int_clamped_default(FSEMU_OPTION_VOLUME, 0, 100, 100);
    if (volume == 0) {
        fsemu_audio_set_muted(true);
    } else {
        fsemu_audio_set_volume(volume);
    }
}
