"""Version 1 of the surface API: what a client of fsapp's surfaces may rely on.

Get it with fsapp.surfaceapi.surface_api(1), the API object (SurfaceAPI), which creates surfaces
(Surface) and loads fonts (Font) and images (Image), and gives timers (Timer), UI frames and the
display scale. A surface draws with a canvas (Canvas) and gets the events defined here.
"""

import errno
import itertools
import os
import weakref
from collections.abc import Callable, Sequence
from enum import IntEnum
from typing import TypeVar

import _fsapp
import _fsapp_font
import _fsapp_image
import _fsapp_surface

from fsapp.timerservice import Timer as _ServiceTimer
from fsapp.timerservice import TimerService as _TimerService

from . import _uiframe
from ._capsule import SurfaceCapsule as _SurfaceCapsule
from ._enums_v1 import Keycode, Keymod, MouseButton, Navigation, Scancode
from ._events_v1 import (
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
    PointerEvent,
    PointerLeave,
    PointerMotion,
    PointerWheel,
    SurfaceEvent,
    SurfaceEventHandler,
)

__all__ = [
    "Band",
    "Canvas",
    "Configure",
    "DeviceKind",
    "Filter",
    "FocusIn",
    "FocusOut",
    "Font",
    "Image",
    "InputDevice",
    "KeyDown",
    "KeyUp",
    "Keycode",
    "Keymod",
    "MouseButton",
    "Navigation",
    "NavigationEvent",
    "OutsideClick",
    "PointerButton",
    "PointerEnter",
    # REVIEW: the type aliases PointerEvent, SurfaceEvent and SurfaceEventHandler are public.
    "PointerEvent",
    "PointerLeave",
    "PointerMotion",
    "PointerWheel",
    "Role",
    "ScaleSupport",
    "Scancode",
    "Surface",
    "SurfaceAPI",
    "SurfaceEvent",
    "SurfaceEventHandler",
    "Timer",
]


class Role(IntEnum):
    """A surface's role, which with its parent decides its place in the stacking order
    (_fsapp_surface.set_role; fsapp/fsapp-stacking.h has the rules)."""

    TOP_LEVEL = _fsapp_surface.ROLE_TOP_LEVEL
    CHILD = _fsapp_surface.ROLE_CHILD
    POPUP = _fsapp_surface.ROLE_POPUP
    OVERLAY = _fsapp_surface.ROLE_OVERLAY


class Band(IntEnum):
    """A top-level's band, from back to front (_fsapp_surface.set_band)."""

    BACKGROUND = _fsapp_surface.BAND_BACKGROUND
    NORMAL = _fsapp_surface.BAND_NORMAL
    PANEL = _fsapp_surface.BAND_PANEL


class ScaleSupport(IntEnum):
    """What a surface's client can draw at (_fsapp_surface.create): ONE, the default, at scale 1
    only, or ANY, at its surface scale, the display scale."""

    ONE = _fsapp_surface.SCALE_ONE
    ANY = _fsapp_surface.SCALE_ANY


class Filter(IntEnum):
    """How an image is sampled when it is drawn at another size (Canvas.draw_image)."""

    NEAREST = _fsapp_image.FILTER_NEAREST
    LINEAR = _fsapp_image.FILTER_LINEAR


_Size = tuple[int, int]
_Rectangle = tuple[int, int, int, int]


