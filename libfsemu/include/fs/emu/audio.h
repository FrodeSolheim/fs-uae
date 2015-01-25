#ifndef FS_EMU_AUDIO_H
#define FS_EMU_AUDIO_H

#include <stdint.h>

int fs_emu_audio_get_volume(void);
void fs_emu_audio_set_volume(int volume);

int fs_emu_audio_get_mute(void);
void fs_emu_audio_set_mute(int mute);

typedef struct fs_emu_audio_stream_options {
    int struct_size;
    int frequency;
    int channels;
    int sample_size;
    int buffer_size;
    int min_buffers;
} fs_emu_audio_stream_options;

void fs_emu_init_audio_stream(int stream,
        fs_emu_audio_stream_options *options);
void fs_emu_init_audio_stream_options(fs_emu_audio_stream_options *options);
void fs_emu_audio_pause_stream(int stream);
void fs_emu_audio_resume_stream(int stream);
int fs_emu_queue_audio_buffer(int stream, int16_t* buffer, int size);
int fs_emu_check_audio_buffer_done(int stream, int buffer);
int fs_emu_get_audio_frequency();

#if 0
// start deprecated
void fs_emu_enable_audio_stream(int stream);
void fs_emu_disable_audio_stream(int stream);
int fs_emu_get_audio_buffer_size();
void fs_emu_audio_sample(int stream, int16_t left, int16_t right);
void fs_emu_set_max_audio_buffers(int buffers);
void fs_emu_set_audio_buffer_frequency(int stream, int frequency);
// end deprecated
#endif

double fs_emu_audio_get_measured_avg_buffer_fill(int stream);
double fs_emu_audio_get_measured_output_frequency();

#ifdef FS_EMU_INTERNAL

void fs_emu_audio_init(void);
void fs_emu_openal_audio_init(void);

extern double g_fs_emu_audio_want_volume;

#endif /* FS_EMU_INTERNAL*/

#endif /* FS_EMU_AUDIO_H */
