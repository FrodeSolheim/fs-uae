from typing import Self

from fsapp.eventservice import EventService
from fsapp.tickservice import TickService
from fsemu.inputdeviceservice import InputDeviceService
from fsemu.inputportservice import InputPortService

from fsuae.input.devicerobotservice import DeviceRobotService
from fsuae.roms.romservice import ROMService
from fsuae.services.pathservice import PathService
from fsuae.uaeconfigservice import UAEConfigService


class ServiceContainer:
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        return cls._instance

    @classmethod
    def _set_instance(cls, instance: Self) -> None:
        assert not hasattr(cls, "_instance")
        cls._instance = instance

    def set_instance(self) -> Self:
        self._set_instance(self)
        return self

    device_robot: DeviceRobotService
    event: EventService
    input_devices: InputDeviceService
    input_ports: InputPortService
    path: PathService
    rom: ROMService
    tick: TickService
    uae_config: UAEConfigService

    def __init__(self) -> None:
        pass
        # self._services: dict[Any, Any] = {}
        # self.event: EventService
        # self.uae_config: UAEConfigService
        # self.input_device: InputDeviceService
        # self.input_port: InputPortService

    # def get(self, class_) -> Any:
