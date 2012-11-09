#ifndef _LIBFSEMU_H_
#define _LIBFSEMU_H_

#include <glib.h>
#include "util.h"
#include "texture.h"

#define FS_EMU_MAX_CHAT_STRING_SIZE 128

void fs_emu_initialize_hud_module();
void fs_emu_initialize_dialog_module();
void fs_emu_initialize_textures();

void fs_emu_render_dialog();
void fs_emu_handle_dialog_action(int action, int state);

void fs_emu_handle_libfsemu_action(int action, int state);

void fs_emu_draw_texture_with_size(int entry, float x, float y, float w,
        float h);
void fs_emu_draw_texture(int entry, float x, float y);
void fs_emu_prepare_texture(int entry, float *tx1, float *ty1, float *tx2,
        float *ty2);

int fs_emu_handle_chat_input(fs_emu_event *event);
int fs_emu_in_chat_mode();
void fs_emu_enable_chat_mode();
void fs_emu_render_chat();

extern char g_fs_emu_chat_string[];
//extern int g_fs_emu_chat_mode;
extern int g_fs_emu_hud_mode;

extern int64_t g_fs_emu_quit_time;
extern int g_fs_emu_emulation_thread_stopped;

//extern char *g_fs_emu_application_title;
extern char *g_fs_emu_title;
extern char *g_fs_emu_sub_title;
//extern char *g_fs_emu_window_title;
extern int g_fs_emu_audio_buffer_underruns;

//extern gchar *g_fs_emu_netplay_server_arg;
//extern gchar *g_fs_emu_netplay_tag_arg;

extern int g_fs_emu_throttling;
extern int g_fs_emu_full_sync_allowed;

#define MAX_PLAYERS 6

typedef struct fs_emu_player {
    char tag[4];
    int ping;
    int lag;
} fs_emu_player;
extern fs_emu_player g_fs_emu_players[MAX_PLAYERS];

void fs_emu_queue_input_event(int input_event);
void fs_emu_queue_input_event_internal(int input_event);

int fs_emu_netplay_connect();
void fs_emu_netplay_start();
int fs_emu_netplay_send_input_event(int input_event);
void fs_emu_netplay_init();

void fs_emu_clear_menu_input_states(int key);
void fs_emu_audio_set_default_pitch(double pitch);
void fs_emu_render_scanlines(uint8_t* out, fs_emu_video_buffer *buffer,
        int cx, int cy, int cw, int ch, int scanline_dark, int scanline_light);
void fs_emu_initialize_opengl();
fs_emu_video_buffer *fs_emu_lock_video_buffer();
void fs_emu_unlock_video_buffer();

void fs_emu_init_render();
int fs_emu_using_shader();
void fs_emu_init_shader();
int fs_emu_render_with_shader(int texture, int texture_width,
        int texture_height, int input_width, int input_height,
        int output_width, int output_height, float x1, float y1,
        float x2, float y2, int render_textured_side, float alpha);

void fs_emu_init_theme();
char *fs_emu_get_theme_resource(const char *name);

#define MAX_CUSTOM_OVERLAYS 20

struct fs_emu_theme {
    char *name;
    char *path;
    float wall_color_1[4];
    float wall_color_2[4];
    float floor_color_1[4];
    float floor_color_2[4];
    int floor_height;
    char *overlay_image;
    float fade_color[4];
    float heading_color[4];
    float item_color[4];

    fs_emu_texture *overlay_textures[MAX_CUSTOM_OVERLAYS];
    int overlay_x[MAX_CUSTOM_OVERLAYS];
    int overlay_y[MAX_CUSTOM_OVERLAYS];
};

extern struct fs_emu_theme g_fs_emu_theme;

extern int g_fs_emu_benchmarking;
extern int64_t g_fs_emu_benchmark_start_time;
extern int g_fs_emu_total_emu_frames;
extern int g_fs_emu_total_sys_frames;
extern int g_fs_emu_repeated_frames;
extern int g_fs_emu_lost_frames;
extern int g_fs_emu_lost_vblanks;
extern fs_emu_stat_queue g_fs_emu_emu_frame_times;
extern fs_emu_stat_queue g_fs_emu_emu2_frame_times;
extern fs_emu_stat_queue g_fs_emu_sys_frame_times;
extern int g_fs_emu_scanlines;
extern int g_fs_emu_scanlines_dark;
extern int g_fs_emu_scanlines_light;

extern fs_emu_texture *g_fs_emu_overlay_texture;

#endif
