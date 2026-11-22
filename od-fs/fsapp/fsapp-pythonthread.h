/*
 * The UI thread: runs the Python UI in UI frames, each handing over its record to the main loop
 * (fsapp-surface.h).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_PYTHONTHREAD_H
#define FSAPP_PYTHONTHREAD_H

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

// Start the UI thread. It imports the Python module module_name and calls its function
// function_name with no arguments (the program's start-up code, which also initialises fsapp's and
// fsgui's Python side), and then runs UI frames until it is stopped. The first UI frame runs right
// after the start-up code. Before each of the others, the UI thread waits until its last record
// has been taken in (when one is in flight), and then sleeps until there is a wake reason
// (fsapp_pythonthread_wake: copied events, app events, a file dialog's result, a Python shortcut
// pressed, a requested UI frame), the next timer is due, or it is stopped; it lets go of the GIL
// meanwhile. Each UI frame runs the file dialogs' results and then fsapp._frame.frame(), which runs
// the due timers (fsapp's timer service), the rest of the UI frame's work and the timers due by
// then, and returns the next timer's time on SDL's clock (or None), and then hands over the UI
// frame's record (fsapp-surface.h), which wakes the main loop. An exception in either ends the
// thread (see fsapp_pythonthread_has_failed). The UI thread does not hold keep drawing
// (fsapp_frame_take_keep_drawing), so the main loop idles while the UI changes nothing.
//
// With FSAPP_TEST_SCENE=<path> set, it starts the test scene's stand-in UI thread instead, with no
// Python UI (fsapp-testscene.h), and sets up the GL grab for the scene (call this after any other
// fsapp_mainthread_set_gl_grab, before the main loop runs). A scene that cannot be read stops the
// program with an error (fsapp_main_stop_with_error), and no UI thread is started.
void fsapp_pythonthread_start(const char *module_name, const char *function_name);

// The UI wake: ends the UI thread's sleep between UI frames (or the next one, when it is not
// sleeping), so that it runs a UI frame once its last record has been taken in. Callable from any
// thread. Called when the host window side copies events for the UI thread, by app events
// (fsapp_events_push), by a file dialog's result, for a Python shortcut pressed
// (fsapp_main_handle_shortcut_event), for a requested UI frame
// (fsapp_pythonthread_on_frame_started) and by fsapp_pythonthread_stop.
void fsapp_pythonthread_wake(void);

// Requests a UI frame at the next host window frame (for animation): requests a host window frame
// (fsapp_frame_request, so that one is drawn also without a pacer), and the host window side wakes
// the UI thread when it starts that frame. So an animation that requests a UI frame in each UI
// frame runs one per host window frame. Callable from any thread.
void fsapp_pythonthread_request_ui_frame(void);

// The host window side, when it starts a frame to draw: wakes the UI thread if a UI frame has been
// requested since the last.
void fsapp_pythonthread_on_frame_started(void);

// Signal the Python thread to quit and wait for it to end, at most timeout_ns. Returns true if the
// thread ended normally (released the GIL), so the calling thread can use the Python API
// afterwards. If it did not end in time, it is detached and false is returned. The UI thread ends
// after its current UI frame, or at once when it is sleeping or waiting for its last record to be
// taken in.
bool fsapp_pythonthread_stop(int64_t timeout_ns);

// Whether the calling thread may free the UI's fonts and images, which are not thread-safe: the UI
// thread while it runs, and any thread before it is started or after it has ended. Their Python
// objects' destructors run on whichever thread drops the last reference.
bool fsapp_pythonthread_can_free_ui_objects(void);

// True if the Python thread has ended because of an error (an exception in the Python start-up
// code or in a frame, which has been logged). The thread does not run the UI anymore.
bool fsapp_pythonthread_has_failed(void);

// The last line of the Python exception (type and message) that ended the Python thread, or NULL
// if it has not failed.
const char *fsapp_pythonthread_get_error(void);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_PYTHONTHREAD_H */
