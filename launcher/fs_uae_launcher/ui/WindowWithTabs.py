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

class WindowWithTabs(fsui.Window):

    def __init__(self, parent, title):
        fsui.Window.__init__(self, parent, title)
        Skin.set_background_color(self)

        if fsui.System.macosx:
            import wx
            self.toolbar = self.CreateToolBar(wx.TB_HORIZONTAL | wx.NO_BORDER
                    | wx.TB_FLAT)
            #        | wx.TB_FLAT | wx.TB_TEXT)
            self.toolbar.SetMargins((0, 0))
            self.toolbar.SetToolBitmapSize((32, 32))
            self.toolbar.Realize()
            self.tab_panel = None
        else:
            self.toolbar = None
            self.tab_panel = TabPanel(self)

        self.layout = fsui.VerticalLayout()
        if self.tab_panel:
            self.layout.add(self.tab_panel, fill=True)
        self.current_tab_group_id = 0
        self.tab_groups = [[]]

    def realize_tabs(self):
        if self.toolbar:
            self.toolbar.Realize()

    def new_tab_group(self):
        self.current_tab_group_id += 1
        self.tab_groups.append([])

    def set_content(self, content):
        self.layout.add(content, expand=True, fill=True)

    def select_tab(self, index, group):
        if self.toolbar:
            pass
        else:
            self.tab_groups[group][index].select()

    def add_tab(self, function, icon, title=""):
        if self.toolbar:
            import wx
            self.add_toolbar_spacer(2)
            tool_id = wx.NewId()
            self.toolbar.AddLabelTool(tool_id, title, icon.bitmap)
            def event_handler(event):
                function()
            self.toolbar.Bind(wx.EVT_TOOL, event_handler, id=tool_id)
            self.add_toolbar_spacer(3)
            self.tab_groups[self.current_tab_group_id].append(tool_id)
        else:
            button = TabButton(self.tab_panel, icon)
            button.group_id = self.current_tab_group_id
            button.on_select = function
            self.tab_panel.add(button)
            self.tab_groups[self.current_tab_group_id].append(button)

    def add_tab_button(self, function, icon, title=""):
        if self.toolbar:
            import wx
            self.add_toolbar_spacer(2)
            tool_id = wx.NewId()
            self.toolbar.AddLabelTool(tool_id, title, icon.bitmap)
            def event_handler(event):
                function()
            self.toolbar.Bind(wx.EVT_TOOL, event_handler, id=tool_id)
            self.add_toolbar_spacer(3)
            self.tab_groups[self.current_tab_group_id].append(tool_id)
        else:
            button = TabButton(self.tab_panel, icon,
                    type=TabButton.TYPE_BUTTON)
            button.group_id = self.current_tab_group_id
            button.on_activate = function
            self.tab_panel.add(button)
            self.tab_groups[self.current_tab_group_id].append(button)

    def add_toolbar_spacer(self, width, height=46):
        import wx
        spacer_control = wx.Control(self.toolbar, size=(width, height))
        self.toolbar.AddControl(spacer_control)

    def add_tab_spacer(self, spacer=0, expand=False):
        if self.toolbar:
            import wx
            spacer_control = wx.Control(self.toolbar, size=(spacer, 1))
            self.toolbar.AddControl(spacer_control)
        else:
            self.tab_panel.add_spacer(spacer, expand=expand)
