import _fsapp

from fsgui.borderlesswindow import BorderlessWindow
from fsgui.button import CustomButton
from fsgui.font import Font
from fsgui.image import Image
from fsgui.widget import Widget

# FIXME: Must send rectangles to fsapp for which the titlebar hit test should be
# ignored - so basically for all interactive child widgets.


class MainWindowTitleBar(BorderlessWindow):
    def __init__(self, title, size, extra_title=""):
        super().__init__("", size=size)
        # self.style.background_color = (56, 56, 56, 255)
        # self.style.background_color = (255, 255, 255, 255)

        # grey_level = 0xFF
        grey_level = 0xF4
        # grey_level = 0x1C
        self.style.background_color = (grey_level, grey_level, grey_level, 255)

        # self.menu_button = MenuButton(self)

        self.minimize_button = MinimizeButton(self)
        self.maximize_button = MaximizeButton(self)
        self.close_button = CloseButton(self)

        # button = Button(self, "Test")
        # button.set_position((500, 4))
        # button.set_size((60, 28))

        self._title = title
        self.extra_title = extra_title
        self.font = Font(weight=Font.BOLD)

        self.on_resize()

    def on_paint(self):
        dc = self.create_dc()

        dc.set_fill_colour(self.style.background_color)
        dc.draw_filled_rectangle((0, 0), self.get_size())

        tw, th = dc.measure_text(self._title)
        # tx = 42
        title_x = 12
        title_y = (self.height - th) // 2
        dc.draw_text(self._title, (title_x, title_y))

        if self.extra_title:
            tw, th = dc.measure_text(self.extra_title)
            title_x = (self.width - tw) // 2
            dc.draw_text(self.extra_title, (title_x, title_y))

        # tx = tx + tw + 12
        # sub_title = "V5 PREVIEW"
        # dc.set_text_colour((0xBB, 0xBB, 0xBB, 0xFF))
        # dc.draw_text(sub_title, (tx, ty))

    def on_resize(self):
        width, height = self.get_size()

        # left_margin = 4
        # self.menu_button.set_position((left_margin, 0))

        right_margin = 0

        self.minimize_button.set_position((width - height * 3 - right_margin, 0))
        self.maximize_button.set_position((width - height * 2 - right_margin, 0))
        self.close_button.set_position((width - height - right_margin, 0))

    # def draw(self):
    #     pass

    # def update(self):
    #     # Calling super update to make sure the widget is drawn
    #     # FIXME: Maybe differentiate between state updates and
    #     # (redraw) update requests...
    #     super().update()


class MainTitleBarButton(CustomButton):
    def __init__(self, parent: Widget, icon_data_name: str):
        super().__init__(parent=parent)

        # Or transparent background / let main titlebar shine through?
        self.style.background_color = (255, 255, 255, 255)

        # print(parent, parent.get_size())
        title_bar_height = parent.get_size()[1]
        self.set_size((title_bar_height, title_bar_height))

        # FIXME: REMOVE PATH
        path = f"window/{icon_data_name}.png"
        self.icon = Image.from_resource(path)
        # print("ICON", self.icon)
        # print(self.get_size())

    # def draw_background(self):
    #     pass

    #     def on_mouse_enter()

    # FIXME: on_paint(self) ?
    def on_paint(self):
        dc = self.create_dc()
        size = self.get_size()
        if self.is_mouse_hovering():
            if self.is_mouse_pressed():
                background_color = (0xCC, 0xCC, 0xCC, 0xFF)
            else:
                background_color = (0xDD, 0xDD, 0xDD, 0xFF)
            dc.set_fill_colour(background_color)
            dc.draw_filled_rectangle((0, 0), size)
        iw, ih = 16, 16
        dc.draw_image(self.icon, ((size[0] - iw) // 2, (size[1] - ih) // 2))


class MenuButton(MainTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "menu")
        # self.set_size(20 )

    def _on_activate(self):
        pass

    # def draw(self):
    #     dc = DrawingContext(self)
    #     ww, wh = self.get_size()
    #     # iw, ih = self.icon.get_size()
    #     # FIXME
    #     iw, ih = 16, 16
    #     dc.draw_image(self.icon, (4 + (ww - iw ) // 2, (wh - ih)// 2))


class MinimizeButton(MainTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "minimize")
        # self.style.background_color = (255, 255, 0, 255)

    def _on_activate(self):
        _fsapp.minimize_window()


class MaximizeButton(MainTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "maximize")
        # self.style.background_color = (0, 255, 0, 255)

    def _on_activate(self) -> None:
        from fsgui.mainwindow import MainWindow

        main_window = MainWindow.get()
        if main_window.fullscreen:
            _fsapp.set_fullscreen(False)
        elif main_window.is_maximized():
            _fsapp.restore_window()
        else:
            _fsapp.maximize_window()


class CloseButton(MainTitleBarButton):
    def __init__(self, parent: Widget):
        super().__init__(parent, "close")
        # self.style.background_color = (255, 0, 0, 255)

    def _on_activate(self) -> None:
        _fsapp.close_window()
