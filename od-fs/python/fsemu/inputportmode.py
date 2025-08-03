import fsemu_c  # type: ignore
import fsemu_inputport  # type: ignore


class InputPortMode:
    def __init__(self, port: "InputPort", name: str) -> None:
        self.name = name

        self._port = port
        self._mode = fsemu_c.inputmode_new()

        fsemu_c.inputmode_set_name(self._mode, name)
        fsemu_inputport.add_mode(self._port._port, self._mode)

    def map(self, trigger: int, action: int) -> None:
        fsemu_c.inputmode_map(self._mode, trigger, action)


from fsemu.inputport import InputPort  # noqa: E402
