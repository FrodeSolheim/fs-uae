"""Surface events' delivery: what fsapp delivers to a surface's handler (the events are version 1's,
fsapp.surfaceapi.v1), and the host window's handler (unstable, for the top).

The surface with keyboard focus gets the keys that do not go to a shortcut or the emulation, as
KeyDown and KeyUp, and navigation as NavigationEvent: from gamepads (the d-pad and the left stick,
the shoulders, south and east, and the menu input), and from the navigation keys after their
KeyDown (the arrows, Return and keypad Enter, Escape and Backspace, Page Up and Down, Home and End).
A surface gets FocusIn when it gains keyboard focus and FocusOut when it loses it (also when the
host window loses focus and gets it back). A surface asks for focus with
_fsapp_surface.request_focus, and may take it only when focusable (_fsapp_surface.set_focusable).

The surface with pointer focus, the topmost one taking pointer input under the pointer (not an
overlay, hidden or empty), gets PointerMotion, PointerButton and PointerWheel, with positions in
its buffer pixels relative to it, and PointerEnter and PointerLeave as the pointer focus comes and
goes (also when the pointer leaves the host window). A press keeps the pointer focus on its surface,
even outside it, until the last button is released (a drag). A press raises the surface's tree's
top-level and, when it is focusable, focuses it, before the press is delivered. While the input
grab is applied, and for the click on the Amiga and the middle button (the grab's toggle), pointer
events go to no surface. Motion is coalesced: a surface gets at most one PointerMotion between its
other events.

A visible popup (role POPUP) is open: it is its tree's key target, and while any popup is open, a
press outside every open popup (anywhere else in the host window) goes to no surface, nor does its
release, and each open popup gets OutsideClick instead.

Every surface with a handler gets Configure: the host window's size in its buffer pixels and its
surface scale, 1 for a surface created with ScaleSupport.ONE (the default) and the display scale for
one created with ScaleSupport.ANY, when the window is created and whenever either changes for it.
The host window's handler (set_window_handler) gets Configure with the host window's size in pixels
and the display scale, and WindowState after it. A handler set later gets the current ones with its
first events. The host window's handler also gets PointerMotion in host window pixels wherever the
pointer is, and PointerLeave when it leaves the host window. The events are delivered in the UI
frame, after the app events and the shortcut callbacks and before the raw SDL events, the configures
first, to the handler set with set_handler. Handlers match on the event's class, and should let
classes they do not know fall through (case _:), since more may come.
"""

import logging
from collections.abc import Callable
from dataclasses import dataclass

import _fsapp_surface

from fsapp.surfaceapi._capsule import SurfaceCapsule
from fsapp.surfaceapi._enums_v1 import Keycode, Keymod, MouseButton, Navigation, Scancode
from fsapp.surfaceapi._events_v1 import (
    Configure,
    DeviceKind,
    FocusIn,
    FocusOut,
    InputDevice,
    KeyDown,
    KeyUp,
    NavigationEvent,
    OutsideClick,
    PointerButton,
    PointerEnter,
    PointerLeave,
    PointerMotion,
    PointerWheel,
    SurfaceEvent,
    SurfaceEventHandler,
)

logger = logging.getLogger(__name__)


@dataclass(frozen=True)
class WindowState:
    """The host window is maximized, or full screen, or neither (both can be true): the host
    window's handler gets it with the current state once the host window exists, and again when
    either changes."""

    maximized: bool
    fullscreen: bool


WindowEvent = Configure | WindowState | PointerMotion | PointerLeave

WindowEventHandler = Callable[[WindowEvent], object]


def set_handler(surface: SurfaceCapsule, handler: SurfaceEventHandler | None) -> None:
    """Sets the handler of the surface's events, or None for none. It is kept until it is replaced
    or the surface is destroyed. An exception from it is logged with the surface, and delivery goes
    on."""
    _fsapp_surface.set_handler(surface, handler)


