from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ...Config import Config
from ...Settings import Settings
from ...I18N import _, ngettext
#from ..LaunchHandler import LaunchHandler
#from .settings.FullscreenCheckBox import FullscreenCheckBox
from ..settings.FullscreenToggleButton import FullscreenToggleButton


class LaunchGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        #self.fullscreen_checkbox = FullscreenCheckBox(self)
        #self.layout.add(self.fullscreen_checkbox)
        self.fullscreen_button = FullscreenToggleButton(self)
        self.layout.add(self.fullscreen_button, fill=True)

        self.layout.add_spacer(0, expand=True)

        self.start_button = fsui.Button(parent, _("Start"))
        self.start_button.on_activate = self.on_start_button
        self.layout.add(self.start_button, margin_left=10)

    def on_start_button(self):
        from ...FSUAELauncher import FSUAELauncher
        FSUAELauncher.start_game()
