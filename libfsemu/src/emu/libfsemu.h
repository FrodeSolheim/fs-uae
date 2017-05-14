#ifndef _LIBFSEMU_H_
#define _LIBFSEMU_H_

#include "util.h"
#include "texture.h"

void fs_emu_initialize_textures();

void fs_emu_draw_texture_with_size(int entry, float x, float y, float w,
        float h);
void fs_emu_draw_texture(int entry, float x, float y);
void fs_emu_prepare_texture(int entry, float *tx1, float *ty1, float *tx2,
        float *ty2);

extern int64_t g_fs_emu_quit_time;
extern int g_fs_emu_emulation_thread_stopped;

//extern char *g_fs_emu_application_title;
extern char *g_fs_emu_title;
extern char *g_fs_emu_sub_title;
//extern char *g_fs_emu_window_title;
extern int g_fs_emu_audio_buffer_underruns;

extern int g_fs_emu_throttling;
extern int g_fs_emu_full_sync_allowed;

#define MAX_PLAYERS 6

typedef struct fs_emu_player {
    char tag[4];
    int ping;
    int lag;
} fs_emu_player;
extern fs_emu_player g_fs_emu_players[MAX_PLAYERS];

bool fs_emu_handle_local_action(int action, int state);
void fs_emu_queue_input_event(int input_event);
void fs_emu_queue_input_event_internal(int input_event);

void fs_emu_clear_menu_input_states(int key);
void fs_emu_audio_set_default_pitch(double pitch);
void fs_emu_initialize_opengl();

void fs_emu_volume_control(int volume);

void fse_init_renderer();

extern int g_fs_emu_benchmarking;
extern int64_t g_fs_emu_benchmark_start_time;
extern int g_fs_emu_total_emu_frames;
extern int g_fs_emu_total_sys_frames;
extern int g_fs_emu_repeated_frames;
extern int g_fs_emu_lost_frames;
extern int64_t g_fs_emu_lost_frame_time;
extern int64_t g_fs_emu_repeated_frame_time;
extern int64_t g_fs_emu_video_mode_change_time;
extern int g_fs_emu_lost_vblanks;
extern fs_emu_stat_queue g_fs_emu_emu_frame_times;
extern fs_emu_stat_queue g_fs_emu_emu2_frame_times;
extern fs_emu_stat_queue g_fs_emu_sys_frame_times;
extern int g_fs_emu_scanlines;
extern int g_fs_emu_scanlines_dark;
extern int g_fs_emu_scanlines_light;

extern fs_emu_texture *g_fs_emu_overlay_texture;

#endif
