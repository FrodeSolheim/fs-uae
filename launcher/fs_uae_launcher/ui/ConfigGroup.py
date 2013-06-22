from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import time
import hashlib
import datetime
from fsgs.FileDatabase import FileDatabase
import fsui as fsui
from ..Config import Config
from ..ConfigurationScanner import ConfigurationScanner
from ..FSUAEDirectories import FSUAEDirectories
from ..I18N import _
from ..Settings import Settings
from .IconButton import IconButton


class ConfigGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        # heading_label = fsui.HeadingLabel(self, _("Configuration"))
        # self.layout.add(heading_label, margin=10)
        # self.layout.add_spacer(0)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        # dummy label for sizing purposes
        #label = fsui.HeadingLabel(self, "")
        #hori_layout.add(label, margin_top=10, margin_bottom=10)

        label_stand_in = fsui.Panel(self)
        tw, th = label_stand_in.measure_text(_("Configuration"))
        label_stand_in.set_min_height(th)
        hori_layout.add(label_stand_in, margin_top=10, margin_bottom=10)
        # self.layout.add_spacer(0)

        self.new_button = IconButton(self, "new_button.png")
        self.new_button.set_tooltip(_("New Configuration"))
        self.new_button.on_activate = self.on_new_button
        hori_layout.add(self.new_button, margin=10,
                        margin_top=0, margin_bottom=0)

        #self.open_button = IconButton(self, "open_button.png")
        #self.open_button.set_tooltip(_("Open Configuration"))
        #self.open_button.disable()
        #self.open_button.on_activate = self.on_open_button
        #hori_layout.add(self.open_button, margin=10)

        self.config_name_field = fsui.TextField(self)
        hori_layout.add(self.config_name_field, expand=True, margin=10,
                        margin_top=0, margin_bottom=0)

        self.save_button = IconButton(self, "save_button.png")
        #self.save_button.disable()
        self.save_button.set_tooltip(_("Save Configuration"))
        self.save_button.on_activate = self.on_save_button
        hori_layout.add(self.save_button, margin=10,
                        margin_top=0, margin_bottom=0)

        self.on_setting("config_name", Settings.get("config_name"))
        self.config_name_field.on_change = self.on_config_name_change

        #Config.add_listener(self)
        Settings.add_listener(self)

        self.on_setting("config_changed", Settings.get("config_changed"))

    def on_destroy(self):
        #Config.remove_listener(self)
        Settings.remove_listener(self)

    #def on_config(self, key, value):
    #    if key == "__changed":
    #        self.save_button.enable(value == "1")

    def on_setting(self, key, value):
        if key == "config_changed":
            self.save_button.enable(value == "1")
        elif key == "config_name":
            if value != self.config_name_field.get_text().strip():
                self.config_name_field.set_text(value)

    def on_config_name_change(self):
        text = self.config_name_field.get_text().strip()
        Settings.set("config_name", text)
        Settings.set("config_changed", "1")
        # FIXME: remove
        #Config.set("title", text)

    def on_new_button(self):
        Config.load_default_config()
        Settings.set("config_changed", "1")
        Settings.set("parent_uuid", "")

    def on_save_button(self):
        print("ConfigGroup.on_save_button")
        #try:
        self.save_config()
        #except Exception:
        #    # FIXME: notify user
        #    pass

    def save_config(self):
        print("ConfigGroup.save_config")
        from ..Database import Database
        database = Database.get_instance()

        name = Settings.get("config_name").strip()
        if not name:
            print("no config_name")
            # FIXME: notify user
            return

        file_name = name + ".fs-uae"
        path = os.path.join(
            FSUAEDirectories.get_configurations_dir(), file_name)
        with open(path, "wb") as f:
            f.write("# FS-UAE configuration saved by FS-UAE Launcher\n")
            f.write("# Last saved: {0}\n".format(
                    datetime.datetime.today().strftime("%Y-%m-%d %H:%M:%S")))
            f.write("\n[fs-uae]\n")
            keys = sorted(Config.config.keys())
            for key in keys:
                value = Config.get(key)
                if key.startswith("__"):
                    continue
                if key in Config.dont_save_keys_set:
                    continue
                #elif key == "joystick_port_2_mode" and value == "nothing":
                #    continue
                #elif key == "joystick_port_3_mode" and value == "nothing":
                #    continue
                if value == Config.default_config.get(key, ""):
                    continue
                if value:
                    f.write("{0} = {1}\n".format(key, value))

        scanner = ConfigurationScanner()
        #search = ConfigurationScanner.create_configuration_search(name)
        #name = scanner.create_configuration_name(name)
        #print("adding", path)
        ## deleting the path from the database first in case it already exists
        #database.delete_configuration(path=path)
        #database.delete_file(path=path)
        #database.add_file(path=path)
        #database.add_configuration(
        #    path=path, uuid="", name=name, scan=0, search=search)

        file_database = FileDatabase.get_instance()
        scanner = ConfigurationScanner()
        print("[save config] adding config", path)
        file_database.delete_file(path=path)
        with open(path, "rb") as f:
            sha1 = hashlib.sha1(f.read()).hexdigest()
        file_database.add_file(path=path, sha1=sha1)

        game_id = database.add_game(
            path=path, name=scanner.create_configuration_name(name))
        database.update_game_search_terms(
            game_id, scanner.create_search_terms(name))

        database.commit()
        file_database.commit()

        Settings.set("config_refresh", str(time.time()))
        Settings.set("config_changed", "0")
