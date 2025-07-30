#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FSUAE_LEGACY

#include <fs/emu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fs/thread.h>

#include "libfsemu.h"
#include "video.h"
#include "video_buffer.h"

static fs_mutex* g_video_buffers_mutex = NULL;
static fs_emu_video_buffer g_video_buffers[3] = {};

static int g_video_buffers_initialized = 0;
static int g_video_buffer_next = 0;
static fs_emu_video_buffer *g_video_buffer_current = g_video_buffers;
static fs_emu_video_buffer *g_video_buffer_locked = NULL;

int fs_emu_video_buffer_init(int width, int height, int bpp) {
    // should only be called once, currently
    if (g_video_buffers_initialized) {
        return 0;
    }
    if (bpp == 0) {
        bpp = g_fs_emu_video_bpp;
    }
    for (int i = 0; i < 3; i++) {
        g_video_buffers[i].width = width;
        g_video_buffers[i].height = height;
        g_video_buffers[i].bpp = bpp;
        g_video_buffers[i].size = width * height * bpp;
        g_video_buffers[i].data = g_malloc0(g_video_buffers[i].size);
        //memset(g_video_buffers[i].data, 0, g_video_buffers[i].size);
        g_video_buffers[i].aspect = 1.0;
        //g_video_buffers[i].buffer_width = width;
        //g_video_buffers[i].buffer_height = height;
    }
    g_video_buffers_mutex = fs_mutex_create();
    g_video_buffers_initialized = 1;
    return 1;
}

static int get_next_video_buffer() {
    g_video_buffer_next++;
    g_video_buffer_next = g_video_buffer_next % 3;
    if (g_video_buffers + g_video_buffer_next == g_video_buffer_locked) {
        g_video_buffer_next++;
        g_video_buffer_next = g_video_buffer_next % 3;
    }
    return g_video_buffer_next;
}

fs_emu_video_buffer *fs_emu_video_buffer_get_current() {
    return g_video_buffer_current;
}

/*
fs_emu_video_buffer *fs_emu_swap_offscreen_video_buffers() {
    get_next_video_buffer();
}
*/

fs_emu_video_buffer *fs_emu_video_buffer_get_available(int copy) {
    fs_mutex_lock(g_video_buffers_mutex);
    int next_buffer = get_next_video_buffer();
    fs_mutex_unlock(g_video_buffers_mutex);

    // FIXME: REMOVE
    //memset((g_video_buffers + next_buffer)->data, 0x0,
    //        (g_video_buffers + next_buffer)->size);

    if (copy) {
        if (g_video_buffer_current) {
            memcpy((g_video_buffers + next_buffer)->data,
                    g_video_buffer_current->data,
                    (g_video_buffers + next_buffer)->size);
        }
        else {
            memset((g_video_buffers + next_buffer)->data, 0x0,
                    (g_video_buffers + next_buffer)->size);
        }
    }
    return g_video_buffers + next_buffer;
}

fs_emu_video_buffer *fs_emu_video_buffer_lock() {
    fs_mutex_lock(g_video_buffers_mutex);
    g_video_buffer_locked = g_video_buffer_current;
    fs_mutex_unlock(g_video_buffers_mutex);
    return g_video_buffer_locked;
}

void fs_emu_video_buffer_unlock() {
    fs_mutex_lock(g_video_buffers_mutex);
    g_video_buffer_locked = NULL;
    fs_mutex_unlock(g_video_buffers_mutex);
}

int fs_emu_video_buffer_grow(fs_emu_video_buffer *buffer, int width,
        int height) {
    int needed_size = width * height * buffer->bpp;
    if (buffer->size >= needed_size) {
        return 0;
    }
    fs_log("fs_emu_grow_render_buffer %dx%d (%d bytes)\n", width, height,
            needed_size);
    free(buffer->data);
    buffer->size = needed_size;
    buffer->data = g_malloc0(buffer->size);
    return 1;
}

static void copy_buffer_data(fs_emu_video_buffer *new_buffer,
        fs_emu_video_buffer *old_buffer) {
    if (!old_buffer) {
        return;
    }

    int src_stride = old_buffer->width * old_buffer->bpp;
    int dst_stride = new_buffer->width * new_buffer->bpp;

    unsigned char *src, *dst;
    int width;
    int first_line, last_line;

#if 0
    int crop = g_fs_emu_video_crop_mode;

    // calculate copy parameters
    if (crop) {
        width = new_buffer->crop.w;
        first_line = new_buffer->crop.y;
        last_line = new_buffer->crop.y + new_buffer->crop.h - 1;

        src = old_buffer->data + src_stride * first_line + \
                new_buffer->crop.x * g_fs_emu_video_bpp;
        dst = new_buffer->data + dst_stride * first_line + \
                new_buffer->crop.x * g_fs_emu_video_bpp;
    }
    else {
#endif
        // no cropping; must copy the entire line
        width = new_buffer->width;
        first_line = 0;
        last_line = new_buffer->height - 1;

        src = old_buffer->data;
        dst = new_buffer->data;
#if 0
    }
#endif
    // actually copy the lines
    for (int y = first_line; y <= last_line; y++) {
        if (new_buffer->line[y]) {
            memcpy(dst, src, width * g_fs_emu_video_bpp);
        }
        src += src_stride;
        dst += dst_stride;
    }
}

void fs_emu_video_buffer_update_lines(fs_emu_video_buffer *buffer) {
    // lines which are not updated in this frame are copied from the last
    // completed video frame
    copy_buffer_data(buffer, g_video_buffer_current);
}

void fs_emu_video_buffer_set_current(fs_emu_video_buffer *buffer) {
    if (buffer == NULL) {
        return;
    }
    if (g_fs_emu_benchmark_start_time > 0) {
        g_fs_emu_total_emu_frames++;
    }

    fs_emu_update_video_stats_1();

    fs_ml_frame_update_begin(buffer->seq);

    fs_mutex_lock(g_video_buffers_mutex);
    g_video_buffer_current = buffer;
    fs_mutex_unlock(g_video_buffers_mutex);

    fs_ml_frame_update_end(buffer->seq);

    fs_emu_update_video_stats_2();

    /*
    fs_ml_lock_buffer_swap();
    fs_ml_signal_buffer_swap();
    fs_ml_unlock_buffer_swap();
*/
}

#endif  // FSUAE_LEGACY
