#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/emu.h>
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void fs_emu_stat_queue_init(fs_emu_stat_queue* stat_queue, int count) {
    stat_queue->queue = g_queue_new();
    stat_queue->max_count = count;
    for (int i = 0; i < count; i++) {
        g_queue_push_head(stat_queue->queue, FS_INT_TO_POINTER(0));
    }

}

void fs_emu_stat_queue_add_entry(fs_emu_stat_queue* stat_queue, int data,
           int threshold) {
    if (stat_queue->queue == NULL) {
        fs_log("fs_emu_stat_queue_add_entry: queue is null");
        return;
    }
    if (stat_queue->clear) {
        for (int i = 0; i < stat_queue->max_count; i++) {
            g_queue_push_head(stat_queue->queue, FS_INT_TO_POINTER(0));
            g_queue_pop_tail(stat_queue->queue);
        }
        stat_queue->count = 0;
        stat_queue->total = 0;
        stat_queue->clear = 0;
    }

    /*
    if (treshold && abs(data) >= treshold) {
        return;
    }
    */

    //fs_log("add %d\n", data);
    g_queue_push_head(stat_queue->queue, FS_INT_TO_POINTER(data));
    int last = FS_POINTER_TO_INT(g_queue_pop_tail(stat_queue->queue));
    // fixme: min and max
    stat_queue->total += data - last;
    //fs_log("data %d last %d total %lld\n", data, last, stat_queue->total);
    stat_queue->seq_no += 1;
    if (stat_queue->count < stat_queue->max_count) {
        stat_queue->count++;
    }
}
