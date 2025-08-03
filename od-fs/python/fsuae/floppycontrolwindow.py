import logging
import os
from typing import Self

from fsgui.button import Button
from fsgui.checkbox import CheckBox
from fsgui.choice import Choice, MenuItem
from fsgui.filedialog import FileDialog
from fsgui.heading import Heading
from fsgui.label import Label
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.slider import DiscreteSlider
from fsgui.spacer import Spacer
from fsgui.widget import Widget
from fsgui.window import Window

from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfig import UAEConfig
from fsuae.uaeconfigwidgets import UaeConfigTextField

logger = logging.getLogger(__name__)


# FIXME
class Params:
    fill: bool = False


class FloppyControlWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("Floppy drives", padding=24, size=(600, 640))

        self.move((176, 36 + 36 + 24))

        self.config = UAEConfig()

        VerticalLayout(gap=24)

        Heading("Floppy drives")
        with VerticalLayout(gap=16):
            FloppyDriveWidget(self.config, 0).fill()
            FloppyDriveWidget(self.config, 1).fill()
            FloppyDriveWidget(self.config, 2).fill()
            FloppyDriveWidget(self.config, 3).fill()

        Heading("Floppy drive emulation speed")
        with HorizontalLayout(gap=64):
            FloppySpeedSlider(self.config)
            FloppySpeedLabel(self.config)

        Heading("Floppy drive sound emulation")
        FloppyDriveSoundChoice(self.config, 0)


class FloppyEnabledCheckBox(CheckBox):
    def __init__(self, config: UAEConfig, drive_index: int):
        super().__init__(f"DF{drive_index}:")
        self.config = config
        self.state = config.floppy_type[drive_index]
        # self.drive_index = drive_index
        # self.state.connect(self.__on_config)

        self.connect(config.floppy_enabled[drive_index], self.set_value)

    def __on_config(self, value):
        pass

    def on_change(self):
        # state = self.config.floppy_type[drive_index]
        logger.debug("on_change %r", self.value)
        if self.value:
            self.state.set(0)
        else:
            self.state.set(-1)


class WriteProtectCheckBox(CheckBox):
    def __init__(self, config: UAEConfig, drive_index: int):
        super().__init__("Write-protect")

        self.state = config.floppy_wp[drive_index]
        self.state.connect(self.__on_config)

        # self.enabled_state = config.floppy_type[drive_index]
        # self.enabled_state.connect(self.__on_config_enabled)

        # self._listeners = []

        self.connect(config.floppy_wp[drive_index], self.__on_config)
        # self.connect(config.floppy_type[drive_index], self.__on_config_enabled)
        # self.connect(config.floppy_enabled[drive_index], self.__on_config_enabled)
        self.connect(config.floppy_enabled[drive_index], self.set_enabled)

    def __on_config(self, value):
        self.set_value(value)

    # def __on_config_enabled(self, value):
    #     # self.set_enabled(value != -1)
    #     self.set_enabled(value)

    def on_change(self):
        self.state.set(self.value)

    # def connect(self, state, listener):
    #     self._listeners.append((state, listener))
    #     state.connect(listener)
    #     listener(state.value)

    # def _on_destroy(self):
    #     for state, listener in self._listeners:
    #         state.disconnect(listener)

    # self.state.disconnect(self.__on_config)
    # self.enabled_state.disconnect(self.__on_config_enabled)


class FloppySpeedSlider(DiscreteSlider):
    def __init__(self, config: UAEConfig) -> None:
        super().__init__(5)
        self.config = config
        self.connect(config.floppy_speed, self.__config)

    def __config(self, value):
        if value == 0:
            value = 5
        elif value == 100:
            value = 1
        elif value == 200:
            value = 2
        elif value == 400:
            value = 3
        elif value == 800:
            value = 4
        else:
            value = 1
        self.set_value(value)

    def on_change(self):
        value = self.value
        if value == 5:
            value = 0
        elif value == 1:
            value = 100
        elif value == 2:
            value = 200
        elif value == 3:
            value = 400
        elif value == 4:
            value = 800
        else:
            value = 100
        self.config.floppy_speed.set(value)


class FloppySpeedLabel(Label):
    def __init__(self, config: UAEConfig) -> None:
        super().__init__()
        self.connect(config.floppy_speed, self.__config)

    def __config(self, value):
        if value == 0:
            text = "Turbo"
        elif value == 100:
            text = "100% (compatible)"
        else:
            text = f"{value}%"
        self.set_text(text)


