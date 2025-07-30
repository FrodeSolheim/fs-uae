#ifndef LIBFSEMU_AUDIO_H_
#define LIBFSEMU_AUDIO_H_

#include <fs/emu.h>
#include <fs/thread.h>
#include "util.h"

#define MAX_STREAMS 4

extern int g_fs_emu_audio_stream_playing[MAX_STREAMS];
extern int64_t g_fs_emu_audio_buffer_underrun_time;

typedef struct fs_emu_audio_driver {
    const char *name;
    void(*init)(void);
    void(*shutdown)(void);
} fs_emu_audio_driver;

void fse_init_audio();
void fs_emu_audio_shutdown();

void fs_emu_audio_video_sync(int time_ms);
void fs_emu_audio_render_debug_info(uint32_t *texture);
void fs_emu_audio_set_default_pitch(double pitch);

#define AUDIO_ENABLED
#define MEMORY_BARRIER __asm__ __volatile__ ("" ::: "memory")

#if 0
// ring buffer for audio, byte size must be a multiple of 4
#define RING_BUFFER_SIZE_FRAMES (4096 * 4)
//#define RING_BUFFER_SIZE_FRAMES (16384 * 4)

#define RING_BUFFER_SIZE_SAMPLES (2 * RING_BUFFER_SIZE_FRAMES)
#define RING_BUFFER_SIZE (4 * RING_BUFFER_SIZE_SAMPLES)

#define FRAME_TIME_LIST_COUNT 256
#define AUDIO_AVG_FILL_QUEUE_COUNT 256
#define AUDIO_FILL_QUEUE_COUNT 256
#define AUDIO_FILL_QUEUE2_COUNT 128
#define AUDIO_IN_FRAME_QUEUE_SIZE 128
#define AUDIO_OUT_FRAME_QUEUE_SIZE 1024
#define FILL_STAT_BUFFER_LENGTH 1024
#define FILL_STAT_BUFFER_ITEMS 512
#define FILL_STAT_BUFFER_ITEMS_AVG 128
#define FILL_STAT_BUFFER_ITEMS_MIN 128

typedef struct audio_stream {
    int enabled;

    unsigned char g_ring_buffer[RING_BUFFER_SIZE];
    int g_ring_read_pos;
    int g_ring_write_pos;

    fs_emu_stat_queue g_frame_times;
    fs_emu_stat_queue g_audio_avg_fill_queue;
    fs_emu_stat_queue g_audio_fill_queue;
    fs_emu_stat_queue g_audio_fill_queue2;
    fs_emu_stat_queue g_audio_underrun_queue;
    fs_mutex *g_audio_fill_queue2_mutex;

    int16_t g_old_left;
    int16_t g_old_right;

    int g_audio_out_pos;
    double g_audio_in_pos;

    fs_emu_stat_queue g_audio_in_frame_times;
    fs_emu_stat_queue g_audio_in_frame_counts;

    int g_audio_in_frame_count;

    fs_emu_stat_queue g_audio_out_frame_times;
    fs_emu_stat_queue g_audio_out_frame_counts;

    // FIXME: SET DEFAULT = 44100.0 ?
    double g_audio_in_frequency;
    // FIXME: SET DEFAULT = 44100.0 ?
    //double g_audio_in_fps;
    // FIXME: SET DEFAULT TO 1.0
    double g_advance;

    int fill_stat_buffer[FILL_STAT_BUFFER_LENGTH];
    int avg_fill_stat_buffer[FILL_STAT_BUFFER_LENGTH];
    int min_fill_stat_buffer[FILL_STAT_BUFFER_LENGTH];
    int avg_min_fill_stat_buffer[FILL_STAT_BUFFER_LENGTH];
    int fill_stat_buffer_avg_sum;
    int fill_stat_buffer_avg;
    int fill_stat_buffer_min;
    int fill_stat_buffer_avg_min_sum;
    int fill_stat_buffer_avg_min;
    volatile int fill_stat_buffer_pos;

    double pid_integral;
    double pid_last_error;
    int pid_last_time;

} audio_stream;
#endif

#endif // LIBFSEMU_AUDIO_H_
