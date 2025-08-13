import os

import fsgui_image  # type: ignore
from fsapp import fsapp


class Image:
    @classmethod
    def from_resource(cls, name: str) -> "Image":
        for images_dir in fsapp.images_dirs:
            image_path = os.path.join(images_dir, name)
            print(image_path)
            if os.path.exists(image_path):
                break
        else:
            raise FileNotFoundError(name)

        image = Image(fsgui_image.load(image_path))
        return image

    def __init__(self, _image):
        self._image = _image
        self.width, self.height = fsgui_image.get_size(self._image)

    # def get_size(self) -> tuple[int, int]:
    #     raise NotImplementedError()

    # def load(self) -> tuple[int, int]:
    #     raise NotImplementedError()