class FloppyTypeChoice(Choice):
    def __init__(self, config: UAEConfig, drive_index: int) -> None:
        self.config = UAEConfig()
        self.state = config.floppy_type[drive_index]

        # d2p = drive_index >= 2
        items = [
            # '',
            MenuItem('3.5" DD', data=0),
            MenuItem('3.5" HD', data=1),
            MenuItem('5.25" (40)', data=2),
            MenuItem('5.25" (80)', data=7),
            MenuItem('3.5" ESCOM', data=3),
            # MenuItem('Bridgeboard 5.25" 40', data=4).set_enabled(d2p),
            # MenuItem('Bridgeboard 5.25" 80', data=6).set_enabled(d2p),
            # MenuItem('Bridgeboard 3.5" 80', data=5).set_enabled(d2p),
        ]
        super().__init__(items)

        # FIXME: set_enabled?
        self.connect_enabled(config.floppy_enabled[drive_index])
        self.connect(config.floppy_type[drive_index], self.__config)

    def __config(self, value) -> None:
        if value == -1:
            index = 0
        else:
            index = self.find_index_by_data(value)
            if index is None:
                logger.warning("Could not find item for value %r", value)
        self.set_index(index)

    def on_change(self) -> None:
        index = self.index
        if index == 0 and self.state.get() == -1:
            # The current choice isn't valid, we want to keep -1. This is
            # controlled by the enable checkbox.
            return
        self.state.set(self.items[self.index].data)


class FloppyDriveSoundChoice(Choice):
    def __init__(self, config: UAEConfig, drive_index: int) -> None:
        # self.config = config
        self.state = config.floppy_sound[drive_index]

        # FIXME: Custom sounds, how are they configured?
        items = [
            "No floppy drive sound",
            "A500 (UAE built-in)",
        ]

        super().__init__(items)

        # FIXME: set_enabled?
        # self.connect_enabled(config.floppy_enabled[drive_index])
        self.connect(self.state, self.__config)

    def __config(self, value) -> None:
        self.set_index(value)

    def on_change(self) -> None:
        index = self.index
        self.state.set(index)


class FloppyDriveWidget(Widget):
    def __init__(self, config, drive_index: int) -> None:
        super().__init__()
        self.config = config

        # FIXME: fill param...!
        # FIXME: pass param instead
        # self.fill = True

        # drive = f"df{drive_index}"

        def on_insert_floppy_accept(dialog: FileDialog) -> None:
            path = dialog.get_file()
            self.text_field.set_text(os.path.basename(path))
            # fsemu.set(f"floppy{drive_index}", path)
            self.config.floppy[drive_index].set(path)

        def on_insert_floppy() -> None:
            FileDialog(self, on_accept=on_insert_floppy_accept).show()

        def on_eject_floppy() -> None:
            self.text_field.set_text("")
            # fsemu.set(f"floppy{drive_index}", "")
            self.config.floppy[drive_index].set("")

        VerticalLayout(gap=8)
        with HorizontalLayout(gap=8):
            # FloppyEnabledCheckBox(f"{drive.upper()}:")
            FloppyEnabledCheckBox(self.config, drive_index)
            Spacer().expand()
            Spacer(width=12)
            FloppyTypeChoice(self.config, drive_index)
            # Choice(
            #     [
            #         '3.5" DD',
            #         '3.5" HD',
            #         '5.25" (40)',
            #         '5.25" (80)',
            #         '3.5" ESCOM',
            #     ]
            # )
            # Spacer(width=12)

            # Actually, maybe set set_enabled take a state var..!
            # FIXME: connect_enabled vs set_enabled...
            # Button("?").connect_enabled(
            #     uae_config.floppy_inserted[drive_index]
            # )

            # Actually, maybe set set_enabled take a state var..!
            # FIXME: connect_enabled vs set_enabled...
            Button("Eject", on_activate=on_eject_floppy).connect(
                self.config.floppy_inserted_filtered[drive_index],
                "set_enabled",
            )
            WriteProtectCheckBox(self.config, drive_index)
            Button("...", on_activate=on_insert_floppy).connect_enabled(
                self.config.floppy_enabled[drive_index]
            )

        # with HorizontalLayout(gap=8):
        #     TextField(f"{drive}.adf")
        #     Button("...")
        # self.text_field = TextField(f"{drive}.adf")

        services = ServiceContainer.instance()
        config = services.uae_config.config2

        # Hmm, maybe...
        # Hmm, maybe Spacer().expand()
        with HorizontalLayout(gap=8):
            self.text_field = (
                UaeConfigTextField(config.floppy_n[drive_index])
                .expand()
                # FIXME: Include this in constructor? Maybe? Maybe not?
                .connect_enabled(self.config.floppy_enabled[drive_index])
            )

        self.text_field.fill = True
