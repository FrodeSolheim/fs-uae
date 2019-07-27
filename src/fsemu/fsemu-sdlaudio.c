#define FSEMU_INTERNAL
#include "fsemu/fsemu-sdlaudio.h"

#include "fsemu/fsemu-audio-buffer.h"
#include "fsemu/fsemu-audio.h"
#include "fsemu/fsemu-log.h"
#include "fsemu/fsemu-time.h"

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

// ---------------------------------------------------------------------------

static struct fsemu_sdlaudio {
    int buffer_bytes;
} fsemu_sdlaudio;

// ---------------------------------------------------------------------------

void fsemu_sdlaudio_callback(void *data, Uint8 *stream, int want_bytes);

// ---------------------------------------------------------------------------

void fsemu_sdlaudio_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_log("[FSEMU] SDL_Init(SDL_INIT_AUDIO\n");
    SDL_Init(SDL_INIT_AUDIO);

    const char *sdl_driver_name = SDL_GetCurrentAudioDriver();
    if (sdl_driver_name) {
        fsemu_log("[FSEMU] SDL audio driver: %s\n", sdl_driver_name);
    } else {
        printf("Audio subsystem not initialized.\n");
        return;
    }

    int device_count = SDL_GetNumAudioDevices(0);
    fsemu_log("[FSEMU] Audio playback devices (%d):\n", device_count);
    for (int i = 0; i < device_count; ++i) {
        printf(
            "[FSEMU] Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = fsemu_audio_frequency();
    want.format = AUDIO_S16LSB;
    want.channels = 2;
    // want.samples = 1024; // 512 frames?
    want.samples = 512;  // 512 frames?
    want.callback = fsemu_sdlaudio_callback;

    if (SDL_OpenAudio(&want, &have) < 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
        return;
    }

    if (have.format != want.format) {
        SDL_Log("We didn't get AUDIO_S16LSB audio format.");
        return;
    }

    fsemu_audio_log("[SDL] Opened audio device\n");
    fsemu_audio_log("[SDL] Frequency: %d\n", have.freq);
    fsemu_audio_log("[SDL] Format: AUDIO_S16LSB (%d)\n", have.format);
    fsemu_audio_log("[SDL] Channels: %d\n", have.channels);
    fsemu_audio_log("[SDL] Samples(?): %d\n", have.samples);
    fsemu_audio_log("[SDL] Buffer(?): %d\n", have.size);

    int buffer = 1000 * have.size / have.freq / 4;
    fsemu_log("[FSEMU] SDL buffer: %d ms\n", buffer);

    fsemu_sdlaudio.buffer_bytes = have.size;  // or 2???

    // Start audio playing
    SDL_PauseAudio(0);
}

// ---------------------------------------------------------------------------

void fsemu_sdlaudio_callback(void *data, Uint8 *stream, int want_bytes)
{
    static int64_t last_time;
    int64_t now = fsemu_time_us();
#if 1
    printf(
        "[FSEMU] Audio buffer: %3d ms (dt %0.1f ms) want %5d B (%4d frames)\n",
        fsemu_audio_buffer_fill_ms(),
        ((now - last_time) / 1000.0),
        want_bytes,
        want_bytes / 4);
#endif

    int wanted_bytes = want_bytes;

#if 0
    if (fsemu_audio_buffer_fill() < want_bytes) {
        printf(
            "[FSEMU] ---------------------- UNDERRUN "
            "----------------------\n");
    }
#endif

    // Temp hack
    if (fsemu_audio_buffer_fill_ms() > 100) {
        printf("[FSEMU] Audio ----- reset -----\n");
        fsemu_audio_log_buffer_stats();
        printf("[FSEMU] Audio -----------------\n");
        // int want = fsemu_audio_frequency() * 50 / 1000 * 4;
        int want = 8192;
        // int want = 0;
        fsemu_audio_buffer.read = fsemu_audio_buffer.write - want;
        if (fsemu_audio_buffer.read < fsemu_audio_buffer.data) {
            fsemu_audio_buffer.read += fsemu_audio_buffer.size;
        }
    }
    // -----------------------------------------------------------------------

    int err;

    uint8_t volatile *read = fsemu_audio_buffer.read;
    uint8_t volatile *write = fsemu_audio_buffer.write;

    int bytes = 0;
    int bytes_written = 0;
    bool wrap = false;

    if (write >= read) {
        bytes = write - read;
    } else {
        bytes = fsemu_audio_buffer.end - read;
        wrap = true;
    }

    if (bytes > want_bytes) {
        bytes = want_bytes;
    }
    // printf("[FSEMU] bytes %d\n", bytes);

    // int error = fsemu_audio_alsa_write((void *) read, bytes);
    // FIXME: Check for underrun
    memcpy(stream, (void *) read, bytes);
    stream += bytes;

    want_bytes -= bytes;
    bytes_written += bytes;
    read += bytes;

    if (want_bytes > 0) {
        printf("[FSEMU] ------------------------------------------ \n");
        if (wrap) {
            read = fsemu_audio_buffer.data;
            bytes = write - read;
            if (bytes > want_bytes) {
                bytes = want_bytes;
            }
            // error = fsemu_audio_alsa_write((void *) read, bytes);
            // FIXME: Check for underrun
            memcpy(stream, (void *) read, bytes);
            stream += bytes;

            want_bytes -= bytes;
            bytes_written += bytes;
            read += bytes;
        }
    }

#if 1
    if (want_bytes > 0) {
        memset(stream, 0, want_bytes);
        printf("[FSEMU] %d bytes short of refilling SDL :(\n", want_bytes);
        fsemu_audio_buffer.add_silence = 1;
    }
#endif

    int buffered_bytes =
        fsemu_sdlaudio.buffer_bytes - wanted_bytes + bytes_written;

    fsemu_audio_register_data_sent(
        buffered_bytes, now, (void *) read, (void *) write);

    last_time = now;
    fsemu_audio_buffer.read = read;

    if (bytes_written != wanted_bytes) {
        fsemu_log("[FSEMU] written_bytes != wanted_bytes\n");
        fsemu_audio_register_underrun();
    }

#if 0
    memcpy(stream, (void *) fsemu_audio_buffer.read, len);
    fsemu_audio_buffer.read += len;
    if (fsemu_audio_buffer.read == fsemu_audio_buffer.end) {
        fsemu_audio_buffer.read = fsemu_audio_buffer.data;
    }
#endif
}

// ---------------------------------------------------------------------------

#endif  // FSEMU_SDL
