from fsgui.parentstack import ParentStack
from fsgui.types import ExpandParam
from fsgui.widget import Widget
from fsgui.widgetorlayout import WidgetOrLayout


class Spacer(WidgetOrLayout):
    def __init__(
        self,
        *,
        parent: Widget | None = None,
        expand: ExpandParam = False,
        height: int = 0,
        width: int = 0,
    ):
        size = (width, height)

        if parent is None:
            parent = ParentStack.parent()
            # layout = parent
        if isinstance(parent, Widget):
            # layout = parent.layout
            parent = parent.layout

        super().__init__(expand=expand, parent=parent, size=size)

        parent.add(self)

    def calculate_min_height(self, width: int) -> int:
        # FIXME: return self.height
        return self.get_size()[1]

    def calculate_min_width(self) -> int:
        # FIXME: return self.width
        return self.get_size()[0]

    # FIXME: Maybe this base method can be moved to WidgetOrLayout actually...
    def destroy(self):
        self.parent = None
