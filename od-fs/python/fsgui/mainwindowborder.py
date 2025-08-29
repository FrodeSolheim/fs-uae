import logging
import os

from fsgui.borderlesswindow import BorderlessWindow
from fsgui.drawingcontext import DrawingContext
from fsgui.types import Size
from fsgui.window import Window

logger = logging.getLogger(__name__)


class MainWindowBorder:
    def __init__(self):
        self._maximized = False
        self._fullscreen = False

        self.params = MainWindowBorderParams()

        # FIXME: Collect all different titlebar and border colors and define in the same place

        grey_level = 0x90  # Between 0x88 and 0x99
        self.params.outline_colour = (grey_level, grey_level, grey_level, 255)
        # self.params.outline_colour = (255, 0, 0, 255)

        self.top_border = _MainWindowTopBorder(self.params)
        self.left_border = _MainWindowLeftBorder(self.params)
        self.right_border = _MainWindowRightBorder(self.params)
        self.bottom_border = _MainWindowBottomBorder(self.params)

        show_extra_borders = True

        # For Wayland at least, when asking for a borderless window we do not get shadows either.
        # Workaround for now at least is to draw an outline around the window.

        # FIXME: Maybe detect Wayland/Linux here... -Not entirely correct yet, we could be
        # running on XWayland...
        if os.environ.get("WAYLAND_DISPLAY"):
            self.outline = 1
        else:
            self.outline = 0

        self.thickness = 0

        if show_extra_borders:
            self.top_border.height = self.outline
            self.left_border.width = self.thickness + self.outline
            self.right_border.width = self.thickness + self.outline
            self.bottom_border.height = self.thickness + self.outline
        else:
            self.top_border.height = 0
            self.left_border.width = 0
            self.right_border.width = 0
            self.bottom_border.height = 0

        # If the width or height is 0, the windows will not be drawn or handled at all by the
        # rendering code, so we can always "show" the borders regardless.

        # FIMXE: Hidden for now -- for now, we just let the background color of the window show
        # through in order to get the outline. Not using thick borders right now.

        self.border_hack = False
        # if self.border_hack:
        # self.top_border.show()
        # self.left_border.show()
        # self.right_border.show()
        # self.bottom_border.show()

    @property
    def top(self) -> int:
        if self._fullscreen:
            return 0
        if self._maximized:
            # FIXME: titlebar height
            return 36
        return 36 + self.outline

    @property
    def left(self) -> int:
        return 0 if (self._fullscreen or self._maximized) else self.left_border.width

    @property
    def right(self) -> int:
        return 0 if (self._fullscreen or self._maximized) else self.right_border.width

    @property
    def bottom(self) -> int:
        return 0 if (self._fullscreen or self._maximized) else self.bottom_border.height

    def _resize_border(self, fullscreen: bool, maximized: bool, size: Size) -> None:
        logger.debug("MainWindow._reposition_borders %r", size)
        self._fullscreen = fullscreen
        self._maximized = maximized

        show_border = not fullscreen and not maximized

        self.top_border.set_visible(show_border)
        self.left_border.set_visible(show_border)
        self.right_border.set_visible(show_border)
        self.bottom_border.set_visible(show_border)

        # When we're not in fullscreen, there is not resizing the hidden borders. Also, the
        # size calculations could end up in negative values to use non-zeroed border values.

        if fullscreen:
            return

        top = self.top

        if maximized:
            self.title_bar.set_position((0, -top))
            self.title_bar.set_width(size[0])
            return

        # top = self.title_bar.height
        left = self.left
        right = self.right
        bottom = self.bottom
        side_height = size[1] - top - bottom
        # print(top)

        # self.title_bar.set_width(size[0])

        # self.border.left_border.set_position((0, top))
        # self.border.left_border.set_height(side_height)

        # self.border.right_border.set_position((size[0] - self.border.right_border.width, top))
        # self.border.right_border.set_height(side_height)

        # self.border.bottom_border.set_width(size[0])

        # FIXME: ... for titlebar, support 1 px borders around the entire thing?
        # FIXME: self.outline

        self.title_bar.set_position((-left + self.outline, -top + self.outline))
        self.title_bar.set_width(size[0] - 2 * self.outline)

        self.top_border.set_position((-left, -top))
        self.top_border.set_size((size[0], self.top))

        self.left_border.set_position((-left, 0))
        self.left_border.set_height(side_height)

        self.right_border.set_position((size[0] - right - left, 0))
        self.right_border.set_height(side_height)

        self.bottom_border.set_position((-left, size[1] - top - bottom))
        self.bottom_border.set_width(size[0])


class MainWindowBorderParams:
    def __init__(self) -> None:
        super().__init__()
        self.outline_colour = (0, 0, 0, 255)


class _MainWindowBorderWindow(BorderlessWindow):
    def __init__(self, params: MainWindowBorderParams) -> None:
        super().__init__()
        self.set_layer(Window.LAYER_HIGHEST)
        # self.outline_colour = (255, 0, 255, 255)
        self.params = params


class _MainWindowTopBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        size = self.size

        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_line((1, 0), (size[0] - 2, 0))
        dc.draw_line((0, 0), (0, size[1] - 1))
        dc.draw_line((size[0] - 1, 0), (size[0] - 1, size[1] - 1))


class _MainWindowLeftBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        size = self.size

        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_line((0, 0), (0, size[1] - 1))


class _MainWindowRightBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        size = self.size

        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_line((size[0] - 1, 0), (size[0] - 1, size[1] - 1))


class _MainWindowBottomBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        size = self.size

        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_line((0, size[1] - 1), (size[0] - 1, size[1] - 1))
