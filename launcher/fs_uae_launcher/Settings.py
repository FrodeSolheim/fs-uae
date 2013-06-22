from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fs as fs
from .Signal import Signal


class Settings:
    irc_server = "irc.fengestad.no"

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
            print("set {0} to {1} (no change)".format(key, value))
            return
        print("set {0} to {1}".format(key, value))
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
        valid_chars = ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "abcdefghijklmnopqrstuvwxyz"
                       "_[]{}|^")
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
#