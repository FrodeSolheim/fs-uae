from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from .Constants import Constants
from .Skin import Skin
from .TabPanel import TabPanel

class TabButton(fsui.Panel):

    TYPE_TAB = 0
    TYPE_BUTTON = 1

    STATE_NORMAL = 0
    STATE_SELECTED = 1

    def __init__(self, parent, icon, type=TYPE_TAB, left_padding=0,
            right_padding=0):
        fsui.Panel.__init__(self, parent, paintable=True)
        Skin.set_background_color(self)
        self.layout = fsui.VerticalLayout()
        #self.set_background_color((0xdd, 0xdd, 0xdd))
        self.set_min_width(Constants.TAB_WIDTH + left_padding + right_padding)
        self.set_min_height(Constants.TAB_HEIGHT)
        self.group_id = 0
        self.icon = icon
        self.type = type
        self.state = self.STATE_NORMAL
        self.hover = False
        self.left_padding = left_padding
        self.right_padding = right_padding

    def select(self):
        self.get_parent().set_selected_tab(self)
        self.on_select()

    def on_paint(self):
        dc = self.create_dc()
        selected = (self.state == self.STATE_SELECTED)
        TabPanel.draw_background(self, dc, selected=selected, hover=self.hover,
                button_style=(self.type == self.TYPE_BUTTON))
        #TabPanel.draw_border(self, dc)

        x = self.left_padding + (self.size[0] - self.left_padding - \
                self.right_padding - self.icon.size[0]) // 2
        # subtracing two because of bottom border
        y = (self.size[1] - 2 - self.icon.size[1]) // 2
        dc.draw_image(self.icon, x, y)

    def check_hover(self):
        # FIXME: check if mouse is hovering, used for example after having
        # dismissed a popup menu
        self.hover = False
        self.refresh()

    def on_mouse_enter(self):
        self.hover = True
        self.refresh()

    def on_mouse_leave(self):
        self.hover = False
        self.refresh()

    def on_select(self):
        pass

    def on_left_down(self):
        if self.type == self.TYPE_TAB:
            self.select()

    def on_left_up(self):
        if self.type == self.TYPE_BUTTON:
            # FIXME: hack to avoid sticky hover due to mouse leave not
            # being sent if on_activate opens modal dialog
            #self.hover = False
            #self.refresh()
            self.on_activate()

    def on_activate(self):
        pass
