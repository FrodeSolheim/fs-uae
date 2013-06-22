from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from .Constants import Constants
from .Skin import Skin

class TabPanel(fsui.Panel):

    def __init__(self, parent, spacing=10):
        fsui.Panel.__init__(self, parent, paintable=True)
        Skin.set_background_color(self)
        self.layout = fsui.HorizontalLayout()
        #self.layout.add_spacer(spacing)
        #self.layout.padding_left = 10
        #self.layout.padding_right = 10

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
                        if child.group_id == tab.group_id:
                            child.state = child.STATE_NORMAL
                            child.refresh()

    def add(self, button, expand=False):
        self.layout.add(button, expand=expand)

    def add_spacer(self, spacer=0, expand=False):
        self.layout.add_spacer(spacer, 0, expand=expand)

    def on_paint(self):
        dc = self.create_dc()
        self.draw_background(self, dc)
        #self.draw_border(self, dc)

    @classmethod
    def draw_border(cls, widget, dc):
        size = widget.size

        line_color_1 = Skin.get_background_color()
        if line_color_1 is not None:
            line_color_1 = line_color_1.mix(fsui.Color(0xff, 0xff, 0xff))
            line_color_2 = line_color_1
        else:
            line_color_1 = fsui.Color(0xff, 0xff, 0xff, 0xa0)
            line_color_2 = line_color_1

        #line_color_1 = fsui.Color(0xff, 0x00, 0x00, 0xff)
        #line_color_2 = fsui.Color(0x00, 0xff, 0x00, 0xff)

        dc.draw_line(0, size[1] - 2, size[0], size[1] - 2, line_color_1)
        dc.draw_line(0, size[1] - 1, size[0], size[1] - 1, line_color_2)

    @classmethod
    def draw_background(cls, widget, dc, selected=False, hover=False,
                button_style=True):
        if selected:
            cls.draw_selected_tab(widget, dc)
        else:
            cls.draw_border(widget, dc)

        size = widget.size
        x = 0
        y = 0
        w = widget.size[0]
        h = widget.size[1] - 2
        if fsui.System.macosx:
            #dc.draw_line(0, 0, w, 0, fsui.Color(198, 198, 198))
            dc.draw_line(0, 0, w, 0, fsui.Color(188, 188, 188))
            y += 1
            h -= 1

        if selected:
            x += 2
            w -= 4
            h += 2

        #if button_style and hover:
        #    x += 6
        #    y += 6
        #    w -= 12
        #    h -= 12

        color_1 = Skin.get_background_color()
        if fsui.System.macosx:
            if selected:
                color_2 = color_1
                color_1 = fsui.Color(0xa7, 0xa7, 0xa7)
            elif hover:
                color_1 = fsui.Color(0xa7, 0xa7, 0xa7)
                color_2 = fsui.Color(0xef, 0xef, 0xef)
            else:
                color_1 = fsui.Color(0xa7, 0xa7, 0xa7)
                color_2 = fsui.Color(0xc0, 0xc0, 0xc0)
        elif color_1 is not None:
            if selected:
                color_2 = color_1
            elif hover:
                color_2 = color_1.copy().lighten()
            else:
                color_2 = color_1.copy().darken(0.08)
        else:
            if selected:
                return
                #color_1 = fsui.Color(0x00, 0x00, 0x00, 0x00)
                #color_2 = color_1
            elif hover:
                color_1 = fsui.Color(0xff, 0xff, 0xff, 0x00)
                color_2 = fsui.Color(0xff, 0xff, 0xff, 0x40)
            else:
                color_1 = fsui.Color(0x00, 0x00, 0x00, 0x00)
                color_2 = fsui.Color(0x00, 0x00, 0x00, 0x20)
        dc.draw_vertical_gradient(x, y, w, h,
                color_1, color_2)

        if fsui.System.macosx and not selected and not hover:
            dc.draw_line(x, y + h - 1, x + w, y + h - 1,
                         fsui.Color(0xa8, 0xa8, 0xa8))

    @classmethod
    def draw_selected_tab(cls, widget, dc):
        size = widget.size

        line_color_1 = Skin.get_background_color()
        if fsui.System.macosx:
            line_color_1 = fsui.Color(0xa7, 0xa7, 0xa7)
            line_color_2 = Skin.get_background_color().mix(
                    fsui.Color(0xff, 0xff, 0xff))
        elif line_color_1 is not None:
            line_color_2 = Skin.get_background_color().mix(
                    fsui.Color(0xff, 0xff, 0xff))
        else:
            line_color_1 = fsui.Color(0xff, 0xff, 0xff, 0x00)
            line_color_2 = fsui.Color(0xff, 0xff, 0xff, 0xa0)

        dc.draw_vertical_gradient(0, 0, 2, widget.size[1],
                line_color_1, line_color_2)
        dc.draw_vertical_gradient(widget.size[0] - 2, 0, 2, widget.size[1],
                line_color_1, line_color_2)
