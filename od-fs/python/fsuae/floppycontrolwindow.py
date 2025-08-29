import logging
import os
from typing import Self

from fsgui.button import Button
from fsgui.checkbox import CheckBox
from fsgui.choice import Choice
from fsgui.filedialog import FileDialog
from fsgui.heading import Heading
from fsgui.item import Item
from fsgui.label import Label
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.slider import DiscreteSlider
from fsgui.spacer import Spacer
from fsgui.widget import Widget
from fsgui.window import Window

from fsuae.servicecontainer import ServiceContainer
from fsuae.uaeconfig import UAEConfig
from fsuae.uaeconfig2 import UAEConfig2
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


# --------------------------------------------------------------------------------------------------


class FloppyEnabledCheckBox(CheckBox):
    """

    FIXME: There is a tiny bit of flickering when enabling this checkbox, and it seems to be
    because we first change this value to True, but on the next update from UAE, the value
    is (still?) False, and then on the next frame it gets changed to True.

    PAINT: CHECKED changed to  True
    apply option floppy3type = 0
    [UAE] [000] Set option floppy3type = "0"
    [UAE] [000]  -- stub -- my_resolvesoftlink
    -- stub -- my_resolvesoftlink
    [UAE] [000]  -- stub -- my_resolvesoftlink
    -- stub -- my_resolvesoftlink
    DISK INSERT (df3):
    DEBUG:fsuae.uaeconfigservice:-----> 'floppy3type' <- '-1'
    PAINT: CHECKED changed to  False
    DEBUG:fsuae.uaeconfigservice:-----> 'floppy3type' <- '0'
    PAINT: CHECKED changed to  True

    !!!
    IMPORTANT: Make sure that the config differ checks against config2!!! to make sure that
    not correct options are reset to the real UAE value
    !!!

    """

    def __init__(self, config2: UAEConfig2, drive_index: int):
        super().__init__(f"DF{drive_index}:")
        self.config = config2
        self.drive_index = drive_index

        def str_to_bool(value: str) -> bool:
            return value != "-1"

        self.checked_state.bind(config2.floppy_n_type[drive_index].map(str_to_bool))

        # self.on(self.checked_state.change, self.__on_change)
        self.on(self.checked_change, self.__on_change)
        # FIXME: Something like this?
        self.on(self.checked_state, lambda _: self.refresh(), immediate=False)

        # self.on()
        # self.checked.change.

        # On - binds/connects to a source - making sure to disconnect from the source when calling
        # destroy
        # self.on(self.checked.change, self.on_change)

    # FIXME: @override base implementation

    # @override
    def __on_change(self, value: bool) -> None:
        print("FloppyEnabledCheckBox")
        if value:
            # FIXME: What type to set to, when enabling?
            self.config.floppy_n_type[self.drive_index].set("0")
        else:
            self.config.floppy_n_type[self.drive_index].set("-1")
            # Good idea to eject disk also, probably...
            self.config.floppy_n[self.drive_index].set("")

    # def __on_change(self, value: bool) -> None:
    #     pass

    # def on_check

    # self.state = config.floppy_type[drive_index]
    # self.drive_index = drive_index
    # self.state.connect(self.__on_config)

    # self.connect(config.floppy_enabled[drive_index], self.set_value)
    # self.connect(config.floppy_enabled[drive_index], self.set_value)

    # FIXME: set_enabled(bool | Var[bool])

    # self.connect(config2.floppy_n_type[drive_index], self.set_value)
    # self.connect_bool(config2.floppy_n_type[drive_index], self.set_enabled)

    # self.connect_bool(config2.floppy_n_type[drive_index], str_to_bool)
    # config2.floppy_n_type[drive_index].connect(self.set_enabled)

    # When the checkbox dies, it is important that the MappedVar disconnects from
    # the source var...

    # self.set_value(config2.floppy_n_type[drive_index].map(str_to_bool))
    # FIXME
    # self.checked.bind(config2.floppy_n_type[drive_index].map(str_to_bool))

    # _enabled_var ?
    # set_enabled must call _set_enabled in this case, so that the var can broadcast to
    # _set_enabled directly and not break the bond.

    # def set_enabled(self, enabled: bool | Var[bool]) -> None:
    #     # FIXME: + Listen
    #     if isinstance(enabled, Var):
    #         super().set_enabled(enabled.value)
    #     else:
    #         super().set_enabled(enabled)

    # def __on_config(self, value):
    #     pass

    # def on_change(self):
    #     # state = self.config.floppy_type[drive_index]
    #     logger.debug("on_change %r", self.value)
    #     if self.value:
    #         self.state.set(0)
    #     else:
    #         self.state.set(-1)


# --------------------------------------------------------------------------------------------------


class WriteProtectCheckBox(CheckBox):
    """
    FIXME: This is a tricky one... floppy{0,1,2,3}wp state is not reflected in currprefs until
    a new disk is inserted, so checking this box will just briefly flash it before turning it on
    again -- and when you actually insert a disk it will "automatically" turn on.

    Maybe better with a dropdown (more) menu where you choose "Insert (write-protected) instead"..?
    """

    def __init__(self, config: UAEConfig2, drive_index: int):
        super().__init__("Write-protect")

        self.drive_index = drive_index
        # self.state = config.floppy_n_wp[drive_index]
        self.bind_checked(config.floppy_n_wp[drive_index].map(lambda x: x == "true"))
        self.on(self.checked_change, self.__on_change)

    def __on_change(self, value: bool):
        from fsuae.message2 import Message2
        from fsuae.messages import post_fsuae_message

        # self.state.set("true" if value else "false")

        if value:
            message = Message2.WRITE_PROTECT_DRIVE_0 + self.drive_index
        else:
            message = Message2.UN_WRITE_PROTECT_DRIVE_0 + self.drive_index
        post_fsuae_message(message)


