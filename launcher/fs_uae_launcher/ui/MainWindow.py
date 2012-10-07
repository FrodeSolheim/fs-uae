from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import uuid
import fs_uae_launcher.fsui as fsui
import fs_uae_launcher.fs as fs
from ..Amiga import Amiga
from ..Config import Config
from ..Signal import Signal
from ..Settings import Settings
from ..netplay.IRC import IRC
#from .FSUAE import FSUAE
#from .ConfigWriter import ConfigWriter
from ..netplay.Netplay import Netplay
from ..Database import Database
from ..GameHandler import GameHandler
from ..I18N import _, ngettext
from .ScreenshotsPanel import ScreenshotsPanel
from .GameInfoPanel import GameInfoPanel
from .TabPanel import TabPanel
from .TabButton import TabButton
from .BottomPanel import BottomPanel
from .Book import Book
from .MainPanel import MainPanel
from .FloppiesPanel import FloppiesPanel
from .CDPanel import CDPanel
from .HardDrivesPanel import HardDrivesPanel
from .HardwarePanel import HardwarePanel
from .InputPanel import InputPanel
#from .CustomPanel import CustomPanel
from .ConfigurationsPanel import ConfigurationsPanel
from .NetplayPanel import NetplayPanel
from .SetupPanel import SetupPanel
from .Constants import Constants
from .Skin import Skin

