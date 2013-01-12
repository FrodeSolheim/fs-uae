from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext
from ..Settings import Settings
from ..Database import Database

class VariantRatingButton(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent, paintable=True)
        self.set_min_width(40)
        self.set_min_height(24)

        self.rating = 0
        Config.add_listener(self)
        self.on_config("variant_rating", "")

    def on_destroy(self):
        Config.remove_listener(self)
        pass

    def on_config(self, key, value):
        if key == "__variant_rating":
            try:
                self.rating = int(value)
            except ValueError:
                self.rating = 0
            self.refresh()

    def on_paint(self):
        dc = self.create_dc()
        size = self.size

        foreground = fsui.Color(0xff, 0xff, 0xff)
        rating = self.rating
        if rating == -3:
            background = fsui.Color(0xa0, 0x00, 0x00)
        elif rating == -1:
            background = fsui.Color(0x00, 0x70, 0x00)
        elif rating == 1:
            background = fsui.Color(0xb0, 0x00, 0x00)
        elif rating == 2:
            background = fsui.Color(0xc0, 0x40, 0x00)
        elif rating == 3:
            background = fsui.Color(0xd0, 0x70, 0x00)
        elif rating == 4:
            background = fsui.Color(0xe0, 0x90, 0x00)
        elif rating >= 5:
            background = fsui.Color(0xf0, 0xa0, 0x00)
        else:
            background = fsui.Color(0xa8, 0xa8, 0xa8)

        if rating == -3:
            text = "BAD"
        elif rating == -1:
            text = "OK"
        elif rating >= 1:
            text = str(rating)
            font = dc.get_font()
            font.set_bold(True)
            dc.set_font(font)
        else:
            text = "-"

        tw, th = dc.measure_text(text)
        dc.draw_rectangle(0, 0, size[0], size[1], background)
        dc.set_text_color(foreground)
        dc.draw_text(text, (size[0] - tw) // 2, (size[1] - th) // 2)
