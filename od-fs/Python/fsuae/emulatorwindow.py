import fsemu
from fsapp.surfaceapi.v1 import Band
from fsgui.surfacewindow import SurfaceWindow


class EmulatorWindow(SurfaceWindow):
    """The emulator window: a top-level in the background band showing the emulation surface
    (created and drawn by fsemu) as its child and key target, so that focusing the window gives the
    Amiga the keys."""

    def __init__(self) -> None:
        super().__init__("Amiga display", fsemu.adopt_emulation_surface, band=Band.BACKGROUND)
