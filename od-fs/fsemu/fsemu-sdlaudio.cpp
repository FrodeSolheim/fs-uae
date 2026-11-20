#define FSEMU_INTERNAL
#include "fsemu-sdlaudio.h"

#include "fs/log.h"
#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"
#include "fsemu-frame.h"
#include "fsemu-option.h"

#ifdef FSEMU_SDL

// FIXME: Emulation thread should call the audio system whenever *starting*?
// on a new frame, so we get a quite regular callback where it makes sense
// to compare audio buffer fill (and figure out the trend).

// From
// https://github.com/PCSX2/pcsx2/blob/master/plugins/spu2-x/src/SndOut_SDL.cpp:
// Samples should vary from [512,8192] according to SDL spec. Take note this is
// the desired sample count and SDL may provide otherwise. Pulseaudio will cut
// this value in half if PA_STREAM_ADJUST_LATENCY is set in the backened, for
// example.

// Idea; give an estimate of audio buffer including the data just sent to the
// audio layer by interpolating the size sent and time passed (based on time
// between callbacks)

// FIXME: SDL_SetAudioStreamFrequencyRatio is now built-in...
// SDL_GetAudioStreamQueued

// ----------------------------------------------------------------------------

static struct fsemu_sdlaudio {
    SDL_AudioStream *stream;
} fsemu_sdlaudio;

// ----------------------------------------------------------------------------

void fsemu_sdlaudio_callback(void *userdata, SDL_AudioStream *stream, int additional_amount,
                             int total_amount);

// ----------------------------------------------------------------------------

static const char *fsemu_sdlaudio_format_name(SDL_AudioFormat format)
{
    switch (format) {
    case SDL_AUDIO_UNKNOWN:
        return "UNKNOWN";
    case SDL_AUDIO_U8:
        return "U8";
    case SDL_AUDIO_S8:
        return "S8";
    case SDL_AUDIO_S16LE:
        return "S16LE";
    case SDL_AUDIO_S16BE:
        return "S16BE";
    case SDL_AUDIO_S32LE:
        return "S32LE";
    case SDL_AUDIO_S32BE:
        return "S32BE";
    case SDL_AUDIO_F32LE:
        return "F32LE";
    case SDL_AUDIO_F32BE:
        return "F32BE";
    }
    return "???";
}

// The device period FS-UAE asks for, in sample frames (SDL_AUDIO_DEVICE_SAMPLE_FRAMES in the
// environment overrides it). TODO: a setting.
#define FSEMU_SDLAUDIO_DEFAULT_PERIOD_FRAMES "512"

// The rate used when the default playback device does not say what its own rate is.
#define FSEMU_SDLAUDIO_FALLBACK_RATE_HZ 48000

void fsemu_sdlaudio_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_audio_log_info("SDL_Init(SDL_INIT_AUDIO)\n");
    SDL_Init(SDL_INIT_AUDIO);

    const char *sdl_driver_name = SDL_GetCurrentAudioDriver();
    if (sdl_driver_name) {
        fsemu_audio_log_info("SDL audio driver: %s\n", sdl_driver_name);
    } else {
        fsemu_audio_log_warning("Audio subsystem not initialized.\n");
        return;
    }

    int device_count;
    SDL_AudioDeviceID *device_ids = SDL_GetAudioPlaybackDevices(&device_count);
    fsemu_audio_log_info("Playback devices (%d):\n", device_count);
    if (device_ids) {
        for (SDL_AudioDeviceID *device_id = device_ids; *device_id; device_id++) {
            fsemu_audio_log_info("Device %d: %s\n", *device_id, SDL_GetAudioDeviceName(*device_id));
        }
        SDL_free(device_ids);
    }

    // The default playback device's own format: the stream is fed at its rate, so that SDL only
    // converts the sample format (S16 to the device's) and does not resample.
    SDL_AudioSpec device_spec;
    SDL_zero(device_spec);
    if (SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &device_spec, NULL) &&
        device_spec.freq > 0) {
        fsemu_audio_log_info("Default playback device: %d Hz, %s, %d channels\n", device_spec.freq,
                             fsemu_sdlaudio_format_name(device_spec.format), device_spec.channels);
    } else {
        fsemu_audio_log_warning("Could not get the default playback device's format (%s), "
                                "using %d Hz\n",
                                SDL_GetError(), FSEMU_SDLAUDIO_FALLBACK_RATE_HZ);
        device_spec.freq = FSEMU_SDLAUDIO_FALLBACK_RATE_HZ;
    }

    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq = device_spec.freq;
    want.format = SDL_AUDIO_S16LE;
    want.channels = 2;

    SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, FSEMU_SDLAUDIO_DEFAULT_PERIOD_FRAMES);
    fsemu_audio_log_info("Asking for %d Hz, %s, %d channels, a device period of %s sample frames "
                         "(default %s)\n",
                         want.freq, fsemu_sdlaudio_format_name(want.format), want.channels,
                         SDL_GetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES),
                         FSEMU_SDLAUDIO_DEFAULT_PERIOD_FRAMES);

    fsemu_sdlaudio.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want,
                                                      fsemu_sdlaudio_callback, NULL);
    if (fsemu_sdlaudio.stream == NULL) {
        fsemu_audio_log_error("Error opening audio device stream: %s\n", SDL_GetError());
        return;
    }

    // What the device was opened with (asked from the device itself: the stream's format is the
    // one it is fed with), and its device period.
    SDL_AudioSpec have;
    SDL_zero(have);
    int period_frames = 0;
    SDL_AudioDeviceID device = SDL_GetAudioStreamDevice(fsemu_sdlaudio.stream);
    if (!SDL_GetAudioDeviceFormat(device, &have, &period_frames) || have.freq <= 0 ||
        period_frames <= 0) {
        fsemu_audio_log_warning("Could not get the opened device's format (%s)\n", SDL_GetError());
        have = want;
        period_frames = SDL_atoi(FSEMU_SDLAUDIO_DEFAULT_PERIOD_FRAMES);
    }
    fsemu_audio_log_info("Opened audio device: %d Hz, %s, %d channels, a device period of %d "
                         "sample frames (%0.2f ms)\n",
                         have.freq, fsemu_sdlaudio_format_name(have.format), have.channels,
                         period_frames, period_frames * 1000.0 / have.freq);
    if (have.freq != want.freq) {
        fsemu_audio_log_warning("The device's rate is not the one asked for: SDL resamples\n");
    }

    // Paula (or whatever feeds the audio ring buffer) produces at the stream's rate.
    fsemu_audio_set_frequency(want.freq);

    if (FS_IsTraceEnabled()) {
        FS_TraceValue values[] = {
            {"rate_hz", have.freq},
            {"period_frames", period_frames},
            {"stream_rate_hz", want.freq},
        };
        FS_AddTraceInstantWithValues("audio device opened", (int64_t)SDL_GetTicksNS(),
                                     fsemu_frame_get_number(), values, 3);
    }

    SDL_ResumeAudioStreamDevice(fsemu_sdlaudio.stream);
}

