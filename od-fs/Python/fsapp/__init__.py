import logging
import os

from fsapp.log import print_test_traceback_maybe
from fsapp.main import init_fsapp_main

logger = logging.getLogger(__name__)


def fsapp_init():
    print("fsapp.fsapp_init")
    print_test_traceback_maybe()

    init_fsapp_main()


class FSApp:
    def __init__(self) -> None:
        self._data_dir = os.environ["FSAPP_DATA_DIR"]
        self._resources_dir = os.environ["FSAPP_RESOURCES_DIR"]

        # FIXME: Replace with PathsService
        # For the paths commented out below: exe_dir = os.path.dirname(sys.executable)

        self.fonts_dirs = list(
            [os.path.join(self._resources_dir, "Fonts")]
            # filter(
            #     os.path.isdir,
            #     [
            #         os.path.join(exe_dir, "od-fs", "fonts"),
            #         os.path.join(exe_dir, "Fonts"),
            #         os.path.join(exe_dir, "..", "..", "Fonts"),  # Windows
            #         os.path.join(exe_dir, "..", "Resources", "Fonts"),  # macOS
            #     ],
            # )
        )

        self.images_dirs = list(
            [os.path.join(self._resources_dir, "Images")]
            # filter(
            #     os.path.isdir,
            #     [
            #         os.path.join(exe_dir, "od-fs", "images"),
            #         os.path.join(exe_dir, "Images"),
            #         os.path.join(exe_dir, "..", "..", "Images"),  # Windows
            #         os.path.join(exe_dir, "..", "Resources", "Images"),  # macOS
            #     ],
            # )
        )

    @property
    def base_dir(self) -> str:
        # FIXME
        return "."

    @property
    def data_dir(self) -> str:
        return self._data_dir

    @property
    def resources_dir(self) -> str:
        return self._resources_dir


fsapp = FSApp()