class Surface:
    """A surface: a rectangle in the host window, with a buffer that its client draws in (from
    canvas) and commits, and the events fsapp routes to it. From SurfaceAPI.create_surface,
    create_child or create_popup; its role, band and scale support are fixed when it is created.

    Positions and sizes are ints in the surface's buffer pixels, relative to the host window. A
    surface's methods are called on the UI thread; once it is destroyed, they raise RuntimeError.
    """

    _capsule: _SurfaceCapsule | None
    _size: _Size
    _scale_support: ScaleSupport
    _visible: bool
    _buffer: bool

    def __init__(self) -> None:
        raise TypeError("surfaces are created by SurfaceAPI.create_surface")

    @classmethod
    def _create(
        cls,
        size: _Size,
        handler: SurfaceEventHandler | None,
        role: Role,
        parent: "Surface | None",
        scale_support: ScaleSupport,
        buffer: bool,
    ) -> "Surface":
        capsule = _SurfaceCapsule(
            _fsapp_surface.create(size, 0 if buffer else 1, scale_support=scale_support)
        )
        if role != Role.TOP_LEVEL:
            _fsapp_surface.set_role(capsule, role, parent._live() if parent else None)
        if handler is not None:
            _fsapp_surface.set_handler(capsule, handler)
        return cls._wrap(capsule, size, scale_support, buffer)

    @classmethod
    def _wrap(
        cls, capsule: _SurfaceCapsule, size: _Size, scale_support: ScaleSupport, buffer: bool
    ) -> "Surface":
        surface = cls.__new__(cls)
        surface._capsule = capsule
        surface._size = size
        surface._scale_support = scale_support
        surface._visible = False
        surface._buffer = buffer
        return surface

    def _live(self) -> _SurfaceCapsule:
        if self._capsule is None:
            raise RuntimeError("the surface is destroyed")
        return self._capsule

    @property
    def size(self) -> _Size:
        """The surface's width and height."""
        self._live()
        return self._size

    def resize(self, width: int, height: int) -> None:
        """Resizes the surface. A surface with a buffer gets a new one at once (draw it all again),
        and its new size shows with its next commit."""
        _fsapp_surface.resize(self._live(), (width, height))
        self._size = (width, height)

    def set_position(self, x: int, y: int) -> None:
        """Places the surface in the host window (a child and a popup too: they do not follow
        their parent)."""
        _fsapp_surface.set_position(self._live(), (x, y))

    @property
    def visible(self) -> bool:
        """Whether the surface is shown (it is hidden when created)."""
        self._live()
        return self._visible

    def show(self) -> None:
        _fsapp_surface.set_visible(self._live(), True)
        self._visible = True

    def hide(self) -> None:
        _fsapp_surface.set_visible(self._live(), False)
        self._visible = False

    def raise_(self) -> None:
        """Raises the surface in front of the others of its kind: a top-level in front of the
        top-levels in its band, an overlay in front of the overlays, a child or popup in front of
        its siblings."""
        _fsapp_surface.raise_surface(self._live())

    def set_translucent(self, translucent: bool) -> None:
        """Whether the surface is blended with its content's alpha, or opaque, as by default."""
        _fsapp_surface.set_translucent(self._live(), translucent)

    def set_focusable(self, focusable: bool) -> None:
        """Whether a top-level's tree may take keyboard focus (not by default)."""
        _fsapp_surface.set_focusable(self._live(), focusable)

    def request_focus(self) -> None:
        """Asks for keyboard focus for the surface's tree; the surface that gets the keys hears it
        from FocusIn. Ignored if the tree's top-level is not focusable or not visible."""
        _fsapp_surface.request_focus(self._live())

    def set_key_target(self, surface: "Surface | None") -> None:
        """Names the surface in this top-level's tree that gets the keys while the tree has
        keyboard focus, or with None this top-level itself, as by default.

        Raises:
            TypeError: If surface is not a Surface of this version.
        """
        target = None if surface is None else _surface_capsule(surface)
        _fsapp_surface.set_key_target(self._live(), target)

    def create_child(self, size: _Size, handler: SurfaceEventHandler | None = None) -> "Surface":
        """A new child of this surface, of the size and with its scale support, hidden. A child is
        stacked with its parent, in front of it."""
        self._live()
        return Surface._create(size, handler, Role.CHILD, self, self._scale_support, True)

    def create_popup(self, size: _Size, handler: SurfaceEventHandler | None = None) -> "Surface":
        """A new popup of this surface, of the size and with its scale support, hidden. A visible
        popup is open: it is its tree's key target, and gets OutsideClick for a press outside every
        open popup."""
        self._live()
        return Surface._create(size, handler, Role.POPUP, self, self._scale_support, True)

    def commit(self, damage: Sequence[_Rectangle] | None = None) -> None:
        """Commits what was drawn inside the damaged rectangles ((x, y, width, height)), or in the
        whole surface with None: it shows with the UI frame."""
        _fsapp_surface.commit(self._live(), None if damage is None else list(damage))

    def canvas(self) -> "Canvas":
        """A new canvas for drawing in the surface's buffer, with origin 0, 0 and the whole buffer
        as its clip (take a new one after a resize).

        Raises:
            RuntimeError: If the surface has no buffer.
        """
        self._live()
        if not self._buffer:
            # REVIEW: RuntimeError for the canvas of a surface without a buffer.
            raise RuntimeError("the surface has no buffer")
        return Canvas._create(self)

    def destroy(self) -> None:
        """Destroys the surface: it disappears with the UI frame."""
        _fsapp_surface.destroy(self._live())
        self._capsule = None


