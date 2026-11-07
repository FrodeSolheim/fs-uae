import ctypes
import logging
from typing import Self

logger = logging.getLogger()

S_OK = 0
CSIDL_DESKTOP = 0
CSIDL_PERSONAL = 5
CSIDL_APPDATA = 26
CSIDL_MYPICTURES = 39
CSIDL_PROFILE = 40
CSIDL_COMMON_DOCUMENTS = 46


class WindowsService:
    _instance: "WindowsService"

    @classmethod
    def instance(cls) -> "WindowsService":
        try:
            return cls._instance
        except AttributeError:
            cls._instance = WindowsService()
            return cls._instance

    @classmethod
    def _set_instance(cls, instance: Self) -> None:
        assert not hasattr(cls, "_instance")
        cls._instance = instance

    def set_instance(self) -> Self:
        self._set_instance(self)
        return self

    def __init__(self) -> None:
        self._common_documents_dir: str | None = None

    def get_csidl_dir(self, csidl: int) -> str:
        from ctypes import windll, wintypes  # type: ignore

        SHGetFolderPath = windll.shell32.SHGetFolderPathW  # type: ignore
        SHGetFolderPath.argtypes = [
            wintypes.HWND,
            ctypes.c_int,
            wintypes.HANDLE,
            wintypes.DWORD,
            wintypes.LPCWSTR,
        ]

        path_buf = ctypes.create_unicode_buffer(wintypes.MAX_PATH)
        result: int = SHGetFolderPath(0, csidl, 0, 0, path_buf)  # type: ignore
        if result != S_OK:
            raise RuntimeError(f"Could not find common directory for CSIDL {csidl}")
        return path_buf.value

    def get_common_documents_dir(self) -> str:
        if self._common_documents_dir is None:
            self._common_documents_dir = self.get_csidl_dir(CSIDL_COMMON_DOCUMENTS)
        return self._common_documents_dir
