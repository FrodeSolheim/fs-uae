from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from .Constants import Constants
from .Skin import Skin

class BottomPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent, paintable=True)
        Skin.set_background_color(self)

        self.color_2 = Skin.get_background_color()
        if self.color_2 is not None:
            self.color_1 = self.color_2.copy().darken(0.08)
            self.line_color_1 = self.color_2.copy().mix(
                    fsui.Color(0xff, 0xff, 0xff))
            self.line_color_2 = self.line_color_1
        else:
	    self.color_1 = None
	    self.line_color_1 = fsui.Color(0xff, 0xff, 0xff)
	    self.line_color_2 = self.line_color_1

    def on_paint(self):
        dc = self.create_dc()
        self.draw_background(dc)

    def draw_background(self, dc):
        size = self.size
        dc.set_color(self.line_color_1)
        dc.draw_line(0, 0, size[0], 0)
        dc.set_color(self.line_color_2)
        dc.draw_line(0, 1, size[0], 1)

        if self.color_1 is not None:
            dc.draw_vertical_gradient(0, 2, self.size[0], self.size[1] - 2,
                    self.color_1, self.color_2)
