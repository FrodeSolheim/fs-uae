from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ..I18N import _
from ..Settings import Settings
from .ConfigurationsBrowser import ConfigurationsBrowser
from .IconButton import IconButton
from .ScanDialog import ScanDialog
from .Skin import Skin
from .VariantsBrowser import VariantsBrowser


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

        _("Filters:")
        #self.filters_label = fsui.Label(self, _("Filters:"))
        #hor_layout.add(
        #    self.filters_label, margin=10, margin_top=0, margin_bottom=0)

        self.text_field = fsui.TextField(self, Settings.get("config_search"))
        self.text_field.on_change = self.on_search_change
        if VariantsBrowser.use_horizontal_layout():
            # window is big enough to use fixed size
            #self.text_field.set_min_width(210)
            self.text_field.set_min_width(229)
            hor_layout.add(
                self.text_field, expand=False, margin=10, margin_top=0,
                margin_bottom=0)
        else:
            hor_layout.add(
                self.text_field, expand=True, margin=10, margin_top=0,
                margin_bottom=0)

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
            hor_layout.add(
                self.refresh_button, margin=10, margin_top=0, margin_bottom=0)

        self.configurations_browser = ConfigurationsBrowser(self)

        if VariantsBrowser.use_horizontal_layout():
            hori_layout = fsui.HorizontalLayout()
            self.layout.add(hori_layout, fill=True, expand=True, margin=10)
            hori_layout.add(self.configurations_browser, fill=True, expand=2)
        else:
            hori_layout = None
            self.layout.add(
                self.configurations_browser, fill=True, expand=3, margin=10)

        if Settings.get("database_feature") == "1":
            self.variants_browser = VariantsBrowser(self)
            if VariantsBrowser.use_horizontal_layout():
                hori_layout.add(
                    self.variants_browser, fill=True, expand=1, margin_left=18)
                #self.variants_browser.set_min_width(Constants.SCREEN_SIZE[0])
                #elf.variants_browser.set_min_width(72)
                self.variants_browser.set_min_width(100)
            else:
                self.layout.add(
                    self.variants_browser, fill=True, expand=1, margin=10,
                    margin_top=20)
        else:
            self.variants_browser = None

        Settings.add_listener(self)
        self.on_setting("parent_uuid", Settings.get("parent_uuid"))

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_setting(self, key, value):
        if key == "parent_uuid":
            if self.variants_browser is not None:
                if VariantsBrowser.use_horizontal_layout():
                    self.variants_browser.show_or_hide(bool(value))
                    self.layout.update()
                else:
                    # always show variants list
                    pass

    def on_verified_button(self):
        pass

    def on_favorite_button(self):
        pass

    def on_refresh_button(self):
        dialog = ScanDialog.refresh_game_database(self.get_window())
        dialog.show_modal()
        dialog.destroy()

    def on_search_change(self):
        text = self.text_field.get_text()
        Settings.set("config_search", text)
