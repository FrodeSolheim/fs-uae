from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from ..Amiga import Amiga
from ..CDManager import CDManager
from ..ChecksumTool import ChecksumTool
from ..Config import Config
from ..FloppyManager import FloppyManager
from ..I18N import _, ngettext
from ..Paths import Paths
from ..Settings import Settings
from .IconButton import IconButton
from .LauncherFileDialog import LauncherFileDialog

class MediaListGroup(fsui.Group):

    def __init__(self, parent, cd_mode):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.cd_mode = cd_mode
        if self.cd_mode:
            self.file_key_prefix = "cdrom_image_"
            self.file_key = "cdrom_image_{0}"
            self.sha1_key = "x_cdrom_image_{0}_sha1"
        else:
            self.file_key_prefix = "floppy_image_"
            self.file_key = "floppy_image_{0}"
            self.sha1_key = "x_floppy_image_{0}_sha1"

        self.heading_label = fsui.HeadingLabel(self, _("Media Swap List"))
        self.layout.add(self.heading_label, margin=10)
        self.layout.add_spacer(0)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, expand=True, fill=True)

        self.list_view = fsui.ListView(self)
        if self.cd_mode:
            self.list_view.set_default_icon(fsui.Image(
                    "fs_uae_launcher:res/cdrom_16.png"))
        else:
            self.list_view.set_default_icon(fsui.Image(
                    "fs_uae_launcher:res/floppy_16.png"))
        hori_layout.add(self.list_view, expand=True, fill=True, margin=10,
                margin_right=0)

        vert_layout = fsui.VerticalLayout()
        hori_layout.add(vert_layout, fill=True)

        add_button = IconButton(self, "add_button.png")
        add_button.set_tooltip(_("Add Files to List"))
        add_button.on_activate = self.on_add_button
        vert_layout.add(add_button, margin=10)

        remove_button = IconButton(self, "remove_button.png")
        remove_button.set_tooltip(_("Remove Selected Files"))
        remove_button.on_activate = self.on_remove_button
        vert_layout.add(remove_button, margin=10)

        clear_button = IconButton(self, "clear_button.png")
        clear_button.set_tooltip(_("Clear List"))
        clear_button.on_activate = self.on_clear_list
        vert_layout.add(clear_button, margin=10)

        self.update_list()
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def on_config(self, key, value):
        if key.startswith(self.file_key_prefix):
            self.update_list()

    def create_list(self):
        items = []
        if self.cd_mode:
            max_items = Amiga.MAX_CDROM_IMAGES
        else:
            max_items = Amiga.MAX_FLOPPY_IMAGES
        for i in range(max_items):
            path = Config.get(self.file_key.format(i))
            sha1 = Config.get(self.sha1_key.format(i))
            if not path:
                continue
            items.append((path, sha1))
        return items

    def update_list(self):
        items = []
        for path, sha1 in self.create_list():
            dir, name = os.path.split(path)
            if dir:
                items.append("{0}\n{1}".format(name, dir))
            else:
                items.append(path)
        self.list_view.set_items(items)

    def on_clear_list(self):
        if self.cd_mode:
            CDManager.clear_cdrom_list()
        else:
            FloppyManager.clear_floppy_list()

    def on_remove_button(self):
        index = self.list_view.get_index()
        existing_items = self.create_list()
        if index >= 0 and index < len(existing_items):
            del existing_items[index]
            self.set_new_config(existing_items)

    def on_add_button(self):
        existing_items = self.create_list()

        default_dir = Settings.get_floppies_dir()
        if self.cd_mode:
            dialog = LauncherFileDialog(self.get_window(),
                    _("Select Multiple CD-ROMs"), "cd", multiple=True)
        else:
            dialog = LauncherFileDialog(self.get_window(),
                    _("Select Multiple Floppies"), "floppy", multiple=True)
        if not dialog.show():
            return
        paths = dialog.get_paths()
        paths.sort()

        checksum_tool = ChecksumTool(self.get_window())
        for i, path in enumerate(paths):
            sha1 = checksum_tool.checksum(path)
            path = Paths.contract_path(path, default_dir)

            dir, file = os.path.split(path)
            if os.path.normcase(os.path.normpath(dir)) == \
                    os.path.normcase(os.path.normpath(default_dir)):
                path = file

            existing_items.append((path, sha1))
        self.set_new_config(existing_items)

    def set_new_config(self, items):
        if self.cd_mode:
            max_items = Amiga.MAX_CDROM_IMAGES
        else:
            max_items = Amiga.MAX_FLOPPY_IMAGES
        set_list = []
        for i in range(max(max_items, len(items))):
            if i >= max_items:
                break
            elif i >= len(items):
                path, sha1 = "", ""
            else:
                path, sha1 = items[i]
            set_list.append((self.file_key.format(i), path))
            set_list.append((self.sha1_key.format(i), sha1))
        Config.set_multiple(set_list)
