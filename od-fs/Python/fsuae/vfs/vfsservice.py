from typing import Self

from fsuae.vfs.vfsicon import VFSIcon


class VFSService:
    _instance: Self | None = None

    @classmethod
    def get(cls):
        if cls._instance is None:
            cls._instance = VFSService()
        return cls._instance

    def __init__(self):
        pass

    def get_icons(self, path):
        if path == "System:":
            return [
                VFSIcon("System:FS-UAE", (30, 20)),
                VFSIcon("System:Prefs", (140, 20)),
            ]
        if path == "System:FS-UAE":
            return [
                VFSIcon("System:FS-UAE/FS-UAE", (30, 20)),
                VFSIcon("System:FS-UAE/Licenses", (140, 20)),
                VFSIcon("System:FS-UAE/About", (30, 130)),
                # VFSIcon("System:FS-UAE/Linux", (140, 20)),
                # VFSIcon("System:FS-UAE/Data", (250, 20)),
                # VFSIcon("System:FS-UAE/Python", (30, 130)),
                VFSIcon("System:FS-UAE/README", (140, 130)),
                VFSIcon("System:FS-UAE/COPYING", (250, 130)),
            ]
