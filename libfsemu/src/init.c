#include <stdlib.h>
#include <string.h>

#include <fs/init.h>
#include <fs/thread.h>

#ifdef USE_GLIB
#include <glib.h>
#endif

static fs_mutex *g_init_mutex;

void fs_init() {
#ifdef USE_GLIB
    g_thread_init(NULL);
#endif
    g_init_mutex = fs_mutex_create();
}

void fs_init_lock() {
    fs_mutex_lock(g_init_mutex);
}

void fs_init_unlock() {
    fs_mutex_unlock(g_init_mutex);
}
