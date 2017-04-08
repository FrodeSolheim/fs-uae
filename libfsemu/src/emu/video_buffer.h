#ifndef LIBFSEMU_VIDEO_BUFFER_H_
#define LIBFSEMU_VIDEO_BUFFER_H_

#include <fs/emu.h>
#include <fs/emu/buffer.h>

fs_emu_video_buffer *fs_emu_video_buffer_lock();
void fs_emu_video_buffer_unlock();

#endif // LIBFSEMU_VIDEO_BUFFER_H_
