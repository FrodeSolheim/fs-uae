import os
from typing import Self


class PathService:
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
        # FIXME
        self._base = "."
        self._data = os.path.join(self._base, "Data")
        self._roms = os.path.join(self._data, "ROM")

    @property
    def base(self) -> str:
        return self._base

    @property
    def data(self) -> str:
        return self._data

    @property
    def rom_dir(self) -> str:
        return self._roms

    @property
    def kickstart_dir(self) -> str:
        return os.path.join(self._roms, "Kickstart")
