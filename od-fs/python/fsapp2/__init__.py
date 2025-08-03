import os
import sys


class FSApp:
    def __init__(self) -> None:
        self._data_dir = os.environ["FSAPP_DATA_DIR"]
        # FIXME..., not refer to FSUAE here?
        # Maybe FSAPP_FONTS_DIR instead?
        # self._system_dir = os.environ["FSUAE_SYSTEM_DIR"]

        exe_dir = os.path.dirname(sys.executable)

        self.fonts_dirs = list(filter(os.path.isdir, [
            os.path.join(exe_dir, "od-fs", "fonts"),
            os.path.join(exe_dir, "Fonts")
        ]))

        self.images_dirs = list(filter(os.path.isdir, [
            os.path.join(exe_dir, "od-fs", "images"),
            os.path.join(exe_dir, "Images")
        ]))

    @property
    def base_dir(self) -> str:
        # FIXME
        return "."

    @property
    def data_dir(self) -> str:
        return self._data_dir


fsapp = FSApp()
