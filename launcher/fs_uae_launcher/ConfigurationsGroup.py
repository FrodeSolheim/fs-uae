from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .IconButton import IconButton
from .ScanDialog import ScanDialog
from .Settings import Settings
from .I18N import _, ngettext

class ConfigurationsGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        #self.layout.add_spacer(20)

        #image = fsui.Image("fs_uae_launcher:res/kickstart.png")
        #self.image_view = fsui.ImageView(self, image)
        #self.layout.add(self.image_view, valign=0.0)

        #self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        label = fsui.HeadingLabel(self, _("Games and Configurations"))
        self.layout2.add(label)
        self.layout2.add_spacer(10)

        self.layout3 = fsui.HorizontalLayout()
        self.layout2.add(self.layout3, fill=True)

        self.scan_button = fsui.Button(self, _("Scan"))
        self.scan_button.set_tooltip(
                _("Scan for Kickstarts, Games and Configurations"))
        self.scan_button.on_activate = self.on_scan_button
        self.layout3.add(self.scan_button)

        self.layout3.add_spacer(40, 0)

        self.filters_label = fsui.Label(self, _("Filters:"))
        self.layout3.add(self.filters_label)

        self.layout3.add_spacer(10)

        self.text_field = fsui.TextField(self, Settings.get("config_search"))
        self.text_field.on_change = self.on_search_change
        self.layout3.add(self.text_field, expand=True)

        self.layout3.add_spacer(10)

        self.favorite_button = IconButton(self, "favorite_button.png")
        self.favorite_button.set_tooltip("Show Only Favorites (On/Off)")
        self.favorite_button.disable()
        #self.favorite_button.on_activate = self.on_new_button
        self.layout3.add(self.favorite_button, fill=True)

        self.layout3.add_spacer(10)

        self.ok_button = IconButton(self, "ok_button.png")
        self.ok_button.set_tooltip("Show Only Verified Configurations (On/Off)")
        self.ok_button.disable()
        #self.new_button.on_activate = self.on_new_button
        self.layout3.add(self.ok_button, fill=True)

    def on_search_change(self):
        text = self.text_field.get_text()
        Settings.set("config_search", text)

    def on_scan_button(self):
        dialog = ScanDialog(self.get_window())
        dialog.show_modal()
        dialog.destroy()
