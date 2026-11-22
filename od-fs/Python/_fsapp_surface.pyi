# _fsapp_surface.pyi
"""Typing stubs for the `_fsapp_surface` C extension (fsapp/fsapp-surface.h).

A surface's setters (and resize) change its client state, which goes to the compositor with the
record of the UI frame they are called in, together with everything else the UI frame changed. What
is drawn in a surface's buffer shows only once the surface is committed, and a new size with the
content committed for it. Call them on the UI thread, inside a UI frame.

Positions, sizes, damage and pointer positions are in the surface's buffer pixels. A surface
created with SCALE_ANY works in pixels of the host window's drawable (device pixels); one with
SCALE_ONE in pixels that fsapp scales by the display scale (at 1.5, a 200 x 100 surface at 10, 10
covers 300 x 150 device pixels at 15, 15).

The drawing functions (and those of _fsapp_font and _fsapp_image) take a clip rectangle last,
(x, y, width, height) in buffer pixels, or None (or left out) for the whole buffer: they draw only
inside it.
"""

from collections.abc import Callable, Sequence
from typing import Final

def create(size: tuple[int, int], special: int, /, scale_support: int = ...) -> object:
    """Creates a surface of the size, without a buffer when special is not 0 (its content then
    comes from elsewhere, or there is none). Its scale support (SCALE_ONE, the default, or
    SCALE_ANY; fsapp.surfaceapi.v1.ScaleSupport) is what its client can draw at, which its Configure
    follows; it is set here only.

    Raises:
        ValueError: If scale_support is not one of the SCALE_ constants.
    """

def commit(surface: object, damage: Sequence[tuple[int, int, int, int]] | None = None) -> None:
    """Commits what was drawn in the surface inside the damaged rectangles ((x, y, width, height)
    in surface pixels), or in the whole surface with none (or an empty sequence): it shows with the
    UI frame's record. What is drawn outside the damage shows only when a later commit's damage
    covers it. The rectangles are clipped to the surface and empty ones dropped; several commits in
    one UI frame are combined, many rectangles merged into their bounding box. A surface without a
    buffer has nothing to commit."""

def resize(surface: object, size: tuple[int, int]) -> None:
    """Resizes the surface. A surface with a buffer gets a new one at once (draw it all again), and
    its new size shows with its next commit, which counts as damage to the whole surface (a commit
    before the resize in the same UI frame is dropped); until then its old content shows at the old
    size. A surface without a buffer changes size with the UI frame's record."""

def destroy(surface: object) -> None:
    """Destroys the surface: its buffer is freed at once, and it disappears with the UI frame's
    record, together with the UI frame's other changes. Freeing the capsule does the same."""

def set_position(surface: object, position: tuple[int, int], /) -> None:
    """Sets the surface's position in the host window, in its buffer pixels (a child's and a
    popup's too: they do not follow their parent)."""

def set_role(surface: object, role: int, parent: object | None = None, /) -> None:
    """Sets the surface's role (one of the ROLE_ constants, fsapp.surfaceapi.v1.Role) and, for a
    child or popup, its parent, which decide its place in the stacking order. A top-level by
    default. A child or popup takes a parent and a top-level or an overlay none: a call that breaks
    this, or names the surface itself, one of its descendants or a destroyed surface as the parent,
    is logged as an error and ignored.

    Raises:
        ValueError: If role is not one of the ROLE_ constants.
    """

def set_band(surface: object, band: int, /) -> None:
    """Sets a top-level's band (one of the BAND_ constants, fsapp.surfaceapi.v1.Band), normal by
    default."""

def set_key_target(top_level: object, surface: object | None, /) -> None:
    """Names the surface in a top-level's tree that gets the keys while the tree has keyboard
    focus (its key target), or with None the top-level itself, as by default. While the key target
    is hidden or out of the tree, the top-level gets the keys. A call for a surface that is not a
    top-level, or naming a surface outside its tree or destroyed, is logged as an error and
    ignored. Unstable, for the top."""

def raise_surface(surface: object, /) -> None:
    """Raises the surface in front of the others of its kind: a top-level in front of the
    top-levels in its band, an overlay in front of the overlays, a child or popup in front of its
    siblings. The request goes with the UI frame's record, applied after its other changes."""

def set_translucent(surface: object, translucent: bool, /) -> None:
    """Sets whether the surface is translucent (blended with its content's alpha) or opaque, as by
    default."""

def set_visible(surface: object, visible: bool, /) -> None:
    """Shows or hides the surface (hidden when created)."""

def set_focusable(surface: object, focusable: bool, /) -> None:
    """Sets whether a top-level's tree may take keyboard focus (not by default). Goes with the UI
    frame's record; a focused tree whose top-level is made unfocusable loses focus, which falls
    back to the topmost focusable top-level."""

def request_focus(surface: object, /) -> None:
    """Asks for keyboard focus for the surface's tree. The request goes with the UI frame's record
    and is applied after the record's other changes, in order with the UI frame's other requests;
    it is ignored if the tree's top-level is not focusable or not visible then. It does not change
    the tree's key target. The surfaces that stop and start being the key target get FocusOut and
    FocusIn (fsapp.surfaceapi.v1)."""

