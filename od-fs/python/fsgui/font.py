from enum import Enum

import fsgui_font  # type: ignore

# import fsapp_font


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
        self.name = name
        self.size = size

        from fsgui.fontmanager import FontManager

        self.font = FontManager.get().get_with_cache(name, size, weight)

        # FIXME: Remove alias?
        self._font = self.font

    def measure_text(self, text: str) -> tuple[int, int]:
        return fsgui_font.measure_text(self.font, text)
