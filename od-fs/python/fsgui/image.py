import os

import fsgui_image  # type: ignore
from fsapp2 import fsapp


class Image:
    @classmethod
    def from_resource(cls, name: str) -> "Image":
        # FIXME
        path = os.path.join(fsapp.data_dir, name)

        # import fsapp

        # image._fsgui_image = fsgui_image.load(path)
        # image.width, image.height = fsgui_image.get_size(image._fsgui_image)

        image = Image(fsgui_image.load(path))
        return image

    def __init__(self, _image):
        self._image = _image
        self.width, self.height = fsgui_image.get_size(self._image)

    # def get_size(self) -> tuple[int, int]:
    #     raise NotImplementedError()

    # def load(self) -> tuple[int, int]:
    #     raise NotImplementedError()
