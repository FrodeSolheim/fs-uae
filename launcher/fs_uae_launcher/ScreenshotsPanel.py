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

BORDER = 5
SCREEN_SIZE = (222, 146)

class ScreenshotsPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.set_background_color((0x44, 0x44, 0x44))
        #self.set_background_color((0x66, 0x66, 0x66))

        self.layout = fsui.HorizontalLayout()
        def get_min_width():
            return 0
        def get_min_height():
            return SCREEN_SIZE[1] + 2 * BORDER
        self.layout.get_min_width = get_min_width
        self.layout.get_min_height = get_min_height
        self.layout.padding_left = BORDER
        self.layout.padding_right = BORDER
        self.layout.padding_top = BORDER
        self.layout.padding_bottom = BORDER

        self.default_image = fsui.Image("fs_uae_launcher:res/screenshot.png")
        self.default_image.resize(SCREEN_SIZE)

        #self.title_views = []
        self.screen_views = []
        self.screen_views.append(fsui.ImageView(self, self.default_image))
        self.layout.add(self.screen_views[0], expand=False, fill=True)


        #self.screen_view = fsui.ImageView(self, self.default_image)
        #self.layout.add(self.screen_view, expand=False, fill=True)
        #self.screen_views.append(

        for i in range(5):
            self.layout.add_spacer(BORDER + 1)
            self.screen_views.append(fsui.ImageView(self, self.default_image))
            self.layout.add(self.screen_views[i + 1], expand=False, fill=True)

        if Settings.get("config_base"):
            self.load_images(Settings.get("config_base"))
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def set_min_screenshots(self, count):
        w = SCREEN_SIZE[0] * count + BORDER * 2 + (BORDER + 1) * (count - 1)
        self.set_min_width(w)

    def load_images(self, name):
        t1 = time.time()
        handler = GameHandler.current()
        for i in range(6):
            image = handler.load_screenshot_preview(i)
            view = self.screen_views[i]
            if image:
                view.set_image(image)
            else:
                view.set_image(self.default_image)

        #self.load_image(name, "Titles", 0, self.title_views)
        #for i in range(5):
        #    if i == 0:
        #        screen_name = name
        #    else:
        #        screen_name = u"{0}_{1}".format(name, i + 1)
        #    self.load_image(screen_name, "Screenshots", i, self.screen_views)
        t2 = time.time()
        print(t2 - t1)

    """
    def load_image(self, name, category, index, views):
        view = views[index]
        if not name:
            view.set_image(self.default_image)
            return
        letter_name = name.upper()
        if letter_name.startswith("THE "):
            letter_name = letter_name[4:]
        if letter_name.startswith("A "):
            letter_name = letter_name[2:]
        for i in range(len(letter_name)):
            letter = letter_name[i]
            if letter in "01234567890":
                letter = "0"
                break
            if letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
                break
        else:
            raise Exception("could not find letter for game")


        if category == "Titles":
            paths = Settings.get_titles_dirs()
        else:
            paths = Settings.get_screenshots_dirs()
        for dir in paths:
            path = os.path.join(dir, letter, name + u".png")
            if not os.path.exists(path):
                print("n/a", path)
                path = os.path.join(dir, letter, name + u".gif")
            if os.path.exists(path):
                #print("found")
                image = fsui.Image(path)
                if image.size[0] < 400:
                    image.resize((image.size[0] * 2, image.size[1] * 2), fsui.Image.NEAREST)
                image.resize(SCREEN_SIZE)
                #print(image)
                view.set_image(image)
                break
        else:
            view.set_image(self.default_image)
        """

    def on_setting(self, key, value):
        if key == "config_name":
            self.load_images(value)

