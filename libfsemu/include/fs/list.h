#ifndef FS_LIST_H_
#define FS_LIST_H_

#include <fs/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fs_list {
    void *data;
    struct fs_list *next;
    struct fs_list *prev;
};

typedef struct fs_list fs_list;

fs_list *fs_list_append(fs_list *list, void *data) FS_WARN_UNUSED_RESULT;
fs_list *fs_list_prepend(fs_list *list, void *data) FS_WARN_UNUSED_RESULT;
fs_list *fs_list_delete_link(fs_list *list, fs_list *link_)
        FS_WARN_UNUSED_RESULT;

void fs_list_free(fs_list *list);
void fs_list_free_1(fs_list *list);

fs_list* fs_list_sort(fs_list *list, fs_compare_func compare_func)
        FS_WARN_UNUSED_RESULT;
void fs_list_foreach(fs_list *list, fs_func func, void *user_data);
fs_list *fs_list_last(fs_list *list);
fs_list *fs_list_nth(fs_list *list, unsigned int n);

#ifdef __cplusplus
}
#endif

#endif // FS_LIST_H_
