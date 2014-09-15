#ifndef LIBFSEMU_UTIL_H_
#define LIBFSEMU_UTIL_H_

#include <fs/emu.h>
#include <fs/glib.h>

typedef struct fs_emu_stat_queue_ {
    int seq_no;
    int count;
    int max_count;
    int clear;
    //int max;
    //int min;
    int64_t total;
    //GList *list;
    //GList *last;
    GQueue* queue;
} fs_emu_stat_queue;

void fs_emu_stat_queue_init(fs_emu_stat_queue* stat_queue, int count);
void fs_emu_stat_queue_add_entry(fs_emu_stat_queue* stat_queue, int data,
        int threshold);

#endif // LIBFSEMU_UTIL_H_
