/*
 * Routing: where one input event goes on the host window side (a shortcut, the Amiga, the surface
 * with keyboard focus, the surface with pointer focus, or nowhere), and the keyboard focus, pointer
 * focus and input grab that decide it.
 *
 * A pure unit with no SDL, clock, thread or other module, so that its tests (od-fs/tests/fsapp)
 * compile its source file alone. The caller passes the applied surfaces in the stacking order
 * (fsapp-stacking.h) with each event, and runs the shortcut registry (fsapp-shortcuts.h) and the
 * menu input recogniser (fsapp-navigation.h) first, passing on their answers.
 *
 * The rules:
 *
 * - Keyboard focus belongs to a surface tree (a top-level and its descendants) or none, and the
 *   focused tree's key target gets the keys: its topmost open (visible) popup, else the surface in
 *   the tree its top-level names (a child, such as the emulation surface in the emulator window's
 *   tree), if it is in the list and visible, else the top-level. A focus request names a
 *   surface, and is granted for its tree when the tree's top-level is in the list, visible and
 *   focusable (the requested surface's own focusable flag does not count), and ignored otherwise.
 *   When the focused tree's top-level has left the list, is hidden or is no longer focusable,
 *   focus falls back to the topmost focusable and visible top-level, else none. This is checked
 *   with every event. FocusOut and FocusIn go to the key targets as the key target changes
 *   (focus_out and focus_in).
 * - The input grab is wanted (toggled by the user) and applied (relative mouse mode) only while
 *   wanted and the focused tree's key target is the emulation surface: focus on another tree
 *   suspends it, and focus back applies it again. The toggle changes nothing while the key target
 *   is another surface; turning the grab on focuses the emulation surface's tree.
 * - The Amiga has the input while the key target is the emulation surface or the grab is applied.
 * - A key the shortcut registry consumed goes to its shortcut. Other keys, and gamepad buttons and
 *   axes, go to the Amiga when it has the input, else to the key target (gamepads through the
 *   navigation mapper), else nowhere.
 * - The menu input goes to the top (FSAPP_ROUTING_SHORTCUT) when the Amiga has the input, else to
 *   the key target (as the menu navigation value), else nowhere.
 * - Pointer events (mouse buttons, motion and the wheel) go to the Amiga while the grab is applied.
 *   Otherwise they go to the pointer focus: the topmost surface taking pointer input under the
 *   pointer (fsapp_routing_hit_test), recomputed with every event while the pointer is in the host
 *   window, except during a drag, from a press that went to a surface until the last such press is
 *   released, when it stays where the drag started. A change of pointer focus gives the surface
 *   losing it PointerLeave and the one getting it PointerEnter, before the event, or after it when
 *   the change comes from the event (a drag's last release, the grab applied or let go). The host
 *   window's mouse leave takes the pointer focus away (after a drag), and the next event with a
 *   position gives it back (the host window's mouse enter has none); the grab applied takes it
 *   away, and the grab let go gives it back where the pointer was last.
 * - A press (any button but the middle one) on a surface raises its tree's top-level within its
 *   band and, when that top-level is focusable, gives the tree keyboard focus, before the press is
 *   delivered. A press of the middle button toggles the grab instead, and goes nowhere, nor does
 *   its release. A left or right press without the grab, on the emulation surface: with a session,
 *   focuses its tree and, when that makes the emulation surface the key target, turns the grab on
 *   and goes to the Amiga (the click on the Amiga); without a session, focuses its tree and goes
 *   to it.
 * - While any popup is open (shown, so not while its tree is hidden), a press (any button, the
 *   middle one too) without the grab applied outside every open popup is the outside click: it is
 *   consumed (no surface gets it, nothing is raised or focused, the grab is not toggled), its
 *   release too, and each open popup is to get OutsideClick. A press on an open popup, or on a
 *   surface above one in its tree (a popup's child or popup), is a normal press. Routing closes no
 *   popup: its client does.
 * - A release goes where its press went (keys, mouse buttons and gamepad buttons), or nowhere if
 *   that surface has left the list, so that neither the Amiga nor the navigation mapper is left
 *   with something held. Axes follow focus.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_ROUTING_H
#define FSAPP_ROUTING_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Mouse buttons, with SDL3's values (SDL_BUTTON_LEFT and so on).
#define FSAPP_ROUTING_BUTTON_LEFT 1
#define FSAPP_ROUTING_BUTTON_MIDDLE 2
#define FSAPP_ROUTING_BUTTON_RIGHT 3

typedef struct fsapp_routing fsapp_routing_t;

// A rectangle in device pixels (a surface's device rectangle). Empty when its width or height is 0
// or less.
typedef struct fsapp_routing_rectangle {
    int x;
    int y;
    int width;
    int height;
} fsapp_routing_rectangle_t;

// A surface in the stacking order (adornments are not in it: they take no input).
typedef struct fsapp_routing_surface {
    // Identifies the surface (never NULL); routing never dereferences it.
    const void *id;
    fsapp_routing_rectangle_t rectangle;
    // Whether it is shown: it and all its ancestors are visible (fsapp-stacking.h).
    bool visible;
    bool focusable;
    // Whether its role takes pointer input (an overlay does not). A hidden surface or one with an
    // empty rectangle takes none either.
    bool takes_pointer;
    // The top-level of its tree (itself for a top-level or an overlay).
    const void *top_level;
    // A top-level's: the surface in its tree it names its key target, or NULL for itself.
    const void *key_target;
    // Whether its role is popup.
    bool popup;
} fsapp_routing_surface_t;

// The applied state an event is routed in.
typedef struct fsapp_routing_input {
    // The surfaces in the stacking order, from back to front.
    const fsapp_routing_surface_t *surfaces;
    int surface_count;
    // The emulation surface's id, or NULL for none.
    const void *emulation_surface;
    // Whether a session runs (set by fsemu): a click on the Amiga grabs only then.
    bool session;
} fsapp_routing_input_t;

typedef enum fsapp_routing_event_kind {
    // The list changed (a surface hidden, destroyed or made unfocusable): only checks focus.
    FSAPP_ROUTING_SURFACES_CHANGED,
    // A request for keyboard focus for surface.
    FSAPP_ROUTING_FOCUS_REQUEST,
    // The user toggled the input grab (Mod+G): off when applied, else on; nothing while a UI
    // surface has focus.
    FSAPP_ROUTING_GRAB_TOGGLE,
    // A key (code: the scancode), pressed or released; also a repeat.
    FSAPP_ROUTING_KEY,
    // A gamepad button (code: SDL_GamepadButton) of device, pressed or released.
    FSAPP_ROUTING_GAMEPAD_BUTTON,
    // A gamepad axis (code: SDL_GamepadAxis) of device moved.
    FSAPP_ROUTING_GAMEPAD_AXIS,
    // The menu input, from the recogniser.
    FSAPP_ROUTING_MENU_INPUT,
    // A mouse button (code: FSAPP_ROUTING_BUTTON_* or another of SDL3's), pressed or released at
    // x, y.
    FSAPP_ROUTING_MOUSE_BUTTON,
    // Mouse motion to x, y.
    FSAPP_ROUTING_MOUSE_MOTION,
    // The wheel turned with the pointer at x, y.
    FSAPP_ROUTING_MOUSE_WHEEL,
    // The pointer entered or left the host window.
    FSAPP_ROUTING_MOUSE_ENTER,
    FSAPP_ROUTING_MOUSE_LEAVE,
} fsapp_routing_event_kind_t;

// A reduced input event.
typedef struct fsapp_routing_event {
    fsapp_routing_event_kind_t kind;
    // FSAPP_ROUTING_FOCUS_REQUEST: the surface.
    const void *surface;
    // The key, button or axis.
    int code;
    // Gamepad events: the device's instance ID (SDL_JoystickID).
    uint32_t device;
    // Pressed (else released).
    bool down;
    // FSAPP_ROUTING_KEY: the shortcut registry consumed it.
    bool shortcut;
    // FSAPP_ROUTING_MOUSE_BUTTON, _MOTION and _WHEEL: the pointer, in device pixels.
    double x;
    double y;
} fsapp_routing_event_t;

typedef enum fsapp_routing_destination {
    FSAPP_ROUTING_NOWHERE,
    // A shortcut: the registry's for a key, the top's menu input callback for the menu input.
    FSAPP_ROUTING_SHORTCUT,
    // The Amiga (fsemu's event hook).
    FSAPP_ROUTING_AMIGA,
    // The surface in the result: as an event object, through the navigation mapper for gamepads.
    // For pointer events, the pointer focus.
    FSAPP_ROUTING_SURFACE,
} fsapp_routing_destination_t;

typedef struct fsapp_routing_result {
    fsapp_routing_destination_t destination;
    // FSAPP_ROUTING_SURFACE: the surface.
    const void *surface;
    // When the key target changed: the surface that stopped being it (NULL for none, or when it has
    // left the list) and the one that became it (NULL for none), for FocusOut and FocusIn.
    const void *focus_out;
    const void *focus_in;
    // The grab's applied state changed: set relative mouse mode to fsapp_routing_is_grab_applied.
    bool grab_changed;
    // A press: the top-level to raise within its band before the press is delivered, else NULL.
    const void *raise;
    // A press outside every open popup (fsapp_routing_is_open_popup), which is consumed: each open
    // popup gets OutsideClick.
    bool outside_click;
    // When pointer focus changed: the surface that gets PointerLeave (NULL for none, or when it has
    // left the list) and the one that gets PointerEnter (NULL for none), before the event, and
    // those after the event (leave_after, enter_after), each at fsapp_routing_get_pointer_position.
    const void *leave;
    const void *enter;
    const void *leave_after;
    const void *enter_after;
} fsapp_routing_result_t;

// Creates routing with no keyboard focus and the input grab off.
fsapp_routing_t *fsapp_routing_create(void);

void fsapp_routing_destroy(fsapp_routing_t *routing);

// Routes an event in the applied state of input (only read during the call), first checking focus
// against its list, and writes where it goes and what changed to result.
void fsapp_routing_handle_event(fsapp_routing_t *routing, const fsapp_routing_input_t *input,
                                const fsapp_routing_event_t *event, fsapp_routing_result_t *result);

// The top-level of the tree with keyboard focus, or NULL.
const void *fsapp_routing_get_focus(const fsapp_routing_t *routing);

// The focused tree's key target (the surface that gets keys), as of the last event, or NULL.
const void *fsapp_routing_get_key_target(const fsapp_routing_t *routing);

// Whether the user wants the input grab (it may be suspended).
bool fsapp_routing_is_grab_wanted(const fsapp_routing_t *routing);

// Whether the input grab is applied (relative mouse mode).
bool fsapp_routing_is_grab_applied(const fsapp_routing_t *routing);

// The pointer's last position (device pixels), from the last pointer event with a position.
void fsapp_routing_get_pointer_position(const fsapp_routing_t *routing, double *x, double *y);

// Whether the surface is an open popup: a popup that is shown.
bool fsapp_routing_is_open_popup(const fsapp_routing_surface_t *surface);

// The topmost surface in the list (back to front) that takes pointer input (visible, not an
// overlay, a rectangle that is not empty) and whose rectangle contains the point (device pixels),
// or NULL.
const void *fsapp_routing_hit_test(const fsapp_routing_surface_t *surfaces, int surface_count,
                                   double x, double y);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_ROUTING_H */
