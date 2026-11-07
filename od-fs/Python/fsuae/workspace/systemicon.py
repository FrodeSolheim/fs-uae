from fsgui.borderlesswindow import BorderlessWindow
from fsgui.font import Font
from fsgui.imagelabel import Image


class SystemIcon(BorderlessWindow):
    def __init__(self):
        size = (100, 100)
        super().__init__("", size=size)
        # self.move((40, 40 + 36))
        self.move((180, 40 + 36))

        self.style.background_color = (0xAA, 0xAA, 0xAA, 0xFF)
        self.image = Image.from_resource("Drive.png")
        self.font = Font(weight=Font.MEDIUM)

    def on_paint(self) -> None:
        dc = self.create_dc()
        dc.set_fill_colour(self.style.background_color)
        dc.draw_filled_rectangle((0, 0), self.get_size())
        dc.draw_image(self.image, (16, 10))
        dc.draw_text("System", (30, 80))
