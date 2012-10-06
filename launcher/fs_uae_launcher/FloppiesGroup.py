from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .FloppyManager import FloppyManager
from .I18N import _, ngettext

class FloppiesGroup(fsui.Group):

    def __init__(self, parent, drives=2, cd_mode=False):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.cd_mode = cd_mode
        self.num_drives = drives

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        if cd_mode:
            self.label = fsui.HeadingLabel(self, _("CD-ROM Drive"))
        else:
            self.label = fsui.HeadingLabel(self, _("Floppy Drives"))
        hori_layout.add(self.label, margin=10)
        hori_layout.add_spacer(0, expand=True)

        self.multi_select_button = fsui.Button(self,
                _("Select Multiple Files..."))
        if self.cd_mode:
            self.multi_select_button.set_tooltip(
                    _("Add Multiple CD-ROMs at Once"))
        else:
            self.multi_select_button.set_tooltip(
                    _("Add Multiple Floppies at Once"))
        self.multi_select_button.on_activate = self.on_multi_select_button

        hori_layout.add(self.multi_select_button, margin_right=10)

        self.layout.add_spacer(0)

        self.selectors = []
        for i in range(drives):
            #self.layout.add_spacer(10)
            from .FloppySelector import FloppySelector
            selector = FloppySelector(parent, i)
            if cd_mode:
                selector.set_cd_mode(True)
            self.selectors.append(selector)
            self.layout.add(selector, fill=True, margin=10)

    def on_multi_select_button(self):
        if self.cd_mode:
            from .CDManager import CDManager
            CDManager.multiselect(self.get_window())
        else:
            from .FloppyManager import FloppyManager
            FloppyManager.multiselect(self.get_window())

    def update_heading_label(self):
        if self.cd_mode:
            if self.num_drives > 1:
                self.label.set_text(_("CD-ROM Drives"))
            else:
                self.label.set_text(_("CD-ROM Drive"))
        else:
            self.label.set_text(_("Floppy Drives"))
