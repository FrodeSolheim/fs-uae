from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from .Constants import Constants
from .Skin import Skin

class TabPanel(fsui.Panel):

    def __init__(self, parent, spacing=10):
        fsui.Panel.__init__(self, parent, paintable=True)
        Skin.set_background_color(self)
        self.layout = fsui.HorizontalLayout()
        self.layout.add_spacer(spacing)

        #self.set_background_color((0xdd, 0xdd, 0xdd))
        self.set_min_height(Constants.TAB_HEIGHT)

    def select_tab(self, index):
        counter = 0
        for child in self.layout.children:
            child = child.element
            if hasattr(child, "type"):
                if child.type == child.TYPE_TAB:
                    if counter == index:
                        child.select()
                    counter = counter + 1

    def set_selected_tab(self, tab):
        for child in self.layout.children:
            child = child.element
            if hasattr(child, "type"):
                if child.type == child.TYPE_TAB:
                    if child == tab:                          
                        child.state = child.STATE_SELECTED
                        child.refresh()
                    elif child.state == child.STATE_SELECTED:
                        child.state = child.STATE_NORMAL
                        child.refresh()

    def add(self, button):
        self.layout.add(button)

    def add_spacer(self, expand=False):
        self.layout.add_spacer(0, expand=expand)

    def on_paint(self):
        dc = self.create_dc()
        self.draw_background(self, dc)
        self.draw_border(self, dc)

    @classmethod
    def draw_border(cls, widget, dc):
        size = widget.size

        line_color_1 = Skin.get_background_color().mix(
                fsui.Color(0xff, 0xff, 0xff))
        line_color_2 = line_color_1

        dc.set_color(line_color_1)
        dc.draw_line(0, size[1] - 2, size[0], size[1] - 2)
        dc.set_color(line_color_2)
        dc.draw_line(0, size[1] - 1, size[0], size[1] - 1)

    @classmethod
    def draw_background(cls, widget, dc, selected=False, hover=False):
        size = widget.size

        color_1 = Skin.get_background_color()
        x = 0
        w = widget.size[0]
        h = widget.size[1] - 2
        if selected:
            color_2 = color_1
            x += 2
            w -= 4
            h += 2
        elif hover:
            color_2 = color_1.copy().lighten()
        else:
            color_2 = color_1.copy().darken(0.08)

        if selected:
            cls.draw_selected_tab(widget, dc)
        else:
            cls.draw_border(widget, dc)
        dc.draw_vertical_gradient(x, 0, w, h,
                color_1, color_2)

    @classmethod
    def draw_selected_tab(cls, widget, dc):
        size = widget.size

        color_1 = Skin.get_background_color()
        color_2 = Skin.get_background_color().mix(
                fsui.Color(0xff, 0xff, 0xff))

        dc.draw_vertical_gradient(0, 0, 2, widget.size[1],
                color_1, color_2)
        dc.draw_vertical_gradient(widget.size[0] - 2, 0, 2, widget.size[1],
                color_1, color_2)
