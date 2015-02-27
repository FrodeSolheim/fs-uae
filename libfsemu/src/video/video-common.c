#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/video.h>
#include <fs/emu/buffer.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

void (*fs_emu_buffer_configure)(int width, int height) = 0;

void (*fs_emu_buffer_finish)(fs_emu_buffer *buffer) = 0;

fs_emu_buffer *(*fs_emu_buffer_get)(void) = 0;

void (*fs_emu_video_create_window)(int width, int height) = 0;

void (*fs_emu_video_render)(void) = 0;

#endif /* FS_EMU_DRIVERS */
