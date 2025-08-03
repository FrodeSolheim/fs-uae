import os
from typing import Optional

import fsgui_font  # type: ignore
from fsapp2 import fsapp

from fsgui.font import Font, FontWeight


class FontManager:
    _instance: Optional["FontManager"] = None

    @classmethod
    def get(cls):
        if cls._instance is None:
            cls._instance = FontManager("__sentinel__")
        return cls._instance

    def __init__(self, sentinel: str):
        if sentinel != "__sentinel__":
            raise Exception("Missing __sentinel__")

        self._font_cache: dict[tuple[str, int, FontWeight], Font] = {}

        # FIXME: Path/directory lookup!
        # self.regular_font_14 = Font(
        #     fsgui_font.load("data/Fonts/Roboto-Regular.ttf", 14)
        # )
        # self.medium_font_14 = Font(
        #     fsgui_font.load("data/Fonts/Roboto-Medium.ttf", 14)
        # )
        # self.semi_bold_font_14 = Font(
        #     fsgui_font.load("data/Fonts/Roboto-SemiBold.ttf", 14)
        # )
        # self.bold_font_14 = Font(
        #     fsgui_font.load("data/Fonts/Roboto-Bold.ttf", 14)
        # )

        # self.regular_font_14 =
        # self.medium_font_14 =
        # self.semi_bold_font_14 =
        # self.bold_font_14 =

    @property
    def regular_font_14(self):
        return Font("Roboto", 14, FontWeight.NORMAL)

    @property
    def medium_font_14(self):
        return Font("Roboto", 14, FontWeight.MEDIUM)

    @property
    def semi_bold_font_14(self):
        return Font("Roboto", 14, FontWeight.SEMI_BOLD)

    @property
    def bold_font_14(self):
        return Font("Roboto", 14, FontWeight.BOLD)

    def get_with_cache(
        self, name: str, size: int, weight: FontWeight = FontWeight.NORMAL
    ):
        if name == "UI":
            name = "Roboto"

        # if name == "Roboto":
        # path = "data/Fonts/Roboto-Regular.ttf"

        cache_key = (name, size, weight)
        try:
            return self._font_cache[cache_key]
        except KeyError:
            pass

        if weight == FontWeight.NORMAL:
            weight_name = "Regular"
        elif weight == FontWeight.MEDIUM:
            weight_name = "Medium"
        elif weight == FontWeight.SEMI_BOLD:
            weight_name = "SemiBold"
        elif weight == FontWeight.BOLD:
            weight_name = "Bold"

        font_file_name = f"{name}-{weight_name}.ttf"
        for fonts_dir in fsapp.fonts_dirs:
            font_path = os.path.join(
                fonts_dir, font_file_name
            )
            print(font_path)
            if os.path.exists(font_path):
                break
        else:
            raise FileNotFoundError(font_file_name)

        font = fsgui_font.load(font_path, size)  # type: ignore
        self._font_cache[cache_key] = font
        return self._font_cache[cache_key]

    def get_bold_ui_font(self) -> Font:
        # return self.bold_font_14
        # return self.medium_font_14
        return self.semi_bold_font_14

    def get_ui_font(self) -> Font:
        return self.regular_font_14

    def get_title_font_temp(self) -> Font:
        # return self.semi_bold_font_16
        return self.bold_font_14

    # # FIXME: size: int or float?
    # def get_font(self, name: str, size: int, weight: FontWeight):
    #     cache_key = (name, size, weight)
    #     # cached_font = self._font_cache.get(cache_key)
    #     # if cached_font:
    #     #     return cached_font
    #     try:
    #         return self._font_cache[cache_key]
    #     except KeyError:
    #         # fsgui_font.load(name, size)
    #         pass
