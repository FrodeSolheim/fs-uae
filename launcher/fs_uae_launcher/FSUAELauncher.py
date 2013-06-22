from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.six as six
import os
import sys
import shutil
import hashlib
import traceback
from fs_uae_launcher.six.moves import configparser
import fs_uae_launcher.fs as fs
import fsui as fsui
from fsgs import fsgs
from .ui.MainWindow import MainWindow

from fsgs.FileDatabase import FileDatabase
from fsgs.Paths import Paths
from fsgs.amiga.Amiga import Amiga
from fsgs.amiga.ROMManager import ROMManager

from .Config import Config
from .ConfigurationScanner import ConfigurationScanner
from .Database import Database
from .FSUAEDirectories import FSUAEDirectories
from .I18N import _, initialize_locale
from .Settings import Settings
from .UpdateManager import UpdateManager


class FSUAELauncher(fsui.Application):

    def __init__(self):
        fsui.Application.__init__(self, "fs-uae-launcher")

    def get_game_database_path(self):
        launcher_dir = FSUAEDirectories.get_launcher_dir()
        path = os.path.join(launcher_dir, "Game Database.sqlite")
        return path

    #def get_overlay_database_path(self):
    #    launcher_dir = FSUAEDirectories.get_launcher_dir()
    #    path = os.path.join(launcher_dir, "Overlay Database.sqlite")
    #    return path

    def on_idle(self):
        fsgs.signal.process()

    def start(self):
        print("FSUAELauncherApplication.start")

        # GameDatabase.set_database_path(self.get_game_database_path())
        # OverlayDatabase.set_database_path(self.get_overlay_database_path())

        self.parse_arguments()
        self.load_settings()

        language = Settings.get("language")
        initialize_locale(language)

        self.config_startup_scan()
        self.kickstart_startup_scan()

        #sys.exit(1)

        # FIXME: should now sanitize check some options -for instance,
        # - check if configured joysticks are still connected
        # - check if paths still exists, etc

        #Config.update_kickstart()

        icon = None

        def check_icon(path):
            path = os.path.join(path, "fs-uae-launcher.ico")
            if os.path.exists(path):
                return path
            return None

        if not icon:
            icon = check_icon("share/fs-uae-launcher")
        if not icon:
            icon = check_icon("launcher/share/fs-uae-launcher")
        # FIXME: should check data directories (XDG_DATA_DIRS) properly
        # instead
        if not icon:
            # this encoding / decoding is a bit ridiculous, but, this is
            # for Python 2.x..
            icon = check_icon(os.path.expanduser(
                "~/.local/share/fs-uae-launcher".encode(
                    sys.getfilesystemencoding())).decode(
                sys.getfilesystemencoding()))
        if not icon:
            icon = check_icon("/usr/local/share/fs-uae-launcher")
        if not icon:
            icon = check_icon("/usr/share/fs-uae-launcher")
        if fs.macosx:
            # Icons come from the app bundles
            icon = None

        plugins_dir = FSUAEDirectories.get_plugins_dir()
        if plugins_dir:
            dont_write_bytecode = sys.dont_write_bytecode
            try:
                self.load_plugins(plugins_dir)
            finally:
                sys.dont_write_bytecode = dont_write_bytecode

        #if "--qt" in sys.argv:
        #    from .ui.settings.SettingsDialog import SettingsDialog
        #    dialog = SettingsDialog(None)
        #    dialog.show()
        #    return

        window = MainWindow(icon=icon)
        MainWindow.instance = window
        window.show()
        UpdateManager.run_update_check()

    def load_plugins(self, plugins_dir):
        print("loading plugins")
        for full_name in os.listdir(plugins_dir):
            name, ext = os.path.splitext(full_name)
            if ext.lower() != ".py":
                continue
            path = os.path.join(plugins_dir, full_name)
            print("loading", path)
            name = "plugin_" + hashlib.sha1(path.encode("UTF-8")).hexdigest()
            print(name)
            import imp
            try:
                #with open(path, "r") as f:
                #    plugin = imp.load_source(name, name, f)
                plugin = imp.load_source(
                    name, path.encode(sys.getfilesystemencoding()))
                print("name:", getattr(plugin, "name", ""))
                print("version:", getattr(plugin, "version", ""))
                plugin.fsgs = fsgs
                plugin.plugin_dir = os.path.dirname(path)
                plugin.fsgs_init()
            except Exception:
                traceback.print_exc()
                continue

    def get_settings_file(self):
        portable_ini = os.path.join(FSUAEDirectories.get_base_dir(),
                                    "Portable.ini")
        launcher_ini = os.path.join(FSUAEDirectories.get_data_dir(),
                                    "FS-UAE Launcher.ini")
        if os.path.exists(portable_ini) or os.path.exists(launcher_ini):
            return launcher_ini
        return os.path.join(fs.get_app_data_dir(), "launcher.settings")

    def load_settings(self):
        path = self.get_settings_file()
        print("loading settings from " + repr(path))
        if not os.path.exists(path):
            print("settings file does not exist")
        if six.PY3:
            cp = configparser.ConfigParser(interpolation=None)
        else:
            cp = configparser.ConfigParser()
        try:
            cp.read([path])
        except Exception as e:
            print(repr(e))
            return

        config = {}
        try:
            keys = cp.options("config")
        except configparser.NoSectionError:
            keys = []
        for key in keys:
            config[key] = fs.from_utf8_str(cp.get("config", key))
        for key, value in six.iteritems(config):
            print("loaded", key, value)
            Config.config[key] = value

        settings = {}
        try:
            keys = cp.options("settings")
        except configparser.NoSectionError:
            keys = []
        for key in keys:
            settings[key] = fs.from_utf8_str(cp.get("settings", key))
        for key, value in six.iteritems(settings):
            #if key in Settings.settings:
            #    # this setting is already initialized, possibly via
            #    # command line arguments
            #    pass
            #else:
            print("-- setting", key, value)
            Settings.settings[key] = value

        #Settings.set("config_search", "")

    def parse_arguments(self):
        pass
        #for arg in sys.argv:
        #    if arg.startswith("--"):
        #        if "=" in arg:
        #            key, value = arg[2:].split("=", 1)
        #            key = key.replace("-", "_")
        #            if key == "base_dir":
        #                Settings.set("base_dir", value)

    def save_settings(self):
        path = self.get_settings_file()
        path += ".part"
        print("writing " + repr(path))

        if six.PY3:
            cp = configparser.ConfigParser(interpolation=None)
        else:
            cp = configparser.ConfigParser()
        cp.add_section("settings")

        for key, value in six.iteritems(Settings.settings):
            #lines.append("{0} = {1}".format(key, value))
            cp.set("settings", str(key), fs.to_utf8_str(value))

        cp.add_section("config")
        #lines.append("[config]")

        for key, value in six.iteritems(Config.config):
            if key.startswith("__"):
                # keys starting with __ are never saved
                continue
            cp.set("config", str(key), fs.to_utf8_str(value))

        if six.PY3:
            with open(path, "w", encoding="UTF-8", newline="\n") as f:
                cp.write(f)
        else:
            with open(path, "w") as f:
                cp.write(f)
        print("moving to " + repr(self.get_settings_file()))
        shutil.move(path, self.get_settings_file())

    def get_dir_mtime_str(self, path):
        try:
            return str(int(os.path.getmtime(path)))
        except Exception:
            return "0"

    def config_startup_scan(self):
        configs_dir = FSUAEDirectories.get_configurations_dir()
        print("config_startup_scan", configs_dir)
        print(Settings.settings)
        settings_mtime = Settings.get("configurations_dir_mtime")
        dir_mtime = self.get_dir_mtime_str(configs_dir)
        if settings_mtime == dir_mtime:
            print("... mtime not changed", settings_mtime, dir_mtime)
            return
        database = Database.get_instance()
        file_database = FileDatabase.get_instance()

        print("... database.find_local_configurations")
        local_configs = Database.get_instance().find_local_configurations()
        print("... walk configs_dir")
        for dir_path, dir_names, file_names in os.walk(configs_dir):
            for file_name in file_names:
                if not file_name.endswith(".fs-uae"):
                    continue
                path = Paths.join(dir_path, file_name)
                if path in local_configs:
                    local_configs[path] = None
                    # already exists in database
                    continue
                name, ext = os.path.splitext(file_name)
                #search = ConfigurationScanner.create_configuration_search(
                # name)
                scanner = ConfigurationScanner()
                print("[startup] adding config", path)
                file_database.delete_file(path=path)
                with open(path, "rb") as f:
                    sha1 = hashlib.sha1(f.read()).hexdigest()
                file_database.add_file(path=path, sha1=sha1)

                game_id = database.add_game(
                    path=path, name=scanner.create_configuration_name(name))
                database.update_game_search_terms(
                    game_id, scanner.create_search_terms(name))

        for path, id in six.iteritems(local_configs):
            if id is not None:
                print("[startup] removing configuration", path)
                database.delete_game(id=id)
                file_database.delete_file(path=path)
        print("... commit")
        database.commit()
        Settings.set("configurations_dir_mtime",
                     self.get_dir_mtime_str(configs_dir))

    def kickstart_startup_scan(self):
        print("kickstart_startup_scan")
        kickstarts_dir = FSUAEDirectories.get_kickstarts_dir()
        if Settings.get("kickstarts_dir_mtime") == \
                self.get_dir_mtime_str(kickstarts_dir):
            print("... mtime not changed")
        else:
            file_database = FileDatabase.get_instance()
            print("... database.find_local_roms")
            local_roms = file_database.find_local_roms()
            print("... walk kickstarts_dir")
            for dir_path, dir_names, file_names in os.walk(kickstarts_dir):
                for file_name in file_names:
                    if not file_name.endswith(".rom"):
                        continue
                    path = Paths.join(dir_path, file_name)
                    if path in local_roms:
                        local_roms[path] = None
                        # already exists in database
                        continue
                    print("[startup] adding kickstart", path)
                    ROMManager.add_rom_to_database(path, file_database)
            print(local_roms)
            for path, id in six.iteritems(local_roms):
                if id is not None:
                    print("[startup] removing kickstart", path)
                    file_database.delete_file(id=id)
            print("... commit")
            file_database.commit()
            Settings.set(
                "kickstarts_dir_mtime",
                self.get_dir_mtime_str(kickstarts_dir))

        amiga = Amiga.get_model_config("A500")
        for sha1 in amiga["kickstarts"]:
            if fsgs.file.find_by_sha1(sha1=sha1):
                break
        else:
            file_database = FileDatabase.get_instance()
            self.amiga_forever_kickstart_scan()
            file_database.commit()

    def amiga_forever_kickstart_scan(self):
        if fs.windows:
            print("amiga forever kickstart scan")
            from win32com.shell import shell, shellcon
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_COMMON_DOCUMENTS, 0, 0)
            path = os.path.join(path, "Amiga Files", "Shared", "rom")
            self.scan_dir_for_kickstarts(path)

    def scan_dir_for_kickstarts(self, scan_dir):
        file_database = FileDatabase.get_instance()
        for dir_path, dir_names, file_names in os.walk(scan_dir):
            for file_name in file_names:
                if not file_name.endswith(".rom"):
                    continue
                path = Paths.join(dir_path, file_name)
                if file_database.find_file(path=path):
                #if path in local_roms:
                #    local_roms[path] = None
                #    # already exists in database
                    continue
                print("[startup] adding kickstart", path)
                ROMManager.add_rom_to_database(path, file_database)

    @classmethod
    def start_game(cls):
        from .netplay.Netplay import Netplay
        if Netplay.game_channel:
            Netplay.start_netplay_game()
        else:
            cls.start_local_game()

    @classmethod
    def start_local_game(cls):
        if not Config.get("x_kickstart_file"):  # or not \
            #  os.path.exists(Config.get("kickstart_file")):
            fsui.show_error(_("No kickstart found for this model. "
                              "Use the 'Import Kickstarts' function from "
                              "the menu."))
            return
        cs = Amiga.get_model_config(Config.get("amiga_model"))["ext_roms"]
        if len(cs) > 0:
            # extended kickstart ROM is needed
            if not Config.get("x_kickstart_ext_file"):
                fsui.show_error(_("No extended kickstart found for this "
                                  "model. Try 'scan' function."))
                return

        if Config.get("x_missing_files"):
            if Config.get("x_downloadable").startswith("http"):
                fsui.show_error(_("This game must be downloaded first."))
                return
            fsui.show_error(_("This game variant cannot be started because "
                              "you don't have all required files."))
            return

        config = Config.copy()
        prepared_config = cls.prepare_config(config)

        model = Config.get("amiga_model")
        if model.startswith("CD32"):
            platform = "CD32"
        elif model == "CDTV":
            platform = "CDTV"
        else:
            platform = "Amiga"
        name = Settings.get("config_name")
        uuid = Config.get("x_game_uuid")
        
        from fsgs.SaveStateHandler import SaveStateHandler
        save_state_handler = SaveStateHandler(fsgs, name, platform, uuid)

        from fsgs.amiga.LaunchHandler import LaunchHandler
        launch_handler = LaunchHandler(fsgs, name, prepared_config,
                                       save_state_handler)

        from .ui.LaunchDialog import LaunchDialog
        from .ui.MainWindow import MainWindow
        dialog = LaunchDialog(MainWindow.instance, launch_handler)

        def on_show_license_information(license_text):
            # FIXME: don't depend on wx here
            import wx
            license_dialog = wx.MessageDialog(
                dialog, license_text, _("Terms of Use"),
                wx.OK | wx.CANCEL | wx.CENTRE)
            license_dialog.CenterOnParent()
            result = license_dialog.ShowModal()
            return result == wx.ID_OK
        fsgs.file.on_show_license_information = on_show_license_information

        dialog.run()
        dialog.show_modal()
        dialog.close()

    @classmethod
    def prepare_config(cls, original_config):
        config = {}
        for key, value in six.iteritems(Settings.settings):
            if key in Config.config_keys:
                print("... ignoring config key from settings:", key)
                continue
            config[key] = value

        config["base_dir"] = FSUAEDirectories.get_base_dir()

        for key, value in six.iteritems(original_config):
            config[key] = value

        if not config["joystick_port_0_mode"]:
            config["joystick_port_0_mode"] = "mouse"
        if not config["joystick_port_1_mode"]:
            if config["amiga_model"].startswith("CD32"):
                config["joystick_port_1_mode"] = "cd32 gamepad"
            else:
                config["joystick_port_1_mode"] = "joystick"
        if not config["joystick_port_2_mode"]:
            config["joystick_port_2_mode"] = "none"
        if not config["joystick_port_3_mode"]:
            config["joystick_port_3_mode"] = "none"

        from .DeviceManager import DeviceManager
        devices = DeviceManager.get_devices_for_ports(config)
        for port in range(4):
            key = "joystick_port_{0}".format(port)
            if not config.get(key):
                # key not set, use calculated default value
                config[key] = devices[port].id

        for remove_key in ["database_username", "database_password"]:
            if remove_key in config:
                del config[remove_key]

        # overwrite netplay config
        if config["__netplay_host"]:
            config["netplay_server"] = config["__netplay_host"]
        if config["__netplay_password"]:
            config["netplay_password"] = config["__netplay_password"]
        if config["__netplay_port"]:
            config["netplay_port"] = config["__netplay_port"]

        # copy actual kickstart options from x_ options
        config["kickstart_file"] = config["x_kickstart_file"]
        config["kickstart_ext_file"] = config["x_kickstart_ext_file"]

        # make sure FS-UAE does not load other config files (Host.fs-uae)
        config["end_config"] = "1"

        # set titles
        #if not config["sub_title"]:
        #    config["sub_title"] = config["amiga_model"] or "A500"

        if config["__netplay_game"]:
            print("\nfixing config for netplay game")
            for key in [x for x in config.keys() if x.startswith("uae_")]:
                print("* removing option", key)
                del config[key]
        return config
