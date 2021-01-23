#ifndef FSEMU_SDLWINDOW_H_
#define FSEMU_SDLWINDOW_H_

#include "fsemu-config.h"

#ifdef FSEMU_SDL

#include "fsemu-sdl.h"

#ifdef FSEMU_OS_MACOS
#define FSEMU_KMOD_SCANCODE SDL_SCANCODE_LGUI
#define FSEMU_KMOD_MOD KMOD_LGUI
#else
#define FSEMU_KMOD_SCANCODE SDL_SCANCODE_LALT
#define FSEMU_KMOD_MOD KMOD_LALT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// This function can be called before fsemu_window_init and
// fsemu_sdlwindow_init.
void fsemu_sdlwindow_set_no_event_polling(bool no_event_polling);

// Initialize the sdlwindow module. Can safely be called more than once.
void fsemu_sdlwindow_init(void);

// Run this periodically to performance maintenance tasks such as toggling
// fullscreen or mouse capture. In general, this should change the actual
// window state to the desired state.
void fsemu_sdlwindow_update(void);

// Poll for and react to events, optionally wait for events until timeout.
// If this function is not called because of an external event loop,
// fsemu_sdlwindow_handle_event must be called with SDL events instead.
void fsemu_sdlwindow_work(int timeout_ms);

// Return true if the sdlwindow module does not poll for events itself.
bool fsemu_sdlwindow_no_event_polling(void);

// Return a pointer to the SDL_Window.
SDL_Window *fsemu_sdlwindow_window(void);

// Check if we have a valid SDL_Window.
bool fsemu_sdlwindow_has_window(void);

bool fsemu_sdlwindow_handle_event(SDL_Event *event);

void fsemu_sdlwindow_notify_frame_rendered_vt(void);

void fsemu_sdlwindow_notify_quit(void);

// Return true if the module believes the window is currently fullscreen.
bool fsemu_sdlwindow_fullscreen(void);

// Create the SDL_Window. Usually called from the fsemu_window module.
SDL_Window *fsemu_sdlwindow_create(void);

// Show the SDL_Window. Usually called from the fsemu_window module.
void fsemu_sdlwindow_show(void);

// Switch the window to fullscreen mode. Internal low-level function, usually
// you'll want to use fsemu_window_set_fullscreen.
void fsemu_sdlwindow_set_fullscreen(bool fullscreen);

// Handle SDL events of window type, called by fsemu_sdlwindow_handle_event.
// This function should usually not be called directly.
bool fsemu_sdlwindow_handle_window_event(SDL_Event *event);

// Set the sdl window if created outside of FSEMU. Beware, might be tricky.
// FIXME: Consider removing/deprecating this.
void fsemu_sdlwindow_set_window(SDL_Window *window);

#ifdef FSEMU_INTERNAL

// Return a pointer to the OpenGL context, if any.
SDL_GLContext fsemu_sdlwindow_gl_context(void);

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDL

#endif  // FSEMU_SDLWINDOW_H_
