from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import traceback
import fsui as fsui
from ...FSUAEDirectories import FSUAEDirectories
from ...I18N import _, ngettext
from ...Settings import Settings
from ..IconButton import IconButton

class ScanPathsGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 10
        self.layout.padding_top = 10
        self.layout.padding_right = 10
        self.layout.padding_bottom = 10

        image = fsui.Image("fs_uae_launcher:res/search_group.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add_spacer(20)
        self.layout.add(self.image_view, valign=0.0)
        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        hlayout = fsui.HorizontalLayout()
        self.layout2.add(hlayout, fill=True)

        self.list_view = fsui.ListView(self)
        self.list_view.set_min_height(130)
        self.list_view.set_default_icon(fsui.Image(
                "fs_uae_launcher:res/folder_16.png"))
        hlayout.add(self.list_view, expand=True, fill=True)
        hlayout.add_spacer(10)

        vlayout = fsui.VerticalLayout()
        hlayout.add(vlayout, fill=True)

        add_button = IconButton(self, "add_button.png")
        add_button.set_tooltip(_("Add Directory to Search Path"))
        #add_button.disable()
        add_button.on_activate = self.on_add_button
        vlayout.add(add_button)
        vlayout.add_spacer(10)

        self.remove_button = IconButton(self, "remove_button.png")
        self.remove_button.set_tooltip(_("Remove Directory from Search Path"))
        self.remove_button.disable()
        self.remove_button.on_activate = self.on_remove_button
        vlayout.add(self.remove_button)

        self.list_view.set_items(self.get_search_path())
        self.list_view.on_select_item = self.on_select_item
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_setting(self, key, value):
        if key == "search_path":
            self.repopulate_list()

    def on_select_item(self, index):
        self.remove_button.enable()

    def repopulate_list(self):
        #paths = self.get_search_path()
        self.list_view.set_items(self.get_search_path())

    @classmethod
    def get_search_path(cls):
        paths = FSUAEDirectories.get_default_search_path()
        search_path = Settings.get("search_path")
        for p in search_path.split(";"):
            p = p.strip()
            if not p:
                continue
            elif p[0] == "-":
                p = p[1:]
                if p in paths:
                    paths.remove(p)
            else:
                if not p in paths:
                    paths.append(p)
        return paths

    def on_add_button(self):
        paths = self.get_search_path()
        #search_path = Settings.get("search_path")

        search_path = Settings.get("search_path")
        search_path = [x.strip() for x in search_path.split(";") if x.strip()]

        dialog = fsui.DirDialog(self.get_window())
        if dialog.show_modal():
            path = dialog.get_path()
            for i in range(len(search_path)):
                if search_path[i].startswith("-"):
                    if path == search_path[i][1:]:
                        search_path.remove(search_path[i])
                        break
                else:
                    if search_path[i] == path:
                        # already added
                        break
            else:
                default_paths = FSUAEDirectories.get_default_search_path()
                if path not in default_paths:
                    search_path.append(path)
            Settings.set("search_path", ";".join(search_path))
        dialog.destroy()

    def on_remove_button(self):
        path = self.list_view.get_item(self.list_view.get_index())
        #search_path = self.get_search_path()

        search_path = Settings.get("search_path")
        search_path = [x.strip() for x in search_path.split(";") if x.strip()]

        for i in range(len(search_path)):
            if search_path[i].startswith("-"):
                if path == search_path[i][1:]:
                    # already removed
                    break
            else:
                if search_path[i] == path:
                    search_path.remove(search_path[i])
                    break
        default_paths = FSUAEDirectories.get_default_search_path()
        if path in default_paths:
            search_path.append("-" + path)
        Settings.set("search_path", ";".join(search_path))
