from fsapp.surfaceapi.v1 import Band, Role

from .types import Borders, PaddingParam, SizeParam
from .window import Window


class BorderlessWindow(Window):
    def __init__(
        self,
        title: str = "",
        padding: PaddingParam = None,
        size: SizeParam = None,
        *,
        role: Role = Role.TOP_LEVEL,
        parent: Window | None = None,
        band: Band = Band.NORMAL,
        buffer: bool = True,
    ) -> None:
        super().__init__(
            title,
            borders=Borders(0, 0, 0, 0),
            padding=padding,
            size=size,
            role=role,
            parent=parent,
            band=band,
            buffer=buffer,
        )

    def paint_border(self) -> None:
        pass
