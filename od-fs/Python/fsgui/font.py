from enum import Enum

from fsapp.surfaceapi.v1 import Font as LoadedFont

from .scale import get_scale, scaled, units_from_pixel_size


class FontWeight(Enum):
    NORMAL = 1
    MEDIUM = 2
    SEMI_BOLD = 3
    BOLD = 4


class Font:
    NORMAL = FontWeight.NORMAL
    MEDIUM = FontWeight.MEDIUM
    SEMI_BOLD = FontWeight.SEMI_BOLD
    BOLD = FontWeight.BOLD

    # def __init__(self, font):
    #     self._font = font

    def __init__(
        self,
        name: str = "UI",
        size: int = 14,
        weight: FontWeight = FontWeight.NORMAL,
    ):
        """size is in design units: the font is loaded at size times the scale, in pixels."""
        self.name = name
        self.size = size
        self.weight = weight

    @property
    def font(self) -> LoadedFont:
        """The loaded font at the current scale."""
        from .fontmanager import FontManager

        return FontManager.get().get_with_cache(self.name, scaled(self.size), self.weight)

    # FIXME: Remove alias?
    @property
    def _font(self) -> LoadedFont:
        return self.font

    def measure_text(self, text: str) -> tuple[int, int]:
        """The text's size in design units (rounded up)."""
        return units_from_pixel_size(self.font.measure(text), get_scale())
