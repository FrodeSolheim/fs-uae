#ifndef FS_HASH_TABLE_H_
#define FS_HASH_TABLE_H_

#include <fs/defines.h>
#include <fs/string.h>
#include <fs/list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _fs_hash_table  fs_hash_table;
typedef struct _fs_hash_table_iter fs_hash_table_iter;

unsigned int fs_direct_hash (const void *v);

fs_hash_table *fs_hash_table_new(fs_hash_func hash_func,
        fs_equal_func key_equal_func);

fs_hash_table *fs_hash_table_new_full(fs_hash_func hash_func,
        fs_equal_func key_equal_func, fs_destroy_notify key_destroy_func,
        fs_destroy_notify value_destroy_func);

void fs_hash_table_destroy(fs_hash_table *hash_table);

void fs_hash_table_remove_all(fs_hash_table *hash_table);
void *fs_hash_table_lookup(fs_hash_table *hash_table, const void *key);
void fs_hash_table_insert(fs_hash_table *hash_table, void *key, void *value);
void fs_hash_table_replace(fs_hash_table *hash_table, void *key, void *value);
unsigned int fs_hash_table_size(fs_hash_table *hash_table);
fs_list *fs_hash_table_get_keys(fs_hash_table *hash_table);
int fs_hash_table_remove(fs_hash_table *hash_table, const void *key);

#ifdef __cplusplus
}
#endif

#endif // FS_HASH_TABLE_H_
