#ifndef FS_INIT_H_
#define FS_INIT_H_

#include <fs/defines.h>

#define FS_INIT_FUNCTION(name) static int g_fs_init_ ## name = 0; \
static void fs_init_function_ ## name ## _2(); \
static void fs_init_function_ ## name () { \
    printf("\n\n\n\n\n\n\n\n init\n\n\n\n\n"); \
    fs_init_lock(); \
    if (g_fs_init_ ## name != 0) { \
        fs_init_unlock(); \
        return; \
    } \
    fs_init_function_ ## name ## _2(); \
    g_fs_init_ ## name = 1; \
    fs_init_unlock(); \
} \
static void fs_init_function_ ## name ## _2 ()


#define FS_INIT(name) if (g_fs_init_ ## name == 0) { \
    fs_init_function_ ## name (); \
}

#ifdef __cplusplus
extern "C" {
#endif

void fs_init();
void fs_init_lock();
void fs_init_unlock();

#ifdef __cplusplus
}
#endif

#endif // FS_INIT_H_
