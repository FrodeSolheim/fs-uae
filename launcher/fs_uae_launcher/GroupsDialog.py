from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .Separator import Separator
from .I18N import _, ngettext

class GroupsDialog(fsui.Dialog):

    def __init__(self, parent, title, width=540):
        fsui.Dialog.__init__(self, parent, title)
        self.layout = fsui.VerticalLayout()
        self.layout.add_spacer(width, 0)
        self.layout.padding_top = 20
        self.layout.padding_right = 20
        self.layout.padding_bottom = 20

        self.groups_layout = fsui.VerticalLayout()
        self.layout.add(self.groups_layout, fill=True)

        self.layout.add_spacer(20)
        self.layout.add_spacer(20)

        self.buttons_layout = fsui.HorizontalLayout()
        self.layout.add(self.buttons_layout, fill=True)

        self.buttons_layout.padding_right = 20
        self.buttons_layout.add_spacer(0, expand=True)

    def add_group(self, group):
        self.groups_layout.add(group, fill=True)

    def add_button(self, button):
        self.buttons_layout.add_spacer(10)
        self.buttons_layout.add(button)

    def add_close_button(self):
        self.close_button = fsui.Button(self, _("Close"))
        self.close_button.on_activate = self.on_close_button
        #self.buttons_layout.add(self.close_button)
        self.add_button(self.close_button)

    def add_separator(self):
        self.groups_layout.add(Separator(self), fill=True)

    def on_close_button(self):
        self.end_modal(False)
