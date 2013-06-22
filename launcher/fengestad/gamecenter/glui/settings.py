from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import pygame
from fengestad import fs

@fs.cache
def get_fullscreen_size():
    pygame.display.init()
    display_info = pygame.display.Info()
    size = (display_info.current_w, display_info.current_h)
    print("fullscreen size:", size)
    return size

class Settings:
    cache_dir_path = None
    games_dir_path = None
    profile_dir_path = None
    #use_fullscreen = False

    if fs.windows:
        fullscreen_menu = False
        window_decorations = False
    else:
        fullscreen_menu = True
        window_decorations = True

    fullscreen_game = True
    #windowed_size = (640, 480)
    windowed_size = None # Maximized
    fullscreen_size = get_fullscreen_size()
    fullscreen_pos = 0, 0

    @classmethod
    def get_games_path(cls):
        path = []
        if cls.games_dir_path:
            path.extend(cls.games_dir_path)
        elif fs.windows:
            path.append(os.path.join(fs.get_documents_dir(),
                    "Games (Ku Game System)"))
        else:
            path.append(os.path.join(fs.get_home_dir(), "Games"))
        return path

    @classmethod
    def get_resources_dir(cls):
        path = os.path.join(cls.get_games_path()[0], "Resources")
        return path

    @classmethod
    def get_config_dir(cls):
        pass

#    @classmethod
#    def get_data_dir(cls):
#        pass

    @classmethod
    def get_profile_dir(cls):
        if cls.profile_dir_path:
            return cls.profile_dir_path
        elif fs.windows:
            return os.path.join(fs.get_documents_dir(),
                    "Gamer Profile (Ku Game System)")
        else:
            return os.path.join(fs.get_home_dir(),
                    "Gamer Profile (Ku Game System)")

    @classmethod
    def get_cache_dir(cls):
        if cls.cache_dir_path:
            return cls.cache_dir_path
        return fs.get_cache_dir()

