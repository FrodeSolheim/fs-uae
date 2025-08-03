from typing import Self

from fsgui.label import Label
from fsgui.layout import VerticalLayout
from fsgui.window import Window
from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfigwidgets import UaeConfigCheckBox


class DisplayWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Display", padding=24, size=(600, 200))
        self.move((176, 320))

        services = ServiceContainer().instance()
        config = services.uae_config.config2

        VerticalLayout(gap=24)

        Label("Centering:")
        UaeConfigCheckBox(
            "Horizontal", config.gfx_center_horizontal, "smart", "none"
        )
        UaeConfigCheckBox(
            "Vertical", config.gfx_center_vertical, "smart", "none"
        )
