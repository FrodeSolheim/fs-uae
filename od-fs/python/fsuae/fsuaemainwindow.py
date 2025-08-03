from fsgui.mainwindow import MainWindow
from fsgui.types import Colour
from fsuae.emulatorwindow import EmulatorWindow


class FSUAEMainWindow(MainWindow):
    def __init__(self, *, fullscreen: bool = False):
        # For temporary ShortcutsWindow
        # height = 36 + int(540 * 1.5) + 36
        # height = int(540 * 1.5) + 36

        # width, height = 1366, 768
        # width = 1366

        height = int(540 * 1.5)
        width = int(height * (4 / 3))

        title = "FS-UAE"
        super().__init__(title, (width, height), fullscreen=fullscreen)
        self.set_background_colour(Colour.GREY_11)
        # self.set_background_colour((0x19, 0x19, 0x19, 0xFF))

        self.emulator_window = EmulatorWindow()
        # self.emulator_window.show()

        # self.emulator = self.emulator_window.special_window

        # self.shortcuts_window = ShortcutsWindow().move((0, 760)).show()
        # self.shortcuts_window = ShortcutsWindow().show()

        # FIXME: Maybe automatically call on_resize after Window creation just
        # before being shown for the first time? If so, we can remove this...
        self.on_resize()

    def on_fullscreen(self) -> None:
        super().on_fullscreen()

    def on_resize(self) -> None:
        print("on_resize")
        wh = self.height
        ww = self.width

        ratio = 4 / 3

        if ww / wh > ratio:
            eh = wh
            ew = int(eh * ratio)
        else:
            ew = ww
            eh = int(ew / ratio)

        ex = (ww - ew) // 2
        ey = (wh - eh) // 2

        self.emulator_window.move((ex, ey))
        self.emulator_window.resize((ew, eh))

        # main_window_size = self.get_size()
        # self.shortcuts_window.move(
        #     (0, main_window_size[1] - self.shortcuts_window.height)
        # )
        # self.shortcuts_window.set_width(main_window_size[0])
