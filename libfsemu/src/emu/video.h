#ifndef LIBFSEMU_VIDEO_H_
#define LIBFSEMU_VIDEO_H_

#include "fs/emu.h"
#include "texture.h"

extern char *g_fs_emu_video_vsync_mode_arg;

extern int g_fs_emu_video_debug;
extern int g_fs_emu_video_fullscreen;
extern int g_fs_emu_video_fullscreen_window;
extern char *g_fs_emu_video_fullscreen_mode;
extern int g_fs_emu_video_vsync;
extern int g_fs_emu_video_crop_mode;
extern int g_fs_ml_opengl_context_stamp;
extern int g_fs_emu_screenshot;

void fs_emu_on_resize(int width, int height);

void fs_emu_video_render_debug_info(uint32_t *texture);

void fs_emu_video_init();
void fs_emu_video_init_opengl();

void fs_emu_video_update_function();
void fs_emu_video_render_function();

void fs_emu_video_after_update();

/* this is used to make sure that changes to menu etc is not done while
 * rendering it... handling of input events for menu actions must be done
 * when holding this lock, as well as updates to menu state, pause state, etc
 */
void fs_emu_video_render_mutex_lock();
void fs_emu_video_render_mutex_unlock();

#endif // LIBFSEMU_VIDEO_H_
