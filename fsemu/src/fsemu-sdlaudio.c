#define FSEMU_INTERNAL
#include "fsemu-sdlaudio.h"

#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"
#include "fsemu-log.h"
#include "fsemu-option.h"
#include "fsemu-time.h"

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

// ----------------------------------------------------------------------------

static struct fsemu_sdlaudio {
    SDL_AudioDeviceID device;
    int buffer_bytes;
} fsemu_sdlaudio;

// ----------------------------------------------------------------------------

void fsemu_sdlaudio_callback(void *data, Uint8 *stream, int want_bytes);

// ----------------------------------------------------------------------------

static const char *fsemu_sdlaudio_format_name(SDL_AudioFormat format)
{
    switch (format) {
        case AUDIO_S8:
            return "S8";
        case AUDIO_U8:
            return "U8";
        case AUDIO_S16LSB:
            return "S16LSB";
        case AUDIO_S16MSB:
            return "S16MSB";
            // case AUDIO_S16SYS:
            // return "S16SYS";
        case AUDIO_U16LSB:
            return "U16LSB";
        case AUDIO_U16MSB:
            return "U16MSB";
            // case AUDIO_U16SYS:
            // return "U16SYS";
        case AUDIO_S32LSB:
            return "S32LSB";
        case AUDIO_S32MSB:
            return "S32MSB";
            // case AUDIO_S32SYS:
            // return "S32SYS";
        case AUDIO_F32LSB:
            return "F32LSB";
        case AUDIO_F32MSB:
            return "F32MSB";
            // case AUDIO_F32SYS:
            // return "F32SYS";
    }
    return "???";
}

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

    int device_count = SDL_GetNumAudioDevices(0);
    fsemu_audio_log_info("Playback devices (%d):\n", device_count);
    for (int i = 0; i < device_count; ++i) {
        fsemu_audio_log_info(
            "Device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = fsemu_audio_frequency();
    want.format = AUDIO_S16LSB;
    want.channels = 2;
    // want.samples = 1024; // 512 frames?
    want.samples = 512;  // 512 frames?
    want.callback = fsemu_sdlaudio_callback;

    const char *device = fsemu_option_const_string(FSEMU_OPTION_AUDIO_DEVICE);
    if (device) {
        fsemu_audio_log_info("Want device via option: %s\n", device);
    } else {
        fsemu_audio_log_info(
            "No device specified via option, using driver default\n");
    }

    fsemu_sdlaudio.device = SDL_OpenAudioDevice(
        device, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (fsemu_sdlaudio.device == 0) {
        fsemu_audio_log_info("Failed to open audio: %s\n", SDL_GetError());
        return;
    }

    fsemu_audio_log_info("Opened audio device\n");
    fsemu_audio_log_info("Frequency: %d\n", have.freq);
    fsemu_audio_log_info("Format: %s (%d)\n",
                         fsemu_sdlaudio_format_name(have.format),
                         have.format);
    fsemu_audio_log_info("Channels: %d\n", have.channels);
    fsemu_audio_log_info("Samples(?): %d\n", have.samples);
    fsemu_audio_log_info("Buffer(?): %d\n", have.size);

    if (have.format != want.format || have.channels != want.channels ||
        have.freq != want.freq) {
        // SDL_Log("We didn't get AUDIO_S16LSB audio format.");
        fsemu_audio_log_info(
            "Didn't get desired audio specs, retrying with SDL conversion\n");
        SDL_CloseAudioDevice(fsemu_sdlaudio.device);

        fsemu_sdlaudio.device =
            SDL_OpenAudioDevice(device, 0, &want, &have, 0);
        if (fsemu_sdlaudio.device == 0) {
            fsemu_audio_log_error("Failed to open audio: %s\n",
                                  SDL_GetError());
            return;
        }

        fsemu_audio_log_info("Opened audio device\n");
        fsemu_audio_log_info("Frequency: %d\n", have.freq);
        fsemu_audio_log_info("Format: %s (%d)d\n",
                             fsemu_sdlaudio_format_name(have.format),
                             have.format);
        fsemu_audio_log_info("Channels: %d\n", have.channels);
        fsemu_audio_log_info("Samples(?): %d\n", have.samples);
        fsemu_audio_log_info("Buffer(?): %d\n", have.size);
    }

    int buffer = 1000 * have.size / have.freq / 4;
    fsemu_audio_log_info("SDL buffer: %d ms\n", buffer);

    fsemu_sdlaudio.buffer_bytes = have.size;  // or 2???

    // Start audio playing
    SDL_PauseAudioDevice(fsemu_sdlaudio.device, 0);
}

// ----------------------------------------------------------------------------

void fsemu_sdlaudio_callback(void *data, Uint8 *stream, int want_bytes)
{
    static int64_t last_time;
    int64_t now = fsemu_time_us();
#if 1
    fsemu_audio_log_trace(
        "Buffer: %3d ms (dt %0.1f ms) want %5d B (%4d frames)\n",
        fsemu_audiobuffer_fill_ms(),
        ((now - last_time) / 1000.0),
        want_bytes,
        want_bytes / 4);
#endif

    int wanted_bytes = want_bytes;

#if 0
    if (fsemu_audiobuffer_fill() < want_bytes) {
        fsemu_audio_log(
            "---------------------- UNDERRUN "
            "----------------------\n");
    }
#endif

    // Temp hack
    if (fsemu_audiobuffer_fill_ms() > 100) {
        fsemu_audio_log("----- reset -----\n");
        fsemu_audio_log_buffer_stats();
        fsemu_audio_log("-----------------\n");
        // int want = fsemu_audio_frequency() * 50 / 1000 * 4;
        int want = 8192;
        // int want = 0;
        fsemu_audiobuffer.read = fsemu_audiobuffer.write - want;
        if (fsemu_audiobuffer.read < fsemu_audiobuffer.data) {
            fsemu_audiobuffer.read += fsemu_audiobuffer.size;
        }
    }
    // -----------------------------------------------------------------------

    uint8_t volatile *read = fsemu_audiobuffer.read;
    uint8_t volatile *write = fsemu_audiobuffer.write;

    int bytes = 0;
    int bytes_written = 0;
    bool wrap = false;

    if (write >= read) {
        bytes = write - read;
    } else {
        bytes = fsemu_audiobuffer.end - read;
        wrap = true;
    }

    if (bytes > want_bytes) {
        bytes = want_bytes;
    }
    // fsemu_audio_log("bytes %d\n", bytes);

    // int error = fsemu_audio_alsa_write((void *) read, bytes);
    // FIXME: Check for underrun
#if 1
    for (int i = 0; i < bytes; i++) {
        stream[i] = read[i];
    }
#else
    memcpy(stream, (void *) read, bytes);
#endif
    stream += bytes;

    want_bytes -= bytes;
    bytes_written += bytes;
    read += bytes;

    if (want_bytes > 0) {
        // fsemu_audio_log("------------------------------------------ \n");
        if (wrap) {
            read = fsemu_audiobuffer.data;
            bytes = write - read;
            if (bytes > want_bytes) {
                bytes = want_bytes;
            }
            // error = fsemu_audio_alsa_write((void *) read, bytes);
            // FIXME: Check for underrun
#if 1
            for (int i = 0; i < bytes; i++) {
                stream[i] = read[i];
            }
#else
            memcpy(stream, (void *) read, bytes);
#endif
            stream += bytes;

            want_bytes -= bytes;
            bytes_written += bytes;
            read += bytes;
        }
    }

#if 1
    if (want_bytes > 0) {
        memset(stream, 0, want_bytes);
        // FIXME: Re-enable with log level
        // fsemu_audio_log("%d bytes short of refilling SDL :(\n", want_bytes);
        fsemu_audiobuffer.add_silence = 1;
    }
#endif

    int buffered_bytes =
        fsemu_sdlaudio.buffer_bytes - wanted_bytes + bytes_written;

    fsemu_audio_register_data_sent(
        buffered_bytes, now, (uintptr_t) read, (uintptr_t) write);

    last_time = now;
    fsemu_audiobuffer.read = read;

    if (bytes_written != wanted_bytes) {
        // FIXME: Re-enable with log level
        // fsemu_audio_log("written_bytes != wanted_bytes\n");
        fsemu_audio_register_underrun();
    }

    fsemu_audio_update_min_fill(read, write);

#if 0
    memcpy(stream, (void *) fsemu_audiobuffer.read, len);
    fsemu_audiobuffer.read += len;
    if (fsemu_audiobuffer.read == fsemu_audiobuffer.end) {
        fsemu_audiobuffer.read = fsemu_audiobuffer.data;
    }
#endif
}

// ----------------------------------------------------------------------------

#endif  // FSEMU_SDL
