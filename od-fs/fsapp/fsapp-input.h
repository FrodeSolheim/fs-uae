/*
 * Input on the host window side: routing (fsapp-routing.h, which has the rules) wired to SDL's
 * events, keyboard focus, the input grab, and the surface events for the UI thread.
 *
 * Each input event that the shortcut registry did not consume (fsapp-main.h) is routed: to the
 * emulation (the event hooks, fsapp-mainthread.h), to the surface with keyboard focus or with
 * pointer focus (as a surface event, delivered to the surface's handler in the next UI frame), to
 * the raw copy of the event for the UI thread, or nowhere:
 *
 * - Keys go to the emulation while the emulation surface is the key target or the input grab is
 *   applied (the emulation has the input), else to the key target as KeyDown and KeyUp, else
 *   nowhere; never in the raw copy. The navigation keys (fsapp-navigation.h) give the key target a
 *   Navigation event after their KeyDown, with the keyboard as the device and the host's
 *   key repeat. Text input events go nowhere.
 * - Gamepad buttons and axes go to the emulation in the same cases, else through the navigation
 *   mapper (fsapp-navigation.h) to the key target as Navigation events, each with its gamepad
 *   and whether it is a repeat, else nowhere; never in the raw copy. A held direction or page
 *   repeats: fsapp_input_get_repeat_deadline_ns tells the main loop's waits when the next repeat is
 *   due, and fsapp_input_handle_repeats gives it, so repeats need no pacer and no other input.
 * - The menu input (the recogniser in fsapp-navigation.h, which sees every gamepad button first):
 *   Guide, or Start and Select together, never reaches the emulation. While the emulation has the
 *   input it goes to the callback registered with _fsapp.register_menu_input
 *   (fsapp_main_handle_menu_input), else to the key target as Navigation MENU, else nowhere.
 *   Start pressed first has reached the emulation at once, so when Select completes the chord the
 *   emulation is sent a release of Start (through the event hooks), and Start's own release is
 *   consumed.
 * - Mouse buttons, motion and the wheel go to the emulation while the input grab is applied, and
 *   otherwise to the surface with pointer focus (the topmost surface taking pointer input under the
 *   pointer, or the one a drag started on; adornments take no input; hit tested at the surfaces'
 *   device rectangles), as PointerButton, PointerMotion and PointerWheel at the position relative
 *   to the surface in its buffer pixels (fsapp_surface_get_relative_position), with PointerLeave
 *   and PointerEnter as the pointer focus changes; never in the raw copy. Motion is coalesced: at
 *   most one PointerMotion per surface waits for the UI thread, replaced by later motion and queued
 *   before any other event for that surface, and only the first wakes the UI thread. The middle
 *   button toggles the grab and goes nowhere.
 * - A press on a surface raises its tree's top-level (fsapp_surface_raise_applied) and, when that
 *   top-level is focusable, focuses the tree, before the press is queued.
 * - The click on the Amiga: a left or right press without the grab applied, on the emulation
 *   surface, focuses its tree. While a session runs (fsapp_input_set_session) and the emulation
 *   surface is then the key target, it also turns the grab on, and the press and its release go
 *   to the emulation only; otherwise, to the emulation surface.
 * - The host window's handler (_fsapp_surface.set_window_handler) gets PointerMotion, in host
 *   window pixels, wherever the pointer is and whichever surface has pointer focus, but not while
 *   the grab is applied, coalesced as a surface's (and queued before any other event), and
 *   PointerLeave when the pointer leaves the host window. REVIEW: This is fsapp's unstable API for
 *   the top (the fullscreen title bar), not the surface API.
 * - Popups: an open popup is its tree's key target. While a popup is open, a press outside every
 *   open popup (the chrome, the Amiga and the popup's own window included) is consumed with its
 *   release, and each open popup gets OutsideClick; its client closes it.
 * - A release goes where its press went.
 * - Hotplug events (keyboards, mice, joysticks and gamepads added or removed) go to the emulation
 *   and to the raw copy, as do no other events: joystick buttons, axes and hats (of devices that
 *   are not gamepads) and the rest go only to the raw copy, but the host window's events
 *   (SDL_EVENT_WINDOW_*), which go to neither.
 *
 * The navigation mapper forgets what is held when the key target changes, so repeats go only to
 * the surface the direction was pressed on.
 *
 * Keyboard focus belongs to a surface tree (a top-level and its children and popups) or none, and
 * the keys go to the tree's key target: its topmost open popup, else the surface its top-level
 * names (fsapp_surface_set_key_target), else the top-level. fsapp grants focus on focus requests
 * (fsapp_surface_request_focus), applied in order when the record of the UI frame that made them is
 * applied, after the record's other changes: a request names a surface and focuses its tree, and is
 * ignored when the tree's top-level is not focusable (fsapp_surface_set_focusable) or not visible.
 * When the focused tree's top-level is destroyed, hidden or made unfocusable, focus falls back to
 * the topmost focusable top-level. The surfaces that stop and start being the key target get
 * FocusOut and FocusIn, and the key target gets FocusOut when the host window loses focus and
 * FocusIn when it gets it back.
 *
 * The input grab has a wanted state, which the user toggles (Mod+G, the middle button; both do
 * nothing while the key target is a surface other than the emulation surface), and an applied state
 * (relative mouse mode, which the program sets): applied only while wanted and the emulation
 * surface is the key target. Turning it on focuses the emulation surface's tree.
 *
 * The host window's configure (fsapp-pixels.h) is sent by the host window side
 * (fsapp_input_send_configure) and delivered in the UI frame, before the other surface events, to
 * the host window's handler (_fsapp_surface.set_window_handler) as it is, and to every surface with
 * a handler as its own configure (fsapp_pixels_get_surface_configure: the host window's size in its
 * buffer pixels and its surface scale). Each gets its configure when it differs from the last it
 * got, so a surface (or handler) set later gets the current one with its first events, changes
 * made before a UI frame takes them reach it as one configure, and a change that leaves a surface's
 * configure as it was sends it none.
 *
 * The host window's state (whether it is maximized and whether it is full screen) is sent the same
 * way (fsapp_input_send_window_state), and delivered to the host window's handler after its
 * configure, as WindowState.
 *
 * Everything here runs on the host window side, unless it says otherwise.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_INPUT_H
#define FSAPP_INPUT_H

#include <Python.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "fsapp-pixels.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_surface;

// Sets the emulation surface: while it is the key target, or the input grab is applied, the input
// goes to the emulation. Call on the main thread before the main loop runs.
void fsapp_input_set_emulation_surface(struct fsapp_surface *surface);

// Sets whether an emulation session runs, for the click on the Amiga (described at the top).
void fsapp_input_set_session(bool session);

// Where an event goes (fsapp_input_handle_event), as bits.
typedef enum fsapp_input_destination {
    // The event hooks (the emulation).
    FSAPP_INPUT_TO_EVENT_HOOKS = 1 << 0,
    // The raw copy for the UI thread.
    FSAPP_INPUT_TO_UI_COPY = 1 << 1,
} fsapp_input_destination_t;

// Routes an event that the shortcut registry did not consume: queues the surface events it gives
// (waking the UI thread), and returns where else it goes (fsapp_input_destination_t bits).
int fsapp_input_handle_event(const SDL_Event *event);

// Gives the focused surface the navigation repeats due at now_ns (SDL's clock), as described at the
// top. Called by the main loop with the events, and while it waits.
void fsapp_input_handle_repeats(int64_t now_ns);

// The time of the next navigation repeat (SDL's clock), for the main loop's waits: returns false
// when nothing repeats.
bool fsapp_input_get_repeat_deadline_ns(int64_t *deadline_ns);

// Toggles the input grab (Mod+G), as described at the top.
void fsapp_input_toggle_grab(void);

// Whether the user wants the input grab (it may be suspended). Any thread. When it changes, fsapp
// pushes the app event FSAPP_MOUSE_GRAB, with intdata 1 when wanted and 0 when not
// (fsapp-events.h).
bool fsapp_input_is_grab_wanted(void);

// Whether the input grab is applied: the program sets relative mouse mode to it.
bool fsapp_input_is_grab_applied(void);

// Called by the surface module when it has applied a record, with the record's focus requests in
// the order they were made (only compared, never dereferenced).
void fsapp_input_on_record_applied(struct fsapp_surface *const *focus_requests, int count);

// _fsapp_surface.take_events(): the surface events queued since the last call, for fsapp's Python
// side (fsapp/surfaceevent.py), as tuples (handler, surface id, kind, fields), with 0 as the host
// window handler's id. The fields are a tuple: (scancode, keycode, modifiers, repeat) for a key
// pressed, (scancode, keycode, modifiers) released, (value, device kind
// (fsapp_navigation_device_kind_t), device id, repeat) for navigation, (x, y) for PointerEnter,
// (x, y, modifiers) for PointerMotion, (x, y, button, down, clicks, modifiers) for PointerButton,
// (x, y, wheel x, wheel y, modifiers) for PointerWheel, and () for the rest. Events for surfaces
// without a handler, or destroyed, are left out. UI thread.
PyObject *fsapp_input_python_take_events(PyObject *self, PyObject *args);

// Sends the host window's configure to the UI, as described at the top (waking the UI thread),
// when none has been sent yet or it differs from the last sent (fsapp_pixels_take_configure).
// Returns whether it was sent.
bool fsapp_input_send_configure(const fsapp_pixels_configure_t *configure);

// The host window's last configure sent: returns false when none has been sent yet (before the host
// window exists). Any thread.
bool fsapp_input_get_configure(fsapp_pixels_configure_t *configure);

// Called by the surface module when a handler is set on the surface (not None), so that it gets
// the current configure with its first events. UI thread.
void fsapp_input_on_handler_set(struct fsapp_surface *surface);

// _fsapp_surface.set_window_handler(handler): the host window's handler (or None for none), which
// gets the configures. UI thread.
PyObject *fsapp_input_python_set_window_handler(PyObject *self, PyObject *args);

// _fsapp_surface.take_configures(): the configures due (as described at the top), as tuples
// (handler, surface id, width, height, scale), with 0 as the host window handler's id. UI thread.
PyObject *fsapp_input_python_take_configures(PyObject *self, PyObject *args);

// The host window's state.
typedef struct fsapp_input_window_state {
    bool maximized;
    bool fullscreen;
} fsapp_input_window_state_t;

// Sends the host window's state to the UI, as described at the top (waking the UI thread), when
// none has been sent yet or it differs from the last sent. Returns whether it was sent.
bool fsapp_input_send_window_state(const fsapp_input_window_state_t *state);

// _fsapp_surface.take_window_state(): the host window's state for its handler, as a tuple (handler,
// maximized, fullscreen), when it differs from the last the handler got (or it has got none), else
// None. UI thread.
PyObject *fsapp_input_python_take_window_state(PyObject *self, PyObject *args);

// The kinds of surface events in _fsapp_surface.take_events().
typedef enum fsapp_input_event_kind {
    FSAPP_INPUT_EVENT_KEY_DOWN,
    FSAPP_INPUT_EVENT_KEY_UP,
    FSAPP_INPUT_EVENT_FOCUS_IN,
    FSAPP_INPUT_EVENT_FOCUS_OUT,
    FSAPP_INPUT_EVENT_NAVIGATION,
    FSAPP_INPUT_EVENT_POINTER_ENTER,
    FSAPP_INPUT_EVENT_POINTER_LEAVE,
    FSAPP_INPUT_EVENT_POINTER_MOTION,
    FSAPP_INPUT_EVENT_POINTER_BUTTON,
    FSAPP_INPUT_EVENT_POINTER_WHEEL,
    FSAPP_INPUT_EVENT_OUTSIDE_CLICK,
} fsapp_input_event_kind_t;

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_INPUT_H */
