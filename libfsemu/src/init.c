#include <fs/init.h>
#include <fs/thread.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

static fs_mutex *g_init_mutex;

void fs_init() {
    g_thread_init(NULL);
    g_init_mutex = fs_mutex_create();
}

void fs_init_lock() {
    fs_mutex_lock(g_init_mutex);
}

void fs_init_unlock() {
    fs_mutex_unlock(g_init_mutex);
}
