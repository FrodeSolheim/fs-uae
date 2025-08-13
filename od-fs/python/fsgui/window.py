import logging
from typing import Self

from fsgui.internal.surface import Surface
from fsgui.types import Borders, PaddingParam, Position, Size, SizeParam
from fsgui.widget import Widget

logger = logging.getLogger(__name__)

# FIXME: Maybe inherit container instead, and have margins as a container
# property?

# FOCUSED_BORDER_COLOR = (0x99, 0xBB, 0xEE, 0xFF)
# FOCUSED_BORDER_COLOR = (0xF4, 0xF4, 0xF4, 0xFF)
FOCUSED_BORDER_COLOR = (0xFF, 0xFF, 0xFF, 0xFF)
UNFOCUSED_BORDER_COLOR = (0xCC, 0xCC, 0xCC, 0xFF)


class Window(Widget):
    LAYER_BACKGROUND = -20
    LAYER_BELOW = -10
    LAYER_NORMAL = 0
    LAYER_ABOVE = 10
    LAYER_HIGHEST = 20

    # LAYER_ON_TOP = 20

    def __init__(
        self,
        title: str = "",
        *,
        borders: Borders | None = None,
        padding: PaddingParam = None,
        size: SizeParam = None,
        special=0,
    ):
        if size is None:
            size = (1, 1)

        # FIXME: self.params (for outline, thickness, colors, etc)

        # FIXME: Handle window offset internally and modify .surface x / y coordinates
        # accordingly? Remove "origin" support from bridge/_frame? Maybe send mouse events to
        # the surface, and then to the window, and handle any coordinate translation there?

        # Could maybe rename borders to frame?  self.frame.xxx?
        # FIXME: Some logic would also be easier if all windows have a sub-widget inside to
        # contain widgets - would remove some origin-compensating code??

        self.outline = 1
        self.thickness = 0

        # Borders outside of the "normal" drawing area. Maybe only the Window
        # class has these? Or maybe they should be moved to the Widget class?
        if borders is not None:
            self._top_border = borders.top
            self._right_border = borders.right
            self._bottom_border = borders.bottom
            self._left_border = borders.left
        else:
            # self._top_border = 37
            # self._right_border = 5
            # self._bottom_border = 5
            # self._left_border = 5
            self._top_border = 36 + self.outline
            self._right_border = self.thickness + self.outline
            self._bottom_border = self.thickness + self.outline
            self._left_border = self.thickness + self.outline

        self._layer = 0
        self._visible = False
        self._title = title
        self._window_position = (0, 0)
        self._window_size = (0, 0)

        # FIXME: Move to Widget?
        # FIXME: Differentiate between Window focus and Widget focus?
        self.focused = False

        # self._focus_on_show = True
        self._focusable = True

        # _update_window_size needs _title_bar property
        self._title_bar = None

        # self._update_window_position(size)
        self._update_window_size(size)

        super().__init__(padding=padding, parent=None, size=size)

        # FIXME: Set in Widget
        # FIXME: Make sure to clear this reference on destroy...
        self._window = self
        self._surface = Surface(self._window_size, special)

        self.surface = self._surface

        self._surface_pos = (self._left_border, self._top_border)

        # self._size = size
        # self.set_size(size)

        # Importing here to sidestep circular import issues
        from fsgui.windowmanager import WindowManager

        WindowManager.get().add_window(self)

        # Maybe a more explicit way to check if the titlebar should be present?

        from fsgui.windowtitlebar import WindowTitleBar

        if self._top_border == 0:
            # self._title_bar = None
            pass
        else:
            # FIXME: duplicate code in _update_window_size
            title_bar_size = (
                size[0] + self._left_border + self._right_border,
                self._top_border - self.outline,
            )
            # title_bar_size = (
            #     size[0] + self._left_border + self._right_border - 2,
            #     self._top_border - 1,
            # )
            self._title_bar = WindowTitleBar(self, self._title, title_bar_size)
            self._title_bar.set_position(
                (-self._left_border + self.outline, -self._top_border + self.outline)
            )
            # self._title_bar.set_position(
            #     (-self._left_border + 1, -self._top_border + 1)
            # )

    # def update(self):
    #     self.surface.update()

    # FIXME: A separate draw_background / erase function do run before draw?

    def center_window(self) -> Self:
        from fsgui.mainwindow import MainWindow

        size = self.get_size()
        main_size = MainWindow.get().get_size()

        # FIXME: Calculation is done without properly considering title bars
        # (both for main window and window) and window borders, so centering
        # may be a bit off (in the vertical direction).

        x = (main_size[0] - size[0]) // 2
        y = (main_size[1] - size[1]) // 2
        self.set_position((x, y))

        return self

    def close(self) -> None:
        # FIXME: Fire away on_close events first (probably, yes)?
        self.on_close()
        # FIXME: Make it possible to veto closing?
        self.destroy()

    def create_window_dc(self) -> "DrawingContext":
        dc = DrawingContext(self)
        # FIXME: Hackish to set origin in DrawingContext constructor and then reset..
        dc.origin = (0, 0)
        return dc

    def destroy(self):
        # Not strictly necessary to run destroy explicitly. It will be done implicitly when the
        # surface is no longer referenced, but a good idea to free the surface buffers already now
        # in case the reference is held somewhere.

        if self.surface is not None:
            self.surface.destroy()

        # Recursively destroy children
        # for child in self._children:
        #     child.destroy()

        super().destroy()

        # FIXME: Not sure if we should call on_destroy early or late
        # Called by super().destroy
        # self._on_destroy()

        from fsgui.windowmanager import WindowManager

        WindowManager.get().remove_window(self)

        # FIXME: Should maybe not be needed, but could manually free surface data and
        # schedule texture deletion now...
        # self._surface.destroy()

    # def _draw_child(self, child: Widget):
    #     child.draw()

    # def get_dc_offset(self) -> Position:
    #     return (self._left_border, self._top_border)

    # Could also be called find_child_widget_at_position (don't return self)
    def find_widget_at_position(self, position: Position):
        """Coordinates are relative to the widget itself.

        Overrides Widget implementation to account for window borders.
        """
        widget = super().find_widget_at_position(
            (position[0] - self._left_border, position[1] - self._top_border)
        )
        if widget is not None:
            return widget

        x, y = position
        ww, wh = self.get_window_size()

        if 0 <= x < ww and 0 <= y < wh:
            # print(x, y)
            if y < self._top_border and self._title_bar is not None:
                # return self._title_bar
                return self._title_bar.find_widget_at_position((position[0], position[1]))

        # x, y = position
        # for widget in reversed(self._children):
        #     wx, wy = widget.get_position()
        #     ww, wh = widget.get_size()
        #     # print(x, y, wx, wy, ww, wh)
        #     if wx <= x < wx + ww and wy <= y < wy + wh:
        #         return widget.find_widget_at_position(
        #             (position[0] - wx, position[1] - wy)
        #         )
        # # No children matched, check if self is within bounds
        # ww, wh = self.get_size()
        # if 0 <= x < ww and 0 <= y < wh:
        #     return self

        # Nope, coordinates are outside even self
        return None

    def focus(self) -> None:
        from fsgui.windowmanager import WindowManager

        WindowManager.get().focus_window(self)

    def get_window_position(self) -> Position:
        return self._window_position

    def get_title(self) -> str:
        return self._title

    def get_window_size(self) -> Size:
        return self._window_size

    def move(self, position: Position) -> Self:
        self._update_window_position(position)
        super().move(position)
        return self

    def on_bring_to_front(self) -> None:
        pass

    def on_blur(self) -> None:
        if self._title_bar is not None:
            self._title_bar.style.background_color = UNFOCUSED_BORDER_COLOR

    def on_close(self) -> None:
        pass

    def on_focus(self) -> None:
        if self._title_bar is not None:
            self._title_bar.style.background_color = FOCUSED_BORDER_COLOR

    def on_paint(self) -> None:
        self.paint_background()
        self.paint_border()

    def paint_background(self) -> None:
        # surface = self.surface.get_surface()
        fill = self.style.background_color
        # fsapp.draw_filled_rectangle(surface, (0, 0), self.get_size(), fill)

        # dc = self.create_dc()
        # dc.set_fill_color(fill)
        # dc.draw_filled_rectangle((0, 0), self.get_size())

        dc = self.create_window_dc()
        dc.set_fill_colour(fill)
        dc.draw_filled_rectangle((self._left_border, self._top_border), self.get_size())
        # self.draw_border()

        # for child in self._children:
        #     # self.draw_child(child)
        #     child.draw_background()

    def paint_border(self) -> None:
        dc = self.create_window_dc()

        # FIXME: Variable from style?
        if self.focused:
            dc.set_fill_colour(FOCUSED_BORDER_COLOR)
        else:
            dc.set_fill_colour(UNFOCUSED_BORDER_COLOR)

        size = self.get_window_size()

        # The top "border" is drawn by the title bar widget
        # dc.draw_filled_rectangle((0, 0), (window_size[0], self._top_border))

        dc.set_pen_colour((0x90, 0x90, 0x90, 0xFF))
        # dc.set_pen_colour((0xFF, 0x00, 0xFF, 0xFF))

        # FIXME: Can skip outer 1 px... -Not anymore
        if self._top_border:
            if self.outline:
                dc.draw_line((0, 0), (size[0] - 1, 0))

        if self._left_border:
            if self.outline:
                dc.draw_line((0, 1), (0, size[1] - 2))
            dc.draw_filled_rectangle(
                (self.outline, self._top_border),
                (
                    self._left_border - self.outline,
                    size[1] - self._top_border - self._bottom_border,
                ),
            )
        if self._right_border:
            if self.outline:
                dc.draw_line((size[0] - 1, 1), (size[0] - 1, size[1] - 2))
            dc.draw_filled_rectangle(
                (self._window_size[0] - self._right_border, self._top_border),
                (
                    self._right_border - self.outline,
                    size[1] - self._top_border - self._bottom_border,
                ),
            )
        if self._bottom_border:
            if self.outline:
                dc.draw_line((0, size[1] - 1), (size[0] - 1, size[1] - 1))
            dc.draw_filled_rectangle(
                (self.outline, size[1] - self._bottom_border),
                (size[0] - 2 * self.outline, self._bottom_border - self.outline),
            )

        # if self._left_border:
        #     line_color = (0xAA, 0xAA, 0xAA, 0xFF)
        #     dc.draw_line((0, 0), (window_size[0] - 1, 0), line_color)
        #     dc.draw_line((0, 1), (0, window_size[1] - 2), line_color)
        #     dc.draw_line(
        #         (window_size[0] - 1, 1),
        #         (window_size[0] - 1, window_size[1] - 2),
        #         line_color,
        #     )
        #     dc.draw_line(
        #         (0, window_size[1] - 1),
        #         (window_size[0] - 1, window_size[1] - 1),
        #         line_color,
        #     )

    def raise_(self) -> None:
        from fsgui.windowmanager import WindowManager

        WindowManager.get().bring_window_to_front(self)

    def resize_to_fit_content(self, min_width: int = 0) -> None:
        if self.layout is None:
            logger.warning("resize_to_fit_content called without layout")
            return

        layout_min_width = self.layout.calculate_min_width()
        width = max(layout_min_width + self.padding[1] + self.padding[3], min_width)
        height = (
            self.layout.calculate_min_height(width - self.padding[1] - self.padding[3])
            + self.padding[0]
            + self.padding[2]
        )
        # print("\n\n\n\nHEIGHT", height)

        self._set_size((width, height))

    def set_focusable(self, focusable) -> None:
        self._focusable = focusable

    # def set_focus_on_show(self, focus_on_show) -> None:
    #     self._focus_on_show = focus_on_show

    def _set_position(self, position: Position) -> None:
        self._update_window_position(position)
        super()._set_position(position)

    def _set_size(self, size: Size) -> None:
        self._update_window_size(size)
        super()._set_size(size)

    def show(self) -> Self:
        """Ensures that the window is visible raised and focused."""
        self.set_visible(True)
        self.raise_()
        if self._focusable:
            self.focus()
        return self

    def update(self) -> None:
        # FIXME: Consider also skipping layout_children if the window is not visible; then later,
        # if the window becomes visible, make sure layout is run and dirty regions are redrawn.

        # if self.layout is not None:
        #     # Set position and size for layout first
        #     self.layout.x = 0
        #     self.layout.y = 0
        #     self.layout.width = self.get_width()
        #     self.layout.height = self.get_height()
        #     self.layout.layout_children()
        self.layout_children()

        if self._surface.get_size() != self.get_window_size():
            logger.debug("Window has changed size, must update surface")
            # fsapp.resize_surface(self._surface)
            self._surface.resize(self.get_window_size())

        # self.draw_background()
        if self.visible and self.width > 0 and self.height > 0:
            self.draw()

        # im = self.surface._image
        # draw = ImageDraw.Draw(im)
        # "BGRA"
        # fill = (0, 255, 0, 255)
        # fill = (255, 0, 0, 255)
        # G B - R
        # fill = (255, 255, 255, 255)
        # draw.rectangle([(0, 0), (im.size[0] - 1, im.size[1] - 1)], fill)
        # print("draw", im.size)
        # draw.line([(200, 10), (400, 10)], fill=128, width=4)

        # surface = self.surface.get_surface()
        # size = self.size
        # fill = self.style.background_color
        # fsapp.draw_filled_rectangle(surface, (0, 0, size[0], size[1]), fill)

    def _update_window_position(self, position: Position) -> None:
        window_x = position[0] - self._left_border
        window_y = position[1] - self._top_border
        self._window_position = (window_x, window_y)
        # self._surface.
        # Let fsgui._frame set the final position, after considering "origin"?
        # self._surface.set_position(self._window_position)

    def _update_window_size(self, size: Size) -> None:
        window_width = size[0] + self._left_border + self._right_border
        window_height = size[1] + self._top_border + self._bottom_border
        self._window_size = (window_width, window_height)

        if self._title_bar is not None:
            title_bar_size = (
                size[0] + self._left_border + self._right_border - 2 * self.outline,
                self._top_border - self.outline,
            )
            # title_bar_size = (
            #     size[0] + self._left_border + self._right_border - 2,
            #     self._top_border - 1,
            # )
            self._title_bar.set_position(
                (-self._left_border + self.outline, -self._top_border + self.outline)
            )
            self._title_bar.set_size(title_bar_size)

    @property
    def layer(self) -> bool:
        return self._layer

    def set_layer(self, layer: int) -> Self:
        from fsgui.windowmanager import WindowManager

        WindowManager.get()._change_window_layer(self, self._layer, layer)
        self._layer = layer

        return self

    @property
    def visible(self) -> bool:
        return self._visible

    # FIXME: Deprecated?
    def is_visible(self) -> bool:
        return self.visible

    def set_visible(self, visible: bool) -> None:
        self._visible = visible
        self._surface.set_visible(visible)


from fsgui.drawingcontext import DrawingContext  # noqa: E402
