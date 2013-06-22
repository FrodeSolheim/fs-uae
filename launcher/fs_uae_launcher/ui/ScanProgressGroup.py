from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import traceback
import fsui as fsui
from ..Settings import Settings
from ..I18N import _, ngettext

class ScanProgressGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 10
        self.layout.padding_top = 10
        self.layout.padding_right = 10
        self.layout.padding_bottom = 10

        #image = fsui.Image("fs_uae_launcher:res/search_group.png")
        #self.image_view = fsui.ImageView(self, image)
        self.layout.add_spacer(20)
        #self.layout.add(self.image_view, valign=0.0)
        self.layout.add_spacer(48)
        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        self.title_label = fsui.HeadingLabel(self, "")
        self.layout2.add(self.title_label)

        #self.layout2.add_spacer(10)
        #hor_layout = fsui.HorizontalLayout()
        #self.layout2.add(hor_layout)

        #self.scan_label = fsui.Label(self, _("Scan for:"))
        #hor_layout.add(self.scan_label)
        #hor_layout.add_spacer(10)

        #self.scan_roms = fsui.CheckBox(self, _("ROMs"))
        #if Settings.get("scan_roms") == "1":
        #    self.scan_roms.check()
        #self.scan_roms.on_change = self.on_change
        #hor_layout.add(self.scan_roms)
        #hor_layout.add_spacer(10)

        #self.scan_files = fsui.CheckBox(self, _("Game Files"))
        #if Settings.get("scan_files") == "1":
        #    self.scan_files.check()
        #self.scan_files.on_change = self.on_change
        #hor_layout.add(self.scan_files)
        #hor_layout.add_spacer(10)

        #self.scan_configs = fsui.CheckBox(self, _("Configurations"))
        #if Settings.get("scan_configs") == "1":
        #    self.scan_configs.check()
        #self.scan_configs.on_change = self.on_change
        #hor_layout.add(self.scan_configs)
        #hor_layout.add_spacer(10)

        self.layout2.add_spacer(10)
        self.status_label = fsui.Label(self, "")
        self.layout2.add(self.status_label)

    #def on_change(self):
    #    value = "1" if self.scan_roms.is_checked() else "0"
    #    Settings.set("scan_roms", value)
    #    value = "1" if self.scan_files.is_checked() else "0"
    #    Settings.set("scan_files", value)
    #    value = "1" if self.scan_configs.is_checked() else "0"
    #    Settings.set("scan_configs", value)
