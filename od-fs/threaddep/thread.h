#ifndef THREADDEP_THREAD_H
#define THREADDEP_THREAD_H

#include <fs/thread.h>

#include "threaddep/sem.h"

// FIXME: Possibly only safe for 64-bit systems now
// SDL3: typedef Uint64 SDL_ThreadID;
// Maybe better to use uint64_t also here...
typedef fs_thread *uae_thread_id;

#include "commpipe.h"

// #define BAD_THREAD NULL
#define BAD_THREAD 0

#define uae_set_thread_priority(thread_id, pri)

// typedef int (*uae_thread_function) (void *);
// typedef fs_thread_function uae_thread_function;
typedef void (*uae_thread_function)(void *);

int uae_start_thread(const char *name,
                     uae_thread_function fn,
                     void *arg,
                     uae_thread_id *tid);
extern int uae_start_thread_fast(void (*f)(void *), void *arg, uae_thread_id *thread);
int uae_wait_thread(uae_thread_id thread);
void uae_end_thread(uae_thread_id *thread);

void uae_register_main_thread(void);
bool uae_is_main_thread(void);

void uae_register_emulation_thread(void);
bool uae_is_emulation_thread(void);

uae_thread_id uae_thread_get_id(void);

/* Do nothing; thread exits if thread function returns.  */
#define UAE_THREAD_EXIT \
    do {                \
    } while (0)

#endif /* THREADDEP_THREAD_H */
