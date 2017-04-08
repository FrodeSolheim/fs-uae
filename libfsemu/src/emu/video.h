#ifndef LIBFSEMU_VIDEO_H_
#define LIBFSEMU_VIDEO_H_

#include "fs/emu.h"
#include "texture.h"

extern char *g_fs_emu_video_vsync_mode_arg;

extern int g_fs_emu_video_debug;
extern int g_fs_emu_video_fullscreen;
extern int g_fs_emu_video_fullscreen_mode;
extern char *g_fs_emu_video_fullscreen_mode_string;
extern int g_fs_emu_video_vsync;
extern int g_fs_emu_video_crop_mode;
extern int g_fs_ml_opengl_context_stamp;
extern int g_fs_emu_screenshot;

extern int g_fs_emu_video_bpp;
extern int g_fs_emu_video_format;
extern int g_fs_emu_texture_format;
extern int g_fs_emu_disable_aspect_correction;

extern int g_fs_emu_video_allow_full_sync;
extern int g_fs_emu_video_frame_rate_host;
extern int g_fs_emu_video_sync_to_vblank;

void fs_emu_on_resize(int width, int height);

void fs_emu_video_render_debug_info(uint32_t *texture);

void fse_init_video(void);
void fse_init_video_options(void);
void fse_init_video_opengl(void);

int fs_emu_video_update_function(void);
void fs_emu_video_render_function(void);

void fs_emu_video_after_update(void);

/* this is used to make sure that changes to menu etc is not done while
 * rendering it... handling of input events for menu actions must be done
 * when holding this lock, as well as updates to menu state, pause state, etc
 */
void fs_emu_video_render_mutex_lock(void);
void fs_emu_video_render_mutex_unlock(void);

void fs_emu_update_video_stats_1(void);
void fs_emu_update_video_stats_2(void);

#endif // LIBFSEMU_VIDEO_H_
