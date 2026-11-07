from typing import Self

from fsgui.layout import VerticalLayout
from fsgui.window import Window


class ChipsetWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Chipset", padding=24, size=(600, 200))
        self.move((176, 160))

        VerticalLayout()

        # services = ServiceContainer().instance()
        # uae_config = services.uae_config.uae_config
