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

// FIXME: SDL_SetAudioStreamFrequencyRatio is now built-in...
// SDL_GetAudioStreamQueued

// ----------------------------------------------------------------------------

static struct fsemu_sdlaudio {
    SDL_AudioDeviceID device;
    SDL_AudioStream* stream;
    int buffer_bytes;
} fsemu_sdlaudio;

// ----------------------------------------------------------------------------

void fsemu_sdlaudio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount,
                             int total_amount);

// ----------------------------------------------------------------------------

#if 1

static const char* fsemu_sdlaudio_format_name(SDL_AudioFormat format) {
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

#endif

void fsemu_sdlaudio_init(void) {
#if 1
    fsemu_return_if_already_initialized();

    fsemu_audio_log_info("SDL_Init(SDL_INIT_AUDIO)\n");
    SDL_Init(SDL_INIT_AUDIO);

    const char* sdl_driver_name = SDL_GetCurrentAudioDriver();
    if (sdl_driver_name) {
        fsemu_audio_log_info("SDL audio driver: %s\n", sdl_driver_name);
    } else {
        fsemu_audio_log_warning("Audio subsystem not initialized.\n");
        return;
    }

    int device_count;
    SDL_AudioDeviceID* device_ids = SDL_GetAudioPlaybackDevices(&device_count);
    fsemu_audio_log_info("Playback devices (%d):\n", device_count);
    for (SDL_AudioDeviceID* device_id = device_ids; *device_id; device_id++) {
        fsemu_audio_log_info("Device %d: %s\n", *device_id, SDL_GetAudioDeviceName(*device_id));
    }

#if 0
    int device_count = SDL_GetNumAudioDevices(0);
    fsemu_audio_log_info("Playback devices (%d):\n", device_count);
    for (int i = 0; i < device_count; ++i) {
        fsemu_audio_log_info(
            "Device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }
#endif

    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = fsemu_audio_frequency();
    want.format = SDL_AUDIO_S16LE;
    want.channels = 2;
    // want.samples = 1024; // 512 frames?
    // want.samples = 512;  // 512 frames?
    // want.callback = fsemu_sdlaudio_callback;

#if 0
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
#endif

    SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, "512");

    // SDL_AudioDeviceID dev =
    // SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want); if (dev
    // == 0) {
    //     SDL_Log("Failed to open audio: %s", SDL_GetError());
    // }
    // // else {
    // //     SDL_ResumeAudioDevice(dev); /* start audio playing. */
    // //     SDL_Delay(5000);  // let device play for 5 seconds
    // //     SDL_CloseAudioDevice(dev);
    // // }

    // SDL_SetAudioStreamGetCallback()

    fsemu_sdlaudio.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want,
                                                      fsemu_sdlaudio_callback, NULL);
    // fsemu_sdlaudio.stream =  SDL_OpenAudioDeviceStream(19, &want,
    // fsemu_sdlaudio_callback, NULL);

    if (fsemu_sdlaudio.stream == NULL) {
        fsemu_audio_log_error("Error opening audio device stream: %s\n", SDL_GetError());
    }

    if (!SDL_GetAudioStreamFormat(fsemu_sdlaudio.stream, &have, NULL)) {
        // FIXME: Log error
    }

    fsemu_audio_log_info("Opened audio device\n");
    fsemu_audio_log_info("Frequency: %d\n", have.freq);
    fsemu_audio_log_info("Format: %s (%d)\n", fsemu_sdlaudio_format_name(have.format), have.format);
    fsemu_audio_log_info("Channels: %d\n", have.channels);
#if 0
    fsemu_audio_log_info("Samples(?): %d\n", have.samples);
    fsemu_audio_log_info("Buffer(?): %d\n", have.size);
#endif

    if (have.format != want.format || have.channels != want.channels || have.freq != want.freq) {
        // SDL_Log("We didn't get AUDIO_S16LSB audio format.");
        fsemu_audio_log_info("Didn't get desired audio specs, retrying with SDL conversion\n");
        SDL_CloseAudioDevice(fsemu_sdlaudio.device);
#if 0
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
#endif
    }

    // FIXME: Guessing/lying here..
    int have_size = 512 * 4;

    int buffer = 1000 * have_size / have.freq / 4;
    fsemu_audio_log_info("SDL buffer: %d ms\n", buffer);

    fsemu_sdlaudio.buffer_bytes = have_size;  // or 2???

    // Start audio playing
    // SDL_ResumeAudioDevice(fsemu_sdlaudio.device);

    SDL_ResumeAudioStreamDevice(fsemu_sdlaudio.stream);
#endif
}

// ----------------------------------------------------------------------------

#if 1

void fsemu_sdlaudio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount,
                             int total_amount) {
    int want_bytes = additional_amount;
    // printf("fsemu_sdlaudio_callback %d\n", want_bytes);

    static int64_t last_time;
    int64_t now = fsemu_time_us();
#if 1
    fsemu_audio_log_trace("Buffer: %3d ms (dt %0.1f ms) want %5d B (%4d frames)\n",
                          fsemu_audiobuffer_fill_ms(), ((now - last_time) / 1000.0), want_bytes,
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

    uint8_t volatile* read = fsemu_audiobuffer.read;
    uint8_t volatile* write = fsemu_audiobuffer.write;

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
    // for (int i = 0; i < bytes; i++) {
    //     stream[i] = read[i];
    // }
    SDL_PutAudioStreamData(stream, (void*)read, bytes);
#else
    memcpy(stream, (void*)read, bytes);
#endif
    // stream += bytes;

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
            // for (int i = 0; i < bytes; i++) {
            //     stream[i] = read[i];
            // }
            SDL_PutAudioStreamData(stream, (void*)read, bytes);
#else
            memcpy(stream, (void*)read, bytes);
#endif
            // stream += bytes;

            want_bytes -= bytes;
            bytes_written += bytes;
            read += bytes;
        }
    }

#if 1
    if (want_bytes > 0) {
        void* silence = malloc(want_bytes);
        memset(silence, 0, want_bytes);
        SDL_PutAudioStreamData(stream, silence, want_bytes);
        free(silence);

        // FIXME: Re-enable with log level
        // fsemu_audio_log("%d bytes short of refilling SDL :(\n", want_bytes);
        fsemu_audiobuffer.add_silence = 1;
    }
#endif

    int buffered_bytes = fsemu_sdlaudio.buffer_bytes - wanted_bytes + bytes_written;

    fsemu_audio_register_data_sent(buffered_bytes, now, (uintptr_t)read, (uintptr_t)write);

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

#endif

#endif  // FSEMU_SDL
