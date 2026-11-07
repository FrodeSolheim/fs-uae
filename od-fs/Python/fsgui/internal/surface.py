# from io import BytesIO
from typing import Tuple

import _fsapp_surface  # type: ignore

from fsgui.internal.id import get_next_id
from fsgui.types import Size


class Surface:
    def __init__(self, size: Tuple[int, int], special=0):
        self._special = special
        # self._buffer = BytesIO()
        # self._buffer.write(b"\xff\xff\x00\xff" * size[0] * size[1])
        # self._buffer = Image.new("RGBA", (640, 480))
        # self._image = Image.frombuffer("RGBA", (640, 480), self._buffer.getbuffer())

        self._position = 0, 0
        self._surface = _fsapp_surface.create(size, special)
        self._size = size
        self._z_index = 0

        # self._image = Image.frombuffer(
        #     "RGBA", size, self._buffer.getbuffer(), "raw", "RGBA", 0, 1
        # )
        # # self.id = id(self)
        # # print(self.id)
        self.id = get_next_id()

    def destroy(self) -> None:
        _fsapp_surface.destroy(self._surface)
        self._surface = None

    # def update(self):
    #     im = self._image
    #     draw = ImageDraw.Draw(im)
    #     # "BGRA"
    #     # fill = (0, 255, 0, 255)
    #     # fill = (255, 0, 0, 255)
    #     # G B - R
    #     fill = (255, 0, 0, 255)
    #     draw.rectangle([(0, 0), (im.size[0] - 1, im.size[1] - 1)], fill)
    #     draw.line([(200, 10), (400, 10)], fill=128, width=4)

    # def get_buffer(self):
    #     # return self._buffer.getbuffer()
    #     # return self._image.tobytes("raw", "RGBA")
    #     return self._image.tobytes("raw", "RGBA")

    def get_size(self) -> Size:
        return self._size

    def get_surface(self):
        return self._surface

    def get_surface_tuple(self):
        return self._surface, self._position

    def resize(self, size: Size):
        _fsapp_surface.resize(self._surface, size)
        self._size = size

    def set_position(self, position: Tuple[int, int]):
        self._position = position
        # fsapp.set_surface_position(self._surface, position)
        _fsapp_surface.set_position(self._surface, position)

    def set_visible(self, visible: bool) -> None:
        _fsapp_surface.set_visible(self._surface, visible)

    def set_z_index(self, z_index: float) -> None:
        if z_index != self._z_index:
            # print(z_index, "vs", self._z_index)
            _fsapp_surface.set_z_index(self._surface, z_index)
            self._z_index = z_index
