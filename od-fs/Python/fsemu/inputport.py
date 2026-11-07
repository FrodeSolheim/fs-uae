import _fsemu_input  # type: ignore
import _fsemu_inputport  # type: ignore
from fsapp.constants import FSEMU_MESSAGE_SET_PORT_DEVICE
from fsapp.main import post_to_fsapp_main
from fsapp.signal import Signal
from fsuae.var import Var

from fsemu.inputdeviceservice import InputDevice
from fsemu.inputportmode import InputPortMode


class InputPort:
    def __init__(self, name: str) -> None:
        self.name = name
        self._port = _fsemu_inputport.new()
        self.instance_id = _fsemu_inputport.get_instance_id(self._port)
        _fsemu_inputport.set_name(self._port, name)
        _fsemu_input.add_port(self._port)  # type: ignore
        self.modes: list[InputPortMode] = []
        self._modes_dict: dict[str, InputPortMode] = {}

        self.device: InputDevice | None = None
        self.device_changed = Signal[[InputDevice | None]]()

        self.auto_device = Var[InputDevice | None](None)

    def create_mode(self, name: str) -> "InputPortMode":
        mode = InputPortMode(name)
        _fsemu_inputport.add_mode(self._port, mode._mode)  # type: ignore

        # mode = fsemu_c.inputmode_new()
        # fsemu_c.inputmode_set_name(mode, name)
        # fsemu_inputport.add_mode(self._port, mode)

        self.modes.append(mode)
        self._modes_dict[name] = mode
        return mode

    def get_mode(self, name: str) -> "InputPortMode":
        return self._modes_dict[name]

    def set_device_by_index(self, index: int) -> None:
        # FIXME: C implementation must do bounds checking!!
        _fsemu_inputport.set_device_by_index(self._port, index)

    def set_device(self, device: InputDevice | None) -> None:
        if device == self.device:
            return
        self.device = device
        self.device_changed.emit(device)
        if device is not None:
            post_to_fsapp_main(
                FSEMU_MESSAGE_SET_PORT_DEVICE,
                f"{self.instance_id}:{device.type}:{device.instance_id}",
            )
        else:
            print("FIXME: SUPPORT SETTING NO DEVICE")
