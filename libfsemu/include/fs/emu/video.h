#ifndef FSE_VIDEO_H
#define FSE_VIDEO_H

#include <fs/ml.h>
#include <fs/emu/buffer.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool fse_drivers(void);

static inline int fs_emu_get_vblank_count(void)
{
    return fs_ml_get_vblank_count();
}

static inline int64_t fs_emu_get_vblank_time(void)
{
    return fs_ml_get_vblank_time();
}

static inline int fs_emu_get_fullscreen_width(void)
{
    return fs_ml_get_fullscreen_width();
}

static inline int fs_emu_get_fullscreen_height(void)
{
    return fs_ml_get_fullscreen_height();
}

static inline int fs_emu_get_windowed_width(void)
{
    return fs_ml_get_windowed_width();
}

static inline int fs_emu_get_windowed_height(void)
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

int fs_emu_get_video_format(void);
int fs_emu_get_texture_format(void);

static inline int fs_emu_buffer_format(void)
{
    return fs_emu_get_video_format();
}

static inline int fs_emu_video_texture_format(void)
{
    return fs_emu_get_texture_format();
}

#ifdef FSE_DRIVERS

#endif

#ifdef FSE_INTERNAL_API

// FIXME: move
int fs_emu_main_loop(void);

void fse_init_video(void);
void fse_init_video_2(void);

void fse_init_legacy_video(void);

void fs_emu_video_dummy_init(void);
void fs_emu_video_sdl_init(void);
void fs_emu_video_sdl_software_init(void);

void fs_emu_video_create_window(int width, int height);

void fs_emu_video_render(void);

typedef struct fse_video {
    bool drivers;
    void (*create_window)(int width, int height);
    void (*render)(void);

    void (*configure_buffer)(int width, int height);
    fs_emu_buffer *(*get_buffer)(void);
    void (*finish_buffer)(fs_emu_buffer *buffer);
} fse_video_t;

extern fse_video_t fse_video;
extern int (*fse_main_loop)(void);

#endif /* FSE_INTERNAL_API */

#ifdef __cplusplus
}
#endif

#endif /* FSE_VIDEO_H */
