from collections.abc import Callable

from fsapp.surfaceapi.v1 import Band
from fsapp.surfaceapi.v1 import Surface as SurfaceHandle

from .borderlesswindow import BorderlessWindow
from .types import PixelPosition


class SurfaceWindow(BorderlessWindow):
    """A window without a buffer that shows a surface drawn elsewhere (such as the emulation
    surface) as its child, filling it, and as its key target, so that focusing the window gives
    that surface the keys. adopt is given the window's surface and returns the content surface, a
    child of it."""

    def __init__(
        self,
        title: str,
        adopt: Callable[[SurfaceHandle], SurfaceHandle],
        *,
        band: Band = Band.NORMAL,
    ) -> None:
        super().__init__(title, band=band, buffer=False)
        self._content = adopt(self.surface.handle)
        self._content.resize(*self.get_surface_size())
        self.surface.set_key_target(self._content)
        self._content.show()

    def on_paint(self) -> None:
        # No buffer to paint in.
        pass

    def place_surfaces(self, position: PixelPosition) -> None:
        super().place_surfaces(position)
        self._content.set_position(*position)
        size = self.get_surface_size()
        if self._content.size != size:
            self._content.resize(*size)
