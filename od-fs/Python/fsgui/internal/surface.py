from fsapp.surfaceapi.v1 import Canvas
from fsapp.surfaceapi.v1 import Surface as SurfaceHandle

from ..types import Size
from .id import get_next_id


class Surface:
    """A window's surface: its version 1 handle (handle), and the position fsgui last placed it
    at."""

    def __init__(self, handle: SurfaceHandle) -> None:
        self.handle = handle
        self._position = 0, 0
        self.id = get_next_id()

    def destroy(self) -> None:
        self.handle.destroy()

    def get_size(self) -> Size:
        return self.handle.size

    def resize(self, size: tuple[int, int]) -> None:
        self.handle.resize(*size)

    def canvas(self) -> Canvas:
        """A new canvas for drawing in the surface's buffer (take a new one after a resize)."""
        return self.handle.canvas()

    def commit(self) -> None:
        """Commits what was drawn in the surface (the whole surface): it shows with the UI frame."""
        self.handle.commit()

    def set_position(self, position: tuple[int, int]) -> None:
        self._position = position
        self.handle.set_position(*position)

    def set_visible(self, visible: bool) -> None:
        if visible:
            self.handle.show()
        else:
            self.handle.hide()

    def set_focusable(self, focusable: bool) -> None:
        """Whether the surface may take keyboard focus."""
        self.handle.set_focusable(focusable)

    def request_focus(self) -> None:
        """Asks fsapp for keyboard focus: the window hears whether it got it from FocusIn."""
        self.handle.request_focus()

    def set_key_target(self, surface: SurfaceHandle | None) -> None:
        """Names the surface in this top-level's tree that gets the keys while the tree has
        keyboard focus, or with None this surface itself."""
        self.handle.set_key_target(surface)

    def raise_surface(self) -> None:
        """Asks fsapp to raise the surface in front of the others in its band (or of its kind)."""
        self.handle.raise_()
