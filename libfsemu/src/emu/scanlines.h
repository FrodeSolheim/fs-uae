#ifndef LIBFSEMU_SCANLINES_H_
#define LIBFSEMU_SCANLINES_H_

#include <fs/emu.h>
#include <fs/emu/buffer.h>

void fs_emu_scanline_filter(uint8_t* out, fs_emu_video_buffer *buffer,
        int cx, int cy, int cw, int ch, int scanline_dark, int scanline_light);
void fs_emu_2xcolor_filter(uint8_t* out, fs_emu_video_buffer *buffer,
        int cx, int cy, int cw, int ch, int scanline_dark, int scanline_light);


#endif // LIBFSEMU_SCANLINES_H_