_T = TypeVar("_T")


def _checked(value: object, cls: type[_T], what: str) -> _T:
    if not isinstance(value, cls):
        raise TypeError(f"not {what} of version 1: {value!r}")
    return value


def _surface_capsule(surface: Surface) -> _SurfaceCapsule:
    return _checked(surface, Surface, "a surface")._live()


_Colour = tuple[int, int, int, int]


class Font:
    """A font at a size in pixels, from SurfaceAPI.load_font."""

    _capsule: object

    def __init__(self) -> None:
        raise TypeError("fonts are loaded by SurfaceAPI.load_font")

    @classmethod
    def _create(cls, capsule: object) -> "Font":
        font = cls.__new__(cls)
        font._capsule = capsule
        return font

    def measure(self, text: str) -> _Size:
        """The width and height of the text on one line, in pixels."""
        return _fsapp_font.measure_text(self._capsule, text)

    def measure_wrapped(self, text: str, wrap_width: int) -> _Size:
        """The width and height of the text wrapped at wrap_width pixels."""
        return _fsapp_font.measure_text_wrapped(self._capsule, text, wrap_width)


class Image:
    """An image, from SurfaceAPI.load_image or load_svg."""

    _capsule: object

    def __init__(self) -> None:
        raise TypeError("images are loaded by SurfaceAPI.load_image")

    @classmethod
    def _create(cls, capsule: object) -> "Image":
        image = cls.__new__(cls)
        image._capsule = capsule
        return image

    @property
    def size(self) -> _Size:
        """The image's width and height in pixels."""
        return _fsapp_image.get_size(self._capsule)

    def pixels(self) -> memoryview:
        """A read-only copy of the image's pixels: premultiplied BGRA (bytes B, G, R, A), rows of
        width * 4 bytes from the top."""
        return memoryview(_fsapp_image.get_pixels(self._capsule))

    def tinted(self, colour: _Colour) -> "Image":
        """A new image of the same shape in the colour: every pixel that is not transparent gets
        the colour, its alpha multiplied by the colour's alpha. The image is unchanged."""
        return Image._create(_fsapp_image.create_tinted(self._capsule, colour))


def _font_capsule(font: Font) -> object:
    return _checked(font, Font, "a font")._capsule


def _image_capsule(image: Image) -> object:
    return _checked(image, Image, "an image")._capsule


def _intersect(a: _Rectangle, b: _Rectangle) -> _Rectangle:
    left = max(a[0], b[0])
    top = max(a[1], b[1])
    right = min(a[0] + a[2], b[0] + b[2])
    bottom = min(a[1] + a[3], b[1] + b[3])
    return left, top, max(0, right - left), max(0, bottom - top)


