#define FSEMU_INTERNAL
#include "fsemu-audiobuffer.h"

#include "fsemu-audio.h"
#include "fsemu-frame.h"
#include "fsemu-time.h"
#include "fsemu-util.h"

#ifdef FSEMU_SAMPLERATE
#include <samplerate.h>
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/*
int fsemu_audiobuffer.size;
volatile uint8_t *fsemu_audiobuffer;
volatile uint8_t *fsemu_audiobuffer.end;
volatile uint8_t *volatile fsemu_audiobuffer.read;
volatile uint8_t *volatile fsemu_audiobuffer.write;
*/
fsemu_audiobuffer_t fsemu_audiobuffer;

static struct {
    bool initialized;
    int bytes_for_frame;
#ifdef FSEMU_SAMPLERATE
    SRC_STATE* src_state;
    double adjustment;
    float src_in[8192];   // FIXME: suitable size?
    float src_out[8192];  // FIXME: suitable size?
#endif
} fsemu_audiobuffer_extra;

void fsemu_audiobuffer_init(void) {
    fsemu_audiobuffer_extra.initialized = true;
#ifdef FSEMU_SAMPLERATE
    int channels = 2;
    int err;
    fsemu_audiobuffer_extra.src_state = src_new(SRC_SINC_BEST_QUALITY, channels, &err);
#endif

    // 1 second ring buffer (2 channels, 2 bytes per sample)
    // fsemu_audiobuffer.size = fsemu_audio_frequency() * 2 * 2;
    // fsemu_audiobuffer.size = fsemu_audio_frequency() * 2 * 2;

    // Make audio buffer_size a multiple of 512, 1024, etc.
    // Between 0.5 and 1 second ring buffer...
    fsemu_audiobuffer.size = 2 << 16;
    fsemu_audiobuffer.data = (uint8_t*)malloc(fsemu_audiobuffer.size);
    fsemu_audiobuffer.end = fsemu_audiobuffer.data + fsemu_audiobuffer.size;
    fsemu_audiobuffer.read = fsemu_audiobuffer.data;
    fsemu_audiobuffer.write = fsemu_audiobuffer.data;

    fsemu_audiobuffer_clear();
}

void fsemu_audiobuffer_clear(void) {
    fsemu_assert(fsemu_audiobuffer_extra.initialized);
#if 1
    for (int i = 0; i < fsemu_audiobuffer.size; i++) {
        fsemu_audiobuffer.data[i] = 0;
    }
#else
    memset((void*)fsemu_audiobuffer.data, 0, fsemu_audiobuffer.size);
#endif
}

int fsemu_audiobuffer_fill(void) {
    uint8_t volatile* read = fsemu_audiobuffer.read;
    uint8_t volatile* write = fsemu_audiobuffer.write;
    if (write >= read) {
        return write - read;
    }
    return (fsemu_audiobuffer.end - read) + (write - fsemu_audiobuffer.data);
}

int fsemu_audiobuffer_fill_ms(void) {
    int frames = fsemu_audiobuffer_fill() / 4;
    return frames * 1000 / fsemu_audio_frequency();
}

int64_t fsemu_audiobuffer_fill_us(void) {
    int frames = fsemu_audiobuffer_fill() / 4;
    return frames * 1000000LL / fsemu_audio_frequency();
}

#if 0
static void fsemu_audiobuffer_update_2(const void *data, int size)
{
    // int add_silence = fsemu_audiobuffer.add_silence;
    // if (add_silence) {
    //     fsemu_audiobuffer.add_silence = 0;
    //     fsemu_audiobuffer_write_silence_ms(add_silence);
    // }

#if 0
    int buffer_bytes = fsemu_audiobuffer_fill();
    int buffer_frames = buffer_bytes / 4;
    int buffer_ms = buffer_frames * 1000 / fsemu_audio_frequency();
    // fsemu_audio_log("[FSE] %2d ms / %4d B + %4d B\n", buffer_ms,
    // buffer_bytes, size);
#endif

    ptrdiff_t available = fsemu_audiobuffer.end - fsemu_audiobuffer.write;
    if (available < size) {
        memcpy((void *) fsemu_audiobuffer.write, data, available);
        fsemu_audiobuffer.write = fsemu_audiobuffer.data;
        data += available;
        size -= available;
    }
    if (size) {
        memcpy((void *) fsemu_audiobuffer.write, data, size);
        fsemu_audiobuffer.write += size;
    }
}
#endif

