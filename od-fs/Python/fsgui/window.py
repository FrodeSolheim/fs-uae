import logging
import weakref
from typing import Self

from fsapp.surfaceapi.v1 import (
    Band,
    Configure,
    FocusIn,
    FocusOut,
    KeyDown,
    KeyUp,
    NavigationEvent,
    OutsideClick,
    PointerButton,
    PointerEnter,
    PointerLeave,
    PointerMotion,
    PointerWheel,
    Role,
    ScaleSupport,
    SurfaceEvent,
)
from fsapp.surfaceapi.v1 import Surface as SurfaceHandle

from .internal.api import get_api
from .internal.surface import Surface
from .scale import get_scale, position_from_pixels, size_to_pixels, to_pixels
from .types import (
    Borders,
    PaddingParam,
    PixelPosition,
    PixelSize,
    PointerPosition,
    Position,
    Size,
    SizeParam,
)
from .widget import Widget

logger = logging.getLogger(__name__)

# FIXME: Maybe inherit container instead, and have margins as a container
# property?

# FOCUSED_BORDER_COLOR = (0x99, 0xBB, 0xEE, 0xFF)
# FOCUSED_BORDER_COLOR = (0xF4, 0xF4, 0xF4, 0xFF)
FOCUSED_BORDER_COLOR = (0xFF, 0xFF, 0xFF, 0xFF)
UNFOCUSED_BORDER_COLOR = (0xCC, 0xCC, 0xCC, 0xFF)

# The height of the title bar of fsgui's decorated windows, in design units. The host window's title
# bar is the top's, with a height of its own.
WINDOW_TITLE_BAR_HEIGHT = 36


