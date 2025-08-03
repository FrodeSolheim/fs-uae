import logging

import fsapp  # type: ignore

from fsgui.internal.flags import Flags
from fsgui.maintitlebar import MainTitleBar
from fsgui.types import Size, TColour
from fsgui.window import Window

logger = logging.getLogger(__name__)


class MainWindow:
    _instance: "MainWindow"

    @classmethod
    def get(cls):
        # print("MainWindow.get", cls._instance)
        # if cls._instance is None:
        #     cls._instance = MainWindow()
        # return cls._instance
        return cls._instance

    @classmethod
    def set(cls, instance: "MainWindow"):
        # print("MainWindow.set", instance)
        cls._instance = instance

    def __init__(self, title: str, size: Size, *, fullscreen: bool = False):
        self._top_border = 36
        self._right_border = 0
        self._bottom_border = 0
        self._left_border = 0

        self._real_width = size[0]
        self._real_height = size[1] + self._top_border

        self._width = size[0]
        self._height = size[1]

        real_size = (self._real_width, self._real_height)

        self._background_colour = (0, 0, 0, 255)

        self._window = fsapp.create_window(title, real_size, fullscreen)

        MainWindow.set(self)

        self._maximized = False
        self._fullscreen = False
        # self._size = size

        main_titlebar_height = self._top_border
        self.title_bar = MainTitleBar(
            title, size=(size[0], main_titlebar_height)
        )
        self.title_bar.set_layer(Window.LAYER_HIGHEST)
        self.title_bar.show()
        self.title_bar.set_position((0, -self._top_border))

    def set_background_colour(self, colour: TColour):
        self._background_colour = colour
        fsapp.set_window_background_color(self._window, colour)

    def get_size(self) -> Size:
        return (self._width, self._height)

    def get_window_size(self) -> Size:
        return (self._real_width, self._real_height)

    @property
    def fullscreen(self) -> bool:
        return self._fullscreen

    def on_fullscreen(self) -> None:
        print("on_fullscreen")
        self.title_bar.set_visible(not self.fullscreen)

    @property
    def height(self) -> int:
        return self._height

    def is_maximized(self):
        return self._maximized

    def on_maximize(self) -> None:
        pass

    def on_resize(self) -> None:
        pass

    # def set_is_maximized(self, maximized):
    #     self._maximized = maximized

    # def set_is_minimized(self, minimized):
    #     self._minimized = minimized

    def _resize(self, size: Size) -> None:
        logger.debug("MainWindow._resize %r", size)
        self.title_bar.set_width(size[0])
        # FIXME: This needs to call MainTitleBar.on_resize somehow...
        self.on_resize()

    def _update_size(self, window_size: Size) -> None:
        # Must update _size before calling _resize()
        self._real_width = window_size[0]
        self._real_height = window_size[1]

        self._width = window_size[0]
        self._height = window_size[1] - self._top_border

        self._resize(window_size)

    def update_state(self, window_size: Size, sdl_window_flags):
        # Must update fullscreen state before updating size, hmm, actually...
        # Well, order is important - resize depends on knowing the correct
        # border size, which is determined by fullscreen mode.

        fullscreen = sdl_window_flags & Flags.SDL_WINDOW_FULLSCREEN
        if fullscreen != self._fullscreen:
            self._fullscreen = fullscreen
            self._top_border = 0 if self.fullscreen else self.title_bar.height

            # FIXME: on_enter_fullscreen, on_leave_fullscreen ?
            self.on_fullscreen()

        if window_size != self.get_window_size():
            self._update_size(window_size)
        maximized = sdl_window_flags & Flags.SDL_WINDOW_MAXIMIZED
        if maximized != self._maximized:
            self._maximized = maximized

    @property
    def width(self) -> int:
        return self._width
