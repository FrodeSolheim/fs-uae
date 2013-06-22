from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from .Constants import Constants
from ..Settings import Settings
from ..Util import memoize

#LEVEL = 0xce
LEVEL = 0xeb
#LEVEL = 0xe4

class Skin:

    EXTRA_GROUP_MARGIN = 0

    @classmethod
    def use_unified_toolbar(cls):
        return False

    @classmethod
    def get_background_color(cls):
        c = cls._get_background_color()
        if c is None:
            return None
        return c.copy()

    @classmethod
    @memoize
    def _get_background_color(cls):
        value = Settings.get("ui_background_color")
        if len(value) == 7 and value[0] == "#":
            def convert(s):
                return int(s, 16)
            r = convert(value[1:3])
            g = convert(value[3:5])
            b = convert(value[5:7])
            return fsui.Color(r, g, b)
        if fsui.System.windows:
            return fsui.Color(LEVEL, LEVEL, LEVEL)
        elif fsui.System.macosx:
            return fsui.Color(237, 237, 237)
        return None

    @classmethod
    def set_background_color(cls, widget):
        color = cls.get_background_color()
        if color is not None:
            widget.set_background_color(cls.get_background_color())

    @classmethod
    def get_window_padding_left(cls):
        return 10 + cls.EXTRA_GROUP_MARGIN

    @classmethod
    def get_window_padding_right(cls):
        return 10 + cls.EXTRA_GROUP_MARGIN

    @classmethod
    def get_window_padding_bottom(cls):
        return 10 + cls.EXTRA_GROUP_MARGIN

    @classmethod
    def get_bottom_margin(cls):
        return 10 + cls.get_window_padding_bottom()

    @classmethod
    def get_bottom_panel_height(cls):
        return Constants.SCREEN_SIZE[1] + 20 + 2 + 1 + 1 + \
                cls.get_bottom_margin()
