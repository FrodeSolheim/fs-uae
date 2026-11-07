from typing import Self

import fsemu
from fsgui.button import Button
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.window import Window
from uae.inputevent import InputEvent

from fsuae.floppycontrolwindow import FloppyDriveWidget
from fsuae.input.inputcontrolwindow import InputControlWidget
from fsuae.servicecontainer import ServiceContainer


class QuickStartWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Quick Start", padding=24)
        self.move((176, 36 + 36 + 24))

        VerticalLayout(gap=24)

        services = ServiceContainer().instance()
        # uae_config = services.uae_config.uae_config

        with HorizontalLayout(gap=12):
            Button("A500").on_activate(lambda: fsemu.set("quickstart", "A500,0"))
            Button("A500+").on_activate(lambda: fsemu.set("quickstart", "A500+,0"))
            Button("A600").on_activate(lambda: fsemu.set("quickstart", "A600,0"))
            Button("A1200").on_activate(lambda: fsemu.set("quickstart", "A1200,0"))
            Button("A3000").on_activate(lambda: fsemu.set("quickstart", "A3000,0"))
            Button("A4000").on_activate(lambda: fsemu.set("quickstart", "A4000,0"))
            Button("Reset").on_activate(lambda: fsemu.post(InputEvent.SPC_HARDRESET))

        with VerticalLayout(gap=12):
            InputControlWidget(
                services.uae_config.config2, services.input_ports, services.input_devices, 0
            ).fill()
            InputControlWidget(
                services.uae_config.config2, services.input_ports, services.input_devices, 1
            ).fill()

        FloppyDriveWidget(services.uae_config.uae_config, 0).fill()

        FloppyDriveWidget(services.uae_config.uae_config, 1).fill()

        self.resize_to_fit_content()
