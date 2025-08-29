# New stable API for low-level GUI?

import _fsapp_surface

# Maybe app surfaces get mouse events / key events (etc)


class AppSurface:
    def __init__(self, size: tuple[int, int], special: int | None = None):
        # The underlying C surface
        self._surface = _fsapp_surface.create(size, special)

        self._z_index = 0
        self._transparency = False

    def got_focus(self) -> None:
        pass

    def lost_focus(self) -> None:
        pass

    def mouse_moved(self) -> None:
        pass

    def key_pressed(self) -> None:
        pass

    def key_released(self) -> None:
        pass

    # ---------------------

    def grab_mouse(self) -> None:
        pass

    def ungrab_mouse(self) -> None:
        pass
