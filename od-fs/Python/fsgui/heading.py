from .font import Font
from .label import Label
from .types import Size
from .widget import Widget


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
