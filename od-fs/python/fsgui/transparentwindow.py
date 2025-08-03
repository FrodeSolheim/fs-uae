from fsgui.borderlesswindow import BorderlessWindow
from fsgui.types import SizeParam


class TransparentWindow(BorderlessWindow):
    def __init__(
        self,
        size: SizeParam = None,
        special: int = 1,
    ) -> None:
        super().__init__(
            size=size,
            special=special,
        )
