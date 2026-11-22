import logging
import os

from fsapp.surfaceapi.v1 import Band
from fsgui.borderlesswindow import BorderlessWindow
from fsgui.scale import get_scale, line_to_pixels, scaled
from fsgui.types import PixelSize
from fsgui.window import Window

from app.mainwindowtitlebar import TITLE_BAR_HEIGHT, MainWindowTitleBar

logger = logging.getLogger(__name__)


# REVIEW: The chrome in pixels rather than design units.
class MainWindowBorder:
    """The main window's title bar, border and outline, drawn by fsgui windows around the client
    area. Its sizes (top, left, right, bottom, outline) are in pixels, as the host window's size and
    fsapp's hit test are, so that the border windows add up exactly to the host window's size and
    the client area starts at a whole pixel at any scale; its windows are placed in design units,
    those pixels over the scale."""

    # Set by the main window.
    title_bar: MainWindowTitleBar

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
        # In design units.
        if os.environ.get("WAYLAND_DISPLAY"):
            self._outline = 1
        else:
            self._outline = 0

        self._thickness = 0

        self._show_extra_borders = show_extra_borders

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
    def outline(self) -> int:
        """The outline's width in pixels, a line."""
        return line_to_pixels(self._outline, get_scale())

    @property
    def _side(self) -> int:
        """The side and bottom borders' thickness in pixels."""
        if not self._show_extra_borders:
            return 0
        return scaled(self._thickness) + self.outline

    @property
    def top(self) -> int:
        if self._fullscreen:
            return 0
        if self._maximized:
            # FIXME: titlebar height
            return scaled(TITLE_BAR_HEIGHT)
        return scaled(TITLE_BAR_HEIGHT) + self.outline

    @property
    def left(self) -> int:
        return 0 if (self._fullscreen or self._maximized) else self._side

    @property
    def right(self) -> int:
        return 0 if (self._fullscreen or self._maximized) else self._side

    @property
    def bottom(self) -> int:
        return 0 if (self._fullscreen or self._maximized) else self._side

    def _resize_border(self, fullscreen: bool, maximized: bool, size: PixelSize) -> None:
        """Places the title bar and the border around the client area of a host window of size (in
        pixels)."""
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

        scale = get_scale()
        top = self.top

        if maximized:
            self.title_bar.set_position((0, -top / scale))
            self.title_bar.set_size((size[0] / scale, top / scale))
            return

        left = self.left
        right = self.right
        bottom = self.bottom
        outline = self.outline
        client_width = size[0] - left - right
        client_height = size[1] - top - bottom

        def place(window: Window, x: int, y: int, width: int, height: int) -> None:
            window.set_position((x / scale, y / scale))
            window.set_size((width / scale, height / scale))

        place(self.title_bar, -left + outline, -top + outline, size[0] - 2 * outline, top - outline)
        place(self.top_border, -left, -top, size[0], top)
        place(self.left_border, -left, 0, left, client_height)
        place(self.right_border, client_width, 0, right, client_height)
        place(self.bottom_border, -left, client_height, size[0], bottom)


class MainWindowBorderParams:
    def __init__(self) -> None:
        super().__init__()
        self.outline_colour = (0, 0, 0, 255)


class _MainWindowBorderWindow(BorderlessWindow):
    def __init__(self, params: MainWindowBorderParams) -> None:
        super().__init__(band=Band.PANEL)
        # The host window's border, not a window of its own: a click on it leaves the keys where
        # they are.
        self.set_focusable(False)
        # self.outline_colour = (255, 0, 255, 255)
        self.params = params


class _MainWindowTopBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_outline((0, 0), self.size, bottom=False)


class _MainWindowLeftBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_outline((0, 0), self.size, top=False, right=False, bottom=False)


class _MainWindowRightBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_outline((0, 0), self.size, top=False, bottom=False, left=False)


class _MainWindowBottomBorder(_MainWindowBorderWindow):
    def on_paint(self) -> None:
        dc = self.create_dc()
        dc.set_pen_colour(self.params.outline_colour)
        dc.draw_outline((0, 0), self.size, top=False, right=False, left=False)
