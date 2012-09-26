from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..Database import Database
from ..I18N import _, ngettext

class ConfigurationsBrowser(fsui.ListView):

    def __init__(self, parent):
        fsui.ListView.__init__(self, parent)
        self.items = []

        Settings.add_listener(self)
        self.update_search()

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_select_item(self):
        index = self.get_index()
        print(index)
        self.load_configuration(self.items[index][0])

    def on_activate_item(self):
        from ..LaunchHandler import LaunchHandler
        LaunchHandler.start_game()

    def on_setting(self, key, value):
        if key == "config_search":
            self.update_search()
        if key == "config_refresh":
            self.update_search()

    def set_items(self, items):
        #print("set_items")
        self.items = items
        #print("has set items", repr(self.items))
        self.set_item_count(len(self.items))
        #print("has set items -- ", id(self.items), repr(self.items))

    def on_get_item_text(self, row, column):
        #print(id(self.items), self.items, row)
        #text = self.items[row][1]
        #if "(" in text:
        #    primary, secondary = text.split("(", 1)
        #    secondary = secondary.replace(u", ", u" \u00b7 ")
        #    text = primary.rstrip() + u" \u2013 " + secondary.lstrip()
        #text = u" " + text
        #return text
        #return self.items[row][1]
        #return u" " + self.items[row][1].replace(u" # ", u" \u2013 ")
        text =  u" " + self.items[row][1]
        text = text.replace(u"\nAmiga \u00b7 ", "\n")
        return text

    def on_get_item_tooltip(self, row, column):
        return self.items[row][1]
        #text = text.replace(u"\nAmiga \u00b7 ", "\n")

    def update_search(self):
        self.search = Settings.get("config_search").strip().lower()
        print("search for", self.search)

        database = Database.get_instance()
        items = database.search_configurations(self.search)
        #print(items)
        self.set_items(items)
        #self.set_items([list(x) for x in items])

    def load_configuration(self, configuration_id):
        database = Database.get_instance()
        config_info = database.get_config(configuration_id)
        if config_info["data"]:
            Config.load_data(config_info["data"])
        else:
            Config.load_file(config_info["path"])
