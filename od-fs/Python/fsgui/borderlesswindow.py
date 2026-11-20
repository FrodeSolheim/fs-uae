from fsapp.surface import SurfaceCapsule

from fsgui.types import Borders, PaddingParam, SizeParam
from fsgui.window import Window


class BorderlessWindow(Window):
    def __init__(
        self,
        title: str = "",
        padding: PaddingParam = None,
        size: SizeParam = None,
        special: int = 0,
        surface: SurfaceCapsule | None = None,
    ) -> None:
        super().__init__(
            title,
            borders=Borders(0, 0, 0, 0),
            padding=padding,
            size=size,
            special=special,
            surface=surface,
        )

    def paint_border(self) -> None:
        pass
