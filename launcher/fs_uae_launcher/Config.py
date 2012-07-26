from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import hashlib
import traceback
import ConfigParser
from .Amiga import Amiga
from .Warnings import Warnings
from .Database import Database

class Config:

    default_config = {
        "accuracy": "1",
        "amiga_model": "A500",
        "hard_drive_0": "",
        "hard_drive_1": "",
        "hard_drive_2": "",
        "hard_drive_3": "",
        "joystick_port_0": "",
        "joystick_port_0_mode": "mouse",
        "joystick_port_1": "",
        "joystick_port_1_mode": "joystick",
        "joystick_port_2": "",
        "joystick_port_2_mode": "nothing",
        "joystick_port_3": "",
        "joystick_port_3_mode": "nothing",
        "kickstart_file": "",
        "kickstart_ext_file": "",
        "x_kickstart_name": "",
        "x_kickstart_sha1": "",
        "x_kickstart_type": "default",
        #"x_check": "",
        "x_ready": "0",
        "x_game_uuid": "",
        "x_game_xml_path": "",
        "title": "",
        "sub_title": "",
        "viewport": "",
        "x_whdload_args": "",
        }

    skip_if_empty = set([
        "hard_drive_0",
        "hard_drive_1",
        "hard_drive_2",
        "hard_drive_3",
        "kickstart_ext_file",
        #"sub_title",
        "title",
        "viewport",
    ])

    # multiple options should be set in this order since some options will
    # implicitly change other options.

    order = [
        "amiga_model",
        "joystick_port_1_mode",
        "joystick_port_0_mode",
        "kickstart_file",
        "kickstart_ext_file",
    ]

    sync_keys = set([
        "accuracy",
        "amiga_model",
        "joystick_port_0",
        "joystick_port_0_mode",
        "joystick_port_1",
        "joystick_port_1_mode",
        "joystick_port_2",
        "joystick_port_2_mode",
        "joystick_port_3",
        "joystick_port_3_mode",
        "x_kickstart_name",
        "x_kickstart_sha1",
        #"x_check",
        "x_ready",
    ])

    checksum_keys = [
        "accuracy",
        "amiga_model",
        "joystick_port_0_mode",
        "joystick_port_1_mode",
        "x_kickstart_sha1",
    ]

    for i in range(4):
        key = "floppy_drive_{0}".format(i)
        skip_if_empty.add(key)
        default_config[key] = ""

        key = "x_floppy_drive_{0}_sha1".format(i)
        default_config[key] = ""

    for i in range(1):
        key = "cdrom_drive_{0}".format(i)
        skip_if_empty.add(key)
        default_config[key] = ""
        key = "x_cdrom_drive_{0}_sha1".format(i)
        default_config[key] = ""
    for i in range(20):
        key = "floppy_image_{0}".format(i)
        skip_if_empty.add(key)
        default_config[key] = ""
        #key2 = "x_floppy_image_{0}_name".format(i)
        key = "x_floppy_image_{0}_sha1".format(i)
        #default_config[key2] = ""
        default_config[key] = ""
        #sync_keys.add(key)

        key = "cdrom_image_{0}".format(i)
        skip_if_empty.add(key)
        default_config[key] = ""
        key = "x_cdrom_image_{0}_sha1".format(i)
        default_config[key] = ""
    for i in range(10):
        key = "hard_drive_{0}".format(i)

    config = default_config.copy()
    config_listeners = []

    @classmethod
    def copy(cls):
        return cls.config.copy()

    @classmethod
    def get(cls, key):
        return cls.config.setdefault(key, "")

    @classmethod
    def add_listener(cls, listener):
        cls.config_listeners.append(listener)

    @classmethod
    def remove_listener(cls, listener):
        cls.config_listeners.remove(listener)

    @classmethod
    def set(cls, key, value):
        if cls.get(key) == value:
            print(u"set {0} to {1} (no change)".format(key, value))
            return
        print(u"set {0} to {1}".format(key, value))
        cls.config[key] = value
        for listener in cls.config_listeners:
            listener.on_config(key, value)
        if key != "x_ready":
            cls.set("x_ready", "0")

    @classmethod
    def set_multiple(cls, items):
        # this will allow us to set multiple keys at a time, to provide
        # "atomic" setting of several keys before notifications are sent
        old = cls.config.copy()
        changed = set()
        for key, value in items:
            if old.get(key, "") == value:
                if value:
                    print(u"set {0} to {1} (no change)".format(key, value))
                continue
            print(u"set {0} to {1}".format(key, value))
            changed.add(key)
            cls.config[key] = value

        # and now broadcast all changed keys at once
        if len(changed) > 0:
            for key in sorted(changed):
                for listener in cls.config_listeners:
                    listener.on_config(key, cls.get(key))
            cls.set("x_ready", "0")

    @classmethod
    def sync_items(cls):
        for key, value in cls.config.iteritems():
            if key in cls.sync_keys:
                yield key, value

    @classmethod
    def checksum(cls):
        return cls.checksum_config(cls.config)

    @classmethod
    def checksum_config(cls, config):
        s = hashlib.sha1()
        for key in cls.checksum_keys:
            value = config[key]
            s.update(unicode(value).encode("UTF-8"))
        return s.hexdigest()

    @classmethod
    def update_kickstart(cls):
        if Config.get("x_kickstart_type") == "default":
            cls.set_kickstart_from_model()

    @classmethod
    def set_kickstart_from_model(cls):
        print("set_kickstart_from_model")
        model = Config.get("amiga_model")
        checksums = Amiga.get_model_config(model)["kickstarts"]
        for checksum in checksums:
            path = Database.get_instance().find_file(sha1=checksum)
            if path:
                #print("set kickstart to", path)
                Config.set("kickstart_file", path)
                # FIXME: set sha1 and name x_options also
                break
        else:
            print("WARNING: no suitable kickstart file found")
            Config.set("kickstart_file", "")
            Warnings.set("hardware", "kickstart",
                         "No suitable kickstart found")
            # FIXME: set sha1 and name x_options also

        checksums = Amiga.get_model_config(model)["ext_roms"]
        if len(checksums) == 0:
            Config.set("kickstart_ext_file", "")
        else:
            for checksum in checksums:
                path = Database.get_instance().find_file(sha1=checksum)
                if path:
                    #print("set kickstart to", path)
                    Config.set("kickstart_ext_file", path)
                    # FIXME: set sha1 and name x_options also
                    break
            else:
                print("WARNING: no suitable kickstart ext file found")
                Config.set("kickstart_ext_file", "")
                Warnings.set("hardware", "kickstart_ext",
                             "No suitable extended kickstart found")
                # FIXME: set sha1 and name x_options also

    @classmethod
    def load_default_config(cls):
        print("load_default_config")
        cls.load({})
        from .Settings import Settings
        # FIXME: remove use of config_base
        Settings.set("config_base", "")
        Settings.set("config_base", "")
        Settings.set("config_name", "")
        Settings.set("config_path", "")
        Settings.set("config_xml_path", "")

    @classmethod
    def load(cls, config):
        from .Settings import Settings
        update_config = {}
        for key, value in cls.default_config.iteritems():
            update_config[key] = value
        for key in cls.config.keys():
            if key not in cls.default_config:
                # this is not a recognized key, so we remove it
                del cls.config[key]

        for key, value in config.iteritems():
            # if this is a settings key, change settings instead
            if key in Settings.default_settings:
                Settings.set(key, value)
            else:
                update_config[key] = value

        #for key, value in update_config.iteritems():
        #    #cls.config[key] = value
        #    cls.set(key, value)
        cls.fix_config(update_config)
        cls.set_multiple(update_config.iteritems())
        cls.update_kickstart()

    @classmethod
    def fix_config(cls, config):
        from .Settings import Settings
        #for i in range(4):
        pass

        print("---", config["joystick_port_0"])
        print("---", config["joystick_port_1"])

        from .DeviceManager import DeviceManager
        available = DeviceManager.get_joystick_names()
        available.append("none")
        available.append("mouse")
        available.append("keyboard")
        available_lower = [x.lower() for x in available]

        # remove devices from available list if specified and found
        try:
            index = available_lower.index(config["joystick_port_1"].lower())
        except ValueError:
            pass
        else:
            del available[index]
            del available_lower[index]
        try:
            index = available_lower.index(config["joystick_port_0"].lower())
        except ValueError:
            pass
        else:
            del available[index]
            del available_lower[index]

        #if config in
        #print("--------------------------------------------")
        if config["joystick_port_1_mode"] in ["joystick", "cd32 gamepad"]:
            if not config["joystick_port_1"]:
                want = Settings.get("primary_joystick").lower()
                #print("want", want)
                try:
                    index = available_lower.index(want)
                except ValueError:
                    index = -1
                print("available", available_lower)
                print("want", repr(want), "index", index)
                if index == -1:
                    index = len(available) - 1
                if index >= 0:
                    config["joystick_port_1"] = available[index]
                    del available[index]
                    del available_lower[index]
        elif config["joystick_port_1_mode"] in ["mouse"]:
            if not config["joystick_port_1"]:
                config["joystick_port_1"] = "mouse"
        elif config["joystick_port_1_mode"] in ["nothing"]:
            if not config["joystick_port_1"]:
                config["joystick_port_1"] = "none"

        if config["joystick_port_0_mode"] in ["joystick", "cd32 gamepad"]:
            if not config["joystick_port_0"]:
                want = Settings.get("secondary_joystick").lower()
                try:
                    index = available_lower.index(want)
                except ValueError:
                    index = -1
                #print("want", want, "index", index)
                if index == -1:
                    index = len(available) - 1
                if index >= 0:
                    config["joystick_port_0"] = available[index]
                    del available[index]
                    del available_lower[index]
        elif config["joystick_port_0_mode"] in ["mouse"]:
            if not config["joystick_port_0"]:
                config["joystick_port_0"] = "mouse"
        elif config["joystick_port_0_mode"] in ["nothing"]:
            if not config["joystick_port_0"]:
                config["joystick_port_0"] = "none"


    @classmethod
    def load_file(cls, path):
        try:
            cls._load_file(path, "")
        except Exception, e:
            # FIXME: errors should be logged / displayed
            cls.load_default_config()
            traceback.print_exc()

    @classmethod
    def load_data(cls, data):
        print("Config.load_data")
        try:
            cls._load_file("", data)
        except Exception, e:
            # FIXME: errors should be logged / displayed
            cls.load_default_config()
            traceback.print_exc()

    @classmethod
    def create_fs_name(cls, name):
        name = name.replace(':', ' - ')
        name = name.replace('*', '-')
        name = name.replace('?', '')
        name = name.replace('/', '-')
        name = name.replace('\\', '-')
        name = name.replace('"', "'")
        for i in range(3):
            name = name.replace('  ', ' ')
        while name.endswith('.'):
            name = name[:-1]
        name = name.strip()
        return name

    @classmethod
    def _load_file(cls, path, data):
        if data:
            print("loading config from data")
        else:
            print("loading config from " + repr(path))
            if not os.path.exists(path):
                print("config file does not exist")
        if data:
            config_xml_path = ""
            from .XMLConfigLoader import XMLConfigLoader
            loader = XMLConfigLoader()
            loader.load_data(data)
            config = loader.get_config()
        elif path.endswith(".xml"):
            config_xml_path = path
            from .XMLConfigLoader import XMLConfigLoader
            loader = XMLConfigLoader()
            loader.load_file(path)
            config = loader.get_config()
        else:
            config_xml_path = ""
            cp = ConfigParser.ConfigParser()
            try:
                cp.read([path])
            except Exception, e:
                print(repr(e))
                return
            config = {}
            try:
                keys = cp.options("config")
            except ConfigParser.NoSectionError:
                keys = []
            for key in keys:
                config[key] = cp.get("config", key)

        cls.load(config)

        config_name = config.get("x_config_name", "")
        if config_name:
            config_name = cls.create_fs_name(config_name)
        else:
            config_name, ext = os.path.splitext(os.path.basename(path))

        if u"(" in config_name:
            config_base = config_name.split(u"(")[0].strip()
        else:
            config_base = config_name
        #game = name

        if not Config.get("title"):
            Config.set("title", config_base)

        from .Settings import Settings
        Settings.set("config_base", config_base)
        Settings.set("config_name", config_name)
        Settings.set("config_path", path)

        Settings.set("config_xml_path", config_xml_path)
