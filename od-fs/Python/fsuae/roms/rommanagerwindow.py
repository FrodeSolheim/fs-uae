from typing import Self

from fsgui.layout import VerticalLayout
from fsgui.window import Window


class ROMManagerWindow(Window):
    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("ROM Manager", padding=24, size=(600, 600))
        self.move((176, 100))

        VerticalLayout()

        # services = ServiceContainer().instance()
        # uae_config = services.uae_config.uae_config
