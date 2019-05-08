#include "sysconfig.h"
#include "sysdeps.h"
#include "uae.h"
#include <fs/thread.h>

int uae_start_thread_fast (void *(*f)(void *), void *arg,
        uae_thread_id *thread)
{
    // FIXME: what is the supposed difference between uae_start_thread and
    // uae_start_thread_fast?
    return uae_start_thread(NULL, f, arg, thread);
}

int uae_start_thread (const char *name, uae_thread_function fn, void *arg,
        uae_thread_id *tid)
{
    int result = 1;
    if (name != NULL) {
        write_log("uae_start_tread \"%s\" function at %p arg %p\n", name,
                fn, arg);
    }
    uae_thread_id thread_id = fs_thread_create(name, fn, arg);
    if (thread_id == NULL) {
        write_log("ERROR creating thread\n");
        result = 0;
    }
    if (tid) {
        *tid = thread_id;
    } else if (thread_id) {
        fs_thread_free(thread_id);
    }
    return result;
}

int uae_wait_thread (uae_thread_id thread)
{
    //SDL_WaitThread (thread, (int*)0);
    fs_thread_wait(thread);
    return 0;
}

void uae_end_thread (uae_thread_id *thread)
{
    if (thread) {
        fs_thread_free(*thread);
        *thread = NULL;
    }
}

#include <SDL.h>

static SDL_threadID g_main_thread_id;
static SDL_threadID g_emulation_thread_id;

void uae_register_main_thread(void)
{
    g_main_thread_id = SDL_ThreadID();

}

bool uae_is_main_thread(void)
{
    return SDL_ThreadID() == g_main_thread_id;
}

bool is_mainthread(void)
{
    return uae_is_main_thread();
}

void uae_register_emulation_thread(void)
{
    g_emulation_thread_id = SDL_ThreadID();
}

bool uae_is_emulation_thread(void)
{
    return SDL_ThreadID() == g_emulation_thread_id;
}

/* FIXME: Move atomic functions to header for inlining? */

uae_atomic atomic_and(volatile uae_atomic *p, uae_u32 v)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_and(p, v);
}

uae_atomic atomic_or(volatile uae_atomic *p, uae_u32 v)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_or(p, v);
}

uae_atomic atomic_inc(volatile uae_atomic *p)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_add(p, 1);
}

uae_atomic atomic_dec(volatile uae_atomic *p)
{
    // FIXME: signature has changed, check return value!
    return __sync_fetch_and_sub(p, 1);
}

uae_u32 atomic_bit_test_and_reset(volatile uae_atomic *p, uae_u32 v)
{
    uae_u32 value = __sync_fetch_and_and(p, ~(1 << v));
    return (value >> v) & 1;
}