class Canvas:
    """Draws in a surface's buffer, from Surface.canvas: shapes, text and images, in the buffer's
    pixels relative to the canvas's origin, within its clip, or directly in the buffer's bytes
    (buffer). Colours are straight RGBA tuples, and nothing is antialiased. What is drawn shows
    once the surface is committed.

    The clip is a stack: push_clip intersects a rectangle with the current clip, pop_clip restores
    the one before. A draw on the canvas of a destroyed surface raises RuntimeError.
    """

    _surface: Surface
    _origin: tuple[int, int]
    # The clip stack in buffer pixels, the whole buffer at the bottom.
    _clips: list[_Rectangle]

    def __init__(self) -> None:
        raise TypeError("canvases are created by Surface.canvas")

    @classmethod
    def _create(cls, surface: Surface) -> "Canvas":
        canvas = cls.__new__(cls)
        canvas._surface = surface
        canvas._origin = (0, 0)
        width, height = surface.size
        canvas._clips = [(0, 0, width, height)]
        return canvas

    @property
    def origin(self) -> tuple[int, int]:
        """The buffer position that the canvas's 0, 0 is at, added to every position."""
        return self._origin

    @origin.setter
    def origin(self, origin: tuple[int, int]) -> None:
        self._origin = origin

    # REVIEW: the clip property makes the clip stack observable.
    @property
    def clip(self) -> _Rectangle:
        """The current clip (x, y, width, height) in canvas coordinates."""
        x, y, width, height = self._clips[-1]
        return x - self._origin[0], y - self._origin[1], width, height

    def push_clip(self, x: int, y: int, width: int, height: int) -> None:
        """Makes the rectangle's intersection with the current clip the clip, until pop_clip."""
        rectangle = (x + self._origin[0], y + self._origin[1], width, height)
        self._clips.append(_intersect(self._clips[-1], rectangle))

    def pop_clip(self) -> None:
        """Restores the clip from before the last push_clip.

        Raises:
            RuntimeError: Without a push_clip to undo.
        """
        if len(self._clips) == 1:
            # REVIEW: RuntimeError for an unbalanced pop_clip.
            raise RuntimeError("pop_clip without push_clip")
        self._clips.pop()

    def _target(self) -> _SurfaceCapsule:
        return self._surface._live()

    def _point(self, x: int, y: int) -> tuple[int, int]:
        return x + self._origin[0], y + self._origin[1]

    def fill_rectangle(self, x: int, y: int, width: int, height: int, colour: _Colour) -> None:
        _fsapp_surface.draw_filled_rectangle(
            self._target(), self._point(x, y), (width, height), colour, self._clips[-1]
        )

    def draw_rectangle(
        self, x: int, y: int, width: int, height: int, colour: _Colour, line_width: int = 1
    ) -> None:
        """Draws the rectangle's outline, line_width pixels wide, inside the rectangle."""
        _fsapp_surface.draw_rectangle(
            self._target(), self._point(x, y), (width, height), colour, line_width, self._clips[-1]
        )

    def draw_line(
        self, x1: int, y1: int, x2: int, y2: int, colour: _Colour, line_width: int = 1
    ) -> None:
        _fsapp_surface.draw_line(
            self._target(),
            self._point(x1, y1),
            self._point(x2, y2),
            colour,
            line_width,
            self._clips[-1],
        )

    def draw_circle(
        self, cx: int, cy: int, radius: int, colour: _Colour, line_width: int = 1
    ) -> None:
        """Draws a circle's outline, line_width pixels wide."""
        _fsapp_surface.draw_circle(
            self._target(), self._point(cx, cy), radius, colour, line_width, self._clips[-1]
        )

    def fill_circle(self, cx: int, cy: int, radius: int, colour: _Colour) -> None:
        _fsapp_surface.draw_filled_circle(
            self._target(), self._point(cx, cy), radius, colour, self._clips[-1]
        )

    def draw_text(self, font: Font, text: str, x: int, y: int, colour: _Colour) -> None:
        """Draws the text on one line with its top left at x, y."""
        capsule = _font_capsule(font)
        _fsapp_font.draw_text(
            capsule, self._target(), text, self._point(x, y), colour, self._clips[-1]
        )

    def draw_text_wrapped(
        self, font: Font, text: str, x: int, y: int, wrap_width: int, colour: _Colour
    ) -> None:
        """Draws the text wrapped at wrap_width pixels with its top left at x, y."""
        capsule = _font_capsule(font)
        _fsapp_font.draw_text_wrapped(
            capsule, self._target(), text, self._point(x, y), wrap_width, colour, self._clips[-1]
        )

    def draw_image(
        self,
        image: Image,
        x: int,
        y: int,
        *,
        width: int | None = None,
        height: int | None = None,
        source: _Rectangle | None = None,
        filter: Filter = Filter.LINEAR,
    ) -> None:
        """Draws the source rectangle of the image (x, y, width, height; the whole image by
        default) into the rectangle at x, y of the width and height (the source's by default),
        scaled with the filter when the sizes differ."""
        capsule = _image_capsule(image)
        target = self._target()
        if source is None:
            source = (0, 0, *image.size)
        destination = (
            *self._point(x, y),
            source[2] if width is None else width,
            source[3] if height is None else height,
        )
        _fsapp_image.draw_part(capsule, target, source, destination, filter, self._clips[-1])

    def buffer(self) -> memoryview:
        """The surface's buffer, writable: premultiplied BGRA (bytes B, G, R, A), rows of
        width * 4 bytes from the top, in buffer pixels (the origin and the clip do not apply).

        It is valid until the end of the UI frame: then it is released, and using it, or making a
        new view from its obj, raises ValueError. A slice or cast of it shares its memory and is
        not released with it: its memory stays valid (the buffer is kept alive), but writing
        through it after the UI frame is an error, and what the surface shows of it is undefined.
        """
        return _fsapp_surface.get_buffer(self._target())


class Timer:
    """A function that runs once on the UI thread after a delay, from SurfaceAPI.call_later."""

    _timer: _ServiceTimer

    def __init__(self) -> None:
        raise TypeError("timers are created by SurfaceAPI.call_later")

    @classmethod
    def _create(cls, timer: _ServiceTimer) -> "Timer":
        handle = cls.__new__(cls)
        handle._timer = timer
        return handle


