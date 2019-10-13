#define FSEMU_INTERNAL
#include "fsemu-audio-buffer.h"

#include "fsemu-audio.h"
#include "fsemu-util.h"

#ifdef FSEMU_SAMPLERATE
#include <samplerate.h>
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/*
int fsemu_audio_buffer.size;
volatile uint8_t *fsemu_audio_buffer;
volatile uint8_t *fsemu_audio_buffer.end;
volatile uint8_t *volatile fsemu_audio_buffer.read;
volatile uint8_t *volatile fsemu_audio_buffer.write;
*/
fsemu_audio_buffer_t fsemu_audio_buffer;

static struct {
#ifdef FSEMU_SAMPLERATE
    SRC_STATE *src_state;
    double adjustment;
    float src_in[8192];   // FIXME: suitable size?
    float src_out[8192];  // FIXME: suitable size?
#endif
} fsemu_audiobuffer_extra;

void fsemu_audio_buffer_init(void)
{
    int channels = 2;
#ifdef FSEMU_SAMPLERATE
    int err;
    fsemu_audiobuffer_extra.src_state =
        src_new(SRC_SINC_BEST_QUALITY, channels, &err);
#endif

    // 1 second ring buffer (2 channels, 2 bytes per sample)
    // fsemu_audio_buffer.size = fsemu_audio_frequency() * 2 * 2;
    // fsemu_audio_buffer.size = fsemu_audio_frequency() * 2 * 2;

    // Make audio buffer_size a multiple of 512, 1024, etc.
    // Between 0.5 and 1 second ring buffer...
    fsemu_audio_buffer.size = 2 << 16;
    fsemu_audio_buffer.data = (uint8_t *) malloc(fsemu_audio_buffer.size);
    fsemu_audio_buffer.end = fsemu_audio_buffer.data + fsemu_audio_buffer.size;
    fsemu_audio_buffer.read = fsemu_audio_buffer.data;
    fsemu_audio_buffer.write = fsemu_audio_buffer.data;

    fsemu_audio_buffer_clear();
}

void fsemu_audio_buffer_clear(void)
{
    memset((void *) fsemu_audio_buffer.data, 0, fsemu_audio_buffer.size);
}

int fsemu_audio_buffer_fill(void)
{
    uint8_t volatile *read = fsemu_audio_buffer.read;
    uint8_t volatile *write = fsemu_audio_buffer.write;
    if (write >= read) {
        return write - read;
    }
    return (fsemu_audio_buffer.end - read) + (write - fsemu_audio_buffer.data);
}

int fsemu_audio_buffer_fill_ms(void)
{
    int frames = fsemu_audio_buffer_fill() / 4;
    return frames * 1000 / fsemu_audio_frequency();
}

int64_t fsemu_audio_buffer_fill_us(void)
{
    int frames = fsemu_audio_buffer_fill() / 4;
    return frames * 1000000LL / fsemu_audio_frequency();
}

#if 0
static void fsemu_audio_buffer_update_2(const void *data, int size)
{
    // int add_silence = fsemu_audio_buffer.add_silence;
    // if (add_silence) {
    //     fsemu_audio_buffer.add_silence = 0;
    //     fsemu_audio_buffer_write_silence_ms(add_silence);
    // }

#if 0
    int buffer_bytes = fsemu_audio_buffer_fill();
    int buffer_frames = buffer_bytes / 4;
    int buffer_ms = buffer_frames * 1000 / fsemu_audio_frequency();
    // fsemu_audio_log("[FSEMU] %2d ms / %4d B + %4d B\n", buffer_ms,
    // buffer_bytes, size);
#endif

    ptrdiff_t available = fsemu_audio_buffer.end - fsemu_audio_buffer.write;
    if (available < size) {
        memcpy((void *) fsemu_audio_buffer.write, data, available);
        fsemu_audio_buffer.write = fsemu_audio_buffer.data;
        data += available;
        size -= available;
    }
    if (size) {
        memcpy((void *) fsemu_audio_buffer.write, data, size);
        fsemu_audio_buffer.write += size;
    }
}
#endif

