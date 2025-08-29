from typing import Self

from fsgui.choice import Choice
from fsgui.layout import VerticalLayout
from fsgui.window import Window

from fsuae.floppycontrolwindow import MenuItem
from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfig import UAEConfig


class CPUWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("CPU", padding=24, size=(600, 200))
        self.move((176, 520))

        VerticalLayout()

        services = ServiceContainer().instance()
        uae_config = services.uae_config.uae_config

        CPUModelChoice(uae_config)


class CPUModelChoice(Choice):
    def __init__(self, config: UAEConfig) -> None:
        # self.config = UAEConfig()
        self.state = config.cpu_model

        # items = []
        # for cpu_type in [
        #     68000,
        #     68010,
        #     68020,
        #     68030,
        #     68040,
        #     68060,
        # ]:
        #     items.append(MenuItem(str(cpu_type), data=cpu_type))

        items = [
            MenuItem(str(x), data=x)
            for x in [
                68000,
                68010,
                68020,
                68030,
                68040,
                68060,
            ]
        ]

        super().__init__(items)

        # FIXME: set_enabled?
        # self.connect_enabled(config.floppy_enabled[drive_index])
        self.connect(self.state, self.__config)

    def __config(self, value) -> None:
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


class MMUModelChoice(Choice):
    def __init__(self, config: UAEConfig) -> None:
        # self.config = UAEConfig()
        self.state = config.cpu_model

        items = [
            MenuItem("None", data=0),
            MenuItem("MMU", data=0),
            MenuItem("EC", data=0),
        ]

        super().__init__(items)

        # FIXME: In order to be able to check multiple config values at the
        # same time in order to determine the correct UI, all config values
        # should be updated before signals/changes are broadcast...?

        # FIXME: Maybe to all logic changes outside UI components, and let UI
        # components be more dumb (yes, please!)

        # So, separate uae_config/state and uae *UI* state/config

        self.connect(self.state, self.__config)

    def __config(self, value) -> None:
        # value = str(value)
        for item in self.items:
            if item.data == value:
                self.set_index(item.index)
                break

    def on_change(self) -> None:
        value = self.items[self.index].data
        self.state.set(value)
