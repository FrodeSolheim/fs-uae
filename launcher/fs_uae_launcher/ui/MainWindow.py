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
from ..Version import Version
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
from .InfoPanel import InfoPanel
from .InputPanel import InputPanel
#from .CustomPanel import CustomPanel
from .ConfigurationsPanel import ConfigurationsPanel
from .NetplayPanel import NetplayPanel
from .SetupPanel import SetupPanel
from .Constants import Constants
from .Skin import Skin
from .WindowWithTabs import WindowWithTabs

class MainWindow(WindowWithTabs):

    instance = None

    def __init__(self, icon):
        title = "FS-UAE Launcher {0}".format(Version.VERSION)
        WindowWithTabs.__init__(self, None, title)
        if icon:
            self.set_icon_from_path(icon)

        self.tab_panels = []
        self.books = []

        self.main_layout = fsui.HorizontalLayout()
        self.set_content(self.main_layout)

        # left border
        self.create_column(0, min_width=Skin.get_window_padding_left(),
                content=False)
        # left content
        if fsui.get_screen_size()[0] > 1024:
            self.create_column(1, min_width=518)
        else:
            self.create_column(1, min_width=400)

        # right content
        right_width = Constants.SCREEN_SIZE[0] * 2 + 21 + 10 + 10
        extra_screen_width = Constants.SCREEN_SIZE[0] + 21
        need_width = 1280
        if self.is_editor_enabled():
            need_width += extra_screen_width
        else:
            if fsui.get_screen_size()[0] >= need_width:
                # make room for one more screenshot
                right_width += extra_screen_width
                pass

        #if Skin.EXTRA_GROUP_MARGIN:
        #    self.main_layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)

        self.create_column(2, min_width=right_width, expand=True)

        # right border
        self.create_column(3, min_width=Skin.get_window_padding_right(),
                content=False)

        if self.is_editor_enabled():
            from ..editor.EditorGroup import EditorGroup
            editor = EditorGroup(self)
            self.main_layout.add(editor, fill=True, expand=True,
                margin_right=20)

        self.realize_tabs()

        was_maximized = Settings.get("maximized") == "1"
        self.set_size(self.layout.get_min_size())

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
        Signal.broadcast("quit")

    def is_editor_enabled(self):
        return "--editor" in sys.argv

    def create_column(self, column, content=True, expand=False, min_width=0):
        layout = fsui.VerticalLayout()
        self.main_layout.add(layout, fill=True, expand=expand)
        if min_width:
            layout.add_spacer(min_width, 0)

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
            self.add_page(column, MainPanel, "tab_main", _("Config"),
                    _("Main Configuration Options"))
            self.add_page(column, InputPanel, "tab_input", _("Input"),
                    _("Input Options"))
            self.add_page(column, FloppiesPanel, "tab_floppies",
                    _("Floppies"), _("Floppy Drives"))
            self.add_page(column, CDPanel, "tab_cdroms", _("CD-ROMs"),
                    _("CD-ROM Drives"))
            self.add_page(column, HardDrivesPanel, "tab_hard_drives",
                    _("Hard Drives"))
            self.add_page(column, HardwarePanel, "tab_hardware",
                    _("Hardware"), _("Hardware Options"))

            icon = fsui.Image("fs_uae_launcher:res/tab_custom.png")
            self.add_tab_button(self.on_custom_button, icon, _("Custom"),
                    _("Edit Custom Options"))

            self.add_tab_spacer(60)

        elif column == 2:
            self.new_tab_group()
            page_index = 0
            self.add_page(column, ConfigurationsPanel, "tab_configs",
                    _("Configurations"), _("Configuration Browser"))
            if Settings.get("netplay_feature") == "1":
                page_index += 1
                self.add_page(column, NetplayPanel, "tab_netplay",
                        _("Net Play"))
            page_index += 1
            page = self.add_page(column, SetupPanel, "tab_setup",
                    _("Kickstart Setup"))
            if page.should_be_automatically_opened():
                default_page_index = page_index

            if fsui.System.macosx:
                self.add_tab_separator()
            else:
                self.add_tab_spacer(10)
                self.add_tab_panel(InfoPanel)
                self.add_tab_spacer(10)

            icon = fsui.Image("fs_uae_launcher:res/tab_scan.png")
            self.add_tab_button(self.on_scan_button, icon, _("Scan"),
                    _("Open Scan Dialog"))
            icon = fsui.Image("fs_uae_launcher:res/tab_settings.png")
            self.add_tab_button(self.on_settings_button, icon, _("Settings"))

            if fsui.System.macosx:
                self.add_tab_panel(InfoPanel, min_width=300)

        # column - 1 is the group id of the tab group
        self.select_tab(default_page_index, column - 1)
        self.books[column].set_page(default_page_index)

    def add_page(self, column, content_class, icon_name, title, tooltip=""):
        book = self.books[column]
        instance = content_class(book)
        book.add_page(instance)
        icon = fsui.Image("fs_uae_launcher:res/{0}.png".format(icon_name))
        def function():
            book.set_page(instance)
        self.add_tab(function, icon, title, tooltip)
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
        from .settings.SettingsDialog import SettingsDialog
        SettingsDialog.run(self)
