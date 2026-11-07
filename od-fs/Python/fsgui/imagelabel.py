from fsgui.image import Image
from fsgui.widget import Widget


class ImageLabel(Widget):
    def __init__(
        self,
        image: Image,
        # *,
        # parent: Widget | None = None,
        # size: Size | None = None,
    ):
        super().__init__()
        self.image = image

    def calculate_min_height(self, width: int) -> int:
        return self.image.height

    def calculate_min_width(self) -> int:
        return self.image.width

    def on_paint(self):
        dc = self.create_dc()
        dc.draw_image(self.image, (0, 0))
