import fsemu_input  # type: ignore
import fsemu_inputport  # type: ignore


class InputPort:
    def __init__(self, name: str) -> None:
        self.name = name

        self._port = fsemu_inputport.new()
        fsemu_inputport.set_name(self._port, name)
        fsemu_input.add_port(self._port)

        self.modes: list[InputPortMode] = []
        self._modes_dict: dict[str, InputPortMode] = {}

    def create_mode(self, name: str) -> "InputPortMode":
        mode = InputPortMode(self, name)

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
        fsemu_inputport.set_device_by_index(self._port, index)


from fsemu.inputportmode import InputPortMode  # noqa: E402
