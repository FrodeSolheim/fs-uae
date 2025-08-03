from .types import Size
from .window import Window


# FIXME: REMOVE
class DecoratedWindow(Window):
    def __init__(self, title: str, size: Size):
        super().__init__(title, size)
