#ifndef LIBFSML_ML_INTERNAL__H_
#define LIBFSML_ML_INTERNAL_H_

#include "fs/ml.h"

extern fs_ml_int_function g_fs_ml_video_update_function;
extern fs_ml_void_function g_fs_ml_video_render_function;
extern fs_ml_void_function g_fs_ml_video_post_render_function;
extern int g_fs_ml_video_width;
extern int g_fs_ml_video_height;
extern int g_fs_ml_target_refresh_rate;
extern int g_fs_ml_target_frame_time;

void fs_ml_initialize_keymap();
int fs_ml_scancode_to_key(int scancode);
void fs_ml_clear_keyboard_modifier_state();
int fs_ml_handle_keyboard_shortcut(fs_ml_event *event);

void fs_ml_audio_init();
void fs_ml_input_init();
void fs_ml_video_init();
void fs_ml_render_init();

//void fs_ml_calibrate_clock();

void fs_ml_prevent_power_saving(void);
void fs_ml_set_fullscreen_extra();

#ifdef WINDOWS
void fs_ml_init_raw_input();
#endif

void fs_ml_configure_window();
void fs_ml_render_iteration();

#define FS_ML_KEYBOARD 63
#define FS_ML_SLOTS 512

#define FS_ML_MAX_JOYSTICKS 63
#define FS_ML_MAX_BUTTONS 63
#define FS_ML_MAX_AXIS 63

extern fs_ml_input_device *g_fs_ml_input_devices;
extern int g_fs_ml_input_device_count;

extern int g_fs_ml_running;

extern int g_fs_ml_video_sync;
extern int g_fs_ml_video_sync_low_latency;
extern int g_fs_ml_vblank_sync;

extern int g_fs_ml_benchmarking;

extern int g_fs_ml_had_input_grab;
extern int g_fs_ml_was_fullscreen;

#endif // LIBFSML_ML_INTERNAL_H_
