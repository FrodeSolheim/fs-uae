from typing import Self

from fsgui.choice import Choice
from fsgui.item import Item
from fsgui.layout import VerticalLayout
from fsgui.window import Window

from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfig2 import UAEConfig2


class AdvChipsetWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Advanced chipset", padding=24, size=(600, 200))
        self.move((176, 220))

        VerticalLayout()

        services = ServiceContainer().instance()
        ChipsetCompatibleChoice(services.uae_config.config2)


class ChipsetCompatibleChoice(Choice):
    def __init__(self, config: UAEConfig2) -> None:
        # self.config = UAEConfig()
        self.state = config.chipset_compatible

        items = [
            Item(str(x), data=x)
            for x in [
                "-",
                "Generic",
                "CDTV",
                "CDTV-CR",
                "CD32",
                "A500",
                "A500+",
                "A600",
                "A1000",
                "A1200",
                "A2000",
                "A3000",
                "A3000T",
                "A4000",
                "A4000T",
                "Velvet",
                "Casablanca",
                "DraCo",
            ]
        ]

        super().__init__(items)

        # FIXME: set_enabled?
        # self.connect_enabled(config.floppy_enabled[drive_index])
        # self.connect(self.state, self.__config)
        self.on(self.state, self.__config)

    def __config(self, value: str) -> None:
        # value = str(value)
        for item in self.items:
            if item.data == value:
                self.set_index(item.index)
                break

    def on_change(self) -> None:
        value = self.items[self.index].data
        self.state.set(value)


# FIXME: Make a choice class that interacts with an UAE option
# (int or list-based) automatically?
