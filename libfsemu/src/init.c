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

    GMemVTable vtable;
    memset(&vtable, 0, sizeof(GMemVTable));
    vtable.malloc = malloc;
    vtable.realloc = realloc;
    vtable.free = free;
    g_mem_set_vtable(&vtable);

#if !GLIB_CHECK_VERSION (2, 32, 0)
    // g_thread_init is deprecated since 2.32, and is only needed for older
    // versions of Glib (quite a bit older than 2.32, actually)
    g_thread_init(NULL );
#endif
#endif

    g_init_mutex = fs_mutex_create();
}

void fs_init_lock() {
    fs_mutex_lock(g_init_mutex);
}

void fs_init_unlock() {
    fs_mutex_unlock(g_init_mutex);
}