# --------------------------------------------------------------------------------------------------


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


# --------------------------------------------------------------------------------------------------


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


# --------------------------------------------------------------------------------------------------


class FloppyTypeChoice(Choice):
    def __init__(self, config2: UAEConfig2, drive_index: int) -> None:
        # self.config = UAEConfig()
        self.config2 = config2
        self.state = self.config2.floppy_n_type[drive_index]

        # d2p = drive_index >= 2
        items = [
            # '',
            Item('3.5" DD', data="0"),
            Item('3.5" HD', data="1"),
            Item('5.25" (40)', data="2"),
            Item('5.25" (80)', data="7"),
            Item('3.5" ESCOM', data="3"),
            # MenuItem('Bridgeboard 5.25" 40', data=4).set_enabled(d2p),
            # MenuItem('Bridgeboard 5.25" 80', data=6).set_enabled(d2p),
            # MenuItem('Bridgeboard 3.5" 80', data=5).set_enabled(d2p),
        ]
        super().__init__(items)

        # self.on(self.config2.floppy_n_enabled[drive_index], self.)
        # self.enabled_state.bind(self.config2.floppy_n_enabled[drive_index])

        self.on(self.config2.floppy_n_type[drive_index], self.__config)
        # FIXME: set_enabled?
        # self.connect_enabled(config.floppy_enabled[drive_index])
        # self.connect(config.floppy_type[drive_index], self.__config)

    def __config(self, value: str) -> None:
        # intvalue = int(value)
        if value == "-1":
            index = 0
        else:
            index = self.find_index_by_data(value)
            if index is None:
                logger.warning("Could not find item for value %r", value)
        if index is not None:
            self.set_index(index)
        else:
            logger.warning("index is None - floppy type '%s' not found", value)

    def on_change(self) -> None:
        index = self.index
        if index == 0 and self.state.value == "-1":
            # The current choice isn't valid, we want to keep -1. This is
            # controlled by the enable checkbox.
            return
        self.state.set(self.items[self.index].data)


# --------------------------------------------------------------------------------------------------


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


# --------------------------------------------------------------------------------------------------


class FloppyDriveWidget(Widget):
    def __init__(self, configx: UAEConfig, drive_index: int) -> None:
        super().__init__()
        self.configx = configx

        services = ServiceContainer.instance()
        config = services.uae_config.config2

        # FIXME: fill param...!
        # FIXME: pass param instead
        # self.fill = True

        # drive = f"df{drive_index}"

        def on_insert_floppy_accept(dialog: FileDialog) -> None:
            path = dialog.get_file()
            self.text_field.set_text(os.path.basename(path))
            # fsemu.set(f"floppy{drive_index}", path)
            config.floppy_n[drive_index].set(path)

        def on_insert_floppy() -> None:
            FileDialog(self, on_accept=on_insert_floppy_accept).show()

        VerticalLayout(gap=8)
        with HorizontalLayout(gap=8):
            # FloppyEnabledCheckBox(f"{drive.upper()}:")
            FloppyEnabledCheckBox(config, drive_index)
            Spacer().expand()
            Spacer(width=12)
            # FloppyTypeChoice(self.config, drive_index)
            FloppyTypeChoice(config, drive_index).bind_enabled(config.floppy_n_enabled[drive_index])
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

            # Button("Eject", on_activate=on_eject_floppy).connect(
            #     self.config.floppy_inserted_filtered[drive_index],
            #     "set_enabled",
            # )

            WriteProtectCheckBox(config, drive_index).bind_enabled(
                config.floppy_n_enabled[drive_index]
            )
            # Button("...", on_activate=on_insert_floppy).on(
            #     config.floppy_n_enabled[drive_index], "set_enabled")

            Button("...", on_activate=on_insert_floppy).bind_enabled(
                config.floppy_n_enabled[drive_index]
            )

            # self.text_field.set_text("")

            # def eject_floppy() -> None:
            #     config.floppy_n[drive_index].set("")

            # Button("Eject").bind_enabled(
            #     config.floppy_n[drive_index].map(lambda x: x != "")
            # ).on_activate(eject_floppy)

            Button("Eject").bind_enabled(
                config.floppy_n[drive_index].map(lambda x: x != "")
            ).on_activate(lambda: config.floppy_n[drive_index].set(""))

            # .connect_enabled(
            #    self.config.floppy_enabled[drive_index]
            # )

        # with HorizontalLayout(gap=8):
        #     TextField(f"{drive}.adf")
        #     Button("...")
        # self.text_field = TextField(f"{drive}.adf")

        # Hmm, maybe...
        # Hmm, maybe Spacer().expand()
        with HorizontalLayout(gap=8):
            self.text_field = (
                UaeConfigTextField(config.floppy_n[drive_index])
                .expand()
                # FIXME: Include this in constructor? Maybe? Maybe not?
                # .connect_enabled(self.config.floppy_enabled[drive_index])
                .bind_enabled(config.floppy_n_enabled[drive_index])
            )

        self.text_field.fill = True
