import os
import sys
from typing import Literal, Self


def get_runtime_mode() -> Literal["development", "portable_system"]:
    print("RUNTIME ----------------------------------")
    print(sys.executable)
    executable_dir = os.path.dirname(sys.executable)
    print(executable_dir)
    # return "portable_system"
    if os.path.exists(os.path.join(executable_dir, "configure.ac")):
        return "development"
    # if os.path.exists(os.path.join(executable_dir, "configure.ac")):
    return "portable_system"

    sys.exit(1)


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
        executable_dir = os.path.dirname(sys.executable)

        runtime_mode = get_runtime_mode()
        if runtime_mode == "portable_system":
            self._base = os.path.normpath(os.path.join(executable_dir, ".."))
        else:  # development?
            self._base = executable_dir
        # FIXME
        # self._base = "."
        print("Base directory:", self._base)

        self._data = os.environ["FSAPP_DATA_DIR"]

        # self._data = os.path.join(self._base, "Data")
        # print("Data directory:", self._data)
        self._roms = os.path.join(self._data, "ROMs")
        if not os.path.isdir(self._roms):
            os.makedirs(self._roms)
        print("ROMs directory:", self._roms)

        # "Legacy" ROM directory
        self._rom = os.path.join(self._data, "ROM")
        print("ROM directory:", self._rom)
        # sys.exit(1)

    @property
    def base(self) -> str:
        return self._base

    @property
    def data(self) -> str:
        return self._data

    @property
    def rom_dir(self) -> str:
        return self._roms

    def get_roms_dirs(self) -> list[str]:
        return [self._roms, self._rom]

    @property
    def kickstart_dir(self) -> str:
        return os.path.join(self._roms, "Kickstart")
