#ifndef FSAPP_PYTHONTHREAD_H
#define FSAPP_PYTHONTHREAD_H

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

extern bool g_python_ready;
extern bool g_python_quit;
extern SDL_Mutex* g_python_mutex;
extern SDL_Condition* g_python_condition;

// void fsapp_pythonthread_init(void);
// SDL_Thread* fsapp_pythonthread_create_and_run(void);

void fsapp_pythonthread_start(void);

void fsapp_pythonthread_stop(void);

#ifdef __cplusplus
}
#endif

#endif  // FSAPP_PYTHONTHREAD_H
