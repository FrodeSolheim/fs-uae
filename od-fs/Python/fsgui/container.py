from fsgui.types import Size
from fsgui.widget import Widget


class Container:
    def __init__(self, parent: Widget, *, size: Size | None = None):
        super().__init__(parent, size=size)
        self._layout = None

        # css: align-items: center
        # css: flex-direction: row

    def draw(self):
        self.draw_children()

    def on_paint(self):
        pass
