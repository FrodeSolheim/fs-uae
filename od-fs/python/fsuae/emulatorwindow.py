from fsgui.specialwindow import SpecialWindow
from fsgui.window import Window
from fsgui.windowmanager import WindowManager


class EmulatorWindow(Window):
    def __init__(self):
        # size = (752, 572)
        # size = (800, 650)

        # UAE_RECT
        # position = (660, 90)
        size = (692, 540)

        super().__init__("Amiga display", size=size)

        special = 0x554145  # "UAE"
        self.special_window = SpecialWindow(size=size, special=special)
        self.special_window.show()
        # self.set_position(position)

        # self.close()
        # self.move((2000, 2000))
        # self.special_window.set_size((int(692 * 1.5), int(540 * 1.5)))
        # self.special_window.set_position((328, 36))

        # FIXME: Only when we're hiding the emulator window itself...
        self.special_window.set_layer(Window.LAYER_BELOW)

    def on_bring_to_front(self) -> None:
        # FIXME: Handle the case where the special window manager is brought
        # to front? Maybe set some kind of non-interactive flag on the special
        # window so that the mouse events are passed through to EmulatorWindow?
        WindowManager().get().bring_window_to_front(self.special_window)

    def on_move(self):
        self.special_window.set_position(self.get_position())

    def on_resize(self):
        self.special_window.set_size(self.get_size())
