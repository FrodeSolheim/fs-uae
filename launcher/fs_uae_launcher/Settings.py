from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fs as fs
from .Util import memoize, get_real_case
from .I18N import _, ngettext

class Settings:
    irc_server = "irc.fengestad.no"

    #nick = "FrodeSolheim"
    #nick.

    #lobby_nick_number = 0
    
    base_dir = ""

    default_settings = {
        "config_xml_path": "",
        "last_scan": "",
        "__netplay_ready": "",
        "config_base": "",
        "config_refresh": "",
        "maximized": "0",
        "fullscreen": "0",
        "fullscreen_mode": "fullscreen",
        "config_search": "",
        "config_name": "Unnamed Configuration",
        "config_path": "",
        "scan_roms": "1",
        "scan_configs": "1",
        "scan_files": "1",
        "primary_joystick": "",
        "secondary_joystick": "",
        "search_path": "",
        "last_cd_dir": "",
        "last_hd_dir": "",
        "last_floppy_dir": "",
        "last_rom_dir": "",
        "window_width": "",
        "window_height": "",
        }

    settings = default_settings.copy()
    settings_listeners = []

    @classmethod
    def get(cls, key):
        return cls.settings.setdefault(key, "")

    @classmethod
    def add_listener(cls, listener):
        cls.settings_listeners.append(listener)

    @classmethod
    def remove_listener(cls, listener):
        cls.settings_listeners.remove(listener)

    @classmethod
    def set(cls, key, value):
        if cls.get(key) == value:
            print(u"set {0} to {1} (no change)".format(key, value))
            return
        print(u"set {0} to {1}".format(key, value))
        cls.settings[key] = value
        for listener in cls.settings_listeners:
            listener.on_setting(key, value)

    @classmethod
    def get_irc_nick(cls):
        value = cls.settings.get("irc_nick", "").strip()
        if value:
            return value
        return fs.get_user_name()

    @classmethod
    def get_irc_nickserv_pass(cls):
        value = cls.settings.get("irc_nickserv_pass", "").strip()
        if value:
            return value
        return ""


    @classmethod
    def get_irc_server(cls):
        value = cls.settings.get("irc_server", "").strip()
        if value:
            return value
        return "irc.fengestad.no"

    @classmethod
    @memoize
    def get_base_dir(cls):
        path = cls.base_dir
        #if "base_dir" in cls.settings.keys():
        #    path = cls.get("base_dir")
        if not path:
            path = os.path.join(fs.get_documents_dir(True), "FS-UAE")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_default_search_path(cls):
        paths = []
        paths.append(cls.get_base_dir())
        paths.append(os.path.join(fs.get_home_dir(),
                "Games", "Amiga"))
        paths.append(os.path.join(fs.get_home_dir(),
                "Games", "CD32"))
        paths.append(os.path.join(fs.get_home_dir(),
                "Games", "CDTV"))
        return paths

    @classmethod
    def get_whdload_dir(cls):
        path = os.path.join(fs.get_home_dir(), "Games", "Amiga", "WHDLoad")
        if os.path.exists(path):
            return path
        path = os.path.join(cls.get_base_dir(), "WHDLoad")
        if os.path.exists(path):
            return path
        return None

    @classmethod
    def get_configurations_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Configurations")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_controllers_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Controllers")
        return path

    @classmethod
    def get_kickstarts_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Kickstarts")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_floppies_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Floppies")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_hard_drives_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Hard Drives")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_cdroms_dir(cls):
        path = os.path.join(cls.get_base_dir(), "CD-ROMs")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_logs_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Logs")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_launcher_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Launcher")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_titles_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Titles")
        return path

    @classmethod
    def get_save_states_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Save States")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_screenshots_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Screenshots")
        return path

    @classmethod
    def get_images_dir(cls):
        path = cls.get_base_dir()
        return path

    @classmethod
    def get_covers_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Covers")
        return path

    @classmethod
    def get_themes_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Themes")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    #@classmethod
    #def get_config_dirs(cls):
    #    paths = [cls.get_floppies_dir(), cls.get_cdroms_dir(),
    #             os.path.expanduser("~/Games/Amiga/Files"),
    #             os.path.expanduser("~/Games/CD32/Files"),
    #             os.path.expanduser("~/Games/CDTV/Files")]
    #    return paths

    @classmethod
    def get_files_dirs(cls):
        paths = [cls.get_floppies_dir(), cls.get_cdroms_dir(),
                 os.path.expanduser("~/Games/Amiga/Files/"),
                 os.path.expanduser("~/Games/CD32/Files"),
                 os.path.expanduser("~/Games/CDTV/Files")
        ]
        return paths

    @classmethod
    def get_titles_dirs(cls):
        paths = [cls.get_titles_dir(),
                 os.path.join(fs.get_home_dir(), "Games",
                         "Amiga", "Titles"),
                 os.path.join(fs.get_home_dir(), "Games",
                         "CD32", "Titles"),
                 os.path.join(fs.get_home_dir(), "Games",
                         "CDTV", "Titles"),
        ]
        return paths

    @classmethod
    def get_screenshots_dirs(cls):
        paths = [cls.get_screenshots_dir(),
                 os.path.join(fs.get_home_dir(), "Games",
                         "Amiga", "Screenshots"),
                 os.path.join(fs.get_home_dir(), "Games",
                         "CD32", "Screenshots"),
                 os.path.join(fs.get_home_dir(), "Games",
                         "CDTV", "Screenshots"),
        ]
        return paths

    @classmethod
    def get_images_dirs(cls):
        paths = [cls.get_images_dir(),
                 os.path.join(fs.get_home_dir(), "Games"),
                 #os.path.join(fs.get_home_dir(), "Games",
                 #        "Amiga", "Images"),
                 #os.path.join(fs.get_home_dir(), "Games",
                 #        "CD32", "Images"),
                 #os.path.join(fs.get_home_dir(), "Games",
                 #        "CDTV", "Images"),
        ]
        return paths

    @classmethod
    def get_covers_dirs(cls):
        paths = [cls.get_covers_dir(),
                 os.path.join(fs.get_home_dir(), "Games",
                         "Amiga", "Covers"),
                 os.path.join(fs.get_home_dir(), "Games",
                         "CD32", "Covers"),
                 os.path.join(fs.get_home_dir(), "Games",
                         "CDTV", "Covers"),
        ]
        return paths

    @classmethod
    def get_themes_dirs(cls):
        paths = [cls.get_themes_dir()]
        return paths

    @classmethod
    def get_configurations_dirs(cls):
        paths = [cls.get_configurations_dir(),
                 #os.path.expanduser("~/Games/Amiga/Configs"),
                 os.path.expanduser("~/Games/Amiga/Data"),
                 #os.path.expanduser("~/Games/CD32/Configs"),
                 os.path.expanduser("~/Games/CD32/Data"),
                 #os.path.expanduser("~/Games/CDTV/Configs"),
                 os.path.expanduser("~/Games/CDTV/Data")
        ]
        return paths
