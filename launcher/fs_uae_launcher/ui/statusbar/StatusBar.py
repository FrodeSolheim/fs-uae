from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsui import Panel, Color, VerticalLayout, HorizontalLayout, Image
from ...Config import Config
from ..Skin import Skin
from .LanguageElement import LanguageElement
from .PlayersElement import PlayersElement
from .ProtectionElement import ProtectionElement
from .WarningsElement import WarningsElement
from .WebLinkElement import WebLinkElement


class StatusBar(Panel):

    def __init__(self, parent):
        Panel.__init__(self, parent, paintable=True)
        self.set_min_height(29)
        #self.set_background_color(Color(0xd8, 0xd8, 0xd8))
        self.layout = VerticalLayout()
        self.hori_layout = HorizontalLayout()
        self.layout.add(self.hori_layout, fill=True, expand=True,
                        margin_top=1)

        element = ProtectionElement(self)
        self.hori_layout.add(element, fill=True)

        element = WarningsElement(self)
        self.hori_layout.add(element, fill=True, expand=True)
        self.hori_layout.add_spacer(16)

        for language, icon_name in reversed([
            ("en", "16/GB"),
            ("de", "16/DE"),
            ("fr", "16/FR"),
            ("es", "16/ES"),
            ("it", "16/IT"),
        ]):
            icon = Image("fs_uae_launcher:res/" + icon_name + ".png")
            element = LanguageElement(self, language, icon)
            self.hori_layout.add(element, fill=True)
        self.hori_layout.add_spacer(16)

        element = PlayersElement(self)
        self.hori_layout.add(element, fill=True)

        # for config_key, icon_name in [
        #     ("database_url", "database_url_16"),
        #     ("hol_url", "hol_url_16"),
        #     ("lemonamiga_url", "lemon_url_16"),
        #     ("mobygames_url", "mobygames_url_16"),
        #     ("wikipedia_url", "wikipedia_url_16"),
        # ]:
        #     icon = Image("fs_uae_launcher:res/" + icon_name + ".png")
        #     element = LinkButtonElement(self, config_key, icon)
        #     self.hori_layout.add(element)

        element = WebLinkElement(self)
        self.hori_layout.add(element, fill=True)

        # this listener is added after all statusbar children have
        # added their listeners, this is important for re-layout...
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def on_config(self, key, value):
        layout = False
        if key in ["languages", "protection"]:
            layout = True
        if layout:
            self.layout.update()

    def on_paint(self):
        dc = self.create_dc()
        color_1 = Skin.get_background_color()
        if color_1 is not None:
            color_1 = color_1.copy().darken(0.12)
        else:
            color_1 = Color(0x00, 0x00, 0x00, 0x30)
        dc.draw_line(0, 0, self.size[0], 0, color_1)
        self.draw_background(self, dc, offset=1, height=self.size[1] - 1)

    @classmethod
    def draw_element_background(cls, widget, dc):
        cls.draw_background(widget, dc)

    @classmethod
    def draw_background(cls, widget, dc, offset=None, height=None):
        size = widget.size
        x = 0
        y = 0
        w = widget.size[0]
        h = widget.size[1]
        if offset is not None:
            y += offset
        if height is not None:
            h = height

        color_1 = Skin.get_background_color()
        color_2 = color_1
        #if fsui.System.macosx:
        #    color_1 = Color(0xa7, 0xa7, 0xa7)
        #    color_2 = Color(0xc0, 0xc0, 0xc0)
        if color_1 is not None:
            color_1 = color_1.copy().darken(0.08)
        else:
            color_1 = Color(0x00, 0x00, 0x00, 0x20)
            color_2 = Color(0x00, 0x00, 0x00, 0x00)
        dc.draw_vertical_gradient(x, y, w, h, color_1, color_2)
