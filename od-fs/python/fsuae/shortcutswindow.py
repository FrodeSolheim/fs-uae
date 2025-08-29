import logging
import sys
from typing import Self

from fsgui.button import Button
from fsgui.layout import VerticalLayout
from fsgui.test.widgettestwindow import WidgetTestWindow
from fsgui.window import Window

import fsuae.messages
from fsuae.aboutwindow import AboutWindow
from fsuae.advchipsetwindow import AdvChipsetWindow
from fsuae.chipsetwindow import ChipsetWindow
from fsuae.cpuwindow import CPUWindow
from fsuae.displaywindow import DisplayWindow
from fsuae.floppycontrolwindow import FloppyControlWindow
from fsuae.input.inputcontrolwindow import InputControlWindow
from fsuae.quickstartwindow import QuickStartWindow
from fsuae.roms.rommanagerwindow import ROMManagerWindow

logger = logging.getLogger(__name__)


class ShortcutsWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Shortcuts", padding=8)
        # HorizontalLayout(gap=4)
        VerticalLayout(gap=8)
        # Label("Temp shortcuts:")
        Button("About", on_activate=lambda: AboutWindow.instance().show().none).fill()
        Button(
            "Widgets",
            on_activate=lambda: WidgetTestWindow.instance().show().none,
        ).fill()
        Button(
            "ROMs",
            on_activate=lambda: ROMManagerWindow.instance().show().none,
        ).fill()
        Button(
            "Quick Start",
            on_activate=lambda: QuickStartWindow.instance().show().none,
        ).fill()
        Button(
            "Floppies",
            on_activate=lambda: FloppyControlWindow.instance().show().none,
        ).fill()
        Button(
            "CPU",
            on_activate=lambda: CPUWindow.instance().show().none,
        ).fill()
        Button(
            "Chipset",
            on_activate=lambda: ChipsetWindow.instance().show().none,
        ).fill()
        Button(
            "Adv",
            on_activate=lambda: AdvChipsetWindow.instance().show().none,
        ).fill()
        Button(
            "Input",
            on_activate=lambda: InputControlWindow.instance().show().none,
        ).fill()
        Button(
            "Display",
            on_activate=lambda: DisplayWindow.instance().show().none,
        ).fill()
        Button(
            "Reset",
            on_activate=lambda: self.reset(),
        ).fill()
        Button(
            "Config",
            on_activate=lambda: self.run_uae(),
        ).fill()
        Button("Cybernetix", on_activate=self.run_cybernetix).fill()

        # services = ServiceContainer.instance()
        # InputDeviceSelector(
        #     services.input_ports, services.input_devices, 0
        # ).set_min_width(300).expand()

        self.resize_to_fit_content()
        self.set_width(119)
        # self.move((180, 36 + 24 + 36))
        # self.move((24, 24 + 36))

        # self.move((1235, 8 + 36))
        self.move((8 + 4, 8 + 36))

    def reset(self) -> None:
        # fsemu.post(uae.INPUTEVENT_SPC_HARDRESET)
        fsuae.messages.post_fsuae_message(fsuae.messages.FSUAE_MESSAGE_RESET)

    def run_uae(self):
        last_arg = sys.argv[-1]
        if last_arg.endswith(".uae"):
            last_arg = last_arg
        else:
            last_arg = "Data/Configurations/StateOfTheArt.uae"
        config = []
        with open(last_arg, "r") as f:
            for line in f.readlines():
                line = line.strip()
                try:
                    key, value = line.split("=", 1)
                except ValueError:
                    pass
                value = value.replace("\\", "/")
                config.append((key, value))

        new_config = []
        for key, value in config:
            if key == "input.joymouse_speed_analog":
                logger.warning("BLOCKING %s", key)
                continue
            new_config.append(f"{key}={value}\n")

        new_config_str = "".join(new_config)
        logger.debug(new_config_str)

        fsuae.messages.post_fsuae_message(
            fsuae.messages.FSUAE_MESSAGE_RESTART_WITH_CONFIG,
            "".join(new_config_str),
        )

    def run_cybernetix(self):
        # import fsemu

        # # last_arg = sys.argv[-1]
        # # if last_arg.endswith(".uae"):
        # # config_file = "Data/Configurations/StateOfTheArt.uae"

        # config_file = """
        # kickstart_rom_file=./Data/ROM/AmigaForever/amiga-os-130.rom
        # kickstart_rom_file_id=C4F0F55F,KS ROM v1.3 (A500,A1000,A2000)
        # floppy0=./Floppies/Cybernetix.adf
        # """
        # config = []
        # for line in config_file.split("\n"):
        #     line = line.strip()
        #     try:
        #         key, value = line.split("=", 1)
        #     except ValueError:
        #         continue
        #     value = value.replace("\\", "/")
        #     config.append((key, value))

        # for key, value in config:
        #     fsemu.set(key, value)

        # fsemu.post(uae.INPUTEVENT_SPC_HARDRESET)

        fsuae.messages.post_fsuae_message(
            fsuae.messages.FSUAE_MESSAGE_RESTART_WITH_CONFIG,
            """
            kickstart_rom_file=./Data/ROM/AmigaForever/amiga-os-130.rom
            kickstart_rom_file_id=C4F0F55F,KS ROM v1.3 (A500,A1000,A2000)
            floppy0=./Floppies/Cybernetix.adf
            """,
        )
