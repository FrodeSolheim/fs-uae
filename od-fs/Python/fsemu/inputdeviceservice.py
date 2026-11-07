import logging
from typing import Self

from fsapp.constants import FSAPP_MESSAGE_INIT_SDL_GAMEPAD
from fsapp.eventname import EventName
from fsapp.eventservice import Event, EventService
from fsapp.main import post_to_fsapp_main
from fsapp.signal import Signal
from sdl3 import SDL_Event

from fsemu.inputdevice import InputDevice

logger = logging.getLogger(__name__)

# FIXME: Move the event binding functionality of Widget (on_* + destroy) to an EventHandler
# superclass? In this case, not super-important, since InputDeviceService will be a singleton
# which lives as long as the program.


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

        self._devices: list[InputDevice] = []

        # Gamepad/(joystick?) devices indexed by SDL instance ID. Hmm; use a common device
        # for both joystick *and* gamepad, ...?
        # -Ignoring "joysticks" for now...

        self._gamepads: dict[int, InputDevice] = {}

        self.device_added = Signal[InputDevice]()
        self.device_removed = Signal[InputDevice]()

        # self.event_service.add_listener("FSEMU_INPUTDEVICE_ADDED", self.__inputdevice_added)
        # self.event_service.add_listener(FSAPP_GAMEPAD_NAME, self.__gamepad_name)

        # FIXME: Can remove this event now?
        # self.event_service.event("FSEMU_INPUTDEVICE_ADDED").connect(self.__inputdevice_added)

        self.event_service.event(EventName.FSAPP_GAMEPAD_NAME).connect(self.__gamepad_name)

        self.event_service.sdl_event(EventName.SDL_EVENT_GAMEPAD_ADDED).connect(
            self.__gamepad_added
        )
        self.event_service.sdl_event(EventName.SDL_EVENT_GAMEPAD_REMAPPED).connect(
            self.__gamepad_remapped
        )
        self.event_service.sdl_event(EventName.SDL_EVENT_GAMEPAD_REMOVED).connect(
            self.__gamepad_removed
        )

        self.event_service.sdl_event(EventName.SDL_EVENT_JOYSTICK_ADDED).connect(
            self.__joystick_added
        )
        self.event_service.sdl_event(EventName.SDL_EVENT_JOYSTICK_REMOVED).connect(
            self.__joystick_removed
        )

        # fsemu input adds these devices automatically - mirroring that here

        device = InputDevice("Keyboard", InputDevice.TYPE_KEYBOARD)
        self._add_device(device)

        device = InputDevice("Mouse", InputDevice.TYPE_MOUSE)
        self._add_device(device)

    def _add_device(self, device: InputDevice) -> None:
        self._devices.append(device)
        self.device_added.emit(device)
        # self.add.broadcast(device)

    @property
    def devices(self) -> list[InputDevice]:
        return self._devices.copy()

    def __joystick_added(self, event: SDL_Event) -> None:
        print("__joystick_added", event)
        self._joystick_or_gamepad_added(InputDevice.TYPE_JOYSTICK, event.jdevice.which)

    def __gamepad_added(self, event: SDL_Event) -> None:
        print("__gamepad_added", event)
        instance_id = event.gdevice.which
        self._joystick_or_gamepad_added(InputDevice.TYPE_GAMEPAD, instance_id)

        # if self._gamepads[device.instance_id]:
        # device = InputDevice("", InputDevice.TYPE_GAMEPAD)
        # device.instance_id = event.gdevice.which
        # self._gamepads[device.instance_id] = device
        # # Wait adding the device until we've gotten the name?
        # # self._devices.append(device)

    def _joystick_or_gamepad_added(self, type: int, instance_id: int) -> None:
        if self._gamepads.get(instance_id) is not None:
            print("Gamepad or joystick already seeen!")
            return

        device = InputDevice(f"Controller-{instance_id}", type, instance_id=instance_id)
        self._gamepads[device.instance_id] = device
        # Wait adding the device until we've gotten the name?
        # self._devices.append(device)

    def __gamepad_name(self, event: Event) -> None:
        # FIXME: _joystick_name as well?
        print("__gamepad_name", event)
        instance_id = event["intdata"]  # type: ignore
        device_name = event["strdata"]  # type: ignore

        device = self._gamepads[instance_id]

        if device in self._devices:
            logger.warning("Device %s (%d) was already added", device_name, instance_id)
            # FIXME: Maybe (?) update device name? If the device was already added as a joystick
            # device and then upgraded (?) to a gamepad device?
        else:
            device.name = device_name
            self._add_device(device)
            post_to_fsapp_main(FSAPP_MESSAGE_INIT_SDL_GAMEPAD, f"{device.instance_id}")

    def __gamepad_remapped(self, event: SDL_Event) -> None:
        print("__gamepad_remapped", event)

    def __joystick_removed(self, event: SDL_Event) -> None:
        print("__joystick_removed", event)
        self._joystick_or_gamepad_removed(event.jdevice.which)

    def __gamepad_removed(self, event: SDL_Event) -> None:
        print("__gamepad_removed", event)
        self._joystick_or_gamepad_removed(event.gdevice.which)

    def _joystick_or_gamepad_removed(self, instance_id: int) -> None:
        try:
            device = self._gamepads[instance_id]
        except KeyError:
            logger.warning("Joystick/gamepad already removed")
            return
        del self._gamepads[device.instance_id]
        self._devices.remove(device)
        self.device_removed.emit(device)
