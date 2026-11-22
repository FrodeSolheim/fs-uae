import logging
from typing import ClassVar, Self

import _fsapp  # type: ignore
import fsemu
from fsapp.surfaceapi.v1 import Keycode, Keymod
from fsgui.button import Button
from fsgui.heading import Heading
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.signal import Signal
from fsgui.window import Window
from uae.inputevent import InputEvent

from fsuae.floppycontrolwindow import FloppyDriveWidget
from fsuae.input.inputcontrolwindow import InputControlWidget
from fsuae.message import Message
from fsuae.messages import post_fsuae_message
from fsuae.servicecontainer import ServiceContainer

logger = logging.getLogger(__name__)


class F12Window(Window):
    _instance: ClassVar[Self]
    # Emitted when the F12 window has opened (been created) or closed (has_instance tells which).
    open_changed: ClassVar[Signal[[]]] = Signal()

    @classmethod
    def has_instance(cls) -> bool:
        return hasattr(cls, "_instance")

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance
            cls.open_changed.emit()

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
            cls.open_changed.emit()
        return cls._instance

    @classmethod
    def toggle(cls) -> None:
        """Open the F12 window, or close it if it is open."""
        if hasattr(cls, "_instance"):
            cls._instance.close()
        else:
            cls.instance().center_window().show()

    @classmethod
    def setup(cls, services: ServiceContainer):
        _fsapp.register_shortcut(Keycode.F12, Keymod.NONE, cls.toggle)

    def __init__(self) -> None:
        super().__init__("Quick settings (Temporary UI)", padding=24)
        self.move((176, 36 + 36 + 24))

        VerticalLayout(gap=24)

        services = ServiceContainer().instance()
        # uae_config = services.uae_config.uae_config

        with HorizontalLayout(gap=12):
            Button("Open data directory").on_activate(self.open_data_folder)

        with HorizontalLayout(gap=12):
            # Basically, use copy_prefs, memory_hardreset as in gui_to_prefs ?
            Button("Reset").on_activate(lambda: fsemu.post(InputEvent.SPC_HARDRESET))

            def quickstart(quickstart_message: int):
                # Quickstart updated changed_prefs
                post_fsuae_message(quickstart_message)
                # Hard reset message implicitly copies changed_prefs -> currprefs,
                # clears memory and then does a hard reset
                post_fsuae_message(Message.HARD_RESET)

            Button("A500").on_activate(lambda: quickstart(Message.QUICKSTART_A500))
            Button("A500+").on_activate(lambda: quickstart(Message.QUICKSTART_A500P))
            Button("A600").on_activate(lambda: quickstart(Message.QUICKSTART_A600))
            Button("A1200").on_activate(lambda: quickstart(Message.QUICKSTART_A1200))
            Button("A3000").on_activate(lambda: quickstart(Message.QUICKSTART_A3000))
            Button("A4000").on_activate(lambda: quickstart(Message.QUICKSTART_A4000))

        Heading("Joystick and mouse ports")
        with VerticalLayout(gap=12):
            InputControlWidget(
                services.uae_config.config2, services.input_ports, services.input_devices, 0
            ).fill()
            InputControlWidget(
                services.uae_config.config2, services.input_ports, services.input_devices, 1
            ).fill()

        Heading("Floppy drives")
        FloppyDriveWidget(services.uae_config.uae_config, 0).fill()
        FloppyDriveWidget(services.uae_config.uae_config, 1).fill()
        FloppyDriveWidget(services.uae_config.uae_config, 2).fill()
        FloppyDriveWidget(services.uae_config.uae_config, 3).fill()

        # with HorizontalLayout(gap=12):
        #     Button("Reset").on_activate(lambda: fsemu.post(InputEvent.SPC_HARDRESET))

        self.resize_to_fit_content()

    def __del__(self) -> None:
        print("__del__", self)

    def open_data_folder(self):
        services = ServiceContainer().instance()
        base_dir = services.path.data
        url = f"file://{base_dir}"
        logger.info("Open data folder %s", url)

        _fsapp.open_url(url)  # type: ignore
