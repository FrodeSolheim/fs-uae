#ifndef LIBFSEMU_SCANLINES_H_
#define LIBFSEMU_SCANLINES_H_

#include <fs/emu.h>

void fs_emu_render_scanlines(uint8_t* out, fs_emu_video_buffer *buffer,
        int cx, int cy, int cw, int ch, int scanline_dark, int scanline_light);

#endif // LIBFSEMU_SCANLINES_H_
