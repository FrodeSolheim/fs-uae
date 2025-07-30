#include "sysconfig.h"
#include "sysdeps.h"

#include <fs/thread.h>

#include "fsemu-sdl.h"
#include "uae.h"

#include <SDL3/SDL.h>



















// An intermediate thread function is needed because the return type of
// uae_thread_function is different.

typedef struct thread_data {
    uae_thread_function function;
    void *data;
} thread_data;

static void* thread_function (void *data)
{
    thread_data *tdata = (struct thread_data *) data;
    tdata->function(tdata->data);
    free(tdata);
    // return 0;
    return NULL;
}

int uae_start_thread_fast(void (*f)(void *), void *arg, uae_thread_id *thread)
{
    // FIXME: what is the supposed difference between uae_start_thread and
    // uae_start_thread_fast?
    #warning Not adjusting thread priority at the moment
    // FIXME: ANSWER - HIGHER THREAD priority if possible
    return uae_start_thread(NULL, f, arg, thread);
}

int uae_start_thread(const char *name,
                     uae_thread_function fn,
                     void *arg,
                     uae_thread_id *tid)
{
    int result = 1;
    if (name != NULL) {
        write_log(
            "uae_start_tread \"%s\" function at %p arg %p\n", name, fn, arg);
    }

    thread_data *tdata = (thread_data *) malloc(sizeof(thread_data));
    tdata->function = fn;
    tdata->data = arg;

    uae_thread_id thread_id = fs_thread_create(name, thread_function, tdata);
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

int uae_wait_thread(uae_thread_id thread)
{
    // SDL_WaitThread (thread, (int*)0);
    fs_thread_wait(thread);
    return 0;
}

void uae_end_thread(uae_thread_id *thread)
{
    if (thread) {
        fs_thread_free(*thread);
        *thread = NULL;
    }
}

static SDL_ThreadID g_main_thread_id;
static SDL_ThreadID g_emulation_thread_id;

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
    // Was a bit of misunderstanding here, is_mainthread should return true
    // if it is the *main emulation thread*. Fixed now. FIXME: Doublecheck
    // that "mainthread" in WinUAE is infact the emulation thread.
    return uae_is_emulation_thread();
}

uae_thread_id uae_thread_get_id(void)
{
    // Casting a long to a pointer isn't very nice,
    // but should work at least
    return (uae_thread_id) SDL_GetCurrentThreadID();
}

void uae_register_emulation_thread(void)
{
    g_emulation_thread_id = SDL_GetCurrentThreadID();
}

bool uae_is_emulation_thread(void)
{
    return SDL_GetCurrentThreadID() == g_emulation_thread_id;
}
