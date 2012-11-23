from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext
from ..Settings import Settings
from .ConfigurationsBrowser import ConfigurationsBrowser
from .IconButton import IconButton
from .ScanDialog import ScanDialog
from .Skin import Skin

class ConfigurationsPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)
        self.layout = fsui.VerticalLayout()

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True)

        label = fsui.HeadingLabel(self, _("Games and Configurations"))
        hor_layout.add(label, margin=10)

        hor_layout.add_spacer(0, expand=True)

        self.filters_label = fsui.Label(self, _("Filters:"))
        hor_layout.add(self.filters_label,
                margin=10, margin_top=0, margin_bottom=0)

        self.text_field = fsui.TextField(self, Settings.get("config_search"))
        self.text_field.on_change = self.on_search_change
        hor_layout.add(self.text_field, expand=True,
                margin=10, margin_top=0, margin_bottom=0)

        #self.favorite_button = IconButton(self, "favorite_button.png")
        #self.favorite_button.set_tooltip(
        #        _("Show Only Favorites (On/Off)"))
        #self.favorite_button.disable()
        #self.favorite_button.on_activate = self.on_favorite_button
        #hor_layout.add(self.favorite_button,
        #        margin=10, margin_top=0, margin_bottom=0)

        #self.verified_button = IconButton(self, "ok_button.png")
        #self.verified_button.set_tooltip(
        #        _("Show Only Verified Configurations (On/Off)"))
        #self.verified_button.disable()
        #self.verified_button.on_activate = self.on_verified_button
        #hor_layout.add(self.verified_button,
        #       margin=10, margin_top=0, margin_bottom=0)

        if Settings.get("database_feature") == "1":
            self.refresh_button = IconButton(self, "refresh_button.png")
            self.refresh_button.set_tooltip(
                    _("Refresh game configurations from database"))
            self.refresh_button.on_activate = self.on_refresh_button
            hor_layout.add(self.refresh_button,
                   margin=10, margin_top=0, margin_bottom=0)

        self.configurations_browser = ConfigurationsBrowser(self)
        self.layout.add(self.configurations_browser, fill=True, expand=True,
                margin=10)

    def on_verified_button(self):
        pass

    def on_favorite_button(self):
        pass

    def on_refresh_button(self):
        dialog = ScanDialog(self.get_window(), minimal=True, interactive=False,
                scan_files=False, scan_roms=False, scan_configs=False)
        dialog.show_modal()
        dialog.destroy()

    def on_search_change(self):
        text = self.text_field.get_text()
        Settings.set("config_search", text)
