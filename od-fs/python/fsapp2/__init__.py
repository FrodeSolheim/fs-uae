import os


class FSApp:
    def __init__(self) -> None:
        self._data_dir = os.environ["FSAPP_DATA_DIR"]

    @property
    def base_dir(self) -> str:
        # FIXME
        return "."

    @property
    def data_dir(self) -> str:
        return self._data_dir


fsapp = FSApp()
