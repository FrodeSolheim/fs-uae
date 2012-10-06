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
from .Constants import Constants
from .BottomPanel import BottomPanel
from .Skin import Skin

BORDER = 20

class ScreenshotsPanel(BottomPanel):

    def __init__(self, parent):
        BottomPanel.__init__(self, parent)
        Skin.set_background_color(self)
        self.layout = fsui.HorizontalLayout()

        def get_min_width():
            return 0
        #def get_min_height():
        #    return Constants.SCREEN_SIZE[1] + 2 * BORDER
        self.layout.get_min_width = get_min_width
        #self.layout.get_min_height = get_min_height
        self.layout.padding_left = BORDER // 2
        self.layout.padding_right = BORDER // 2
        self.layout.padding_top = BORDER + 2
        self.layout.padding_bottom = Skin.get_bottom_margin()

        self.default_image = fsui.Image("fs_uae_launcher:res/screenshot.png")
        #self.default_image.resize(Constants.SCREEN_SIZE)
        self.screenshot_overlay = fsui.Image(
                "fs_uae_launcher:res/screenshot_overlay.png")

        #self.title_views = []
        #self.screen_views = []
        #self.screen_views.append(fsui.ImageView(self, self.default_image))
        #self.layout.add(self.screen_views[0], expand=False, fill=True)


        #self.screen_view = fsui.ImageView(self, self.default_image)
        #self.layout.add(self.screen_view, expand=False, fill=True)
        #self.screen_views.append(

        #for i in range(5):
        #    #self.layout.add_spacer(BORDER + 1)
        #    self.layout.add_spacer(BORDER)
        #    self.screen_views.append(fsui.ImageView(self, self.default_image))
        #    self.layout.add(self.screen_views[i + 1], expand=False, fill=True)
        self.images = [self.default_image for x in range(6)]

        if Settings.get("config_base"):
            self.load_images(Settings.get("config_base"))
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def set_min_screenshots(self, count):
        #w = SCREEN_SIZE[0] * count + BORDER * 2 + (BORDER + 1) * (count - 1)
        w = Constants.SCREEN_SIZE[0] * count + BORDER * 2 + (BORDER) * (count - 1)
        self.set_min_width(w)

    def load_images(self, name):
        t1 = time.time()
        handler = GameHandler.current()
        for i in range(6):
            image = handler.load_screenshot_preview(i)
            #view = self.screen_views[i]
            if image:
                #view.set_image(image)
                self.images[i] = image
                self.refresh()
            else:
                #view.set_image(self.default_image)
                self.images[i] = self.default_image
                self.refresh()

        #self.load_image(name, "Titles", 0, self.title_views)
        #for i in range(5):
        #    if i == 0:
        #        screen_name = name
        #    else:
        #        screen_name = u"{0}_{1}".format(name, i + 1)
        #    self.load_image(screen_name, "Screenshots", i, self.screen_views)
        t2 = time.time()
        print(t2 - t1)

    def on_setting(self, key, value):
        if key == "config_name":
            self.load_images(value)

    def on_paint(self):
        dc = self.create_dc()
        self.draw_background(dc)
        size = self.size

        y = 2 + 20
        x = 10
        for i in range(6):
            if x >= size[0]:
                break
            image = self.images[i]
            dc.draw_image(image, x + 1, y + 1)
            dc.draw_image(self.screenshot_overlay, x - 10, y - 10)
            x = x + Constants.SCREEN_SIZE[0] + 20
