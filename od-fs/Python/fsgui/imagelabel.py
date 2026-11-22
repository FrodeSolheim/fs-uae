from .image import Image
from .scale import get_scale, units_from_pixel_size
from .types import Size
from .widget import Widget


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

    def calculate_min_height(self, width: float) -> float:
        return self._image_size()[1]

    def calculate_min_width(self) -> float:
        return self._image_size()[0]

    def _image_size(self) -> Size:
        """The image's size in pixels in design units (rounded up)."""
        return units_from_pixel_size((self.image.width, self.image.height), get_scale())

    def on_paint(self):
        dc = self.create_dc()
        dc.draw_image(self.image, (0, 0))