_api_numbers = itertools.count()


class SurfaceAPI:
    """A client's entry to version 1 of the surface API, from fsapp.surfaceapi.surface_api(1): it
    creates surfaces, loads fonts and images, and gives timers, UI frames and the display scale.
    Its methods are called on the UI thread, except request_ui_frame."""

    def __init__(self) -> None:
        self._number = next(_api_numbers)
        # Its UI frame callback goes with it.
        weakref.finalize(self, _uiframe.set_callback, self._number, None)

    def create_surface(
        self,
        size: _Size,
        handler: SurfaceEventHandler | None = None,
        *,
        role: Role = Role.TOP_LEVEL,
        band: Band = Band.NORMAL,
        scale_support: ScaleSupport = ScaleSupport.ONE,
        buffer: bool = True,
    ) -> Surface:
        """A new top-level or overlay surface of the size, hidden and not focusable, with handler
        getting its events (Configure first). band is a top-level's (an overlay ignores it).
        buffer=False makes a surface without a buffer: a rectangle with nothing to draw.

        Raises:
            ValueError: If role is not TOP_LEVEL or OVERLAY.
        """
        if role not in (Role.TOP_LEVEL, Role.OVERLAY):
            raise ValueError(f"a surface is created as a top-level or an overlay, not {role!r}")
        # REVIEW: buffer=False is for a top-level whose content is another surface (the emulator
        # window's); whether plugins need it.
        surface = Surface._create(size, handler, role, None, scale_support, buffer)
        if band != Band.NORMAL:
            _fsapp_surface.set_band(surface._live(), band)
        return surface

    def load_font(self, path: str, size_px: int) -> Font:
        """The TrueType font file at the path, at the size in pixels.

        Raises:
            FileNotFoundError: If there is no file at the path.
            OSError: If the font cannot be loaded.
        """
        return Font._create(_load(path, "the font", lambda: _fsapp_font.load(path, size_px)))

    def load_image(self, path: str) -> Image:
        """The image file at the path (PNG, JPEG and the other formats SDL_image reads).

        Raises:
            FileNotFoundError: If there is no file at the path.
            OSError: If the image cannot be loaded.
        """
        return Image._create(_load(path, "the image", lambda: _fsapp_image.load(path)))

    def load_svg(self, path: str, size: _Size) -> Image:
        """The SVG file at the path, rendered at the size (width, height) in pixels.

        Raises:
            FileNotFoundError: If there is no file at the path.
            OSError: If the image cannot be loaded.
        """
        return Image._create(
            _load(path, "the SVG image", lambda: _fsapp_image.load_sized_svg(path, *size))
        )

    def call_later(self, seconds: float, callback: Callable[[], object]) -> Timer:
        """Calls callback once on the UI thread after the delay, at the start of the first UI frame
        after it (or at the end of the UI frame that set it, for a delay of 0). An exception in it
        is logged."""
        return Timer._create(_TimerService.get().call_after(seconds, callback))

    def cancel(self, timer: Timer) -> None:
        """Cancels the timer, so that its function is not called (if it has not been already).

        Raises:
            TypeError: If timer is not a Timer of this version.
        """
        _checked(timer, Timer, "a timer")._timer.cancel()

    def request_ui_frame(self) -> None:
        """Requests a UI frame at the next host window frame, for animation: the UI thread sleeps
        between UI frames until something wakes it (events, a due timer or this request).
        Callable from any thread."""
        _fsapp.request_ui_frame()

    def on_ui_frame(self, callback: Callable[[], object] | None) -> None:
        """Sets the API object's UI frame callback, or with None removes it. It runs every UI
        frame, after the events are delivered and the due timers have run, the API objects'
        callbacks in the order the API objects were created, until the API object is gone. An
        exception in it is logged."""
        _uiframe.set_callback(self._number, callback)

    @property
    def display_scale(self) -> float:
        """The display scale now: how many pixels the UI draws per design unit. A surface with
        ScaleSupport.ANY gets its surface scale, the display scale, in every Configure."""
        return _fsapp.get_display_scale()


def _load(path: str, what: str, load: Callable[[], object]) -> object:
    """The capsule from load, FileNotFoundError without a file at the path, or OSError when load
    fails."""
    if not os.path.isfile(path):
        raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), path)
    try:
        return load()
    except RuntimeError as e:
        raise OSError(f"cannot load {what} {path}") from e
