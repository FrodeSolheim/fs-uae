#ifndef FSUAE_MAINTHREAD_H
#define FSUAE_MAINTHREAD_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Run fsapp's main loop with the program's hooks, until quit is requested.
void fsuae_mainthread_run(void);

// Ask the main loop to end, and so FS-UAE to quit (main stops the emulation and the UI thread).
// fsapp calls it for quit requests from the user (set with fsapp_main_set_quit_function).
void fsuae_mainthread_quit(void);

// Whether quit has been requested (fsuae_mainthread_quit).
bool fsuae_mainthread_is_quitting(void);

#ifdef __cplusplus
}
#endif

#endif /* FSUAE_MAINTHREAD_H */
