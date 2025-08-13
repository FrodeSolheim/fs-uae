import logging
from typing import Self

import fsuae
from fsgui.font import Font
from fsgui.imagelabel import Image, ImageLabel
from fsgui.label import Label
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.window import Window

logger = logging.getLogger(__name__)


class AboutWindow(Window):
    _instance = None

    @classmethod
    def instance(cls) -> Self:
        if cls._instance is None:
            cls._instance = cls()
        return cls._instance

    @classmethod
    def set_instance(cls, instance: Self):
        cls._instance = instance

    def __init__(self):
        # size = (460, 400)
        super().__init__("About")
        self.move((300, 300))

        # background_colour
        # position
        # layout.margins (or self.padding - or both)
        # size

        # Advantage of padding is that you can have default window/widget
        # padding..

        # background_colour = (0xEE, 0xEE, 0xEE, 0xFF)
        background_colour = (0xF7, 0xF7, 0xF7, 0xFF)
        # background_colour = (0xFA, 0xFA, 0xFA, 0xFF)
        # background_colour = (0xFE, 0xFE, 0xFE, 0xFF)
        # background_colour = (0xFF, 0xFF, 0xFF, 0xFF)

        self.style.background_color = background_colour
        # FIXME: Or
        self.background_color = background_colour

        # self.padding = (20, 20, 20, 20)
        self.padding = (24, 24, 24, 24)

        VerticalLayout(gap=24)

        with HorizontalLayout(gap=12):
            ImageLabel(Image.from_resource("Icons/FS-UAE/64.png"))
            with VerticalLayout(gap=2).fill(False):
                Label(f"FS-UAE {fsuae.version}", font=Font("UI", 20, Font.BOLD))
                Label("Copyright \u00a9 2011-2025 Frode Solheim and contributors.")

        Label(fsuae.about_full, wrap=True)

        self.resize_to_fit_content(580)
        self.center_window()

    def on_close(self):
        logger.debug("on_close %r", self)
        AboutWindow.set_instance(None)

    # FIXME: Or this...
    def _on_destroy(self):
        logger.debug("_on_destroy %r", self)
