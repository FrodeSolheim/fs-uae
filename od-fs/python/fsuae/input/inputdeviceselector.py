import logging

from fsemu.inputdevice import InputDevice
from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService
from fsgui.choice import Choice
from fsgui.item import Item

from fsuae.uaeconfig2 import UAEConfig2

logger = logging.getLogger(__name__)


class InputDeviceSelector(Choice):
    def __init__(
        self,
        config: UAEConfig2,
        input_port_service: InputPortService,
        input_device_service: InputDeviceService,
        port_index: int,
    ) -> None:
        items: list[Item] = []
        items.append(Item("Auto", data=None))
        items.append(Item("No device", data=None))
        for device in input_device_service.devices:
            items.append(Item(device.name, data=device))

        super().__init__(items)

        self.config = config
        self.input_port_service = input_port_service
        self.input_device_service = input_device_service
        self.port_index = port_index

        if port_index == 0:
            self.on(config.joystick_port_1, self.__joystick_port)
        else:
            self.on(config.joystick_port_0, self.__joystick_port)

        self.on(self.input_device_service.device_added, self.__device_added)
        self.on(self.input_device_service.device_removed, self.__device_removed)

        # FIXME: what about "immediate mode" for this one?
        self.on(
            self.input_port_service.ports[port_index].device_changed, self.__port_device_changed
        )
        # FIXME: Forcing "immediate" mode for now
        # FIXME: Maybe InputDevice.device should be a Var by itself...!
        self.__port_device_changed(self.input_port_service.ports[port_index].device)

        self.on(self.input_port_service.ports[port_index].auto_device, self.__port_auto_device)
        # FIXME: Immediate
        self.__port_auto_device(self.input_port_service.ports[port_index].auto_device.value)

    def __joystick_port(self, value: str) -> None:
        logger.debug("InputDeviceSelector.__joystick_port, new value is '%s'", value)
        if value == "" or value == "auto":
            self.set_index(0)
        elif value == "none":
            self.set_index(1)
        else:
            for i, item in enumerate(self.items):
                if item.text == value:
                    logger.debug("Found item at index %d", i)
                    self.set_index(i)
                    break

    def __port_auto_device(self, device: InputDevice | None) -> None:
        print("__port_auto_device")
        # The auto device for a port is which device the port will be switched to if put in auto
        # mode, so not necessary the same device as is currently used.

        if device is None:
            device_name = "No device"
        else:
            device_name = device.name
        self.items[0].text = f"Auto ({device_name})"
        self.refresh()

    def __port_device_changed(self, device: InputDevice | None) -> None:
        print("")
        print("PORT DEVICE CHANGED")
        # index = self.find_index_by_data(device)
        # if index is not None:
        #     self.set_index(index)
        # else:
        #     logger.warning("Did not find device %r", device)
        # if self.index == 0:
        #     if device is None:
        #         device_name = "None"
        #     else:
        #         device_name = device.name
        #     self.items[0].text = f"Auto ({device_name})"

    def __device_added(self, device: InputDevice) -> None:
        logger.debug("(EVENT) ADD DEVICE %r", device.name)
        self.add_item(Item(device.name, data=device))

    def __device_removed(self, device: InputDevice) -> None:
        logger.debug("DEVICE REMOVED %r", device.name)
        # FIXME! FIXME! FIXME! FIXME! FIXME! FIXME! FIXME! FIXME! FIXME! FIXME! FIXME! FIXME!
        self.remove_item_by_data(device)

    def on_change(self) -> None:
        logger.debug("%r on_change %r", self, self.index)
        # FIXME: Maybe not a good idea, replace with name?
        # Yes, replace with set_device_by_type_and_instance_id
        # index = self.index

        if self.index == 0:
            device_name = "auto"  # or device_name = ""
        elif self.index == 1:
            device_name = "none"
        else:
            device: InputDevice = self.items[self.index].data
            device_name = device.name

        # device: InputDevice | None = self.items[self.index].data
        # if device is None:
        #     # Auto...
        #     device_name = ""
        # else:
        #     device_name = device.name

        # Set device name state vars and let DeviceRobotService do the actual work of switching
        # devices

        if self.port_index == 0:
            var = self.config.joystick_port_1
        else:
            var = self.config.joystick_port_0

        # If the device was inserted in another port, remove it first

        for v in [
            self.config.joystick_port_0,
            self.config.joystick_port_1,
            self.config.joystick_port_2,
            self.config.joystick_port_3,
        ]:
            if v == var:
                continue
            if v.value == device_name:
                v.set("auto")  # or v.set("")

        # And then set the config for the chosen port

        var.set(device_name)

        #     for cfg in

        #     self.config.joystick_port_1.set(device_name)
        # elif self.port_index == 1:
        #     self.config.joystick_port_0.set(device_name)

        # # FIXME: set_device_by_index works synchronously now - maybe a better idea to do this via
        # # events pushed to the main thread instead?
        # port = self.input_port_service.ports[self.port_index]
        # # port.set_device_by_index(index)
        # device: InputDevice = self.items[self.index].data

        # from fsapp.constants import FSEMU_MESSAGE_SET_PORT_DEVICE
        # from fsapp.main import post_to_fsapp_main

        # # post_to_fsapp_main(FSEMU_MESSAGE_SET_PORT_DEVICE, f"{port.instance_id}:{device.instance_id}")
        # # FIXME: Maybe have an fsemu instance id which is separate from SDL ??? For now, we send
        # # device type and (sdl) instance id
        # post_to_fsapp_main(
        #     FSEMU_MESSAGE_SET_PORT_DEVICE, f"{port.instance_id}:{device.type}:{device.instance_id}"
        # )
