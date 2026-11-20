from fsapp.surface import SurfaceCapsule

from fsgui.borderlesswindow import BorderlessWindow
from fsgui.types import SizeParam


class SpecialWindow(BorderlessWindow):
    """A window for a surface created elsewhere (an _fsapp_surface capsule), which has no buffer
    of its own, such as the emulation surface."""

    def __init__(self, *, size: SizeParam, surface: SurfaceCapsule) -> None:
        super().__init__("", size=size, special=1, surface=surface)
