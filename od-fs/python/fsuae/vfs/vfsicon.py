import os
from typing import Self

from fsgui.image import Image


class VFSIcon:
    _instance: Self | None = None

    @staticmethod
    def get_name_from_path(path: str) -> str:
        if path.endswith(":"):
            name = path[:-1]
        else:
            name = path.split(":")[1].split("/")[-1]
        return name

    def __init__(self, path: str, position: tuple[int, int]):
        self.path = path
        self.name = self.get_name_from_path(path)

        self.position = position
        # self.x = 0
        # self.y = 0
        self.size = (76, 76)
        # self.width = 76
        # self.height = 76
        self.image = None
        self.selected_image = None

    def get_image(self) -> Image:
        if self.image is None:
            self.load_image()
        return self.image

    def load_image(self) -> None:
        resource_path = "Icons/" + self.path.replace(":", "/") + ".png"
        if os.path.exists("data/" + resource_path):
            self.image = Image.from_resource(resource_path)
        else:
            self.image = Image.from_resource("Icons/Directory.png")