def set_handler(surface: object, handler: Callable[..., object] | None, /) -> None:
    """Sets the callable that gets the surface's events (fsapp.surfaceevent.set_handler wraps it),
    or None for none. It is kept until it is replaced or the surface is destroyed.

    Raises:
        TypeError: If the handler is not callable.
        ValueError: If the surface is destroyed.
    """

def get_id(surface: object, /) -> int:
    """The surface's id (its address), as take_events names it."""

EVENT_KEY_DOWN: Final[int]
EVENT_KEY_UP: Final[int]
EVENT_FOCUS_IN: Final[int]
EVENT_FOCUS_OUT: Final[int]
EVENT_NAVIGATION: Final[int]
EVENT_POINTER_ENTER: Final[int]
EVENT_POINTER_LEAVE: Final[int]
EVENT_POINTER_MOTION: Final[int]
EVENT_POINTER_BUTTON: Final[int]
EVENT_POINTER_WHEEL: Final[int]
EVENT_OUTSIDE_CLICK: Final[int]
DEVICE_KEYBOARD: Final[int]
DEVICE_GAMEPAD: Final[int]
ROLE_TOP_LEVEL: Final[int]
ROLE_CHILD: Final[int]
ROLE_POPUP: Final[int]
ROLE_OVERLAY: Final[int]
BAND_BACKGROUND: Final[int]
BAND_NORMAL: Final[int]
BAND_PANEL: Final[int]
SCALE_ONE: Final[int]
SCALE_ANY: Final[int]

_Clip = tuple[int, int, int, int] | None

def draw_filled_rectangle(
    surface: object,
    position: tuple[int, int],
    size: tuple[int, int],
    colour: tuple[int, int, int, int],
    clip: _Clip = None,
    /,
) -> None:
    """Fills a rectangle of the surface's buffer with a colour (straight RGBA). It shows once
    committed."""

def draw_rectangle(
    surface: object,
    position: tuple[int, int],
    size: tuple[int, int],
    colour: tuple[int, int, int, int],
    width: int,
    clip: _Clip = None,
    /,
) -> None:
    """Draws a rectangle's outline of the width (pixels) inside the rectangle in the surface's
    buffer (all of it when it is that narrow). It shows once committed."""

def draw_line(
    surface: object,
    start: tuple[int, int],
    end: tuple[int, int],
    colour: tuple[int, int, int, int],
    width: int,
    clip: _Clip = None,
    /,
) -> None:
    """Draws a line of the width (pixels) in the surface's buffer. It shows once committed."""

def draw_circle(
    surface: object,
    center: tuple[int, int],
    radius: int,
    colour: tuple[int, int, int, int],
    width: int,
    clip: _Clip = None,
    /,
) -> None:
    """Draws a circle's outline of the width (pixels) in the surface's buffer. It shows once
    committed."""

def draw_filled_circle(
    surface: object,
    center: tuple[int, int],
    radius: int,
    colour: tuple[int, int, int, int],
    clip: _Clip = None,
    /,
) -> None:
    """Draws a filled circle in the surface's buffer. It shows once committed."""

def get_buffer(surface: object, /) -> memoryview:
    """The surface's buffer as a writable memoryview: premultiplied, bytes B, G, R, A, rows of
    width * 4 bytes. It is released at the end of the UI frame (using it then raises ValueError);
    it shows once committed.

    Raises:
        ValueError: If the surface has no buffer.
    """

def set_window_handler(handler: Callable[..., object] | None, /) -> None:
    """Sets the callable that gets the host window's events (configures, and pointer motion and
    leave in take_events; fsapp.surfaceevent.set_window_handler wraps it), or None for none.

    Raises:
        TypeError: If the handler is not callable.
    """

def take_configures() -> list[tuple[Callable[..., object], int, int, int, float]]:
    """Takes the configures due, for fsapp.surfaceevent, as tuples (handler, surface id, width,
    height, scale), with 0 as the id of the host window's handler: for each surface with a handler,
    the host window's size in its buffer pixels and its surface scale, and for the host window's
    handler, the host window's size in pixels and the display scale, each when it differs from the
    last it got."""

def take_window_state() -> tuple[Callable[..., object], bool, bool] | None:
    """Takes the host window's state for its handler, for fsapp.surfaceevent, as a tuple (handler,
    maximized, fullscreen) when it differs from the last the handler got (or the handler has got
    none), else None."""

def take_events() -> list[tuple[Callable[..., object], int, int, tuple]]:
    """Takes the surface events queued since the last call, for fsapp.surfaceevent, as tuples
    (handler, surface id, kind, fields), kind one of the EVENT_ constants and surface id 0 for the
    host window's handler. The fields: (scancode, keycode, modifiers, repeat) for EVENT_KEY_DOWN,
    (scancode, keycode, modifiers) for EVENT_KEY_UP, (value, device kind, device id, repeat) for
    EVENT_NAVIGATION (the value an fsapp.surfaceapi.v1.Navigation, the kind one of the DEVICE_
    constants, the id an SDL instance ID, 0 for the keyboard), (x, y) for EVENT_POINTER_ENTER,
    (x, y, modifiers) for EVENT_POINTER_MOTION, (x, y, button, down, clicks, modifiers) for
    EVENT_POINTER_BUTTON, (x, y, dx, dy, modifiers) for EVENT_POINTER_WHEEL (positions in the
    surface's buffer pixels relative to it, or in pixels relative to the host window for its
    handler), and () for the others. Events for surfaces without a handler or destroyed are left
    out."""
