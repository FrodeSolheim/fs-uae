from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals
from fsgs.util.GameNameUtil import GameNameUtil

import fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..Database import Database


class ConfigurationsBrowser(fsui.VerticalItemView):

    def __init__(self, parent):
        fsui.VerticalItemView.__init__(self, parent)
        self.items = []
        self.game_icon = fsui.Image("fs_uae_launcher:res/16/controller.png")
        self.config_icon = fsui.Image(
            "fs_uae_launcher:res/fsuae_config_16.png")
        Settings.add_listener(self)
        self.update_search()

        self.manual_download_icon = fsui.Image(
            "fs_uae_launcher:res/16/page_white_world.png")
        self.auto_download_icon = fsui.Image(
            "fs_uae_launcher:res/16/arrow_down_2.png")

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_select_item(self, index):
        #self.load_configuration(self.items[index][str("uuid")])
        self.load_configuration(self.items[index])

    def on_activate_item(self, index):
        from ..FSUAELauncher import FSUAELauncher
        FSUAELauncher.start_game()

    def on_setting(self, key, _value):
        if key == "config_search":
            self.update_search()
            if len(self.items) > 0:
                self.select_item(0)
            else:
                Settings.set("parent_uuid", "")
        if key == "config_refresh":
            self.update_search()
            self.select_item(None)
            old_parent_uuid = Settings.get("parent_uuid")
            if old_parent_uuid:
                Settings.set("parent_uuid", "")
                Settings.set("parent_uuid", old_parent_uuid)

    def set_items(self, items):
        self.items = items
        #self.set_item_count(len(self.items))
        #self.model.setRowCount(len(items))
        #for i, item in enumerate(items):
        #    from PySide.QtGui import QStandardItem
        #    self.model.setItem(i, 0, QStandardItem(item[1]))
        self.update()

    def get_item_count(self):
        return len(self.items)

    def get_item_text(self, index):
        item = self.items[index]
        name = item[str("name")]
        platform = item[str("platform")] or ""
        if "[" in name:
            name, extra = name.split("[", 1)
            name = name.strip()
            extra = extra.strip(" ]") + " \u00b7 "
        else:
            extra = ""
        return "{0}\n{1}{2}".format(name, extra, platform)

    def get_item_search_text(self, index):
        #return self.items[index][3]
        # FIXME: lower-case search string?
        return self.items[index][str("sort_key")]

    def get_item_icon(self, index):
        if self.items[index][str("have")] == 1:
            return self.manual_download_icon
        elif self.items[index][str("have")] == 2:
            return self.auto_download_icon
        elif self.items[index][str("have")] == 4:
            return self.game_icon
        else:
            return self.config_icon

    #def on_get_item_tooltip(self, row, column):
    #    return self.items[row][1]
    #    #text = text.replace("\nAmiga \u00b7 ", "\n")

    def update_search(self):
        self.search = Settings.get("config_search").strip().lower()
        print("search for", self.search)
        words = []
        special = []
        for word in self.search.split(" "):
            word = word.strip()
            if not word:
                continue
            if ":" in word[1:-1]:
                special.append(word)
            else:
                words.append(word)
        terms = GameNameUtil.extract_search_terms(" ".join(words))
        terms.update(special)

        database = Database.get_instance()
        #items = database.search_configurations(self.search)
        items = database.find_games_new(" ".join(terms))

        self.set_items(items)

    def load_configuration(self, item):
        if item[str("uuid")]:
            Settings.set("parent_uuid", item[str("uuid")])
        else:
            config_path = Database.get_instance().decode_path(
                item[str("path")])
            print("load config from", config_path)
            Config.load_file(config_path)
            Settings.set("parent_uuid", "")

        # database = Database.get_instance()
        # config_info = database.get_config(configuration_id)
        # if config_info["data"]:
        #     Config.load_data(config_info["data"])
        #     Settings.set("parent_uuid", "")
        # elif config_info["path"]:
        #     Config.load_file(config_info["path"])
        #     Settings.set("parent_uuid", "")
        # else:
        #     Settings.set("parent_uuid", config_info["uuid"])
