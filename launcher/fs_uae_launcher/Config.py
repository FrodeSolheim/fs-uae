from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import hashlib
import traceback
import ConfigParser
from .Amiga import Amiga
from .Database import Database
from .Settings import Settings
from .Signal import Signal
from .Util import expand_path
from .ValueConfigLoader import ValueConfigLoader
from .Version import Version
from .Warnings import Warnings
from .XMLConfigLoader import XMLConfigLoader

# the order of the following keys is significant (for some keys).
# multiple options should be set in this order since some options will
# implicitly change other options. examples:
# - amiga model can change kickstart and joystick modes
# - file options should be set before corresponding sha1 options

cfg = [
    ("amiga_model",           "A500",     "checksum", "sync"),
    ("ntsc_mode",             "",         "checksum", "sync"),
    ("accuracy",              "",         "checksum", "sync"),
    ("chip_memory",           "",         "checksum", "sync"),
    ("slow_memory",           "",         "checksum", "sync"),
    ("fast_memory",           "",         "checksum", "sync"),
    ("zorro_iii_memory",      "",         "checksum", "sync"),
    ("bsdsocket_library",     "",         "checksum", "sync"),
    ("uaegfx_card",           "",         "checksum", "sync"),
    ("joystick_port_0",       ""),
    ("joystick_port_0_mode",  "",         "checksum", "sync"),
    ("joystick_port_0_autofire",  "",     "checksum", "sync"),
    ("joystick_port_1",       ""),
    ("joystick_port_1_mode",  "",         "checksum", "sync"),
    ("joystick_port_1_autofire",  "",     "checksum", "sync"),
    ("joystick_port_2",       ""),
    ("joystick_port_2_mode",  "",         "checksum", "sync"),
    ("joystick_port_2_autofire",  "",     "checksum", "sync"),
    ("joystick_port_3",       ""),
    ("joystick_port_3_mode",  "",         "checksum", "sync"),
    ("joystick_port_3_autofire",  "",     "checksum", "sync"),

    ("kickstart_file",        ""),
    ("x_kickstart_file",      "",                             "nosave"),
    ("x_kickstart_file_sha1", "",         "checksum", "sync", "nosave"),
    ("kickstart_ext_file",    ""),
    ("x_kickstart_ext_file",  "",                             "nosave"),
    ("x_kickstart_ext_file_sha1", "",     "checksum", "sync", "nosave"),

    ("x_whdload_args",        "",         "checksum", "sync"),
    ("floppy_drive_count",    "",         "checksum", "sync", "custom"),
    ("floppy_drive_speed",    "",         "checksum", "sync", "custom"),
    ("cdrom_drive_count",     "",         "checksum", "sync", "custom"),

    ("__netplay_game",        "",         "checksum", "sync"),
    ("__netplay_password",    "",         "checksum", "sync"),
    ("__netplay_players",     "",         "checksum", "sync"),
    ("__netplay_port",        "",                     "sync"),
    ("__netplay_addresses",   "",         "checksum", "sync"),
    ("__netplay_host",        ""),
    ("__netplay_ready",       "0"),
    ("__netplay_state_dir_name",  "",     "checksum", "sync"),
    ("__version",             Version.VERSION),
    ("x_game_uuid",           ""),
    ("x_game_xml_path",       ""),
    ("title",                 "",                             "custom"),
    ("sub_title",             "",                             "custom"),
    ("viewport",              "",                             "custom"),

    ("year",                  ""),
    ("developer",             ""),
    ("publisher",             ""),
    ("languages",             ""),
    ("hol_url",               ""),
    ("wikipedia_url",         ""),
    ("database_url",          ""),
    ("lemon_url",             ""),
    ("mobygames_url",         ""),
    ("__variant_rating",      ""),

    ("screen1_sha1",          ""),
    ("screen2_sha1",          ""),
    ("screen3_sha1",          ""),
    ("screen4_sha1",          ""),
    ("screen5_sha1",          ""),
    ("front_sha1",            ""),
    ("title_sha1",            ""),

]

for i in range(Amiga.MAX_FLOPPY_DRIVES):
    cfg.append(("floppy_drive_{0}".format(i), ""))
    cfg.append(("x_floppy_drive_{0}_sha1".format(i), "", "checksum", "sync", "nosave"))
for i in range(Amiga.MAX_FLOPPY_IMAGES):
    cfg.append(("floppy_image_{0}".format(i), ""))
    cfg.append(("x_floppy_image_{0}_sha1".format(i), "", "checksum", "sync", "nosave"))
