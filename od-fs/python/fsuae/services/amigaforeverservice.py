import logging
import os
import sys
from typing import Self

from fsuae.services.windowsservice import WindowsService

logger = logging.getLogger()


class AmigaForeverService:
    _instance: "AmigaForeverService"

    @classmethod
    def instance(cls) -> "AmigaForeverService":
        try:
            return cls._instance
        except AttributeError:
            cls._instance = AmigaForeverService()
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
        self.windows_service = WindowsService.instance()

        self._amiga_files_dir: str | None = None

    def get_amiga_files_dir(self) -> str:
        if self._amiga_files_dir is None:
            self._amiga_files_dir = self._get_amiga_files_dir()
        return self._amiga_files_dir

    def _get_amiga_files_dir(self) -> str:
        if sys.platform == "win32":
            path = os.path.join(self.windows_service.get_common_documents_dir(), "Amiga Files")
            return path
        elif sys.platform == "darwin":
            # FIXME: Maybe Amiga Forever can be installed using Wine on macOS also? However, does
            # anyone actually do this? Let's just return a path. Then we don't have to check for
            # None. The path may not actually exist on any os, anyway.
            wine_prefix = os.path.expanduser("~/.wine")
            path = os.path.join(
                wine_prefix, "drive_c", "users", "Public", "Documents", "Amiga Files"
            )
            return path
        else:
            wine_prefix = os.path.expanduser("~/.wine")
            path = os.path.join(
                wine_prefix, "drive_c", "users", "Public", "Documents", "Amiga Files"
            )
            return path

    def get_shared_rom_dir(self) -> str:
        return os.path.join(self.get_amiga_files_dir(), "Shared", "rom")
