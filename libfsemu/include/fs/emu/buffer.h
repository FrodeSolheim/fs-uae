#ifndef FS_EMU_BUFFER_H
#define FS_EMU_BUFFER_H

#include <fs/emu.h>

typedef struct fs_emu_buffer {
    int seq;
    void *data;
    int width;
    int height;
    int stride;
    int bpp;
    double aspect;
    int size;
    fs_emu_rect crop;
    //int buffer_width;
    //int buffer_height;
    char line[FS_EMU_MAX_LINES];
    int flags;
} fs_emu_buffer;

void fs_emu_buffer_configure(int width, int height);
fs_emu_buffer *fs_emu_buffer_get(void);
void fs_emu_buffer_finish(fs_emu_buffer *buffer);

#ifdef FSE_INTERNAL_API

void fs_emu_buffer_init(void);

int fs_emu_buffer_next(void);

int fs_emu_buffer_current(void);

void fs_emu_buffer_set_current(int buffer);

int fs_emu_buffer_lock(void);

void fs_emu_buffer_unlock(void);

int fs_emu_buffer_wait(int number, int timeout);

#endif /* FSE_INTERNAL_API */

/* Old functions */

typedef fs_emu_buffer fs_emu_video_buffer;
int fs_emu_video_buffer_init(int width, int height, int bpp);
fs_emu_video_buffer *fs_emu_video_buffer_get_available(int copy);
void fs_emu_video_buffer_update_lines(fs_emu_video_buffer *buffer);
void fs_emu_video_buffer_set_current(fs_emu_video_buffer *buffer);
fs_emu_video_buffer *fs_emu_video_buffer_get_current();
int fs_emu_video_buffer_grow(fs_emu_video_buffer *buffer, int width,
        int height);

#endif /* FS_EMU_BUFFER_H */