void fsemu_audiobuffer_update(const void* void_data, int size) {
    // printf("fsemu_audiobuffer_update data=%p size=%d\n", void_data, size);
    fsemu_assert(fsemu_audiobuffer_extra.initialized);
    // Casting to char pointer to be able to do byte pointer arithmetic.
    const uint8_t* data = (const uint8_t*)void_data;

    fsemu_audiobuffer_extra.bytes_for_frame += size;

    int add_silence = fsemu_audiobuffer.add_silence;
    if (add_silence) {
        fsemu_audiobuffer.add_silence = 0;
        fsemu_audiobuffer_write_silence_ms(add_silence);
    }

#ifdef FSEMU_SAMPLERATE
    bool use_samplerate = true;
    if (use_samplerate) {
        // ptrdiff_t available = fsemu_audiobuffer.end -
        // fsemu_audiobuffer.write;

        int samples = size / 2;
        int frames = samples / 2;
        // int16_t *intdata = data;
        // int16_t *intdata_p = intdata;
        float* floatdata = fsemu_audiobuffer_extra.src_in;
        // float *floatdata_p = floatdata;
        /*
        for (int i = 0; i < samples; i++) {
            *floatdata_p = *intdata_p / 32
        }
        */
        src_short_to_float_array((const short*)data, floatdata, samples);

        SRC_DATA src_data;
        // data_in       : A pointer to the input data samples.
        // input_frames  : The number of frames of data pointed to by data_in.
        // data_out      : A pointer to the output data samples.
        // output_frames : Maximum number of frames pointer to by data_out.
        // src_ratio     : Equal to output_sample_rate / input_sample_rate.
        // end_of_input  : Equal to 0 if more input data is available and 1
        //                 otherwise.
        src_data.data_in = fsemu_audiobuffer_extra.src_in;
        src_data.input_frames = frames;
        src_data.data_out = fsemu_audiobuffer_extra.src_out;
        src_data.output_frames = 8192 / 2;  // FIXME
        src_data.src_ratio = 1.0 + fsemu_audiobuffer_extra.adjustment;
        src_data.end_of_input = 0;

        src_process(fsemu_audiobuffer_extra.src_state, &src_data);

        int size_out = src_data.output_frames_gen * 4;
        // float *floatdata_p = fsemu_audiobuffer_extra.src_out;
        floatdata = fsemu_audiobuffer_extra.src_out;

        ptrdiff_t available = fsemu_audiobuffer.end - fsemu_audiobuffer.write;
        if (available < size_out) {
#if 1
            for (int i = 0; i < available / 2; i++) {
                int16_t value = (int16_t)(floatdata[i] * 32767);
                fsemu_audiobuffer.write[i * 2] = value & 0xff;
                fsemu_audiobuffer.write[i * 2 + 1] = (value >> 8) & 0xff;
            }
#else
            src_float_to_short_array(floatdata, (short*)fsemu_audiobuffer.write, available / 2);
#endif
            fsemu_audiobuffer.write = fsemu_audiobuffer.data;
            data += available;
            size_out -= available;

            floatdata += available / 2;
        }
        if (size_out) {
#if 1
            for (int i = 0; i < size_out / 2; i++) {
                int16_t value = (int16_t)(floatdata[i] * 32767);
                fsemu_audiobuffer.write[i * 2] = value & 0xff;
                fsemu_audiobuffer.write[i * 2 + 1] = (value >> 8) & 0xff;
            }
#else
            src_float_to_short_array(floatdata, (short*)fsemu_audiobuffer.write, size_out / 2);
#endif
            fsemu_audiobuffer.write += size_out;
        }
        return;
    }
#endif

    ptrdiff_t available = fsemu_audiobuffer.end - fsemu_audiobuffer.write;
    // printf("end %p - write %p = available %ld\n", fsemu_audiobuffer.end, fsemu_audiobuffer.write,
    // available);
    if (available < size) {
#if 1
        for (int i = 0; i < available; i++) {
            fsemu_audiobuffer.write[i] = data[i];
        }
#else
        memcpy((void*)fsemu_audiobuffer.write, data, available);
#endif
        fsemu_audiobuffer.write = fsemu_audiobuffer.data;
        data += available;
        size -= available;
    }
    if (size) {
#if 1
        for (int i = 0; i < size; i++) {
            fsemu_audiobuffer.write[i] = data[i];
        }
#else
        memcpy((void*)fsemu_audiobuffer.write, data, size);
#endif
        fsemu_audiobuffer.write += size;
    }
}

void fsemu_audiobuffer_frame_done(void) {
    int samples = fsemu_audiobuffer_extra.bytes_for_frame / 2;
    int frames = samples / 2;
    fsemu_frame_log_epoch("Audio done for frame: %d samples / 2 = %d frames\n", samples, frames);
    fsemu_audiobuffer_extra.bytes_for_frame = 0;
}

void fsemu_audiobuffer_write_silence(int size) {
    // FIXME: Use a static array instead and avoid malloc?
    // Can write repeatedly from the same buffer;
    /*
    void *data = malloc(size);
    memset(data, 0, sizeof(data));
    fsemu_audiobuffer_update(data, size);
    free(data);
    */

    static uint8_t data[512];  // silence
    while (size) {
        int chunk = MIN(size, 512);
        fsemu_audiobuffer_update(data, size);
        size = size - chunk;
    }
}

void fsemu_audiobuffer_write_silence_ms(int ms) {
    int bytes = 4 * ms * fsemu_audio_frequency() / 1000;
    fsemu_audiobuffer_write_silence(bytes);
}

#ifdef FSEMU_SAMPLERATE

double fsemu_audiobuffer_adjustment(void) {
    return fsemu_audiobuffer_extra.adjustment;
}

void fsemu_audiobuffer_set_adjustment(double adjustment) {
    fsemu_frame_log_epoch("Audio %s%0.5f\n", adjustment >= 0 ? "+" : "", adjustment);
    fsemu_audiobuffer_extra.adjustment = adjustment;
}

#endif

int fsemu_audiobuffer_calculate_target(void) {
    // FIXME: We can do better. We don't need 20 ms of buffer at the *end*
    // of a frame if the frame was done generating after 2ms from frame
    // begin; then we really only need a few ms of buffer, since we will
    // soon get more data. So, we should probably take (moving average)
    // time to generate frame into account.
    // return 10000;

    // Using one video frame duration for audio latency
    int target_latency_us = 1000000 / fsemu_frame_rate_hz();

#if 0
    // Using one frame + 33% for audio buffer target
    target_latency_us *= 1.33;
#endif

    return target_latency_us;
}

#if 0
#define KP 0.000000250
#define KI 0.000000001
#define KD 0.000000000
#endif

#if 0
#define KP (0.000001750 / 2)
#define KI (0.0000000035)
#define KD (0.00001)
#endif

#define KP 0.000000100
#define KI 0.000000001
#define KD 0.000000000

static double pid_controller_step(int* error_out, int* error_sum_out, int* latency_out) {
#if 1
    int latency = fsemu_audio_latency_us();

    static fsemu_mavgi_t latency_mavg;
#define PID_LATENCY_VALUES 64
    static int latency_values[PID_LATENCY_VALUES];
    int latency_avg_2 = fsemu_mavgi(&latency_mavg, latency_values, PID_LATENCY_VALUES, latency);
    if (latency_out) {
        *latency_out = latency;
    }
#endif

    // fsemu_audio_frame_stats_t *stats =
    //     &fsemu_audio
    //          .stats[fsemu_frame_counter())];

    fsemu_audio_frame_stats_t stats;
    fsemu_audio_frame_stats(fsemu_frame_counter() - 1, &stats);

#if 0
    // FIXME: Re-enable later while testing audio latency
    if (stats.avg_latency_us != latency_avg_2) {
        printf(
            "WARNING: Adjust %d vs %d\n", stats.avg_latency_us, latency_avg_2);
    }
#endif

    int target_latency_us = fsemu_audiobuffer_calculate_target();
    int error = target_latency_us - stats.avg_latency_us;

    static fsemu_mavgi_t error_mavg;
#define PID_ERROR_VALUES 128
    static int error_values[PID_ERROR_VALUES];
    fsemu_mavgi(&error_mavg, error_values, PID_ERROR_VALUES, error);
    int error_sum = error_mavg.sum;

#if 0
    static int64_t last_last_time;
    static int64_t last_time;
    static int last_last_error;
    static int last_error;
    int64_t now = fsemu_time_us();

    if (last_last_time == 0) {
        last_time = now;
        last_last_time = now;
    }
    int dt = (int) (now - last_last_time);
    double derivative;
    if (dt > 0) {
        derivative = (last_last_error - error) / dt;
    } else {
        derivative = 0;
    }
    last_last_error = last_error;
    last_last_time = last_time;
    last_error = error;
    last_time = now;
#else
    double derivative = 0;
#endif

    double output = KP * error + KI * error_sum + KD * derivative;

    *error_out = error;
    *error_sum_out = error_sum;

    return MAX(-0.5, MIN(0.5, output));
}

double fsemu_audiobuffer_calculate_adjustment(void) {
    double frame_rate_adjust = 0.0;
    // frame_rate_adjust = fsemu_frame_rate_multiplier();
    // frame_rate_adjust = 60 / 50.0;

    // frame_rate_adjust = 0.0;

    int error, error_sum, latency;
    double pid_adjust = pid_controller_step(&error, &error_sum, &latency);

    double adjust = frame_rate_adjust + pid_adjust;
    // double adjust = frame_rate_adjust;
#if 0
    printf("%0.1f Adjust %0.5f (%0.5f + PID %0.5f) Err %d sum %d\n",
           latency / 1000.0,
           adjust,
           frame_rate_adjust,
           pid_adjust,
           error,
           error_sum);
#endif
    fsemu_audio_log_trace("%0.1f Adjust %0.5f (%0.5f + PID %0.5f) Err %d sum %d\n",
                          latency / 1000.0, adjust, frame_rate_adjust, pid_adjust, error,
                          error_sum);
    return adjust;
}
