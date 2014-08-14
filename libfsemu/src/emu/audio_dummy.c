#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if !defined(USE_OPENAL)

#include <fs/emu.h>
#include <stdint.h>

void fs_emu_init_audio_stream(int stream,
        fs_emu_audio_stream_options *options) {

}

void fs_emu_init_audio_stream_options(fs_emu_audio_stream_options *options) {
    options->frequency = 44100;
    options->channels = 2;
    options->sample_size = 16;
    options->buffer_size = 512;
    options->min_buffers = 1;
}

void fs_emu_audio_init() {

}

void fs_emu_audio_set_default_pitch(double pitch) {

}

double fs_emu_audio_get_measured_avg_buffer_fill(int stream) {
    return 0.0;
}

void fs_emu_audio_shutdown() {

}

void fs_emu_audio_video_sync(int time_ms) {

}

int fs_emu_queue_audio_buffer(int stream, int16_t* buffer, int size) {
    return 1;
}

int fs_emu_check_audio_buffer_done(int stream, int buffer) {
    return 0;
}

double fs_emu_audio_get_volume() {
    return 1.0;
}

void fs_emu_audio_set_volume(double volume) {

}

void fs_emu_audio_render_debug_info(uint32_t *texture) {

}

#else

int libfsemu_audio_dummy_dummy;

#endif
