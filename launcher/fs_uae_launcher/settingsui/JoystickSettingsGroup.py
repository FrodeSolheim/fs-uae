from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sys
import subprocess
import fs_uae_launcher.fsui as fsui
from ..IconButton import IconButton
from ..I18N import _, ngettext

class JoystickSettingsGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20

        image = fsui.Image("fs_uae_launcher:res/joystick.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        heading = _("Joystick Configuration")
        label = fsui.HeadingLabel(self, heading)
        self.layout2.add(label)

        self.layout2.add_spacer(10)

        self.gamepad_config_button = fsui.Button(self,
                _("Configure Joystick Buttons"))
        self.gamepad_config_button.set_tooltip(
                _("Open Tool to Configure Button Mapping"))
        self.gamepad_config_button.on_activate = self.on_gamepad_config_button
        self.layout2.add(self.gamepad_config_button)

        self.layout.add_spacer(10)

    def on_gamepad_config_button(self):
        args = [sys.executable] + sys.argv[:]
        args.append("--joystick-config")
        print("start gamepad config, args =", args)
        subprocess.Popen(args, close_fds=True)
