from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..IconButton import IconButton
from ..Settings import Settings
from ..I18N import _, ngettext
from .ConfigurationsBrowser import ConfigurationsBrowser
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

        self.configurations_browser = ConfigurationsBrowser(self)
        self.layout.add(self.configurations_browser, fill=True, expand=True,
                margin=10)

    def on_verified_button(self):
        pass

    def on_favorite_button(self):
        pass

    def on_search_change(self):
        text = self.text_field.get_text()
        Settings.set("config_search", text)