class MainWindow(fsui.Window):

    instance = None

    def __init__(self, icon):
        fsui.Window.__init__(self, None, "FS-UAE Launcher")
        Skin.set_background_color(self)

        #self.set_size((1000, 600))
        if icon:
            self.set_icon_from_path(icon)

        self.layout = fsui.HorizontalLayout()

        self.tab_panels = []
        self.books = []
        # left border
        self.create_column(0, min_width=Skin.get_window_padding_left(),
                content=False)
        # left content
        if fsui.get_screen_size()[0] > 1024:
            self.create_column(1, min_width=514)
        else:
            self.create_column(1, min_width=400)

        # right content
        right_width = Constants.SCREEN_SIZE[0] * 2 + 20 + 10 + 10
        extra_screen_width = Constants.SCREEN_SIZE[0] + 20
        need_width = 1280
        if self.is_editor_enabled():
            need_width += extra_screen_width
        else:
            if fsui.get_screen_size()[0] >= need_width:
                # make room for one more screenshot
                right_width += extra_screen_width
                pass
        
        #if Skin.EXTRA_GROUP_MARGIN:
        #    self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        
        self.create_column(2, min_width=right_width, expand=True)

        # right border
        self.create_column(3, min_width=Skin.get_window_padding_right(),
                content=False)

        if self.is_editor_enabled():
            from ..editor.EditorGroup import EditorGroup
            editor = EditorGroup(self)
            self.layout.add(editor, fill=True, expand=True,
                margin_right=20)

        was_maximized = Settings.get("maximized") == "1"

        self.set_size(self.layout.get_min_size())
        #fsui.call_later(500, self.on_timer)

        self.center_on_screen()
        if was_maximized:
            self.maximize()

    def on_resize(self):
        print("on_resize, size =", self.get_size(), self.is_maximized())
        if self.is_maximized():
            Settings.set("maximized", "1")
        else:
            Settings.set("maximized", "0")
        fsui.Window.on_resize(self)

    def on_destroy(self):
        print("MainWindow.destroy")
        IRC.stop()
        #Config.set("__quit", "1")
        Signal.broadcast("quit")

    def is_editor_enabled(self):
        return "--editor" in sys.argv

    def create_column(self, column, content=True, expand=False, min_width=0):
        layout = fsui.VerticalLayout()
        self.layout.add(layout, fill=True, expand=expand)
        if min_width:
            layout.add_spacer(min_width, 0)
        if min_width < 10:
            tab_panel = TabPanel(self, spacing=min_width)
        else:
            tab_panel = TabPanel(self)
        self.tab_panels.append(tab_panel)

        layout.add(tab_panel, fill=True)
        layout.add_spacer(0, 10 + Skin.EXTRA_GROUP_MARGIN)

        if content:
            book = Book(self)
            Skin.set_background_color(book)
            if column == 1:
                margin_right = Skin.EXTRA_GROUP_MARGIN
            else:
                margin_right = 0
            layout.add(book, fill=True, expand=True, margin_right=margin_right)
            self.books.append(book)
            self.add_column_content(column)
        else:
            layout.add_spacer(0, expand=True)
            self.books.append(None)

        layout.add_spacer(0, 10 + Skin.EXTRA_GROUP_MARGIN)
        if fsui.get_screen_size()[1] >= 768:
            right_margin = 0
            if column == 0:
                bottom_panel = BottomPanel(self)
            elif column == 1:
                bottom_panel = GameInfoPanel(self)
            elif column == 2:
                bottom_panel = ScreenshotsPanel(self)
                right_margin = -10 - Skin.EXTRA_GROUP_MARGIN
            else:
                bottom_panel = None
            # FIXME:
            if bottom_panel is None:
                layout.add_spacer(0, Skin.get_bottom_panel_height())
            else:
                bottom_panel.set_min_height(Skin.get_bottom_panel_height())
                layout.add(bottom_panel, fill=True, margin_right=right_margin)
        elif column == 1:
            from .LaunchGroup import LaunchGroup
            group = LaunchGroup(self)
            layout.add(group, fill=True, margin=10, margin_top=0)
            layout.add_spacer(0, 10)

    def add_column_content(self, column):
        default_page_index = 0
        if column == 1:
            self.add_content(column, MainPanel, "tab_main")
            self.add_content(column, InputPanel, "tab_input")
            self.add_content(column, FloppiesPanel, "tab_floppies")
            self.add_content(column, CDPanel, "tab_cdroms")
            self.add_content(column, HardDrivesPanel, "tab_hard_drives")
            self.add_content(column, HardwarePanel, "tab_hardware")
            #self.add_content(column, CustomPanel, "tab_custom")

            tab_panel = self.tab_panels[column]
            icon = fsui.Image("fs_uae_launcher:res/tab_custom.png")
            button = TabButton(tab_panel, icon, type=TabButton.TYPE_BUTTON)
            button.on_activate = self.on_custom_button
            #tab_panel.add_spacer(expand=True)
            tab_panel.add(button)

        elif column == 2:
            page_index = 0
            self.add_content(column, ConfigurationsPanel, "tab_configs")
            if Settings.get("netplay_feature") == "1":
                page_index += 1
                self.add_content(column, NetplayPanel, "tab_netplay")
            page_index += 1
            page = self.add_content(column, SetupPanel, "tab_setup")
            if page.should_be_automatically_opened():
                default_page_index = page_index

            tab_panel = self.tab_panels[column]
            tab_panel.add_spacer(expand=True)

            icon = fsui.Image("fs_uae_launcher:res/tab_scan.png")
            button = TabButton(tab_panel, icon, type=TabButton.TYPE_BUTTON)
            button.on_activate = self.on_scan_button
            tab_panel.add(button)

            icon = fsui.Image("fs_uae_launcher:res/tab_settings.png")
            button = TabButton(tab_panel, icon, type=TabButton.TYPE_BUTTON)
            button.on_activate = self.on_settings_button
            tab_panel.add(button)

        self.tab_panels[column].select_tab(default_page_index)
        self.books[column].set_page(default_page_index)

    def add_content(self, column, content_class, icon_name):
        book = self.books[column]
        instance = content_class(book)
        book.add_page(instance)
        tab_panel = self.tab_panels[column]

        icon = fsui.Image("fs_uae_launcher:res/{0}.png".format(icon_name))
        button = TabButton(tab_panel, icon)
        def function():
            book.set_page(instance)
        button.on_select = function
        tab_panel.add(button)
        return instance

    def on_custom_button(self):
        from .config.ConfigDialog import ConfigDialog
        ConfigDialog.run(self.get_window(), ConfigDialog.CUSTOM_OPTIONS)


    def on_scan_button(self):
        from .ScanDialog import ScanDialog
        dialog = ScanDialog(self.get_window())
        dialog.show_modal()
        dialog.destroy()

    def on_settings_button(self):
        from ..settingsui.SettingsDialog import SettingsDialog
        SettingsDialog.run(self)
