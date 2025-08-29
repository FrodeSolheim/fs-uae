import logging
from typing import Literal

from fsapp.eventhandler import EventHandler
from fsapp.tickservice import TickService
from fsemu.inputdevice import InputDevice
from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputport import InputPort
from fsemu.inputportservice import InputPortService

from fsuae.reactive2 import Var
from fsuae.uaeconfig2 import UAEConfig2

logger = logging.getLogger(__name__)


# FIXME: Use fsemu.inputport (probably/maybe). For now, just a small data class used to shadow
# the "real" ports.


# class InputPort2:
#     def __init__(self) -> None:
#         self.device_name: str = ""
#         self.device: InputDevice | None = None

#         self.configured_device: InputDevice | None = None


class DeviceRobotService(EventHandler):
    """
    This service handles actually changing active device when config value changes, as well as
    changing suitable devices automatically chosen device is blank/auto.
    """

    def __init__(
        self,
        ports: list[InputPort],
        config: list[Var[str]],  # FIXME: State[str] using "interface" instead?
        # FIXME: Config -> AmigaContext / UAEContext ..?
        config2: UAEConfig2,
        input_port_service: InputPortService,
        input_device_service: InputDeviceService,
        tick_service: TickService,
    ) -> None:
        super().__init__()

        # The actual ports containing information about the actual devices currently inserted
        self.ports = ports

        # List of config vars with what devices the user has requested / configured for each port
        self.config = config

        self._config = config2
        self._input_port_service = input_port_service
        self._input_device_service = input_device_service

        self._recalculate_needed = False

        # self._ports = [InputPort2(), InputPort2(), InputPort2(), InputPort2()]

        # These will fire if the user has actively changed device in port (or set to Auto), either
        # directly or indirectly via loading a config.

        self.on(self._config.joystick_port_0, self.__joystick_port_0)
        self.on(self._config.joystick_port_1, self.__joystick_port_1)

        # We will want to recalculate devices whenever devices are added or removed

        self.on(self._input_device_service.device_added, self.__device_added)
        self.on(self._input_device_service.device_removed, self.__device_removed)

        self.on(tick_service.tick, self.__tick)

        # self.auto_devices = [Var[InputDevice | None](None) for _port in self.ports]

        # tick_service.register

    def __tick(self) -> None:
        # print("tick", self._recalculate_needed)
        if self._recalculate_needed:
            self.recalculate_devices()
            self._recalculate_needed = False

    # def _auto_fill(self, port_index: int, type: str) -> None:
    #     devices: list[InputDevice | None] = [None, None, None, None]
    #     for i, port in enumerate(self._ports):
    #         devices[i] = port.device

    #     devices[port_index] = None

    def get_port_mode(
        self, port_index: int
    ) -> Literal["mouse"] | Literal["joystick"] | Literal["none"]:
        if port_index == 1:
            return "mouse"
        elif port_index > 1:
            return "none"
        return "joystick"

    def recalculate_devices(self) -> None:
        logger.debug("DeviceRobotService.recalculate_devices")

        # Could also use (unique) device names here - not that important
        # used_devices: set[InputDevice] = set()

        all_devices = InputDeviceService.instance().devices
        device_port: dict[InputDevice, int | None] = {device: None for device in all_devices}

        # for i, port in enumerate(self.ports):
        #     if port.device is not None:
        #         device_port[port.device] = i

        def key_function(device: InputDevice):
            return (
                {
                    InputDevice.TYPE_GAMEPAD: 0,
                    InputDevice.TYPE_JOYSTICK: 1,  # FIXME: Merge with gamepad
                    InputDevice.TYPE_KEYBOARD: 2,
                    InputDevice.TYPE_MOUSE: 3,
                }[device.type],
                device.name,
            )

        all_devices.sort(key=key_function)
        print("SORTED DEVICES")
        for device in all_devices:
            print("-", device.name)

        # for device in all_devices:
        #     device_port[device] = None

        # ports = [InputPort2(), InputPort2(), InputPort2(), InputPort2()]

        # FIXME: code smell
        # self._ports[0].device = self._input_port_service.ports[0].device
        # self._ports[1].device = self._input_port_service.ports[1].device
        # self._ports[2].device = None
        # self._ports[3].device = None

        # for port__x in self._ports:
        #     if port__x.device is not None:
        #         used_devices.add(port__x.device)

        # def auto_fill(port_index: int, type: str) -> None:
        #     # FIXME: Actually look up...
        #     if port_index == 0:
        #         mode = "mouse"
        #     else:
        #         mode = "joystick"

        # First we need to check explicitly configured devices and mark them as used, so the
        # auto-fill algorithm does not get confused and use unavailable devices.

        for p in [0, 1, 2, 3]:
            value = self.config[p].value
            if value in ["", "auto", "none"]:
                continue
            for device in all_devices:
                if device.name == value:
                    device_port[device] = p
                    print("Already configured device", device)
                    break

        # def auto_fill(p: int, type: str):
        def auto_fill(p: int):
            # mode = config.get("joystick_port_{0}_mode".format(port))
            mode = self.get_port_mode(p)
            # if not mode:
            #     mode = cls.get_calculated_port_mode(config, port)
            # val = config.get("joystick_port_{0}".format(port))
            value = self.config[p].value

            print("auto-fill", p, "mode", repr(mode), "value", repr(value))

            if value == "" or value == "auto":
                pass
            else:
                # specific device chosen
                # for dev in cls.devices:
                #     if dev.id == val:
                #         ports[port] = dev
                #         break

                # for device in all_devices:
                #     if device.name == val:
                #         device_port[device] = p
                #         print("Using", device)
                return

            if mode == "none":
                return
            elif mode == "mouse":
                for device in all_devices:
                    print("?", device.type, device.name, device_port[device])
                    if device.type == InputDevice.TYPE_MOUSE:
                        if device_port[device] is None:
                            device_port[device] = p
                            print("Using", device)
                            return
            else:
                for device in all_devices:
                    print("?", device.type, device.name, device_port[device])
                    if device.type != InputDevice.TYPE_MOUSE:
                        if device_port[device] is None:
                            print("Using", device)
                            device_port[device] = p
                            return

            # if type == "mouse":
            #     # print("a", mode)
            #     if mode != "mouse":
            #         return
            #     # print("b")
            #     for dev in cls.devices:
            #         # print("c")
            #         if dev.type == "mouse" and dev.port is None:
            #             # print("d")
            #             ports[port] = dev
            #             dev.port = port
            #             return
            # elif type == "joystick":
            #     if mode == "cd32 gamepad":
            #         prefs = cls.get_preferred_gamepads()
            #     elif mode == "joystick":
            #         prefs = cls.get_preferred_joysticks()
            #     else:
            #         return
            #     # try to find an available preferred device first
            #     for pref in prefs:
            #         for dev in cls.devices:
            #             if dev.cmp_id == pref and dev.port is None:
            #                 ports[port] = dev
            #                 dev.port = port
            #                 return
            #     # find first suitable device
            #     for dev in cls.devices:
            #         if dev.type == "joystick" and dev.port is None:
            #             ports[port] = dev
            #             dev.port = port
            #             return
            #     for dev in cls.devices:
            #         if dev.type == "keyboard" and dev.port is None:
            #             ports[port] = dev
            #             dev.port = port
            #             return

        # FIXME
        # port_0_mode = "mouse"
        # port_1_mode = "joystick"

        if self.get_port_mode(0) == "mouse" and self.get_port_mode(0) == "mouse":
            fill_order = [1, 0, 2, 3]
        else:
            fill_order = [0, 1, 2, 3]
        for p in fill_order:
            auto_fill(p)

        for p in [0, 1, 2, 3]:
            print("PORT", p, end=" ")
            for device, port_index in device_port.items():
                if port_index == p:
                    print(device.name)
                    # No-op if the device is already inserted
                    # self.ports[p].set_device(device)
                    break
            else:
                print("None")
                # self.ports[p].set_device(None)

        for p in [0, 1, 2, 3]:
            for device, port_index in device_port.items():
                if port_index == p:
                    self.ports[p].set_device(device)
                    break
            else:
                self.ports[p].set_device(None)

        # NOTE: After auto_fill, the "auto" value of each port will just be the next available
        # device of the given type... FIXME: Sort devices and put keyboards last!

        def find_next_device(mouse: bool, allow_port: int = -1) -> InputDevice | None:
            for device in all_devices:
                print("?", device.type, device.name, device_port[device])
                if (mouse and device.type == InputDevice.TYPE_MOUSE) or (
                    not mouse and device.type != InputDevice.TYPE_MOUSE
                ):
                    if device_port[device] is None or device_port[device] == allow_port:
                        device_port[device] = p
                        print("Using", device)
                        return device
            return None

        # FIXME: Not quite true - must temporarily remove the device in the port in question?

        print("AUTO DEVICES --------------------------")
        for p in [0, 1, 2, 3]:
            mode = self.get_port_mode(p)
            auto_device: InputDevice | None
            if mode == "none":
                auto_device = None
            elif mode == "mouse":
                auto_device = find_next_device(mouse=True, allow_port=p)
            else:
                auto_device = find_next_device(mouse=False, allow_port=p)
            # self.auto_devices[p].set(auto_device)
            print(auto_device)
            self.ports[p].auto_device.set(auto_device)

    #     self.apply_changes()

    # def apply_changes(self):
    #     logger.debug("DeviceRobotService.apply_changes")
    #     print("DeviceRobotService.apply_changes")
    #     for i, port2 in enumerate(self._ports):
    #         print(i, port2, port2.device, port2.configured_device)
    #         if port2.device != port2.configured_device:
    #             # FIXME: Insert!

    #             #     # post_to_fsapp_main(FSEMU_MESSAGE_SET_PORT_DEVICE, f"{port.instance_id}:{device.instance_id}")
    #             #     # FIXME: Maybe have an fsemu instance id which is separate from SDL ??? For now, we send
    #             #     # device type and (sdl) instance id
    #             device = port2.device
    #             port = self._input_port_service.ports[i]
    #             # if device is not None:
    #             #     post_to_fsapp_main(
    #             #         FSEMU_MESSAGE_SET_PORT_DEVICE,
    #             #         f"{port.instance_id}:{device.type}:{device.instance_id}",
    #             #     )
    #             # else:
    #             #     # FIXME: Remove device!
    #             #     pass
    #             port.set_device(device)
    #             port2.configured_device = port2.device

    def _update_port(self, index: int, value: str) -> None:
        # FIXME: Must remember to make sure devices has unique names (#2, #3, etc)
        # self._ports[index].device_name = value
        self._recalculate_needed = True

    def __joystick_port_0(self, value: str) -> None:
        print("DeviceRobotService.__joystick_port_0", value)
        self._update_port(0, value)

    def __joystick_port_1(self, value: str) -> None:
        print("DeviceRobotService.__joystick_port_1", value)
        self._update_port(1, value)

    def __device_added(self, device: InputDevice) -> None:
        logger.debug("ADDED DEVICE %r", device.name)
        self._recalculate_needed = True

    def __device_removed(self, device: InputDevice) -> None:
        logger.debug("REMOVED DEVICE %r", device.name)
        self._recalculate_needed = True

        # FIXME: Remove device (if used) right away, or wait for recalculate and do it there?

        # for port in self._ports:
        #     if port.device == device:
        #         port.device = None
        #         port.device_name = ""
        #         break

    # def on_change(self) -> None:
    #     logger.debug("%r on_change %r", self, self.index)
    #     # FIXME: Maybe not a good idea, replace with name?
    #     # Yes, replace with set_device_by_type_and_instance_id
    #     # index = self.index

    #     # FIXME: set_device_by_index works synchronously now - maybe a better idea to do this via
    #     # events pushed to the main thread instead?
    #     port = self._input_port_service.ports[self.port_index]
    #     # port.set_device_by_index(index)
    #     device: InputDevice = self.items[self.index].data

    #     from fsapp.constants import FSEMU_MESSAGE_SET_PORT_DEVICE
    #     from fsapp.main import post_to_fsapp_main

    #     # post_to_fsapp_main(FSEMU_MESSAGE_SET_PORT_DEVICE, f"{port.instance_id}:{device.instance_id}")
    #     # FIXME: Maybe have an fsemu instance id which is separate from SDL ??? For now, we send
    #     # device type and (sdl) instance id
    #     post_to_fsapp_main(
    #         FSEMU_MESSAGE_SET_PORT_DEVICE, f"{port.instance_id}:{device.type}:{device.instance_id}"
    #     )
