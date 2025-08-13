from fsapp.tickservice import TickService
from fsgui.mainwindow import MainWindow
from fsgui.types import Colour

from fsgui.windowmanager import WindowManager
from fsuae import version
from fsuae.emulatorwindow import EmulatorWindow


class FSUAEMainWindow(MainWindow):
    def __init__(self, *, fullscreen: bool = False):
        # For temporary ShortcutsWindow
        # height = 36 + int(540 * 1.5) + 36
        # height = int(540 * 1.5) + 36

        # width, height = 1366, 768
        # width = 1366

        # height = int(540 * 1.5)
        height = int((512 + 16) * 1.5)
        width = int(height * (4 / 3))
        print("FS-UAE desired window client size", width, height)

        delim = "  \u00b7  "
        # space = " " * 20
        f12_help = "F12 to toggle menu"
        mouse_help = "Alt+G or middle click to toggle mouse"
        extra_title = f"{f12_help}{delim}{mouse_help}"

        title = f"FS-UAE  {version} ALPHA"
        super().__init__(title, (width, height), fullscreen=fullscreen, extra_title=extra_title)
        # self.set_background_colour(Colour.GREY_11)
        # self.set_background_colour(Colour.RED)

        # We want this color to be distinct from dark Amiga grey colors, so it should not be near
        # 0x00, 0x11 or 0x22, (etc). 0x1C seems to be nice middle-ground that's obviously not
        # supposed to be black, makes black stand out, and does not blend in Amiga backgrounds.
        grey_level = 0x1C
        # Something like 0x19 is also fine, but 0x1C seems to better on a wider range of monitors.
        # grey_level = 0x19
        self.set_background_colour((grey_level, grey_level, grey_level, 0xFF))

        self.emulator_window = EmulatorWindow()
        # self.emulator_window.show()

        # self.emulator = self.emulator_window.special_window

        # self.shortcuts_window = ShortcutsWindow().move((0, 760)).show()
        # self.shortcuts_window = ShortcutsWindow().show()

        # FIXME: Maybe automatically call on_resize after Window creation just
        # before being shown for the first time? If so, we can remove this...
        self.on_resize()

        TickService().get().register(self.__on_tick)

    def __on_tick(self) -> None:
        # Circular import; not very great design
        from fsuae.f12window import F12Window

        if self.fullscreen:
            mouse_pos = WindowManager.get().get_mouse_position()
            # print(mouse_pos)
            ui_is_open = F12Window.has_instance()

            if mouse_pos[1] == 0 or ui_is_open:
                self.title_bar.set_y(0)
                self.title_bar.set_width(self.width)
                self.title_bar.set_visible(True)
                # self.title_bar.set_width(1920)
            elif self.title_bar.visible and 0 <= mouse_pos[1] < self.title_bar.height:
                # We want to continue showing the title bar as long as the mouse cursor is
                # hovering above it.
                pass
            else:
                self.title_bar.set_visible(False)
        

    # MainWindow does not get destroyed as such, currently...
    # def on_destroy(self) -> None:
    #     TickService().get().unregister(self.__on_tick)
    #     super().on_destroy()


    def on_fullscreen(self) -> None:
        super().on_fullscreen()

    def on_resize(self) -> None:
        # ww, wh = self.size
        ww, wh = self.width, self.height
        print("on_resize, fit emulator video within", ww, wh)

        # UAE_RECT - fullscreen positioning of output rect here

        # border = 14 # (540 - 512 - 16) / 2
        if self.fullscreen:
            border = 12  # 1080 - 528 * 2 / 2
            border = (12 * wh) // 1080  # 12 px on 1080p, scale with resolution
        else:
            border = 0

        fit_w = ww
        fit_h = wh - border * 2
        off_x = 0
        off_y = border

        ratio = 4 / 3

        if fit_w / fit_h > ratio:
            eh = fit_h
            ew = int(eh * ratio)
        else:
            ew = fit_w
            eh = int(ew / ratio)

        ex = off_x + (fit_w - ew) // 2
        ey = off_y + (fit_h - eh) // 2

        print("Emulator window at", (ex, ey), "size", (ew, eh))
        self.emulator_window.move((ex, ey))
        self.emulator_window.resize((ew, eh))

        # main_window_size = self.get_size()
        # self.shortcuts_window.move(
        #     (0, main_window_size[1] - self.shortcuts_window.height)
        # )
        # self.shortcuts_window.set_width(main_window_size[0])
