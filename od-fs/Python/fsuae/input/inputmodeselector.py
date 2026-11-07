import logging

from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService
from fsgui.choice import Choice

logger = logging.getLogger(__name__)


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
