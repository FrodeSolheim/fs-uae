#ifndef LIBFSEMU_INCLUDE_FS_EMU_VIDEO_H_
#define LIBFSEMU_INCLUDE_FS_EMU_VIDEO_H_

#include <fs/ml.h>

static inline int fs_emu_get_vblank_count() {
    return fs_ml_get_vblank_count();
}

static inline int64_t fs_emu_get_vblank_time() {
    return fs_ml_get_vblank_time();
}

static inline int fs_emu_get_fullscreen_width() {
    return fs_ml_get_fullscreen_width();
}

static inline int fs_emu_get_fullscreen_height() {
    return fs_ml_get_fullscreen_height();
}

static inline int fs_emu_get_windowed_width() {
    return fs_ml_get_windowed_width();
}

static inline int fs_emu_get_windowed_height() {
    return fs_ml_get_windowed_height();
}

#define FS_EMU_VIDEO_FORMAT_RGBA 0x1908
#define FS_EMU_VIDEO_FORMAT_BGRA 0x80e1

int fs_emu_get_video_format();
int fs_emu_get_texture_format();

#endif // LIBFSEMU_INCLUDE_FS_EMU_VIDEO_H_
