from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from .IconButton import IconButton
from .Settings import Settings
from .I18N import _, ngettext

class ScanPathsGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_right = 20
        self.layout.add_spacer(20)

        image = fsui.Image("fs_uae_launcher:res/search_group.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        hlayout = fsui.HorizontalLayout()
        self.layout2.add(hlayout, fill=True)
        #hlayout.padding_left = 20
        #hlayout.padding_right = 20

        #self.text = fsui.TextArea(self, read_only=True)
        #self.text.set_min_height(100)
        #hlayout.add(self.text, expand=True, fill=True)
        self.list_view = fsui.ListView(self)
        self.list_view.set_min_height(130)
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

        self.layout2.add_spacer(10)

        """
        hor_layout = fsui.HorizontalLayout()
        self.layout2.add(hor_layout)

        self.scan_label = fsui.Label(self, _("Scan for:"))
        hor_layout.add(self.scan_label)
        hor_layout.add_spacer(10)

        self.scan_roms = fsui.CheckBox(self, _("ROMs"))
        if Settings.get("scan_roms") == "1":
            self.scan_roms.check()
        self.scan_roms.on_change = self.on_change
        hor_layout.add(self.scan_roms)
        hor_layout.add_spacer(10)

        self.scan_files = fsui.CheckBox(self, _("Game Files"))
        if Settings.get("scan_files") == "1":
            self.scan_files.check()
        self.scan_files.on_change = self.on_change
        hor_layout.add(self.scan_files)
        hor_layout.add_spacer(10)

        self.scan_configs = fsui.CheckBox(self, _("Configurations"))
        if Settings.get("scan_configs") == "1":
            self.scan_configs.check()
        self.scan_configs.on_change = self.on_change
        hor_layout.add(self.scan_configs)
        hor_layout.add_spacer(10)
        """

        self.list_view.set_items(self.get_search_path())
        self.list_view.on_select_item = self.on_select_item
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_setting(self, key, value):
        if key == "search_path":
            self.repopulate_list()

    def on_select_item(self):
        self.remove_button.enable()

    def repopulate_list(self):
        #paths = self.get_search_path()
        self.list_view.set_items(self.get_search_path())

    def get_search_path(self):
        paths = Settings.get_default_search_path()
        search_path = Settings.get("search_path")
        for p in search_path.split(u";"):
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

    def on_change(self):
        value = "1" if self.scan_roms.is_checked() else "0"
        Settings.set("scan_roms", value)
        value = "1" if self.scan_files.is_checked() else "0"
        Settings.set("scan_files", value)
        value = "1" if self.scan_configs.is_checked() else "0"
        Settings.set("scan_configs", value)

    def on_add_button(self):
        paths = self.get_search_path()
        #search_path = Settings.get("search_path")

        search_path = Settings.get("search_path")
        search_path = [x.strip() for x in search_path.split(u";") if x.strip()]

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
                default_paths = Settings.get_default_search_path()
                if path not in default_paths:
                    search_path.append(path)
            Settings.set("search_path", u";".join(search_path))
        dialog.destroy()

    def on_remove_button(self):
        path = self.list_view.get_item(self.list_view.get_index())
        #search_path = self.get_search_path()

        search_path = Settings.get("search_path")
        search_path = [x.strip() for x in search_path.split(u";") if x.strip()]

        for i in range(len(search_path)):
            if search_path[i].startswith("-"):
                if path == search_path[i][1:]:
                    # already removed
                    break
            else:
                if search_path[i] == path:
                    search_path.remove(search_path[i])
                    break
        default_paths = Settings.get_default_search_path()
        if path in default_paths:
            search_path.append(u"-" + path)
        Settings.set("search_path", u";".join(search_path))
