from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..I18N import _, ngettext
from ..LaunchHandler import LaunchHandler
from .settings.FullscreenCheckBox import FullscreenCheckBox

class LaunchGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        self.fullscreen_checkbox = FullscreenCheckBox(self)
        self.layout.add(self.fullscreen_checkbox)

        self.layout.add_spacer(0, expand=True)

        self.start_button = fsui.Button(parent, _("Start"))
        self.start_button.on_activate = self.on_start_button
        self.layout.add(self.start_button, margin_left=20)

    def on_start_button(self):
        #from ..netplay.Netplay import Netplay
        #if Netplay.game_channel:
        #    Netplay.start_netplay_game()
        #else:
        #    from ..LaunchHandler import LaunchHandler
        LaunchHandler.start_game()
