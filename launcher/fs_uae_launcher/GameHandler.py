from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from .ui.Constants import Constants
from .Config import Config
from .Paths import Paths
from .Settings import Settings

class GameHandler:

    @staticmethod
    def current():
        model = Config.get("amiga_model")
        if model == "CD32":
            platform = "CD32"
        elif model == "CDTV":
            platform = "CDTV"
        else:
            platform = "Amiga"
        name = Settings.get("config_name")
        uuid = Config.get("x_game_uuid")
        return GameHandler(name, platform, uuid)

    def __init__(self, name, platform, uuid):
        self.uuid = uuid

        self.config_name = name
        if "(" in name:
            parts = name.split("(", 1)
            self.name, self.variant = parts
            self.name = self.name.strip()
            self.variant = self.variant.strip()
            if self.variant.endswith(u")"):
                self.variant = self.variant[:-1]
            self.variant = self.variant.replace(") (", ", ")
            self.variant = self.variant.replace(")(", ", ")
        else:
            self.name = name
            self.variant = ""
        self.platform = platform

    def get_name(self):
        return self.name

    def get_variant(self):
        return self.variant

    def get_override_path(self, name):
        path = Config.get(name)
        if not path:
            return ""
        path = Paths.expand_path(path)
        return path

    def get_screenshot_path(self, number):
        if number == 0:
            path = self.get_override_path("title_image")
        else:
            path = self.get_override_path("screen{0}_image".format(number))
        if path and os.path.exists(path):
            return path
        if self.uuid:
            if number == 0:
                name = "title.png"
            else:
                name = "screen{0}.png".format(number)
            paths = Settings.get_images_dirs()
            for dir in paths:
                p = os.path.join(dir, self.platform, u"Images",
                        self.uuid[:2], self.uuid, name)
                if os.path.exists(p):
                    return p
                p = os.path.join(dir, self.platform, u"Thumbnails",
                        self.uuid[:2], self.uuid, name)
                if os.path.exists(p):
                    return p
        letter = self.get_letter(self.name)
        if not letter:
            return None
        name = self.name
        if number == 0:
            override_dir = Config.get("titles_dir")
            if override_dir:
                paths = [Paths.expand_path(override_dir)]
            else:
                paths = Settings.get_titles_dirs()
        else:
            override_dir = Config.get("screenshots_dir")
            if override_dir:
                paths = [Paths.expand_path(override_dir)]
            else:
                paths = Settings.get_screenshots_dirs()
        if number >= 2:
            name = u"{0}_{1}".format(name, number)
        for dir in paths:
            path = os.path.join(dir, letter, name + u".png")
            if os.path.exists(path):
                return path
            path = os.path.join(dir, letter, name + u".gif")
            if os.path.exists(path):
                return path
            path = os.path.join(dir, name + u".png")
            if os.path.exists(path):
                return path
            path = os.path.join(dir, letter, name + u".gif")
            if os.path.exists(path):
                return path
        return None

    def load_screenshot(self, number):
        path = self.get_screenshot_path(number)
        if path:
            return fsui.Image(path)

    def load_screenshot_preview(self, number):
        image = self.load_screenshot(number)
        if image is None:
            return image
        if image.size == Constants.SCREEN_SIZE:
            return image
        if image.size[0] < 400:
            image.resize((image.size[0] * 2, image.size[1] * 2),
                    fsui.Image.NEAREST)
        image.resize(Constants.SCREEN_SIZE)
        return image

    def get_cover_path(self):
        path = self.get_override_path("cover_image")
        if path and os.path.exists(path):
            return path
        if self.uuid:
            paths = Settings.get_images_dirs()
            for dir in paths:
                p = os.path.join(dir, self.platform, u"Images",
                        self.uuid[:2], self.uuid, u"front.png")
                if os.path.exists(p):
                    return p
                p = os.path.join(dir, self.platform, u"Thumbnails",
                        self.uuid[:2], self.uuid, u"front.png")
                if os.path.exists(p):
                    return p
        letter = self.get_letter(self.name)
        if not letter:
            return None
        name = self.name
        override_dir = Config.get("covers_dir")
        if override_dir:
            paths = [Paths.expand_path(override_dir)]
        else:
            paths = Settings.get_covers_dirs()
        for dir in paths:
            path = os.path.join(dir, letter, name + u".jpg")
            if os.path.exists(path):
                return path
            path = os.path.join(dir, letter, name + u".png")
            if os.path.exists(path):
                return path
            path = os.path.join(dir, name + u".jpg")
            if os.path.exists(path):
                return path
            path = os.path.join(dir, name + u".png")
            if os.path.exists(path):
                return path
        return None

    def load_cover(self):
        path = self.get_cover_path()
        print(path)
        if path:
            return fsui.Image(path)

    def load_cover_preview(self):
        image = self.load_cover()
        if image is None:
            return image
        image.resize(Constants.COVER_SIZE)
        return image

    def get_theme_path(self):
        letter = self.get_letter(self.name)
        if not letter:
            return None
        paths = Settings.get_themes_dirs()
        for dir in paths:
            path = os.path.join(dir, letter, self.name)
            if os.path.exists(path):
                return path
        return None

    def _get_state_dir(self):
        config_name = self.config_name
        if not config_name:
            config_name = "Default"

        # use a temporary state dir, for now, to avoid problems with
        # floppy overlays etc interfering with net play
        from .netplay.Netplay import Netplay
        if Netplay.enabled:
            # it is possible to manually specify the state dir
            config_name = Config.get("__netplay_state_dir_name")
            if not config_name:
                # this is the default behavior, create a clean state
                # dir for the net play session
                netplay_game = Config.get("__netplay_game")
                if netplay_game:
                    config_name = "Net Play ({0})".format(netplay_game)

        letter = self.get_letter(config_name)
        if not letter:
            config_name = "Default"
            letter = self.get_letter(config_name)
        # we use an existing state dir in a "letter" dir if it exists
        # (legacy support).
        path = os.path.join(Settings.get_save_states_dir(), letter,
                config_name)
        if os.path.exists(path):
            return path
        # if not, we use a direct subfolder of save states dir
        path = os.path.join(Settings.get_save_states_dir(), config_name)
        return path

    def get_state_dir(self):
        state_dir = self._get_state_dir()
        if not os.path.exists(state_dir):
            os.makedirs(state_dir)
        return state_dir

    @staticmethod
    def get_letter(name):
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
            return None
        return letter
