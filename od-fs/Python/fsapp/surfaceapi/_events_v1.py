"""The surface events of the surface API's version 1: what a surface's handler gets
(fsapp.surfaceevent delivers them and describes their routing)."""

from collections.abc import Callable
from dataclasses import dataclass
from enum import IntEnum

from ._enums_v1 import Keycode, Keymod, MouseButton, Navigation, Scancode


@dataclass(frozen=True)
class KeyDown:
    """A key pressed, or repeated by the host's key repeat (repeat).

    A key that SDL has no name for (a character key of a national layout, whose keycode is its
    character) has a plain int as its keycode.
    """

    scancode: Scancode | int
    keycode: Keycode | int
    modifiers: Keymod
    repeat: bool


@dataclass(frozen=True)
class KeyUp:
    """A key released (to the surface its press went to). Fields as KeyDown's; never a repeat."""

    scancode: Scancode | int
    keycode: Keycode | int
    modifiers: Keymod


@dataclass(frozen=True)
class FocusIn:
    """The surface has gained keyboard focus, or the host window has got focus back."""


@dataclass(frozen=True)
class FocusOut:
    """The surface has lost keyboard focus, or the host window has lost focus."""


class DeviceKind(IntEnum):
    """The kind of an input device."""

    KEYBOARD = 0
    GAMEPAD = 1


@dataclass(frozen=True)
class InputDevice:
    """An input device: the keyboard (all keyboards are one device, id 0), or a gamepad by its SDL
    instance ID (SDL_JoystickID), which stays the same while it is connected."""

    kind: DeviceKind
    id: int


@dataclass(frozen=True)
class NavigationEvent:
    """Navigation (value) from an input device, a repeat (repeat) of a held direction or page.

    Gamepads repeat directions and pages 300 ms after the press and then every 75 ms, keys with the
    host's key repeat; ACCEPT, BACK and MENU never repeat. MENU is the menu input (Guide, or Start
    and Select together).
    """

    value: Navigation
    device: InputDevice
    repeat: bool


@dataclass(frozen=True)
class Configure:
    """The host window's size (width, height) in the surface's buffer pixels, rounded down, and its
    surface scale (scale: how many buffer pixels to draw per design unit, fractional as it may be);
    for the host window's handler, its size in pixels and the display scale."""

    width: int
    height: int
    scale: float


@dataclass(frozen=True)
class PointerEnter:
    """The surface has got pointer focus, with the pointer at x, y (the surface's buffer pixels,
    relative to it)."""

    x: float
    y: float


@dataclass(frozen=True)
class PointerLeave:
    """The surface has lost pointer focus (for the host window's handler: the pointer has left the
    host window)."""


@dataclass(frozen=True)
class PointerMotion:
    """The pointer has moved to x, y (the surface's buffer pixels, relative to it; for the host
    window's handler, pixels relative to the host window), with the modifier keys held."""

    x: float
    y: float
    modifiers: Keymod


@dataclass(frozen=True)
class PointerButton:
    """A mouse button pressed (down) or released at x, y (the surface's buffer pixels, relative to
    it). clicks is SDL's count of quick clicks in the same place (1 for a single click, 2 for a
    double click)."""

    x: float
    y: float
    button: MouseButton | int
    down: bool
    clicks: int
    modifiers: Keymod


@dataclass(frozen=True)
class PointerWheel:
    """The wheel turned with the pointer at x, y (the surface's buffer pixels, relative to it), by
    dx, dy (fractions of a notch; positive is away from the user, or to the right), with the
    direction the user chose for the system applied."""

    x: float
    y: float
    dx: float
    dy: float
    modifiers: Keymod


@dataclass(frozen=True)
class OutsideClick:
    """For an open popup: a press outside every open popup, which no surface gets (it is consumed,
    with its release). The popup's client closes it, if it should."""


PointerEvent = PointerEnter | PointerLeave | PointerMotion | PointerButton | PointerWheel

SurfaceEvent = (
    KeyDown | KeyUp | FocusIn | FocusOut | NavigationEvent | Configure | PointerEvent | OutsideClick
)

SurfaceEventHandler = Callable[[SurfaceEvent], object]
