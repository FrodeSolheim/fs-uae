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

extern void (*fs_emu_buffer_configure)(int width, int height);

extern fs_emu_buffer *(*fs_emu_buffer_get)(void);

extern void (*fs_emu_buffer_finish)(fs_emu_buffer *buffer);

#ifdef FS_EMU_INTERNAL

void fs_emu_buffer_init(void);

int fs_emu_buffer_next(void);

int fs_emu_buffer_current(void);

void fs_emu_buffer_set_current(int buffer);

int fs_emu_buffer_lock(void);

void fs_emu_buffer_unlock(void);

int fs_emu_buffer_wait(int number, int timeout);

#endif /* FS_EMU_INTERNAL */

#endif // FS_EMU_BUFFER_H
