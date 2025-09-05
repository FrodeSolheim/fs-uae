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

        _image = fsgui_image.load(image_path)  # type: ignore
        image = Image(_image)  # type: ignore
        return image

    def __init__(self, _image: object) -> None:
        self._image = _image
        self.width: int
        self.height: int
        self.width, self.height = fsgui_image.get_size(self._image)  # type: ignore

    def tint(self, r: int, g: int, b: int, a: int = 255) -> None:
        """Tint this image with the specified RGBA color."""
        fsgui_image.tint(self._image, r, g, b, a)  # type: ignore

    # def get_size(self) -> tuple[int, int]:
    #     raise NotImplementedError()

    # def load(self) -> tuple[int, int]:
    #     raise NotImplementedError()