class Window(Widget):
    # The window's position is its surface's, set in the record without a repaint.
    _repaint_on_move = False

    def __init__(
        self,
        title: str = "",
        *,
        borders: Borders | None = None,
        padding: PaddingParam = None,
        size: SizeParam = None,
        role: Role = Role.TOP_LEVEL,
        parent: "Window | None" = None,
        band: Band = Band.NORMAL,
        buffer: bool = True,
    ):
        """A window, hidden, of its role in fsapp's stacking order: a top-level (in the band) or an
        overlay, or a child or popup of the parent window. buffer=False makes a window without a
        buffer, which draws nothing itself (a window showing another surface)."""
        # FIXME: self.params (for outline, thickness, colors, etc)

        # FIXME: Handle window offset internally and modify .surface x / y coordinates
        # accordingly? Remove "origin" support from bridge/_frame? Maybe send mouse events to
        # the surface, and then to the window, and handle any coordinate translation there?

        # Could maybe rename borders to frame?  self.frame.xxx?
        # FIXME: Some logic would also be easier if all windows have a sub-widget inside to
        # contain widgets - would remove some origin-compensating code??

        # Borders outside of the "normal" drawing area. Maybe only the Window
        # class has these? Or maybe they should be moved to the Widget class?
        self._set_borders(borders)

        # For a new scale (rescale): the minimum width the window was last fitted to its content
        # with (resize_to_fit_content), to fit it again, and whether its width and height were given
        # since, which it keeps.
        self._fit_min_width: float | None = None
        self._width_given = False
        self._height_given = False

        # Dirty: something the window draws has changed since it last painted (Widget.refresh), so
        # it must be painted and committed again. A window starts dirty.
        self._dirty = True

        # Its place in fsapp's stacking order, fixed when its surface is created.
        if (parent is not None) != (role in (Role.CHILD, Role.POPUP)):
            raise ValueError("a child or popup window has a parent, and only they have one")
        self._role = role
        self._parent_window = parent
        self._band = band
        self._visible = False
        self._title = title
        self._window_position = (0, 0)
        self._window_size = (0, 0)

        # FIXME: Move to Widget?
        # FIXME: Differentiate between Window focus and Widget focus?
        # Whether the window's surface has keyboard focus (FocusIn and FocusOut).
        self.focused = False

        # self._focus_on_show = True
        # Whether the window may take keyboard focus (its surface's focusable flag).
        self._focusable = True

        # _update_window_size needs _title_bar property
        self._title_bar = None

        if size is None:
            size = (1, 1)

        # self._update_window_position(size)
        self._update_window_size(size)

        super().__init__(padding=padding, parent=None, size=size)

        # FIXME: Set in Widget
        # FIXME: Make sure to clear this reference on destroy...
        self._window = self
        # Weak, so that the handler does not keep the window alive.
        window_ref = weakref.ref(self)

        def handle_surface_event(event: SurfaceEvent) -> None:
            window = window_ref()
            if window is not None:
                window._handle_surface_event(event)

        # fsgui draws at any scale (the display scale), so its surfaces support any.
        size_pixels = self.get_surface_size()
        if parent is None:
            handle = get_api().create_surface(
                size_pixels,
                handle_surface_event,
                role=role,
                band=band,
                scale_support=ScaleSupport.ANY,
                buffer=buffer,
            )
        elif role == Role.POPUP:
            handle = parent._surface.handle.create_popup(size_pixels, handle_surface_event)
        else:
            handle = parent._surface.handle.create_child(size_pixels, handle_surface_event)
        self._surface = Surface(handle)
        self._surface.set_focusable(self._focusable)

        self.surface = self._surface

        self._surface_pos = (self._left_border, self._top_border)

        # self._size = size
        # self.set_size(size)

        # Importing here to sidestep circular import issues
        from .windowmanager import WindowManager

        WindowManager.get().add_window(self)

        # Maybe a more explicit way to check if the titlebar should be present?

        from .windowtitlebar import WindowTitleBar

        if self._top_border == 0:
            # self._title_bar = None
            pass
        else:
            title_bar_height = self._top_border - self.outline
            self._title_bar = WindowTitleBar(self, self._title, (1, title_bar_height))
            self._reposition_and_resize_title_bar(size)

    # def update(self):
    #     self.surface.update()

    # FIXME: A separate draw_background / erase function do run before draw?

    def center_window(self) -> Self:
        from .windowmanager import get_client_size

        size = self.get_size()
        main_size = get_client_size()

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
        return self._create_dc((0, 0), self.get_window_size())

    def destroy(self) -> None:
        self._close_popups()

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

        from .windowmanager import WindowManager

        WindowManager.get().remove_window(self)

        # FIXME: Should maybe not be needed, but could manually free surface data and
        # schedule texture deletion now...
        # self._surface.destroy()

    # def _draw_child(self, child: Widget):
    #     child.draw()

    # def get_dc_offset(self) -> Position:
    #     return (self._left_border, self._top_border)

    # Could also be called find_child_widget_at_position (don't return self)
    def find_widget_at_position(self, position: PointerPosition):
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
        """Asks for keyboard focus for the window (ignored unless it is focusable and visible). The
        window manager's focus_window follows when it is granted (FocusIn)."""
        self._surface.request_focus()

    def _handle_surface_event(self, event: SurfaceEvent) -> None:
        from .windowmanager import WindowManager

        window_manager = WindowManager.get()
        match event:
            case PointerEnter(x=x, y=y) | PointerMotion(x=x, y=y):
                window_manager.handle_pointer_motion(self, self._from_pixels(x, y))
            case PointerLeave():
                window_manager.handle_pointer_leave(self)
            case PointerButton(x=x, y=y, button=button, down=down, clicks=clicks):
                window_manager.handle_pointer_button(
                    self, self._from_pixels(x, y), button, down, clicks
                )
            case PointerWheel(x=x, y=y, dx=dx, dy=dy):
                window_manager.handle_pointer_wheel(self, self._from_pixels(x, y), dx, dy)
            case KeyDown():
                self.on_key_down(event)
            case KeyUp():
                self.on_key_up(event)
            case NavigationEvent():
                logger.debug("%r: %r", self, event)
                self.on_navigation(event)
            case OutsideClick():
                self.on_outside_click()
            # A popup that has the keys leaves the focused look on its tree's top-level window.
            case FocusIn():
                window_manager.focus_window(self.get_top_level_window())
            case FocusOut():
                window_manager.unfocus_window(self.get_top_level_window())
            case Configure(scale=surface_scale):
                window_manager.handle_surface_scale(surface_scale)
            case _:
                pass

    def _from_pixels(self, x: float, y: float) -> PointerPosition:
        """A pointer position in pixels relative to the window's surface, in design units relative
        to the window."""
        return position_from_pixels((x, y), get_scale())

    def get_window_position(self) -> Position:
        return self._window_position

    def get_title(self) -> str:
        return self._title

    def get_window_size(self) -> Size:
        return self._window_size

    def get_surface_position(self) -> PixelPosition:
        """The window's position (with its borders) in pixels, relative to the main window's client
        area: rounded on its own, so that the window keeps its size in pixels when it moves."""
        scale = get_scale()
        return (
            to_pixels(self._window_position[0], scale),
            to_pixels(self._window_position[1], scale),
        )

    def get_surface(self) -> SurfaceHandle:
        """The window's surface (version 1 of the surface API), for calls that take one."""
        return self._surface.handle

    def get_surface_size(self) -> PixelSize:
        """The window's size (with its borders) in pixels: its surface's size, rounded as the
        drawing context rounds the window's far edges."""
        return size_to_pixels(self._window_size, get_scale())

    def place_surfaces(self, position: PixelPosition) -> None:
        """Places the window's surface at position (pixels in the host window), as fsgui does in
        every UI frame after the window's update. A window with child surfaces places them too."""
        self._surface.set_position(position)

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

    def on_key_down(self, event: KeyDown) -> None:
        """A key pressed while the window has keyboard focus."""

    def on_key_up(self, event: KeyUp) -> None:
        """A key released while the window has keyboard focus."""

    def on_navigation(self, event: NavigationEvent) -> None:
        """Navigation from a gamepad or a navigation key while the window has keyboard focus (after
        the key's on_key_down)."""

    def on_outside_click(self) -> None:
        """A press outside every open popup, for a popup window (which no window gets)."""

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
        # Below the title bar from edge to edge: the outline, a line, can be thinner than its unit,
        # and the background fills the pixels between it and the content.
        window_width, window_height = self.get_window_size()
        dc.draw_filled_rectangle(
            (0, self._top_border), (window_width, window_height - self._top_border)
        )
        # self.draw_border()

        # for child in self._children:
        #     # self.draw_child(child)
        #     child.draw_background()

    def paint_border(self) -> None:
        dc = self.create_window_dc()
        size = self.get_window_size()
        outline = self.outline

        # The top "border" is drawn by the title bar widget. The outline is drawn around it, and the
        # title bar's colour fills the pixels between them.
        if self._title_bar is not None:
            dc.set_fill_colour(self._title_bar.style.background_color)
            dc.draw_filled_rectangle((0, 0), (size[0], self._top_border))
        dc.set_pen_colour((0x90, 0x90, 0x90, 0xFF))
        dc.draw_outline(
            (0, 0),
            size,
            top=bool(self._top_border),
            right=bool(self._right_border),
            bottom=bool(self._bottom_border),
            left=bool(self._left_border),
        )

        # FIXME: Variable from style?
        if self.focused:
            dc.set_fill_colour(FOCUSED_BORDER_COLOR)
        else:
            dc.set_fill_colour(UNFOCUSED_BORDER_COLOR)
        side_height = size[1] - self._top_border - self._bottom_border
        if self._left_border:
            dc.draw_filled_rectangle(
                (outline, self._top_border), (self._left_border - outline, side_height)
            )
        if self._right_border:
            dc.draw_filled_rectangle(
                (size[0] - self._right_border, self._top_border),
                (self._right_border - outline, side_height),
            )
        if self._bottom_border:
            dc.draw_filled_rectangle(
                (outline, size[1] - self._bottom_border),
                (size[0] - 2 * outline, self._bottom_border - outline),
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

    def rescale(self) -> None:
        """The scale has changed (WindowManager.rescale_windows): the window keeps its position and
        size, fitted to its content again if it was (minimum sizes from text may have changed),
        takes a surface of its new size in pixels and is painted again."""
        if self._fit_min_width is not None and self.layout is not None:
            width, height = self._calculate_fit_size(self._fit_min_width)
            if self._width_given:
                width = self.get_width()
            if self._height_given:
                height = self.get_height()
            self._set_size((width, height))
        self.mark_dirty()

    def raise_(self) -> None:
        from .windowmanager import WindowManager

        WindowManager.get().bring_window_to_front(self)

    def resize(self, size: Size) -> Self:
        self.set_size(size)
        return self

    def resize_to_fit_content(self, min_width: float = 0) -> None:
        """Resizes the window to its layout's minimum size, at least min_width wide. It is fitted
        again when the scale changes."""
        if self.layout is None:
            logger.warning("resize_to_fit_content called without layout")
            return

        self._fit_min_width = min_width
        self._width_given = False
        self._height_given = False
        self._set_size(self._calculate_fit_size(min_width))

    def _calculate_fit_size(self, min_width: float) -> Size:
        """The layout's minimum size, at least min_width wide."""
        assert self.layout is not None
        layout_min_width = self.layout.calculate_min_width()
        width = max(layout_min_width + self.padding[1] + self.padding[3], min_width)
        height = (
            self.layout.calculate_min_height(width - self.padding[1] - self.padding[3])
            + self.padding[0]
            + self.padding[2]
        )
        return width, height

    def set_size(self, size: Size) -> None:
        self._width_given = True
        self._height_given = True
        self._set_size(size)

    def set_width(self, width: float) -> Self:
        self._width_given = True
        self._set_size((width, self.get_height()))
        return self

    def set_height(self, height: float) -> Self:
        self._height_given = True
        self._set_size((self.get_width(), height))
        return self

    def set_focusable(self, focusable: bool) -> None:
        self._focusable = focusable
        self._surface.set_focusable(focusable)

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

        # A widget that the layout moves or resizes makes the window dirty (Widget._set_position,
        # Widget._set_size).
        # REVIEW: The layout runs in every UI frame; it could run only when something asks for it
        # (a widget added, removed or resized, its text or the window's size changed).
        self.layout_children()

        surface_size = self.get_surface_size()
        if self._surface.get_size() != surface_size:
            logger.debug("Window has changed size, must update surface")
            self._surface.resize(surface_size)
            # The resized surface's buffer has lost its content.
            self._dirty = True

        # self.draw_background()
        # A hidden window is not painted, and stays dirty until it is shown.
        if self._dirty and self.visible and self.width > 0 and self.height > 0:
            # Cleared before painting, so that a refresh while painting is not lost.
            self._dirty = False
            self.draw()
            # Nothing drawn shows before it is committed. A dirty window is painted and committed
            # whole.
            self._surface.commit()

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

    def _set_borders(self, borders: Borders | None) -> None:
        # The outline is a border of one design unit, outside the title bar.
        self.outline = 1
        self.thickness = 0
        if borders is not None:
            self._top_border = borders.top
            self._right_border = borders.right
            self._bottom_border = borders.bottom
            self._left_border = borders.left
        else:
            self._top_border = WINDOW_TITLE_BAR_HEIGHT + self.outline
            self._right_border = self.thickness + self.outline
            self._bottom_border = self.thickness + self.outline
            self._left_border = self.thickness + self.outline

    def _update_window_position(self, position: Position) -> None:
        window_x = position[0] - self._left_border
        window_y = position[1] - self._top_border
        self._window_position = (window_x, window_y)
        # self._surface.
        # Let fsgui._frame set the final position, after considering "origin"?
        # self._surface.set_position(self._window_position)

    def _reposition_and_resize_title_bar(self, size: Size) -> None:
        if self._title_bar is None:
            return

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

    def _update_window_size(self, size: Size) -> None:
        window_width = size[0] + self._left_border + self._right_border
        window_height = size[1] + self._top_border + self._bottom_border
        self._window_size = (window_width, window_height)

        if self._title_bar is not None:
            self._reposition_and_resize_title_bar(size)

    def _close_popups(self) -> None:
        """Closes the popups below the window in its tree: they are not open while it is hidden, and
        would otherwise stay in the window manager after it is destroyed."""
        from .windowmanager import WindowManager

        for window in WindowManager.get().get_windows():
            if window._role != Role.POPUP or window.is_destroyed():
                continue
            ancestor = window._parent_window
            while ancestor is not None and ancestor is not self:
                ancestor = ancestor._parent_window
            if ancestor is self:
                window.close()

    def get_top_level_window(self) -> "Window":
        """The top-level window of the window's tree (itself unless it is a child or popup)."""
        window = self
        while window._parent_window is not None:
            window = window._parent_window
        return window

    @property
    def visible(self) -> bool:
        return self._visible

    # FIXME: Deprecated?
    def is_visible(self) -> bool:
        return self.visible

    def mark_dirty(self) -> None:
        """Marks the window dirty: something it draws has changed, so it is painted and committed
        (whole) in the UI frame's window update, or, while it is hidden, when it is shown again.

        Making a clean window dirty requests a UI frame: the UI thread sleeps between UI frames,
        and a window made dirty after its update in this UI frame (by a later window's update, or
        an event handler after fsgui's frame) is only painted in the next.
        """
        if self._dirty:
            return
        self._dirty = True
        get_api().request_ui_frame()

    def set_visible(self, visible: bool) -> None:
        if not visible:
            self._close_popups()
        self._visible = visible
        self._surface.set_visible(visible)
        if visible and self._dirty:
            # A window shown after its update in this UI frame is painted in the next.
            get_api().request_ui_frame()


from .drawingcontext import DrawingContext  # noqa: E402