def set_window_handler(handler: WindowEventHandler | None) -> None:
    """Sets the handler of the host window's events, or None for none. It gets the current
    Configure in the next UI frame (once the host window exists). An exception from it is logged,
    and delivery goes on."""
    _fsapp_surface.set_window_handler(handler)


def _deliver_to_window_handler(handler: WindowEventHandler, event: WindowEvent) -> None:
    try:
        handler(event)
    except Exception:
        logger.exception("Error in the host window's event handler for %r", event)


def _scancode(value: int) -> Scancode | int:
    try:
        return Scancode(value)
    except ValueError:
        return value


def _keycode(value: int) -> Keycode | int:
    try:
        return Keycode(value)
    except ValueError:
        return value


def _mouse_button(value: int) -> MouseButton | int:
    try:
        return MouseButton(value)
    except ValueError:
        return value


def _device_kind(value: int) -> DeviceKind:
    if value == _fsapp_surface.DEVICE_GAMEPAD:
        return DeviceKind.GAMEPAD
    return DeviceKind.KEYBOARD


def _make_event(kind: int, fields: tuple) -> SurfaceEvent | None:
    """The event of a take_events tuple's kind and fields (_fsapp_surface.take_events)."""
    if kind == _fsapp_surface.EVENT_KEY_DOWN:
        scancode, keycode, modifiers, repeat = fields
        return KeyDown(_scancode(scancode), _keycode(keycode), Keymod(modifiers), repeat)
    if kind == _fsapp_surface.EVENT_KEY_UP:
        scancode, keycode, modifiers = fields
        return KeyUp(_scancode(scancode), _keycode(keycode), Keymod(modifiers))
    if kind == _fsapp_surface.EVENT_FOCUS_IN:
        return FocusIn()
    if kind == _fsapp_surface.EVENT_FOCUS_OUT:
        return FocusOut()
    if kind == _fsapp_surface.EVENT_NAVIGATION:
        value, device_kind, device_id, repeat = fields
        return NavigationEvent(
            Navigation(value), InputDevice(_device_kind(device_kind), device_id), repeat
        )
    if kind == _fsapp_surface.EVENT_POINTER_ENTER:
        x, y = fields
        return PointerEnter(x, y)
    if kind == _fsapp_surface.EVENT_POINTER_LEAVE:
        return PointerLeave()
    if kind == _fsapp_surface.EVENT_POINTER_MOTION:
        x, y, modifiers = fields
        return PointerMotion(x, y, Keymod(modifiers))
    if kind == _fsapp_surface.EVENT_POINTER_BUTTON:
        x, y, button, down, clicks, modifiers = fields
        return PointerButton(x, y, _mouse_button(button), down, clicks, Keymod(modifiers))
    if kind == _fsapp_surface.EVENT_POINTER_WHEEL:
        x, y, dx, dy, modifiers = fields
        return PointerWheel(x, y, dx, dy, Keymod(modifiers))
    if kind == _fsapp_surface.EVENT_OUTSIDE_CLICK:
        return OutsideClick()
    return None


def deliver() -> None:
    """Delivers the surface events queued since the last call to their handlers (fsapp._frame calls
    it in the UI frame)."""
    for handler, surface_id, width, height, scale in _fsapp_surface.take_configures():
        configure = Configure(width, height, scale)
        try:
            handler(configure)
        except Exception:
            if surface_id == 0:
                logger.exception("Error in the host window's event handler for %r", configure)
            else:
                logger.exception(
                    "Error in the event handler of surface 0x%x for %r", surface_id, configure
                )
    window_state = _fsapp_surface.take_window_state()
    if window_state is not None:
        handler, maximized, fullscreen = window_state
        _deliver_to_window_handler(handler, WindowState(maximized, fullscreen))
    for handler, surface_id, kind, fields in _fsapp_surface.take_events():
        event = _make_event(kind, fields)
        if event is None:
            continue
        try:
            handler(event)
        except Exception:
            if surface_id == 0:
                logger.exception("Error in the host window's event handler for %r", event)
            else:
                logger.exception(
                    "Error in the event handler of surface 0x%x for %r", surface_id, event
                )
