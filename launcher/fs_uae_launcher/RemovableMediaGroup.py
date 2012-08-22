from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Amiga import Amiga
from .Config import Config
from .Settings import Settings
from .IconButton import IconButton
from .CDManager import CDManager
from .FloppyManager import FloppyManager
from .FloppiesGroup import FloppiesGroup
from .I18N import _, ngettext

class RemovableMediaGroup(FloppiesGroup):

    def __init__(self, parent, drives):
        FloppiesGroup.__init__(self, parent, drives)
        #self.layout2.add_spacer(10)
        self.layout3 = fsui.HorizontalLayout()
        self.layout.add(self.layout3, fill=True)

        self.layout3.add_spacer(0, expand=True)

        #self.layout3.add_spacer(10)
        #self.hds_button = IconButton(self, "hd_button.png")
        ##self.hds_button = fsui.Button(self, _("Hard Drives"))
        #self.hds_button.on_activate = self.on_hds_button
        #self.layout3.add(self.hds_button)

        """
        self.layout3.add_spacer(10)
        self.multi_select_button = fsui.Button(self,
                _("Select Files..."))
        self.multi_select_button.set_tooltip(_("Add Multiple Files at Once"))
        self.multi_select_button.on_activate = self.on_multi_select_button
        self.layout3.add(self.multi_select_button)

        self.layout3.add_spacer(10)
        self.more_button = fsui.Button(self, _("Floppies..."))
        self.more_button.on_activate = self.on_floppies_button
        self.layout3.add(self.more_button)

        self.layout3.add_spacer(10)
        self.cdroms_button = IconButton(self, "cd_button.png")
        #self.cdroms_button = fsui.Button(self, _("CD-ROMs..."))
        self.cdroms_button.set_tooltip(_("CD-ROMs"))
        self.cdroms_button.on_activate = self.on_cdroms_button
        self.layout3.add(self.cdroms_button, fill=True)
        """

        self.cd_mode = False
        #self.label.set_text(_("Removable Media"))

        self.update_media_type()
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def on_config(self, key, value):
        if key == "amiga_model":
            self.update_media_type()

    def update_media_type(self):
        self.set_cd_mode(Amiga.is_cd_based())

    def set_cd_mode(self, cd_mode):
        if self.cd_mode == cd_mode:
            return
        self.cd_mode = cd_mode
        #if self.cd_mode:
        #    self.label.set_text(_("CD-ROM Drives"))
        #else:
        #    self.label.set_text(_("Floppy Drives"))
        for selector in self.selectors:
            selector.set_cd_mode(cd_mode)
        #if cd_mode:
        #    image = fsui.Image("fs_uae_launcher:res/cd_group.png")
        #else:
        #    image = fsui.Image("fs_uae_launcher:res/floppy_group.png")
        #self.image_view.set_image(image)
        self.update_heading_label()
        self.selectors[1].enable(not self.cd_mode)

    """
    def on_multi_select_button(self):
        if self.cd_mode:
            CDManager.multiselect(self.get_window())
        else:
            FloppyManager.multiselect(self.get_window())

    def on_cdroms_button(self):
        from .CDDialog import CDDialog
        dialog = CDDialog(self.get_window())
        dialog.show_modal()
        dialog.destroy()

    def on_floppies_button(self):
        from .FloppiesDialog import FloppiesDialog
        dialog = FloppiesDialog(self.get_window())
        dialog.show_modal()
        dialog.destroy()
    """