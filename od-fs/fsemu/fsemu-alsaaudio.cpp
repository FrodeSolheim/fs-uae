#define FSEMU_INTERNAL
#include "fsemu-alsaaudio.h"

#include "fsemu-audio.h"
#include "fsemu-audiobuffer.h"
#include "fsemu-log.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"

#ifdef FSEMU_ALSA

#include <alsa/asoundlib.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

static struct {
    int buffer_bytes;
} fsemu_alsaaudio;

snd_pcm_t* playback_handle;

#if 0
int playback_callback(snd_pcm_sframes_t nframes)
{
    int err;

    printf("playback callback called with %u frames\n", nframes);

    /* ... fill buf with data ... */

    if ((err = snd_pcm_writei(playback_handle, buf, nframes)) < 0) {
        fprintf(stderr, "write failed (%s)\n", snd_strerror(err));
    }

    return err;
}
#endif

static void fsemu_alsaaudio_handle_underrun(void) {
    // fsemu_audio_log_inflight_estimate();
    fsemu_audio_log("----------\n");
    if (fsemu_audio_log_buffer_stats() <= 1) {
        // We ran out of data for realz (probably), so add some silence to
        // the buffer to aid in recovery.
        fsemu_audiobuffer.add_silence = 1;

        // FIXME: Get definitive information about underrun from ALSA ?
        fsemu_audio_register_underrun();
    }
    fsemu_audio_log("----------\n");

    // FIXME: Race condition (with emulation thread) on updating the write
    // buffer
    // fsemu_audiobuffer_write_silence_ms(1);
}

static int fsemu_alsaaudio_write(void* buffer, int bytes) {
    int err;
    if ((err = snd_pcm_writei(playback_handle, buffer, bytes / 4)) < 0) {
        fprintf(stderr, "write failed (%s)\n", snd_strerror(err));

        fsemu_alsaaudio_handle_underrun();

        if (err = snd_pcm_recover(playback_handle, err, 0)) {
            fprintf(stderr, "snd_pcm_recover (%s)\n", snd_strerror(err));
        } else {
            if ((err = snd_pcm_writei(playback_handle, buffer, bytes / 4)) < 0) {
                fprintf(stderr, "write failed!!! (%s)\n", snd_strerror(err));
                // FIXME: Not recovering?
            }
        }
    }
}

