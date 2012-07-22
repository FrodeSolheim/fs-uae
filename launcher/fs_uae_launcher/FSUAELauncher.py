from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import shutil
import ConfigParser
import fs_uae_launcher.fsui as fsui
from .MainWindow import MainWindow
import fs_uae_launcher.fs as fs

from .Settings import Settings
from .Config import Config

class FSUAELauncher(fsui.Application):

    def on_create(self):
        print("FSUAELauncherApplication.on_create")

        self.load_settings()

        # FIXME: should now sanitize check some options -for instance,
        # - check if configured joysticks are still connected
        # - check if paths still exists, etc

        Config.update_kickstart()

        icon = None
        if fs.windows:
            if os.path.exists("share/fs-uae-launcher/fs-uae-launcher.ico"):
                icon = "share/fs-uae-launcher/fs-uae-launcher.ico"
            elif os.path.exists("launcher/share/fs-uae-launcher/fs-uae-launcher.ico"):
                icon = "launcher/share/fs-uae-launcher/fs-uae-launcher.ico"
        window = MainWindow(icon=icon)
        window.show()

    def get_settings_file(self):
        return os.path.join(fs.get_app_data_dir(), "launcher.settings")

    def load_settings(self):
        path = self.get_settings_file()
        print("loading settings from " + repr(path))
        if not os.path.exists(path):
            print("settings file does not exist")
        cp = ConfigParser.ConfigParser()
        try:
            cp.read([path])
        except Exception, e:
            print(repr(e))
            return
        settings = {}
        try:
            keys = cp.options("settings")
        except ConfigParser.NoSectionError:
            keys = []
        for key in keys:
            settings[key] = cp.get("settings", key)
        config = {}
        try:
            keys = cp.options("config")
        except ConfigParser.NoSectionError:
            keys = []
        for key in keys:
            config[key] = cp.get("config", key)

        for key, value in settings.iteritems():
            Settings.settings[key] = value
        for key, value in config.iteritems():
            print("loaded", key, value)
            Config.config[key] = value

    def save_settings(self):
        path = self.get_settings_file()
        path = path + u".part"
        print("writing " + repr(path))

        cp = ConfigParser.ConfigParser()
        cp.add_section("settings")

        for key, value in Settings.settings.iteritems():
            #lines.append(u"{0} = {1}".format(key, value))
            cp.set("settings", key, value)

        cp.add_section("config")
        #lines.append(u"[config]")

        for key, value in Config.config.iteritems():
            #lines.append(u"{0} = {1}".format(key, value))
            cp.set("config", key, value)

        with open(path, "wb") as f:
            cp.write(f)
            #for line in lines:
            #    f.write(line + u"\n".encode("UTF-8"))
        print("moving to " + repr(self.get_settings_file()))
        shutil.move(path, self.get_settings_file())
