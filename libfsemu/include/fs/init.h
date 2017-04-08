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

#define FS_INIT_ONCE() static int initialized; \
    if (initialized) { \
        return; \
    } \
    initialized = 1

#define FSE_INIT_ONCE() static int initialized; \
    if (initialized) { \
        return; \
    } \
    initialized = 1

#ifdef __cplusplus
extern "C" {
#endif

void fs_init(void);
void fs_init_lock(void);
void fs_init_unlock(void);

#ifdef __cplusplus
}
#endif

#endif // FS_INIT_H_
