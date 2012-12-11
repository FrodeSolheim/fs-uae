from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import time
import traceback
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..Database import Database
from ..GameHandler import GameHandler
from .BottomPanel import BottomPanel
from .Constants import Constants
from .ImageLoader import ImageLoader
from .LaunchGroup import LaunchGroup
from .Skin import Skin

#BORDER = 5
#COVER_SIZE = (110, 146)
BORDER = 20
#COVER_SIZE = (117, 156)
#COVER_SIZE = (110, 140)

class GameInfoPanel(BottomPanel):

    def __init__(self, parent):
        BottomPanel.__init__(self, parent)
        Skin.set_background_color(self)
        #self.set_background_color((0xdd, 0xdd, 0xdd))

        self.layout = fsui.HorizontalLayout()

        #def get_min_height():
        #    return Constants.COVER_SIZE[1] + 2 * BORDER
        #self.layout.get_min_height = get_min_height

        self.layout.padding_left = BORDER // 2
        self.layout.padding_right = BORDER // 2
        self.layout.padding_top = BORDER + 2
        self.layout.padding_bottom = Skin.get_bottom_margin()

        self.default_image = fsui.Image("fs_uae_launcher:res/cover.png")
        #self.default_image.resize(Constants.COVER_SIZE)
        self.cover_overlay = fsui.Image(
                "fs_uae_launcher:res/cover_overlay.png")
        self.cover_overlay_square = fsui.Image(
                "fs_uae_launcher:res/cover_overlay_square.png")

        #self.cover_view = fsui.ImageView(self, self.default_image)
        #self.layout.add(self.cover_view, expand=False, fill=True)
        self.layout.add_spacer(Constants.COVER_SIZE[0])

        #self.panel = fsui.Panel(self)
        #self.panel.set_background_color((0xdd, 0xdd, 0xdd))
        #self.layout.add(self.panel, expand=True, fill=True)

        vert_layout = fsui.VerticalLayout()
        self.layout.add(vert_layout, expand=True, fill=True)
        #self.panel.layout.padding_top = 10
        vert_layout.padding_left = BORDER

        #def label_min_width():
        #    return 330
        #self.title_label = fsui.HeadingLabel(self)
        #self.title_label.get_min_width = label_min_width
        #vert_layout.add(self.title_label)

        #self.sub_title_label = fsui.Label(self)
        #self.sub_title_label.get_min_width = label_min_width
        #vert_layout.add(self.sub_title_label)

        self.title = ""
        self.sub_title = ""

        vert_layout.add_spacer(0, expand=True)

        self.launch_group = LaunchGroup(self)
        vert_layout.add(self.launch_group, fill=True)

        self.load_info()
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def load_info(self):
        #t1 = time.time()
        handler = GameHandler.current()
        name = handler.get_name()
        variant = handler.get_variant()
        if variant.startswith("Amiga, "):
            variant = variant[7:]
        self.title = name
        self.sub_title = variant

        #image = handler.load_cover_preview()
        #if image:
        #    #self.cover_view.set_image(image)
        #    self.image = image
        #else:
        #    #self.cover_view.set_image(self.default_image)
        #    self.image = self.default_image
        #self.refresh()

        path = handler.get_cover_path()
        loader = ImageLoader.get()
        def on_load(request):
            #print("on_load, request.image =", request.image, request.image.size)
            if request.image:
                self.image = request.image
            else:
                self.image = self.default_image
            self.refresh()

        self.requests = loader.load_image(path,
                size=Constants.COVER_SIZE, on_load=on_load,
                is_cover=True)
        self.image = self.default_image
        self.refresh()

        #t2 = time.time()
        #print(t2 - t1)
        self.layout.update()

    def on_setting(self, key, value):
        if key == "config_name":
            self.load_info()

    def on_paint(self):
        dc = self.create_dc()
        self.draw_background(dc)

        y = 2 + 20
        x = 10

        image = self.image
        #dc.draw_image(image, x, y)
        if image.size[0] == image.size[1]:
            cover_overlay = self.cover_overlay_square
            y_offset = 28
            title_x = 10
        else:
            cover_overlay = self.cover_overlay
            y_offset = 0
            title_x = 10 + Constants.COVER_SIZE[0] + 20
        dc.draw_image(image, x + 1, y + 1 + y_offset)
        dc.draw_image(cover_overlay, x - 10, y - 10 + y_offset)

        font = dc.get_font()
        font.set_bold(True)
        dc.set_font(font)

        x = 10 + Constants.COVER_SIZE[0] + 20
        dc.draw_text(self.title, title_x, y)
        tw, th = dc.measure_text(self.title)
        #y += int(th * 1.2)
        y += 24

        font.set_bold(False)
        dc.set_font(font)
        color = dc.get_text_color()
        color.mix(self.get_background_color(), 0.33)
        dc.set_text_color(color)
        dc.draw_text(self.sub_title, x, y)
