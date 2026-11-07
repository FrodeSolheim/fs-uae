import fsemu_c  # type: ignore


class InputPortMode:
    def __init__(self, name: str) -> None:
        self.name = name

        # self._port = port
        self._mode = fsemu_c.inputmode_new()  # type: ignore

        fsemu_c.inputmode_set_name(self._mode, name)  # type: ignore
        # _fsemu_inputport.add_mode(self._port._port, self._mode)

    def map(self, trigger: int, action: int) -> None:
        fsemu_c.inputmode_map(self._mode, trigger, action)  # type: ignore
