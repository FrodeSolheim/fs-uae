#ifndef FS_EMU_VIDEO_H
#define FS_EMU_VIDEO_H

#include <fs/ml.h>

static inline int fs_emu_get_vblank_count()
{
    return fs_ml_get_vblank_count();
}

static inline int64_t fs_emu_get_vblank_time()
{
    return fs_ml_get_vblank_time();
}

static inline int fs_emu_get_fullscreen_width()
{
    return fs_ml_get_fullscreen_width();
}

static inline int fs_emu_get_fullscreen_height()
{
    return fs_ml_get_fullscreen_height();
}

static inline int fs_emu_get_windowed_width()
{
    return fs_ml_get_windowed_width();
}

static inline int fs_emu_get_windowed_height()
{
    return fs_ml_get_windowed_height();
}

//#define FS_EMU_VIDEO_FORMAT_RGB  0x1907 // GL_RGB
//#define FS_EMU_VIDEO_FORMAT_RGBA 0x1908 // GL_RGBA
//#define FS_EMU_VIDEO_FORMAT_BGRA 0x80e1 // GL_BGRA

#define FS_EMU_VIDEO_FORMAT_BGRA     0
#define FS_EMU_VIDEO_FORMAT_RGBA     1
#define FS_EMU_VIDEO_FORMAT_RGB      2
#define FS_EMU_VIDEO_FORMAT_R5G6B5   3
#define FS_EMU_VIDEO_FORMAT_R5G5B5A1 4

int fs_emu_get_video_format();
int fs_emu_get_texture_format();

static inline int fs_emu_buffer_format()
{
    return fs_emu_get_video_format();
}

static inline int fs_emu_video_texture_format()
{
    return fs_emu_get_texture_format();
}

#ifdef FS_EMU_DRIVERS

#endif

#ifdef FS_EMU_INTERNAL

// FIXME: move
int fs_emu_main_loop(void);

void fs_emu_video_init(void);

void fs_emu_video_dummy_init(void);
void fs_emu_video_sdl_init(void);
void fs_emu_video_sdl_software_init(void);

extern void (*fs_emu_video_create_window)(int width, int height);

extern void (*fs_emu_video_render)(void);

#endif /* FS_EMU_INTERNAL */

#endif /* FS_EMU_VIDEO_H */