static void fsemu_alsaaudio_callback(snd_pcm_sframes_t want_frames) {
    static int64_t last_time;
    int64_t now = fsemu_time_us();
#if 0
    printf("[FSE] Audio buffer: %3d ms (dt %2d ms) want %5d B (%4d frames)\n",
           fsemu_audiobuffer_fill_ms(),
           (int)((now - last_time) / 1000),
           (int) want_frames * 4,
           (int) want_frames);
#endif
    int want_bytes = want_frames * 4;
#if 0

    // -----------------------------------------------------------------------
    if (fsemu_audiobuffer_fill() < want_bytes) {
        printf(
            "[FSE] ---------------------- UNDERRUN "
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

    int err;

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
    // printf("[FSE] bytes %d\n", bytes);

    int error = fsemu_alsaaudio_write((void*)read, bytes);
    // FIXME: Check for underrun
    want_bytes -= bytes;
    bytes_written += bytes;
    read += bytes;

    if (want_bytes > 0) {
        if (wrap) {
            read = fsemu_audiobuffer.data;
            bytes = write - read;
            if (bytes > want_bytes) {
                bytes = want_bytes;
            }
            error = fsemu_alsaaudio_write((void*)read, bytes);
            // FIXME: Check for underrun
            want_bytes -= bytes;
            bytes_written += bytes;
            read += bytes;
        }
    }

#if 1
    // there isn't a buffer underrun until alsa says so
    if (want_bytes > 0) {
        // printf("[FSE] %d bytes short of refilling ALSA\n", want_bytes);
    }
#endif

    int wanted_bytes = want_frames * 4;
    int buffered_bytes = fsemu_audio_alsa.buffer_bytes - wanted_bytes + bytes_written;
    fsemu_audio_register_data_sent(buffered_bytes, now, (void*)read, (void*)write);

    last_time = now;
    fsemu_audiobuffer.read = read;
}

static void* fsemu_alsaaudio_thread(void* data) {
    fsemu_thread_set_priority();

    snd_pcm_sframes_t frames_to_deliver;
    int err;
    while (1) {
        /* wait till the interface is ready for data, or 1 second
           has elapsed.
        */

        if ((err = snd_pcm_wait(playback_handle, 1000)) < 0) {
            fprintf(stderr, "poll failed %d (%s)\n", err, strerror(err));

            fsemu_alsaaudio_handle_underrun();

            if (err = snd_pcm_recover(playback_handle, err, 0)) {
                fprintf(stderr, "snd_pcm_recover (%s)\n", snd_strerror(err));
            }
            continue;
        }

        /* find out how much space is available for playback data */

        if ((frames_to_deliver = snd_pcm_avail_update(playback_handle)) < 0) {
            err = -frames_to_deliver;
            if (err == EPIPE) {
                fprintf(stderr, "an xrun occured\n");
            } else {
                fsemu_warning("Unknown ALSA error from snd_pcm_avail_update (%d)", err);
                fprintf(stderr, "unknown ALSA avail update return value (%d)\n", err);
            }

            fsemu_alsaaudio_handle_underrun();

            if (err = snd_pcm_recover(playback_handle, err, 0)) {
                fprintf(stderr, "snd_pcm_recover (%s)\n", snd_strerror(err));
                // FIXME: fsemu_warning
                break;
            }
            continue;
        }

        // frames_to_deliver =
        //    frames_to_deliver > 4096 ? 4096 : frames_to_deliver;

        /* deliver the data */
        /*
        if (fsemu_alsaaudio_callback(frames_to_deliver) != frames_to_deliver)
        { fprintf(stderr, "playback callback failed\n"); break;
        }
        */

        /* We deliver as much data as we can, so the ALSA buffer is as full
           as possible. This makes estimating (combined) buffer fill easier. */
        fsemu_alsaaudio_callback(frames_to_deliver);
        // FIXME: period
        // fsemu_alsaaudio_callback(128);
    }

    snd_pcm_close(playback_handle);

    return NULL;
}

void fsemu_alsaaudio_init(void) {
    fsemu_audio_log("Initialize ALSA audio driver\n");

    snd_pcm_hw_params_t* hw_params;
    snd_pcm_sw_params_t* sw_params;
    int nfds;
    int err;
    struct pollfd* pfds;

    // const char *device_name = "default";
    const char* device_name = "hw:0,0";

    if ((err = snd_pcm_open(&playback_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %s (%s)\n", device_name, snd_strerror(err));
        return;
    }
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
        return;
    }
    if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
        return;
    }

    if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params,
                                            SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
        return;
    }
    if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) <
        0) {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
        return;
    }

    snd_pcm_uframes_t buffer_size = 256;
    snd_pcm_uframes_t period_size = 64;

    if ((err = snd_pcm_hw_params_set_buffer_size_near(playback_handle, hw_params, &buffer_size)) <
        0) {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
        return;
    }

    if ((err = snd_pcm_hw_params_set_period_size_near(playback_handle, hw_params, &period_size,
                                                      NULL)) < 0) {
        fprintf(stderr, "cannot set period size (%s)\n", snd_strerror(err));
        return;
    }

    unsigned int frequency = fsemu_audio_frequency();

    if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &frequency, 0)) < 0) {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
        return;
    }
    if (frequency != fsemu_audio_frequency()) {
        fsemu_warning("Configured ALSA frequency not same as configured");
        // FIXME: Handle
    }

    if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)) < 0) {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
        return;
    }
    if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
        return;
    }

    fsemu_audio_log("ALSA frequency: %u Hz)\n", frequency);

    if (err = snd_pcm_hw_params_get_period_size(hw_params, &period_size, NULL) < 0) {
        fsemu_warning("[FSE] Could not query ALSA period size\n");
        snd_strerror(err);
    }
    fsemu_audio_log("ALSA period size: %ld frames\n", period_size);

    buffer_size = 0;
    if (err = snd_pcm_hw_params_get_buffer_size(hw_params, &buffer_size) < 0) {
        fsemu_warning("[FSE] Could not query ALSA period size\n");
        snd_strerror(err);
    }
    fsemu_audio_log("ALSA buffer size: %ld frames (%ld ms)\n", buffer_size,
                    buffer_size * 1000 / fsemu_audio_frequency());

    fsemu_audio_alsa.buffer_bytes = buffer_size * 4;

    snd_pcm_hw_params_free(hw_params);

    /* tell ALSA to wake us up whenever XXXX or more frames
       of playback data can be delivered. Also, tell
       ALSA that we'll start the device ourselves.
    */

    if ((err = snd_pcm_sw_params_malloc(&sw_params)) < 0) {
        fprintf(stderr, "cannot allocate software parameters structure (%s)\n", snd_strerror(err));
        return;
    }
    if ((err = snd_pcm_sw_params_current(playback_handle, sw_params)) < 0) {
        fprintf(stderr, "cannot initialize software parameters structure (%s)\n",
                snd_strerror(err));
        return;
    }
#if 0
    if ((err = snd_pcm_sw_params_set_avail_min(
             playback_handle, sw_params, period)) < 0) {
        fprintf(stderr,
                "cannot set minimum available count (%s)\n",
                snd_strerror(err));
        return;
    }
#endif
    if ((err = snd_pcm_sw_params_set_start_threshold(playback_handle, sw_params, 0U)) < 0) {
        fprintf(stderr, "cannot set start mode (%s)\n", snd_strerror(err));
        return;
    }
    if ((err = snd_pcm_sw_params(playback_handle, sw_params)) < 0) {
        fprintf(stderr, "cannot set software parameters (%s)\n", snd_strerror(err));
        return;
    }

    /* the interface will interrupt the kernel every 4096 frames, and ALSA
       will wake up this program very soon after that.
    */

    if ((err = snd_pcm_prepare(playback_handle)) < 0) {
        fprintf(stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
        return;
    }

    fsemu_thread_create("alsa", fsemu_alsaaudio_thread, NULL);
}

#endif  // FSEMU_ALSA
