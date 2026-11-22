from .parentstack import ParentStack
from .types import ExpandParam
from .widget import Widget
from .widgetorlayout import WidgetOrLayout


class Spacer(WidgetOrLayout):
    def __init__(
        self,
        *,
        parent: Widget | None = None,
        expand: ExpandParam = False,
        height: float = 0,
        width: float = 0,
    ):
        size = (width, height)
        # The minimum size.
        self._min_size = size

        if parent is None:
            parent = ParentStack.parent()
            # layout = parent
        if isinstance(parent, Widget):
            # layout = parent.layout
            parent = parent.layout

        super().__init__(expand=expand, parent=parent, size=size)

        parent.add(self)

    def calculate_min_height(self, width: float) -> float:
        return self._min_size[1]

    def calculate_min_width(self) -> float:
        return self._min_size[0]

    # FIXME: Maybe this base method can be moved to WidgetOrLayout actually...
    def destroy(self):
        self.parent = None
