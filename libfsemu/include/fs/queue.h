#ifndef FS_QUEUE_H_
#define FS_QUEUE_H_

#include <fs/defines.h>
#include <fs/list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fs_queue {
  fs_list *head;
  fs_list *tail;
  unsigned int length;
};

typedef struct fs_queue fs_queue;

fs_queue *fs_queue_new(void);
void fs_queue_unlink(fs_queue *queue, fs_list *link_);
void fs_queue_delete_link(fs_queue *queue, fs_list *link_);
void fs_queue_insert_before(fs_queue *queue, fs_list *sibling, void *data);
fs_list *fs_queue_peek_head_link(fs_queue *queue);
fs_list *fs_queue_peek_nth_link(fs_queue *queue, unsigned int n);
void fs_queue_push_tail(fs_queue *queue, void *data);
void *fs_queue_pop_head(fs_queue *queue);
void *fs_queue_pop_tail(fs_queue *queue);
void fs_queue_push_head(fs_queue *queue, void *data);
unsigned int fs_queue_get_length(fs_queue *queue);

#ifdef __cplusplus
}
#endif

#endif // FS_QUEUE_H_
