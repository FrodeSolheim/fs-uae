from collections.abc import Callable

import _fsapp

from fsgui.widget import Widget
from fsgui.window import Window


class FileDialog:
    def __init__(
        self,
        parent: Widget | None,
        *,
        on_accept: Callable,
        on_cancel: Callable | None = None,
    ):
        # on_accept, on_cancel

        self._window: Window | None = None

        if parent is not None:
            self._window = parent.get_window()
        else:
            self._window = None

        self._files = []
        self._on_accept = on_accept
        self._on_cancel = on_cancel

    def _add_file(self, path):
        """Callback from C code when a file result is added."""
        self._files.append(path)

    def get_file(self) -> str:
        assert self._files[0] is not None
        return self._files[0]

    def on_accept(self):
        if self._on_accept is not None:
            self._on_accept(self)

    def on_cancel(self):
        if self._on_cancel is not None:
            self._on_cancel(self)

    def open(self):
        # FIXME: DEPRECATED
        self.show()

    def show(self) -> None:
        if self._window is not None:
            surface = self._window._surface.get_surface()
        else:
            surface = None
        _fsapp.open_file_dialog(surface, self)
