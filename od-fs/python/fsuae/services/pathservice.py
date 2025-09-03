import logging
import os
import sys
from typing import Literal, Self

from fsuae.services.amigaforeverservice import AmigaForeverService

logger = logging.getLogger()


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
        # FIXME: Inject instead
        self.amiga_forever_service = AmigaForeverService.instance()

        # executable_dir = os.path.dirname(sys.executable)
        # runtime_mode = get_runtime_mode()
        # if runtime_mode == "portable_system":
        #     self._base = os.path.normpath(os.path.join(executable_dir, ".."))
        # else:  # development?
        #     self._base = executable_dir
        # print("Base directory:", self._base)

        # FIXME: Do we want to run an early python init in the main thread before getting too far?
        # Then we can calculate plugin dir(s) in Python and report that back to C/C++ code...

        # FIXME:
        # self._cache = os.environ["FSAPP_CACHE_DIR"]
        self._data_dir = os.environ["FSAPP_DATA_DIR"]

        logger.info("Data: %r", self._data_dir)

        # ROM / rom / ROMs / roms
        self._rom_dirs = self._find_dirs([self._data_dir], ["ROM", "ROMs"])
        logger.info("ROM: %r", self._rom_dirs)

        # Kickstart / kickstart / Kickstarts / kickstarts

        # The combination of different ROM dirs and Kickstart dirs may be a bit much, especially
        # when lowercase variants are included. Maybe drop the flexibility?

        self._kickstart_dirs = self._find_dirs(self._rom_dirs, ["Kickstart", "Kickstarts"])
        self._kickstart_dirs.append(self.amiga_forever_service.get_shared_rom_dir())
        logger.info("Kickstart: %r", self._kickstart_dirs)

        # self._data = os.path.join(self._base, "Data")
        # print("Data directory:", self._data)
        self._roms = os.path.join(self._data_dir, "ROMs")
        if not os.path.isdir(self._roms):
            os.makedirs(self._roms)
        print("ROMs directory:", self._roms)

        # "Legacy" ROM directory
        self._rom = os.path.join(self._data_dir, "ROM")
        print("ROM directory:", self._rom)
        # sys.exit(1)

    def _find_dirs(
        self, parent_dirs: list[str], names: list[str], include_lowercase: bool = True
    ) -> list[str]:
        result: list[str] = []

        for parent_dir in parent_dirs:
            for name in names:
                path = os.path.join(parent_dir, name)
                result.append(path)

                if is_case_sensitive() and include_lowercase:
                    name_lower = name.lower()
                    if name_lower != name:
                        path = os.path.join(parent_dir, name_lower)
                        result.append(path)

        return result

    # @property
    # def base(self) -> str:
    #     return self._base

    @property
    def data(self) -> str:
        return self._data_dir

    @property
    def rom_dir(self) -> str:
        return self._roms

    def get_roms_dirs(self) -> list[str]:
        return [self._roms, self._rom]

    def get_kickstart_dirs(self) -> list[str]:
        return self._kickstart_dirs

    @property
    def kickstart_dir(self) -> str:
        return os.path.join(self._roms, "Kickstart")


# def get_runtime_mode() -> Literal["development", "portable_system"]:
#     if os.getenv("FSAPP_DEVELOPMENT_DIR"):
#         return "development"

#     # FIXME: Check FSAPP_DEV
#     print("RUNTIME ----------------------------------")
#     print(sys.executable)
#     executable_dir = os.path.dirname(sys.executable)
#     print(executable_dir)
#     return "portable_system"


def is_case_sensitive() -> bool:
    if sys.platform == "win32" or sys.platform == "darwin":
        return False
    else:
        return True
