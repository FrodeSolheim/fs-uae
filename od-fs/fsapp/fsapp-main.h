/*
 * fsapp's start-up, the host window's events, shortcuts and quitting.
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

// fsapp's handling of an event the shortcut registry did not consume (window operations, quitting),
// after routing (fsapp-input.h), and its raw copy for the UI thread when copy_for_ui is set.
void fsapp_main_handle_event(SDL_Event *event, bool copy_for_ui);

void fsapp_main_handle_dialog_result(void);

// Shortcuts: keys (with modifiers) of the program's, looked up in fsapp's shortcut registry
// (fsapp-shortcuts.h has the rules) for each key event before anything else sees it. The key events
// of a shortcut are consumed: they reach neither routing (fsapp-input.h), the event hooks (the
// Amiga), fsapp's handling nor the copy for the UI thread. fsapp registers F11 and Mod+F
// (fullscreen), Mod+G (the input grab, fsapp_input_toggle_grab) and Mod+Q (quit); Python code
// registers with _fsapp.register_shortcut, whose callback runs on the UI thread in its next UI
// frame.

// Mod, the modifier of the program's shortcuts: Left Alt, or Left GUI (Cmd) on macOS. The modifier
// key itself is no shortcut, so its own events go on (to the Amiga).
SDL_Keymod fsapp_main_get_shortcut_mod(void);

// Called on the main thread (the host window side) when the shortcut is pressed (not for its
// repeats or its release).
typedef void (*fsapp_main_shortcut_callback_t)(void *data);

// Registers a shortcut: the key with the modifiers that must be down (others may be down too;
// fsapp_main_get_shortcut_mod() for Mod), and the callback called with data when it is pressed.
// Returns its id (above 0), or 0 if the key is already registered with the same modifiers. Call
// after fsapp_main_init; any thread.
int fsapp_main_register_shortcut(SDL_Keycode key, SDL_Keymod modifiers,
                                 fsapp_main_shortcut_callback_t callback, void *data);

// Looks up a key event in the shortcut registry: returns true if it belongs to a shortcut (and is
// consumed), after calling its callback (or queueing a Python one) on the shortcut's press, and
// false for any other event. Host window side, before the event hooks.
bool fsapp_main_handle_shortcut_event(const SDL_Event *event);

// The menu input (Guide, or Start and Select together; fsapp-input.h) while the emulation has the
// input: queues the callback registered with _fsapp.register_menu_input for the UI thread's next UI
// frame, with the shortcuts' callbacks, and returns true; returns false and does nothing if none is
// registered. The callback is registered once, and never unregistered. Host window side.
bool fsapp_main_handle_menu_input(void);

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
// the same). The size is in pixels. Without the window or the context, the program stops with an
// error.
fsapp_window_t *fsapp_main_create_window(const char *title, int width, int height, bool fullscreen);

// Sets the colour the host window is cleared with. Called on the UI thread (_fsapp's
// set_window_background_color does the same).
void fsapp_main_set_window_background_color(fsapp_window_t *window, SDL_Color color);

// The host window's background colour, set from the UI (the colour the compositor clears with).
// Call on the main loop's host window side (fsapp-mainthread.h), once the host window exists.
SDL_Color fsapp_main_get_background_color(void);

// Whether the mouse is in relative mode for the emulation (input grab): set on the main thread by
// the program's mouse grab policy, together with SDL's relative mouse mode. False before the host
// window exists.
bool fsapp_main_is_relative_mouse(void);
void fsapp_main_set_relative_mouse(bool relative_mouse);

// The host window's size in window coordinates, for host window operations (SDL's own unit), and
// its size in pixels, which surfaces and layout are in. They differ with high pixel density. Call
// on the main loop's host window side (fsapp-mainthread.h), once the host window exists; the
// drawing side has them in the frame's snapshot.
void fsapp_main_get_window_size(int *width, int *height);
void fsapp_main_get_window_size_in_pixels(int *width, int *height);

// Converts the mouse position of a mouse event (motion, button or wheel) from window coordinates to
// pixels, in place; relative motion is left as it is. Called on the host window side when the
// event arrives, before routing and the copy for the UI.
void fsapp_main_convert_event_to_pixels(SDL_Event *event);

extern SDL_Window *g_window;
extern int g_window_width;
extern int g_window_height;

extern fsapp_channel_t *g_fsapp_main_channel;

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_MAIN_H */
