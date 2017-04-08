#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/buffer.h>
#include <fs/emu/benchmark.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <fs/thread.h>
#include <string.h>

static fs_mutex* g_fs_emu_buffer_mutex = NULL;
static fs_condition* g_fs_emu_buffer_condition = NULL;

static int g_video_buffers_initialized = 0;
static int g_video_buffer_next = 0;
static int g_video_buffer_locked = -1;
static int g_video_buffer_current = 0;
static int g_video_buffer_number = -1;

// static fs_emu_video_buffer *g_video_buffer_current = g_video_buffers;
// static fs_emu_video_buffer *g_video_buffer_locked = NULL;

void fs_emu_buffer_init(void)
{
    g_fs_emu_buffer_mutex = fs_mutex_create();
    g_fs_emu_buffer_condition = fs_condition_create();
    // FIXME: destroy mutex on quit
}

int fs_emu_buffer_current(void)
{
    return g_video_buffer_current;
}

void fs_emu_buffer_set_current(int buffer)
{
    // printf("fs_emu_buffer_set_current\n");
#if 0
    if (buffer == NULL) {
        return;
    }
    if (g_fs_emu_benchmark_start_time > 0) {
        g_fs_emu_total_emu_frames++;
    }

    fs_emu_update_video_stats_1();

    fs_ml_frame_update_begin(buffer->seq);
#endif
    // printf("lock\n");
    fs_mutex_lock(g_fs_emu_buffer_mutex);
    g_video_buffer_current = buffer;
    g_video_buffer_number += 1;
    g_fs_emu_benchmark_emu_frame_count += 1;
    // printf("%d\n", g_video_buffer_number);
    fs_condition_signal(g_fs_emu_buffer_condition);
    fs_mutex_unlock(g_fs_emu_buffer_mutex);
#if 0
    fs_ml_frame_update_end(buffer->seq);

    fs_emu_update_video_stats_2();
#endif
}

int fs_emu_buffer_wait(int number, int timeout)
{
    int64_t wait_until = fs_condition_get_wait_end_time(timeout * 1000);
    fs_mutex_lock(g_fs_emu_buffer_mutex);
    while (number > g_video_buffer_number) {
        // printf("%d %d\n", number, g_video_buffer_number);
        if (!fs_condition_wait_until(g_fs_emu_buffer_condition,
                                     g_fs_emu_buffer_mutex, wait_until)) {
            /* timeout */
            fs_mutex_unlock(g_fs_emu_buffer_mutex);
            return -1;
        }
    }
    /* also lock buffer */
    g_video_buffer_locked = g_video_buffer_current;
    int result = g_video_buffer_number;
    fs_mutex_unlock(g_fs_emu_buffer_mutex);
    // printf("returning %d\n", result);
    return result;
}


static int get_next_video_buffer()
{
    g_video_buffer_next++;
    g_video_buffer_next = g_video_buffer_next % 3;
    if (g_video_buffer_locked == g_video_buffer_next) {
    // if (g_video_buffers + g_video_buffer_next == g_video_buffer_locked) {
        g_video_buffer_next++;
        g_video_buffer_next = g_video_buffer_next % 3;
    }
    return g_video_buffer_next;
}

int fs_emu_buffer_next(void)
{
    fs_mutex_lock(g_fs_emu_buffer_mutex);
    int next_buffer = get_next_video_buffer();
    fs_mutex_unlock(g_fs_emu_buffer_mutex);
    return next_buffer;
}

#if 0
fs_emu_video_buffer *fs_emu_video_buffer_get_available(int copy)
{
    fs_mutex_lock(g_fs_emu_buffer_mutex);
    int next_buffer = get_next_video_buffer();
    fs_mutex_unlock(g_fs_emu_buffer_mutex);

    // FIXME: REMOVE
    //memset((g_video_buffers + next_buffer)->data, 0x0,
    //        (g_video_buffers + next_buffer)->size);

    if (copy) {
        if (g_video_buffer_current) {
            memcpy((g_video_buffers + next_buffer)->data,
                    g_video_buffer_current->data,
                    (g_video_buffers + next_buffer)->size);
        } else {
            memset((g_video_buffers + next_buffer)->data, 0x0,
                    (g_video_buffers + next_buffer)->size);
        }
    }
    return g_video_buffers + next_buffer;
}
#endif

int fs_emu_buffer_lock(void)
{
    fs_mutex_lock(g_fs_emu_buffer_mutex);
    if (g_video_buffer_locked == -1) {
        g_video_buffer_locked = g_video_buffer_current;
    } else {
        /* already locked */
    }
    fs_mutex_unlock(g_fs_emu_buffer_mutex);
    return g_video_buffer_locked;
}

void fs_emu_buffer_unlock(void)
{
    fs_mutex_lock(g_fs_emu_buffer_mutex);
    g_video_buffer_locked = -1;
    fs_mutex_unlock(g_fs_emu_buffer_mutex);
}