void fsemu_sdlaudio_pause(void)
{
    if (fsemu_sdlaudio.stream) {
        SDL_PauseAudioStreamDevice(fsemu_sdlaudio.stream);
        SDL_ClearAudioStream(fsemu_sdlaudio.stream);
    }
}

void fsemu_sdlaudio_resume(void)
{
    if (fsemu_sdlaudio.stream) {
        SDL_ResumeAudioStreamDevice(fsemu_sdlaudio.stream);
    }
}

void fsemu_sdlaudio_clear(void)
{
    if (fsemu_sdlaudio.stream) {
        SDL_ClearAudioStream(fsemu_sdlaudio.stream);
    }
}

// ----------------------------------------------------------------------------

#if 1

// Names the audio callback's track in the timing trace, once per thread (SDL may move the callback
// to another thread when the device changes).
static void fsemu_sdlaudio_trace_name_thread(void)
{
    static thread_local bool named;
    if (!named) {
        FS_SetTraceThreadName("audio");
        named = true;
    }
}

// What the callback hands over at a time: room for more than SDL asks for (512 sample frames).
// Static, so that the callback allocates nothing; only the callback uses it.
#define FSEMU_SDLAUDIO_CHUNK_BYTES 16384
static uint8_t fsemu_sdlaudio_chunk[FSEMU_SDLAUDIO_CHUNK_BYTES];

void fsemu_sdlaudio_callback(void *userdata, SDL_AudioStream *stream, int additional_amount,
                             int total_amount)
{
    if (FS_IsTraceEnabled()) {
        fsemu_sdlaudio_trace_name_thread();
    }
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    int wanted_bytes = additional_amount - additional_amount % FSEMU_AUDIORING_FRAME_BYTES;
    if (wanted_bytes <= 0) {
        return;
    }

    // Hands over exactly what SDL asks for: the audio there is, and silence for the missing part
    // (an audio underrun, counted by the audio ring buffer).
    fsemu_audioring_read_t result = {};
    int missing_bytes = 0;
    for (int done = 0; done < wanted_bytes; done += FSEMU_SDLAUDIO_CHUNK_BYTES) {
        int size = SDL_min(wanted_bytes - done, FSEMU_SDLAUDIO_CHUNK_BYTES);
        fsemu_audiobuffer_read(fsemu_sdlaudio_chunk, size, &result);
        SDL_PutAudioStreamData(stream, fsemu_sdlaudio_chunk, size);
        missing_bytes += result.missing_bytes;
    }
    int ring_bytes = (int)(result.write_position - result.read_position);

    if (missing_bytes > 0 && FS_IsTraceEnabled()) {
        FS_AddTraceInstant("audio underrun", now_ns, fsemu_frame_get_number(), "missing_us",
                           fsemu_audio_bytes_to_ns(missing_bytes));
    }

    // The audio in flight: what was just handed over, which the device now plays (the silence
    // for an audio underrun too, since it plays before the audio written later). Any buffering
    // below SDL (a sound server's) is not known.
    fsemu_audio_register_data_sent(wanted_bytes, now_ns, result.read_position,
                                   result.write_position);

    if (FS_IsTraceEnabled()) {
        // The audio buffer level as of this callback: what is left in the audio ring buffer and
        // the audio in flight (nothing is newly written yet).
        FS_TraceValue level = {"level_us", fsemu_audio_bytes_to_ns(ring_bytes + wanted_bytes)};
        FS_AddTraceCounter("audio buffer level", now_ns, &level, 1);
    }

    fsemu_audio_update_min_fill(ring_bytes);
}

// ----------------------------------------------------------------------------

#endif

#endif // FSEMU_SDL
