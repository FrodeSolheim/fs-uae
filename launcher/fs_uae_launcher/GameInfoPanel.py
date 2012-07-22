from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import time
import traceback
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .Database import Database
from .GameHandler import GameHandler

#BORDER = 5
#COVER_SIZE = (110, 146)
BORDER = 0
COVER_SIZE = (117, 156)

class GameInfoPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        #self.set_background_color((0xbb, 0xbb, 0xbb))
        #self.set_background_color((0x44, 0x44, 0x44))

        self.layout = fsui.HorizontalLayout()

        def get_min_height():
            return COVER_SIZE[1] + 2 * BORDER
        self.layout.get_min_height = get_min_height

        self.layout.padding_left = BORDER
        self.layout.padding_right = BORDER
        self.layout.padding_top = BORDER
        self.layout.padding_bottom = BORDER

        self.default_image = fsui.Image("fs_uae_launcher:res/cover.png")
        self.default_image.resize(COVER_SIZE)

        self.cover_view = fsui.ImageView(self, self.default_image)
        self.layout.add(self.cover_view, expand=False, fill=True)
        self.layout.add_spacer(0)

        self.panel = fsui.Panel(self)
        #self.panel.set_background_color((0x44, 0x44, 0x44))
        #self.panel.set_background_color((0xbb, 0xbb, 0xbb))
        self.panel.set_background_color((0xcc, 0xcc, 0xcc))
        self.layout.add(self.panel, expand=True, fill=True)
        self.panel.layout = fsui.VerticalLayout()
        self.panel.layout.padding_top = 10
        self.panel.layout.padding_left = 10

        def label_min_width():
            return 330

        self.title_label = fsui.HeadingLabel(self.panel)
        self.title_label.get_min_width = label_min_width
        self.panel.layout.add(self.title_label)

        self.sub_title_label = fsui.Label(self.panel)
        self.sub_title_label.get_min_width = label_min_width
        self.panel.layout.add(self.sub_title_label)

        self.load_info()
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def load_info(self):
        t1 = time.time()
        handler = GameHandler.current()
        name = handler.get_name()
        variant = handler.get_variant()
        if variant.startswith("Amiga, "):
            variant = variant[7:]
        self.title_label.set_text(name)
        self.sub_title_label.set_text(variant)

        image = handler.load_cover_preview()
        if image:
            self.cover_view.set_image(image)
        else:
            self.cover_view.set_image(self.default_image)

        t2 = time.time()
        print(t2 - t1)
        self.layout.update()

    def on_setting(self, key, value):
        if key == "config_name":
            self.load_info()

