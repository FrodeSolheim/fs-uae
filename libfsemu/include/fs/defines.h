#ifndef FS_DEFINES_H_
#define FS_DEFINES_H_

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#define FS_SENTINEL __attribute__((__sentinel__))
#define FS_MALLOC __attribute__((__malloc__))
#define FS_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#define FS_PATH_MAX 2048

typedef int fs_bool;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*fs_compare_func)(const void *a, const void *b);
typedef int (*fs_compare_data_func)(const void *a, const void *b,
        void *user_data);
typedef int (*fs_equal_func)(const void *a, const void *b);
typedef void (*fs_destroy_notify)(void *data);
typedef void (*fs_func)(void * data, void *user_data);
typedef unsigned int (*fs_hash_func)(const void *key);
typedef void (*fs_h_func)(void *key, void *value, void *user_data);
typedef void (*fs_free_func)(void *data);

#ifdef __cplusplus
}
#endif

#endif // FS_DEFINES_H_
