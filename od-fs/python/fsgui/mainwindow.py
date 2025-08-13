import logging

import _fsapp

from fsgui.internal.flags import Flags
from fsgui.mainwindowborder import MainWindowBorder
from fsgui.mainwindowtitlebar import MainWindowTitleBar
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

    def __init__(self, title: str, size: Size, *, fullscreen: bool = False, extra_title: str=""):
        self._top_border = 36

        self.show_extra_borders = True
        self.extra_title = extra_title

        # FIXME: Possible some duplicate code with window here, maybe have a WindowDecoration
        # helper class

        # MainWindow cannot draw decorations itself sing the normal surface drawing code, because
        # there is no backing surface (on purpose, to conserve resources). So we need to either
        # create separate surposes for the border parts when we want custom borders, or some kind
        # of API to instruct the C renderer how to draw the border.

        # self.frame = MainWindowDecorations()

        self.border = MainWindowBorder()

        main_titlebar_height = self._top_border
        self.title_bar = MainWindowTitleBar(
            title, size=(size[0], main_titlebar_height), extra_title=extra_title
        )
        # FIXME: Must affect Z layering (does not do so now)
        self.title_bar.set_layer(Window.LAYER_HIGHEST)
        self.title_bar.show()
        self.border.title_bar = self.title_bar

        self._real_width = size[0] + self.border.left + self.border.right
        self._real_height = size[1] + self.border.top + self.border.bottom

        print("FS-UAE real size", self._real_width, self._real_height)

        self._width = size[0]
        self._height = size[1]

        real_size = (self._real_width, self._real_height)

        self._background_colour = (0, 0, 0, 255)

        self._window = _fsapp.create_window(title, real_size, fullscreen)

        MainWindow.set(self)

        self._maximized = False
        self._fullscreen = fullscreen

        # self._size = size

        # Account for all windows being offset by origin; origin starts below title bar
        # self.title_bar.set_position((0, -self._top_border))

        # self.border._resize_border(self._fullscreen, real_size)
        # self.border._fullscreen = fullscreen
        # self.border._resize_border(fullscreen, real_size)
        # self.border._resize_border(self._fullscreen, self._maximized, real_size)
        self._update_border()

    def set_background_colour(self, colour: TColour):
        self._background_colour = colour
        _fsapp.set_window_background_color(self._window, colour)

    def get_size(self) -> Size:
        return (self._width, self._height)

    def get_window_size(self) -> Size:
        return (self._real_width, self._real_height)

    @property
    def fullscreen(self) -> bool:
        return self._fullscreen

    def on_fullscreen(self) -> None:
        # FIXME: IMPORTANT! size is incorrect, must be updated first ?!?
        print("on_fullscreen size =", self.width, self.height)
        self.title_bar.set_visible(not self.fullscreen)
        # if self.fullscreen:
        #     self.title_bar.set_y(0)
        #     self.title_bar.set_width(self.width)
        #     # self.title_bar.set_width(1920)

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

    def _update_border(self):
        self.border._resize_border(
            self._fullscreen, self._maximized, (self._real_width, self._real_height)
        )

    def _resize(self, size: Size) -> None:
        logger.debug("MainWindow._resize %r", size)
        # self.border._resize_border(self._fullscreen, self._maximized, size)
        # self.border._resize_border(size)
        self._update_border()

        # FIXME: This needs to call MainTitleBar.on_resize somehow...
        self.on_resize()

    def _update_size(self, window_size: Size) -> None:
        # Must update _size before calling _resize()
        self._real_width = window_size[0]
        self._real_height = window_size[1]

        # self._width = window_size[0]
        # self._height = window_size[1] - self._top_border

        self._width = window_size[0] - self.border.left - self.border.right
        self._height = window_size[1] - self.border.top - self.border.bottom

        self._resize(window_size)

    def update_size(self, window_size: Size) -> None:
        if window_size != self.get_window_size():
            self._update_size(window_size)

    def update_maximized(self, maximized: bool) -> None:
        if maximized != self._maximized:
            self._maximized = maximized
            # FIXME: Can we remove this later by making sure the border's resize method is
            # called sufficiently early?
            # self.border._maximized = self._maximized
            self._update_border()

    def update_fullscreen(self, fullscreen: bool) -> None:
        # The real size may not have changed, but the client certain will, if the title bar is
        # toggled, so we need to call _update_size here.
        if fullscreen != self._fullscreen:
            # The order of operations are imporant, first the _fullscreen property must be set
            # so _update_order works correctly.
            self._fullscreen = fullscreen
            # Then we must call _update_border, so client size calculations are correct afterwards
            self._update_border()
            # And then we can update the client size
            self._update_size((self._real_width, self._real_height))

            # self._top_border = 0 if self.fullscreen else self.title_bar.height

            # FIXME: Can we remove this later by making sure the border's resize method is
            # called sufficiently early?
            # self.border._fullscreen = self._fullscreen
            # self.border._resize_border()
            # self._update_border()

            # FIXME: on_enter_fullscreen, on_leave_fullscreen ?
            self.on_fullscreen()

    # # FIXME: SOON UNUSED / DEPRECATED
    # def update_state(self, window_size: Size, sdl_window_flags):
    #     # Must update fullscreen state before updating size, hmm, actually...
    #     # Well, order is important - resize depends on knowing the correct
    #     # border size, which is determined by fullscreen mode.

    #     fullscreen = sdl_window_flags & Flags.SDL_WINDOW_FULLSCREEN
    #     if fullscreen != self._fullscreen:
    #         self._fullscreen = fullscreen
    #         self._top_border = 0 if self.fullscreen else self.title_bar.height

    #         # FIXME: on_enter_fullscreen, on_leave_fullscreen ?
    #         self.on_fullscreen()

    #     if window_size != self.get_window_size():
    #         self._update_size(window_size)
    #     maximized = sdl_window_flags & Flags.SDL_WINDOW_MAXIMIZED
    #     if maximized != self._maximized:
    #         self._maximized = maximized

    @property
    def width(self) -> int:
        return self._width

    # def on_paint(self) -> None:
    #     dc = DrawingContext(self)
    #     # FIXME: Hackish to set origin in DrawingConext constructor and then reset..
    #     dc.origin = (0, 0)
    #     return dc

    #     self.paint_background()
    #     self.paint_border()

    # def paint_background(self) -> None:
    #     # surface = self.surface.get_surface()
    #     fill = self.style.background_color
    #     # fsapp.draw_filled_rectangle(surface, (0, 0), self.get_size(), fill)

    #     # dc = self.create_dc()
    #     # dc.set_fill_color(fill)
    #     # dc.draw_filled_rectangle((0, 0), self.get_size())

    #     dc = self.create_window_dc()
    #     dc.set_fill_colour(fill)
    #     dc.draw_filled_rectangle((self._left_border, self._top_border), self.get_size())
    #     # self.draw_border()

    #     # for child in self._children:
    #     #     # self.draw_child(child)
    #     #     child.draw_background()
