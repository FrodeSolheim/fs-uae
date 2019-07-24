#define FSEMU_INTERNAL
#include "fsemu/fsemu-audio-buffer.h"

#include "fsemu/fsemu-audio.h"

/*
int fsemu_audio_buffer.size;
volatile uint8_t *fsemu_audio_buffer;
volatile uint8_t *fsemu_audio_buffer.end;
volatile uint8_t *volatile fsemu_audio_buffer.read;
volatile uint8_t *volatile fsemu_audio_buffer.write;
*/
fsemu_audio_buffer_t fsemu_audio_buffer;

void fsemu_audio_buffer_init(void)
{
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

void fsemu_audio_buffer_update(void *data, int size)
{
    int add_silence = fsemu_audio_buffer.add_silence;
    if (add_silence) {
        fsemu_audio_buffer.add_silence = 0;
        fsemu_audio_buffer_write_silence_ms(add_silence);
    }

#if 1
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
