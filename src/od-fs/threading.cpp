#include "sysconfig.h"
#include "sysdeps.h"
#include <fs/thread.h>

int uae_start_thread_fast (void *(*f)(void *), void *arg,
        uae_thread_id *thread) {
    // FIXME: what is the supposed difference between uae_start_thread and
    // uae_start_thread_fast?
    return uae_start_thread("(fast)", f, arg, thread);
}

int uae_start_thread (const char *name, uae_thread_function fn, void *arg,
        uae_thread_id *tid) {
    int result = 1;
    write_log("uae_start_tread \"%s\" function at %p arg %p\n", name,
            fn, arg);
#if 0
    uae_thread_id thread_id = SDL_CreateThread(fn, arg);
#endif
    uae_thread_id thread_id = fs_thread_create(fn, arg);
    if (thread_id == NULL) {
        write_log("ERROR creating thread\n");
        result = 0;
    }
    if (tid) {
        *tid = thread_id;
    }
    return result;
}

int uae_wait_thread (uae_thread_id thread) {
    //SDL_WaitThread (thread, (int*)0);
    fs_thread_wait(thread);
    return 0;
}

void uae_end_thread (uae_thread_id *thread) {
    STUB("");
    //SDL_KillThread(SDL_Thread *thread);
    fs_thread_destroy(*thread);
    if (thread) {
        *thread = NULL;
    }
}
