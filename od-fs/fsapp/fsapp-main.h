/*
 * fsapp's start-up, the host window's events, quitting and the input grab state.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_MAIN_H
#define FSAPP_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "fsapp-channel.h"
#include "fsapp-window.h"

extern uint32_t SDL_EVENT_FSAPP_WINDOW;
extern uint32_t SDL_EVENT_FSAPP_CUSTOM;

bool fsapp_main_init(int argc, char *argv[], int *error);

// Sends Python's output to the log (fsapp/log.py): sys.stdout and sys.stderr hand their lines to
// it, and the root logger's handler the logging module's records. Call it on the thread that
// initialised Python, right after Python is initialised and its path set, before any other of the
// program's Python code is imported. Returns false (and logs why) if it could not.
bool fsapp_main_install_python_log(void);

void fsapp_main_handle_event(SDL_Event *event);

void fsapp_main_handle_dialog_result(void);

// What fsapp calls when the user asks to quit: the quit shortcut, closing the host window, the
// system's quit request (SDL_EVENT_QUIT) and _fsapp.quit() from Python. fsapp does not quit by
// itself; the program sets the quit function, on the main thread before the main loop runs, and
// decides what quitting means. Called on the main thread (and from Python, on the UI thread).
typedef void (*fsapp_main_quit_function_t)(void);

void fsapp_main_set_quit_function(fsapp_main_quit_function_t quit_function);

// Ask the program to quit, by calling the quit function, and wake the main loop
// (fsapp_frame_wake), so that it sees the request also while it idles. Does nothing (with a
// warning) if no quit function is set.
void fsapp_main_quit(void);

// Stop the program because of an error it cannot run with, such as no OpenGL 2.1 context or a GL
// function missing: logs the message, keeps it (only the first, if called again) and asks to quit
// (fsapp_main_quit). The program shows the message after the main loop has ended (see
// fsapp_main_get_error) and exits with an error. Call on the main thread.
void fsapp_main_stop_with_error(const char *message);

// The message given to fsapp_main_stop_with_error, or NULL if it has not been called. Call on the
// main thread.
const char *fsapp_main_get_error(void);

// Asks the main thread to create the host window (fsapp supports one), with its GL context, and
// returns its record at once; the window itself exists once the main loop has handled the request
// (g_fsapp_window and g_window are set then). Called on the UI thread (_fsapp's create_window does
// the same). Without the window or the context, the program stops with an error.
fsapp_window_t *fsapp_main_create_window(const char *title, int width, int height, bool fullscreen);

// Sets the colour the host window is cleared with. Called on the UI thread (_fsapp's
// set_window_background_color does the same).
void fsapp_main_set_window_background_color(fsapp_window_t *window, SDL_Color color);

// The host window's background colour, set from the UI (the colour the compositor clears with).
// Call on the main loop's host window side (fsapp-mainthread.h), once the host window exists.
SDL_Color fsapp_main_get_background_color(void);

// Whether the mouse is in relative mode for the emulation (input grab): set on the main thread by
// the program's mouse grab policy, together with SDL's relative mouse mode, and read by the
// emulation's event hook. False before the host window exists.
bool fsapp_main_is_relative_mouse(void);
void fsapp_main_set_relative_mouse(bool relative_mouse);

// The host window's size, and its size in pixels (which differs with high-DPI scaling). Call on
// the main loop's host window side (fsapp-mainthread.h), once the host window exists; the drawing
// side has them in the frame's snapshot.
void fsapp_main_get_window_size(int *width, int *height);
void fsapp_main_get_window_size_in_pixels(int *width, int *height);

extern SDL_Window *g_window;
extern int g_window_width;
extern int g_window_height;

extern bool g_fsapp_force_ui_cursor;
// Whether the mouse is grabbed (main thread). When it changes, fsapp pushes the app event
// FSAPP_MOUSE_GRAB, with intdata 1 when grabbed and 0 when released (fsapp-events.h).
extern bool g_fsapp_mouse_grab;

extern fsapp_channel_t *g_fsapp_main_channel;

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_MAIN_H */
