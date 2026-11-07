from fsgui.font import Font
from fsgui.label import Label
from fsgui.types import Size
from fsgui.widget import Widget


class Heading(Label):
    def __init__(
        self,
        text: str,
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
    ):
        font = Font("UI", weight=Font.BOLD)
        super().__init__(text, font=font, parent=parent, size=size)
