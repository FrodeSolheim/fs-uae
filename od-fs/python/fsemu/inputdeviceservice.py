import logging
from typing import Self

from fsemu.inputdevice import InputDevice
from fsemu.signal import Signal
from fsuae.eventservice import Event, EventService

logger = logging.getLogger(__name__)


class InputDeviceService:
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

    def __init__(self, event_service: EventService) -> None:
        self.event_service = event_service
        # self.event_service.add_listener(self.on_gamepad_added, "GAMEPAD_ADDED")

        self._devices: list[InputDevice] = []
        self.device_added = Signal()
        self.device_removed = Signal()

        self.event_service.add_listener(
            self.__inputdevice_added, "FSEMU_INPUTDEVICE_ADDED"
        )

    def _add_device(self, device: InputDevice) -> None:
        self._devices.append(device)
        self.device_added.broadcast(device)
        # self.add.broadcast(device)

    @property
    def devices(self) -> list[InputDevice]:
        return self._devices.copy()

    def on_gamepad_added(self, event: Event):
        logger.debug("on_gamepad_added")

    def __inputdevice_added(self, event: Event):
        logger.debug("EVENT %s with strdata", event["type"], event["strdata"])

        device = InputDevice(name=event["strdata"], type=event["intdata"])
        self._add_device(device)