void fsemu_audio_buffer_update(const void *data, int size)
{
    int add_silence = fsemu_audio_buffer.add_silence;
    if (add_silence) {
        fsemu_audio_buffer.add_silence = 0;
        fsemu_audio_buffer_write_silence_ms(add_silence);
    }

#ifdef FSEMU_SAMPLERATE
    bool use_samplerate = true;
    if (use_samplerate) {
        // ptrdiff_t available = fsemu_audio_buffer.end -
        // fsemu_audio_buffer.write;

        int samples = size / 2;
        int frames = samples / 2;
        // int16_t *intdata = data;
        // int16_t *intdata_p = intdata;
        float *floatdata = fsemu_audiobuffer_extra.src_in;
        // float *floatdata_p = floatdata;
        /*
        for (int i = 0; i < samples; i++) {
            *floatdata_p = *intdata_p / 32
        }
        */
        src_short_to_float_array(data, floatdata, samples);

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

        ptrdiff_t available =
            fsemu_audio_buffer.end - fsemu_audio_buffer.write;
        if (available < size_out) {
            // memcpy((void *) fsemu_audio_buffer.write, data, available);
            src_float_to_short_array(
                floatdata, (short *) fsemu_audio_buffer.write, available / 2);
            fsemu_audio_buffer.write = fsemu_audio_buffer.data;
            data += available;
            size_out -= available;

            floatdata += available / 2;
        }
        if (size_out) {
            // memcpy((void *) fsemu_audio_buffer.write, data, size);
            src_float_to_short_array(
                floatdata, (short *) fsemu_audio_buffer.write, size_out / 2);
            fsemu_audio_buffer.write += size_out;
        }
        return;
    }
#endif

    ptrdiff_t available = fsemu_audio_buffer.end - fsemu_audio_buffer.write;
    if (available < size) {
        memcpy((void *) fsemu_audio_buffer.write, data, available);
        fsemu_audio_buffer.write = fsemu_audio_buffer.data;
        data += available;
        size -= available;
    }
    if (size) {
        memcpy((void *) fsemu_audio_buffer.write, data, size);
        fsemu_audio_buffer.write += size;
    }
}

void fsemu_audio_buffer_write_silence(int size)
{
    // FIXME: Use a static array instead and avoid malloc?
    // Can write repeatedly from the same buffer;
    /*
    void *data = malloc(size);
    memset(data, 0, sizeof(data));
    fsemu_audio_buffer_update(data, size);
    free(data);
    */

    static uint8_t data[512];  // silence
    while (size) {
        int chunk = MIN(size, 512);
        fsemu_audio_buffer_update(data, size);
        size = size - chunk;
    }
}

void fsemu_audio_buffer_write_silence_ms(int ms)
{
    int bytes = 4 * ms * fsemu_audio_frequency() / 1000;
    fsemu_audio_buffer_write_silence(bytes);
}

#ifdef FSEMU_SAMPLERATE

double fsemu_audio_buffer_adjustment(void)
{
    return fsemu_audiobuffer_extra.adjustment;
}

void fsemu_audio_buffer_set_adjustment(double adjustment)
{
    fsemu_audiobuffer_extra.adjustment = adjustment;
}

#endif

double fsemu_audio_buffer_calculate_adjustment(void)
{
    // printf("wait for frame\n");
    // fs_emu_wait_for_frame(g_fs_uae_frame);

    // fsemu_audio_start_frame(g_fs_uae_frame);
    // Latency information is now updated

    // int current = fsemu_audio_latency_us();

    static fsemu_mavgi_t latency_mavg;
    static int latency_values[16];
    int latency = fsemu_mavgi(
        &latency_mavg, latency_values, 16, fsemu_audio_latency_us());

    // int latency = FSEMU_MAVGI(8, fsemu_audio_latency_us());
    // printf("[FSEMU] %0.1f\n", (latency + 500) / 1000.0);

    // FIXME: Depend on emulation hz?
    int64_t target = 20 * 1000;
    int diff = latency - target;
    // printf("[FSEMU] Diff %d\n", diff);
    static bool was_outside;

    // diff = 0;

    if (diff > 1000 || (was_outside && diff > 250)) {
        was_outside = true;
        if (diff > 3000) {
            // amiga_set_audio_frequency_adjust(-0.01);
            return -0.01;
        } else if (diff > 2000) {
            // amiga_set_audio_frequency_adjust(-0.002);
            return -0.002;
        } else if (diff > 1000) {
            // amiga_set_audio_frequency_adjust(-0.001);
            return -0.001;
        } else {
            // was_outside
            // amiga_set_audio_frequency_adjust(-0.0005);
            return -0.0005;
        }
    } else if (diff < -1000 || (was_outside && diff < -250)) {
        was_outside = true;
        if (diff < -3000) {
            // amiga_set_audio_frequency_adjust(0.01);
            return 0.01;
        } else if (diff < -2000) {
            // amiga_set_audio_frequency_adjust(0.002);
            return 0.002;
        } else if (diff < -1000) {
            // amiga_set_audio_frequency_adjust(0.001);
            return 0.001;
        } else {
            // was_outside
            // amiga_set_audio_frequency_adjust(0.0005);
            return 0.0005;
        }
    } else {
        was_outside = false;
        // amiga_set_audio_frequency_adjust(0.0);
        return 0.0;
    }
}
