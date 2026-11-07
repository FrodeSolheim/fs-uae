from fsgui.button import Size
from fsgui.font import Font
from fsgui.window import Window

from fsuae.vfs.vfsservice import VFSService


class ShellWindow(Window):
    _instance = None

    def __init__(self, path: str, size: Size | None = None):
        # size = (460, 400)
        if size is None:
            size = (320, 160)

        if path.endswith(":"):
            name = path[:-1]
        else:
            name = path.split(":")[1].split("/")[-1]

        super().__init__(name, size=size)
        self.move((160, 110))

        self.style.background_color = (0xEE, 0xEE, 0xEE, 0xFF)
        self.font = Font(weight=Font.MEDIUM)
        self.vfs_service = VFSService.get()
        self.icons = self.vfs_service.get_icons(path)

    def on_paint(self):
        self.paint_border()

        dc = self.create_dc()
        dc.set_fill_colour(self.style.background_color)
        dc.draw_filled_rectangle((0, 0), self.get_size())

        for icon in self.icons:
            image = icon.get_image()

            dc.draw_image(image, icon.position)
            tw, _ = dc.measure_text(icon.name)
            y = icon.position[1] + icon.size[1]
            x = icon.position[0] + (icon.size[0] - tw) // 2
            dc.draw_text(icon.name, (x, y))