for i in range(Amiga.MAX_CDROM_DRIVES):
    cfg.append(("cdrom_drive_{0}".format(i), ""))
    cfg.append(("x_cdrom_drive_{0}_sha1".format(i), "", "checksum", "sync", "nosave"))
for i in range(Amiga.MAX_CDROM_IMAGES):
    cfg.append(("cdrom_image_{0}".format(i), ""))
    cfg.append(("x_cdrom_image_{0}_sha1".format(i), "", "checksum", "sync", "nosave"))
for i in range(Amiga.MAX_HARD_DRIVES):
    cfg.append(("hard_drive_{0}".format(i), ""))
    cfg.append(("x_hard_drive_{0}_sha1".format(i), "", "checksum", "sync", "nosave"))

class Config:

    config_keys = [x[0] for x in cfg]

    default_config = {}
    for c in cfg:
        default_config[c[0]] = c[1]

    key_order = [x[0] for x in cfg]
    checksum_keys = [x[0] for x in cfg if "checksum" in x]
    sync_keys_list = [x[0] for x in cfg if "sync" in x]
    sync_keys_set = set(sync_keys_list)
    no_custom_config = [x[0] for x in cfg if not "custom" in x]
    dont_save_keys_set = set([x[0] for x in cfg if "nosave" in x])

    reset_values = {}
    for i in range(Amiga.MAX_FLOPPY_DRIVES):
        reset_values["floppy_drive_{0}".format(i)] = \
                ("x_floppy_drive_{0}_sha1".format(i), "")
    for i in range(Amiga.MAX_FLOPPY_IMAGES):
        reset_values["floppy_image_{0}".format(i)] = \
                ("x_floppy_image_{0}_sha1".format(i), "")
    for i in range(Amiga.MAX_CDROM_DRIVES):
        reset_values["cdrom_drive_{0}".format(i)] = \
                ("x_cdrom_drive_{0}_sha1".format(i), "")
    for i in range(Amiga.MAX_CDROM_IMAGES):
        reset_values["cdrom_image_{0}".format(i)] = \
                ("x_cdrom_image_{0}_sha1".format(i), "")
    for i in range(Amiga.MAX_HARD_DRIVES):
        reset_values["hard_drive_{0}".format(i)] = \
                ("x_hard_drive_{0}_sha1".format(i), "")
    reset_values["x_kickstart_file"] = ("x_kickstart_file_sha1", "")
    reset_values["x_kickstart_ext_file"] = ("x_kickstart_ext_file_sha1", "")

    config = default_config.copy()
    #config_listeners = []

    @classmethod
    def copy(cls):
        return cls.config.copy()

    @classmethod
    def get(cls, key):
        return cls.config.setdefault(key, "")

    @classmethod
    def add_listener(cls, listener):
        #cls.config_listeners.append(listener)
        Signal.add_listener("config", listener)

    @classmethod
    def remove_listener(cls, listener):
        #cls.config_listeners.remove(listener)
        Signal.remove_listener("config", listener)

    @classmethod
    def set(cls, key, value):
        #if cls.get(key) == value:
        #    print(u"set {0} to {1} (no change)".format(key, value))
        #    return
        #print(u"set {0} to {1}".format(key, value))
        #cls.config[key] = value
        #for listener in cls.config_listeners:
        #    listener.on_config(key, value)
        #if key != "__netplay_ready":
        #    cls.set("__netplay_ready", "0")
        cls.set_multiple([(key, value)])

    @classmethod
    def set_multiple(cls, items):
        # this will allow us to set multiple keys at a time, to provide
        # "atomic" setting of several keys before notifications are sent

        items = list(items)
        item_keys = [x[0] for x in items]

        changed_keys = set()
        def add_changed_key(key):
            try:
                priority = cls.key_order.index(key)
            except ValueError:
                priority = 1000
            changed_keys.add((priority, key))
        def change(key, value):
            #print("change", key, value)
            if old_config.get(key, "") == value:
                if value:
                    print(u"set {0} to {1} (no change)".format(key, value))
                return
            print(u"set {0} to {1}".format(key, value))
            add_changed_key(key)
            cls.config[key] = value

        old_config = cls.config.copy()
        for key, value in items:
            change(key, value)
            try:
                reset_key, reset_value = cls.reset_values[key]
                #print("  ---", reset_key, reset_value)
            except KeyError:
                pass
            else:
                if reset_key not in item_keys:
                    print(" -- reset --", reset_key, "to", repr(reset_value))
                    change(reset_key, reset_value)

        # and now broadcast all changed keys at once
        if len(changed_keys) > 0:
            for priority, key in sorted(changed_keys):
                #for listener in cls.config_listeners:
                #    listener.on_config(key, cls.get(key))
                Signal.broadcast("config", key, cls.get(key))
            changed_keys = [x[1] for x in changed_keys]
            if "__netplay_ready" not in changed_keys:
                cls.set("__netplay_ready", "0")

        if len(changed_keys) > 0:
            Settings.set("config_changed", "1")

    @classmethod
    def update_from_config_dict(cls, config_dict):
        changes = []
        for key, value in config_dict.iteritems():
            if key in cls.config:
                if cls.config[key] != value:
                    changes.append((key, value))
            else:
                changes.append((key, value))
        cls.set_multiple(changes)

    @classmethod
    def sync_items(cls):
        for key, value in cls.config.iteritems():
            if key in cls.sync_keys_set:
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
    def update_kickstart_in_config_dict(cls, config_dict):
        print("update_kickstart_in_config")
        model = config_dict.setdefault("amiga_model",
                cls.default_config["amiga_model"])

        kickstart_file = config_dict.setdefault("kickstart_file", "")
        if kickstart_file:
            config_dict["x_kickstart_file"] = config_dict["kickstart_file"]
            if kickstart_file == "internal":
                config_dict["x_kickstart_file_sha1"] = Amiga.INTERNAL_ROM_SHA1
            else:
                # FIXME: set checksum
                pass
        else:
            checksums = Amiga.get_model_config(model)["kickstarts"]
            for checksum in checksums:
                path = Database.get_instance().find_file(sha1=checksum)
                if path:
                    config_dict["x_kickstart_file"] = path
                    config_dict["x_kickstart_file_sha1"] = checksum
                    break
            else:
                print("WARNING: no suitable kickstart file found")
                config_dict["x_kickstart_file"] = ""
                config_dict["x_kickstart_file_sha1"], ""

        if config_dict.setdefault("kickstart_ext_file", ""):
            config_dict["x_kickstart_ext_file"] = \
                    config_dict["kickstart_ext_file"]
            # FIXME: set checksum
        else:
            checksums = Amiga.get_model_config(model)["ext_roms"]
            if len(checksums) == 0:
                config_dict["x_kickstart_ext_file"] = ""
                config_dict["x_kickstart_ext_file_sha1"] = ""
            else:
                for checksum in checksums:
                    path = Database.get_instance().find_file(sha1=checksum)
                    if path:
                        config_dict["x_kickstart_ext_file"] = path
                        config_dict["x_kickstart_ext_file_sha1"] = checksum
                        break
                else:
                    #print("WARNING: no suitable kickstart ext file found")
                    config_dict["x_kickstart_ext_file"] = ""
                    config_dict["x_kickstart_ext_file_sha1"] = ""
                    #Warnings.set("hardware", "kickstart_ext",
                    #             "No suitable extended kickstart found")
                    # FIXME: set sha1 and name x_options also

    @classmethod
    def update_kickstart(cls):
        cls.set_kickstart_from_model()

    @classmethod
    def set_kickstart_from_model(cls):
        print("set_kickstart_from_model")
        config_dict = cls.config.copy()
        cls.update_kickstart_in_config_dict(config_dict)
        cls.update_from_config_dict(config_dict)

    @classmethod
    def load_default_config(cls):
        print("load_default_config")
        cls.load({})
        #from .Settings import Settings
        # FIXME: remove use of config_base
        Settings.set("config_base", "")
        Settings.set("config_name", "Unnamed Configuration")
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
            if key in Settings.initialize_from_config:
                Settings.set(key, value)
            else:
                update_config[key] = value

        #for key, value in update_config.iteritems():
        #    #cls.config[key] = value
        #    cls.set(key, value)
        cls.update_kickstart_in_config_dict(update_config)
        cls.fix_loaded_config(update_config)
        cls.set_multiple(update_config.iteritems())
        Settings.set("config_changed", "0")

        #cls.update_kickstart()

    @classmethod
    def fix_joystick_ports(cls, config):
        #from .Settings import Settings

        print("---", config["joystick_port_0"])
        print("---", config["joystick_port_1"])

        from .DeviceManager import DeviceManager
        available = DeviceManager.get_joystick_names()
        available.extend(["none", "mouse", "keyboard"])
        available_lower = [x.lower() for x in available]

        device_ids = DeviceManager.get_joystick_ids()
        device_ids.extend(["none", "mouse", "keyboard"])

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
                    config["joystick_port_1"] = device_ids[index]
                    del available[index]
                    del available_lower[index]
                    del device_ids[index]
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
                    config["joystick_port_0"] = device_ids[index]
                    del available[index]
                    del available_lower[index]
                    del device_ids[index]
        elif config["joystick_port_0_mode"] in ["mouse"]:
            if not config["joystick_port_0"]:
                config["joystick_port_0"] = "mouse"
        elif config["joystick_port_0_mode"] in ["nothing"]:
            if not config["joystick_port_0"]:
                config["joystick_port_0"] = "none"

    @classmethod
    def fix_loaded_config(cls, config):
        #cls.fix_joystick_ports(config)

        from .ChecksumTool import ChecksumTool
        # FIXME: parent
        checksum_tool = ChecksumTool(None)
        def fix_file_checksum(sha1_key, key, base_dir, is_rom=False):
            path = config.get(key, "")
            # hack to synchronize URLs
            if path.startswith("http://") or path.startswith("https://"):
                sha1 = path
                config[sha1_key] = sha1
                return
            path = expand_path(path)
            sha1 = config.get(sha1_key, "")
            if not path:
                return
            if sha1:
                # assuming sha1 is correct
                return
            if not os.path.exists(path):
                print(repr(path), "does not exist")
                path = os.path.join(base_dir, path)
                if not os.path.exists(path):
                    print(repr(path), "does not exist")
                    return
            if os.path.isdir(path):
                # could set a fake checksum here or something, to indicate
                # that it isn't supposed to be set..
                return
            print("checksumming", repr(path))
            if os.path.getsize(path) > 64 * 1024 * 1024:
                # not checksumming large files righ now
                print("not checksumming large file")
                return
            if is_rom:
                sha1 = checksum_tool.checksum_rom(path)
            else:
                sha1 = checksum_tool.checksum(path)
            config[sha1_key] = sha1

        for i in range(Amiga.MAX_FLOPPY_DRIVES):
            fix_file_checksum("x_floppy_drive_{0}_sha1".format(i),
                    "floppy_drive_{0}".format(i),
                    Settings.get_floppies_dir())
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
            fix_file_checksum("x_floppy_image_{0}_sha1".format(i),
                    "floppy_image_{0}".format(i),
                    Settings.get_floppies_dir())
        for i in range(Amiga.MAX_CDROM_DRIVES):
            fix_file_checksum("x_cdrom_drive_{0}_sha1".format(i),
                    "cdrom_drive_{0}".format(i),
                    Settings.get_cdroms_dir())
        for i in range(Amiga.MAX_CDROM_IMAGES):
            fix_file_checksum("x_cdrom_image_{0}_sha1".format(i),
                    "cdrom_image_{0}".format(i),
                    Settings.get_cdroms_dir())
        for i in range(Amiga.MAX_HARD_DRIVES):
            fix_file_checksum("x_hard_drive_{0}_sha1".format(i),
                    "hard_drive_{0}".format(i),
                    Settings.get_hard_drives_dir())

        # FIXME: need to handle checksums for Cloanto here
        fix_file_checksum("x_kickstart_file_sha1", "x_kickstart_file",
                Settings.get_kickstarts_dir(), is_rom=True)
        fix_file_checksum("x_kickstart_ext_file_sha1", "x_kickstart_ext_file",
                Settings.get_kickstarts_dir(), is_rom=True)


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
            loader = XMLConfigLoader()
            loader.load_data(data)
            config = loader.get_config()
        elif path.endswith(".xml"):
            config_xml_path = path
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
            try:
                keys = cp.options("fs-uae")
            except ConfigParser.NoSectionError:
                keys = []
            for key in keys:
                config[key] = cp.get("fs-uae", key)

        from .Settings import Settings
        Settings.set("config_path", path)

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

        #if not Config.get("title"):
        #    Config.set("title", config_base)

        Settings.set("config_base", config_base)
        Settings.set("config_name", config_name)
        Settings.set("config_xml_path", config_xml_path)
        Settings.set("config_changed", "0")

    @classmethod
    def load_values(cls, values, uuid=""):
        print("loading config values", values)

        value_config_loader = ValueConfigLoader(uuid=uuid)
        value_config_loader.load_values(values)
        config = value_config_loader.get_config()
        config["x_config_uuid"] = uuid

        from .Settings import Settings
        Settings.set("config_path", "")

        cls.load(config)

        config_name = config.get("x_config_name", "")
        if config_name:
            config_name = cls.create_fs_name(config_name)
        #else:
        #    config_name, ext = os.path.splitext(os.path.basename(path))

        if u"(" in config_name:
            config_base = config_name.split(u"(")[0].strip()
        else:
            config_base = config_name
        #game = name

        #if not Config.get("title"):
        #    Config.set("title", config_base)

        Settings.set("config_base", config_base)
        Settings.set("config_name", config_name)
        Settings.set("config_xml_path", "")
        Settings.set("config_changed", "0")
