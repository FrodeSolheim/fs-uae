from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import subprocess
import fsui as fsui
from ...Config import Config
from ...I18N import _, ngettext
from ...Settings import Settings

class JoystickSettingsPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()
        self.layout.padding_top = 20
        #self.layout.padding_right = 20
        self.layout.padding_bottom = 20

        from .PreferredJoysticksGroup import PreferredJoysticksGroup
        self.pref_group = PreferredJoysticksGroup(self)
        self.layout.add(self.pref_group, fill=True)

        self.layout.add_spacer(20)

        from .JoystickSettingsGroup import JoystickSettingsGroup
        self.joystick_settings_group = JoystickSettingsGroup(self)
        self.layout.add(self.joystick_settings_group, fill=True)

