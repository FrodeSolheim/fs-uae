import logging
from typing import Self

from fsemu.inputdevice import InputDevice
from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService
from fsgui.choice import Choice
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.widget import Widget
from fsgui.window import Window
from fsuae.servicecontainer import ServiceContainer

logger = logging.getLogger(__name__)


class InputControlWindow(Window):
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            del cls._instance

        if not hasattr(cls, "_instance"):
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    def __init__(self) -> None:
        super().__init__("InputControl", padding=24)

        # self.move((660, 82))
        # 80)

        VerticalLayout(gap=24)

        # Heading("Input ports")

        # FIXME: Should layouts fill by default? Maybe...

        services = ServiceContainer.instance()

        # input_device_service = InputDeviceService.instance()
        # input_port_service = InputPortService.instance()
        # InputControlWidget(input_port_service, input_device_service, 0).fill()
        # InputControlWidget(input_port_service, input_device_service, 1).fill()

        for port_index in range(2):
            with VerticalLayout(gap=8).fill():
                InputModeSelector(
                    services.input_ports, services.input_devices, port_index
                ).fill()

                InputDeviceSelector(
                    services.input_ports, services.input_devices, port_index
                ).fill()

        self.resize_to_fit_content()
        # self.set_width(692)
        self.set_width(280)
        self.move((24, 586))


class InputControlWidget(Widget):
    def __init__(
        self,
        input_port_service: InputPortService,
        input_device_service: InputDeviceService,
        port_index: int,
    ) -> None:
        super().__init__()

        HorizontalLayout(gap=12)

        # FIXME: # .set_width(300) does not have any effect
        # FIXME: # .set_min_width instead?
        InputModeSelector(
            input_port_service, input_device_service, port_index
        ).set_min_width(200)

        InputDeviceSelector(
            input_port_service, input_device_service, port_index
        ).expand()


class InputModeSelector(Choice):
    def __init__(
        self,
        input_port_service: InputPortService,
        input_device_service: InputDeviceService,
        port_index: int,
    ) -> None:
        items = ["Amiga Joystick", "Amiga Mouse", "CD32 Gamepad"]
        # for device in input_device_service.devices:
        #     items.append(device.name)

        super().__init__(items)
        self.input_port_service = input_port_service
        self.input_device_service = input_device_service
        self.port_index = port_index

        if self.port_index == 1:
            self.set_index(1)

        # self.listen(
        #     self.input_device_service.device_added,
        #     self.__device_added_listener,
        # )

    def on_change(self):
        logger.debug("%r on_change %r", self, self.index)
        # # FIXME: Maybe not a good idea, replace with name?
        # index = self.index
        # self.input_port_service.ports[self.port_index].set_device_by_index(index)


class InputDeviceSelector(Choice):
    def __init__(
        self,
        input_port_service: InputPortService,
        input_device_service: InputDeviceService,
        port_index: int,
    ) -> None:
        items = []
        for device in input_device_service.devices:
            items.append(device.name)

        super().__init__(items)
        self.input_port_service = input_port_service
        self.input_device_service = input_device_service
        self.port_index = port_index

        if self.port_index == 1:
            self.set_index(1)

        self.listen(
            self.input_device_service.device_added,
            self.__device_added_listener,
        )

    def __device_added_listener(self, device: InputDevice) -> None:
        logger.debug("(EVENT) ADD DEVICE %r", device.name)
        self.add_item(device.name)

    def on_change(self):
        logger.debug("%r on_change %r", self, self.index)
        # FIXME: Maybe not a good idea, replace with name?
        index = self.index
        self.input_port_service.ports[self.port_index].set_device_by_index(
            index
        )
