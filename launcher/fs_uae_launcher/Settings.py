from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fs as fs
from .Signal import Signal
from .Util import memoize, get_real_case
from .I18N import _, ngettext

class Settings:
    irc_server = "irc.fengestad.no"

    #nick = "FrodeSolheim"
    #nick.

    #lobby_nick_number = 0

    base_dir = ""

    default_settings = {
        "__netplay_ready": "",
        "automatic_input_grab": "",
        "builtin_configs": "",
        "config_base": "",
        "config_changed": "0",
        "config_name": "Unnamed Configuration",
        "config_path": "",
        "config_refresh": "",
        "config_search": "",
        "config_xml_path": "",
        "configurations_dir_mtime": "",
        "database_feature": "",
        "database_password": "",
        "database_username": "",
        "fsaa": "",
        "floppy_drive_volume": "",
        "fullscreen": "",
        "fullscreen_mode": "fullscreen",
        "game_uuid": "",
        "initial_input_grab": "",
        "irc_nick": "",
        "irc_server": "",
        "keep_aspect": "",
        "kickstarts_dir_mtime": "",
        "kickstart_setup": "",
        "last_cd_dir": "",
        "last_floppy_dir": "",
        "last_hd_dir": "",
        "last_rom_dir": "",
        "last_scan": "",
        "low_latency_vsync": "",
        "maximized": "0",
        "middle_click_ungrab": "",
        "mouse_speed": "",
        "netplay_feature": "",
        "netplay_tag": "",
        "parent_uuid": "",
        "primary_joystick": "",
        "rtg_scanlines": "",
        "scan_configs": "1",
        "scan_files": "1",
        "scan_roms": "1",
        "scanlines": "",
        "search_path": "",
        "secondary_joystick": "",
        "swap_ctrl_keys": "",
        "texture_filter": "",
        "texture_format": "",
        "ui_background_color": "",
        "video_format": "",
        "video_sync": "",
        "video_sync_method": "",
        "zoom": "",
        #"window_width": "",
        #"window_height": "",
    }

    settings = default_settings.copy()
    #settings_listeners = []

    initialize_from_config = set([
        "fullscreen",
    ])

    @classmethod
    def get(cls, key):
        #return cls.settings.setdefault(key, "")
        return cls.settings.get(key, "")

    @classmethod
    def add_listener(cls, listener):
        #cls.settings_listeners.append(listener)
        Signal.add_listener("setting", listener)

    @classmethod
    def remove_listener(cls, listener):
        #cls.settings_listeners.remove(listener)
        Signal.remove_listener("setting", listener)

    @classmethod
    def set(cls, key, value):
        if cls.get(key) == value:
            print(u"set {0} to {1} (no change)".format(key, value))
            return
        print(u"set {0} to {1}".format(key, value))
        cls.settings[key] = value
        #for listener in cls.settings_listeners:
        #    listener.on_setting(key, value)
        Signal.broadcast("setting", key, value)

    @classmethod
    def get_irc_nick(cls):
        value = cls.settings.get("irc_nick", "").strip()
        if not value:
            value = fs.get_user_name()
        # these are probably valid too: \`
        valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                      "abcdefghijklmnopqrstuvwxyz" \
                      "_[]{}|^"
        extra_valid_chars = "0123456789-"
        nick = ""
        for c in value:
            if c in valid_chars:
                nick = nick + c
                if extra_valid_chars:
                    valid_chars += extra_valid_chars
                    extra_valid_chars = ""
        if not nick:
            nick = "User"
        return nick

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
    def read_custom_path(cls, name):
        for app in ["fs-uae-launcher", "fs-uae"]:
            key_path = os.path.join(fs.get_app_config_dir(app), name)
            print("- checking", key_path)
            if os.path.exists(key_path):
                try:
                    with open(key_path, "rb") as f:
                        path = f.read().strip()
                        break
                except Exception, e:
                    print("error reading custom path", repr(e))
        else:
            return None
        lpath = path.lower()
        if lpath.startswith("$home/") or lpath.startswith("$home\\"):
            path = os.path.join(fs.get_home_dir(), path[6:])
        return path

    @classmethod
    @memoize
    def get_base_dir(cls):
        path = cls.base_dir
        if not path:
            path = cls.read_custom_path("base-dir")
        if not path:
            path = os.path.join(fs.get_documents_dir(True), "FS-UAE")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        print("base dir is", path)
        return path

    @classmethod
    def get_default_search_path(cls):
        paths = []
        path = cls.get_base_dir()
        if fs.windows:
            path = path.replace("/", "\\")
        paths.append(path)

        if fs.windows:
            from win32com.shell import shell, shellcon
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_COMMON_DOCUMENTS, 0, 0)
            path = fs.unicode_path(path)
            path = os.path.join(path, "Amiga Files")
            if os.path.exists(path):
                print("\n\n\n", path, "exists!!\n\n\n")
                paths.append(path)
        return paths

    @classmethod
    def get_whdload_dir(cls):
        #path = os.path.join(fs.get_home_dir(), "Games", "Amiga", "WHDLoad")
        #if os.path.exists(path):
        #    return path
        path = os.path.join(cls.get_hard_drives_dir(), "WHDLoad")
        if os.path.exists(path):
            path = get_real_case(path)
            return path
        return None

    @classmethod
    def get_configurations_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Configurations")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_controllers_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Controllers")
        path = get_real_case(path)
        return path

    @classmethod
    def get_kickstarts_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Kickstarts")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_floppies_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Floppies")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_hard_drives_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Hard Drives")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_cdroms_dir(cls):
        path = os.path.join(cls.get_base_dir(), "CD-ROMs")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_logs_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Logs")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_launcher_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Launcher")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
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
        path = get_real_case(path)
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

    @classmethod
    def get_cache_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Cache")
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
