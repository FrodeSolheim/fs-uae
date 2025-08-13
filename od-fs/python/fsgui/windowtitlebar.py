import logging

from fsgui.button import CustomButton
from fsgui.image import Image
from fsgui.types import Size
from fsgui.widget import Widget
from fsgui.window import Window
from fsgui.windowmanager import WindowManager

logger = logging.getLogger(__name__)


class WindowTitleBar(Widget):
    def __init__(self, parent: Window, title: str, size: Size):
        super().__init__(parent=parent, size=size)

        # FIXME: Variable from style?
        self.style.background_color = (0xCC, 0xCC, 0xCC, 0xFF)
        self._title = title

        # self.menu_button = WindowMenuButton(self)
        # self.minimize_button = WindowMinimizeButton(self)
        # self.maximize_button = WindowMaximizeButton(self)
        self.close_button = WindowCloseButton(self)

        # button = Button(self, "Test")
        # button.set_position((500, 4))
        # button.set_size((60, 28))

        self._start_drag_mouse_position = (0, 0)
        self._start_drag_window_position = (0, 0)

        self.on_resize()

    def on_left_press(self):
        # from fsgui.windowmanager import WindowManager

        # print("Start dragging")

        self._start_drag_mouse_position = WindowManager.get().get_mouse_position()
        self._start_drag_window_position = self.get_window().get_position()

        # print(self._start_drag_mouse_position, self._start_drag_window_position)
        # FIXME: Maybe a capture method to start capturing the mouse
        # (and receiving mouse positions)

    def on_mouse_motion(self):
        # FIXME: Document if/how you get can on_mouse_motion *without* having
        # implicit mouse capture?

        mouse_position = WindowManager.get().get_mouse_position()
        # print("on_mouse_motion", mouse_position)

        dx = mouse_position[0] - self._start_drag_mouse_position[0]
        dy = mouse_position[1] - self._start_drag_mouse_position[1]

        self.get_window().set_position(
            (
                self._start_drag_window_position[0] + dx,
                self._start_drag_window_position[1] + dy,
            )
        )

    def on_paint(self):
        dc = self.create_dc()

        dc.set_fill_colour(self.style.background_color)
        dc.draw_filled_rectangle((0, 0), self.get_size())
        # dc.draw_text(self._title, (16, 10))

        title_x = 12

        dc.set_font(dc.get_bold_ui_font())
        _, th = dc.measure_text(self._title)
        dc.draw_text(self._title, (title_x, (self.height - th) // 2))

        # Must (currently) call draw to draw children (seems like a bad idea)
        # super().draw()

    def on_resize(self):
        width, height = self.get_size()

        # FIXME: Get these from WindowTitlebarParams or something like that
        # left_margin = 4
        # right_margin = 4
        # right_margin = 4 + 1 # 4 + self.outline
        # right_margin =  # self.outline
        right_margin = 0

        # self.menu_button.set_position((left_margin, 0))
        # self.minimize_button.set_position((width - height * 3 - right_margin, 0))
        # self.maximize_button.set_position((width - height * 2 - right_margin, 0))
        self.close_button.set_position((width - height - right_margin, 0))

    # def draw(self):
    #     pass

    # def update(self):
    #     # Calling super update to make sure the widget is drawn
    #     # FIXME: Maybe differentiate between state updates and
    #     # (redraw) update requests...
    #     super().update()


class WindowTitleBarButton(CustomButton):
    def __init__(self, parent: Widget, icon_data_name: str):
        super().__init__(parent=parent)

        # Or transparent background / let main titlebar shine through?
        # FIXME: This is not working...
        self.style.background_color = (255, 255, 255, 255)

        # print(parent, parent.get_size())
        title_bar_height = parent.get_size()[1]

        # # FIXME: title_bar_height = 36
        self.set_size((title_bar_height, title_bar_height))

        # FIXME: REMOVE PATH
        path = f"window/{icon_data_name}.png"
        self.icon = Image.from_resource(path)
        # print(self.get_size())

    # def draw_background(self):
    #     pass

    #     def on_mouse_enter()

    # FIXME: on_paint(self) ?
    def on_paint(self):
        dc = self.create_dc()

        # FIXME: Should draw be responsible for drawing children or not?
        # self.draw_children()

        size = self.get_size()
        if self.is_mouse_hovering():
            if self.is_mouse_pressed():
                background_color = (0xEE, 0xEE, 0xEE, 0xFF)
            else:
                background_color = (0xDD, 0xDD, 0xDD, 0xFF)
            dc.set_fill_colour(background_color)
            dc.draw_filled_rectangle((0, 0), size)
        iw, ih = 16, 16
        dc.draw_image(self.icon, ((size[0] - iw) // 2, (size[1] - ih) // 2))


class WindowMenuButton(WindowTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "menu")

    def _on_activate(self):
        logger.warning("FIXME: Not implemented: menu")


class WindowMinimizeButton(WindowTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "minimize")

    def _on_activate(self):
        logger.warning("FIXME: Not implemented: minimize")


class WindowMaximizeButton(WindowTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "maximize")

    def _on_activate(self):
        logger.warning("FIXME: Not implemented: restore or maximize")


class WindowCloseButton(WindowTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "close")

    def _on_activate(self):
        # FIXME: window.request_close (?)
        self.get_window().close()
