from typing import Self

from fsemu.inputport import InputPort


class InputPortService:
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

    def __init__(self) -> None:
        self.ports: list[InputPort] = []

    def add_port(self, port: InputPort) -> None:
        self.ports.append(port)
