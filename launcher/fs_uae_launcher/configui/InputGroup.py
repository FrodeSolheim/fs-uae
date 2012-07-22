from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sys
import subprocess
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..IconButton import IconButton
from ..DeviceManager import DeviceManager
from ..I18N import _, ngettext
from .InputSelector import InputSelector

joystick_mode_values = ["nothing", "mouse", "joystick"]
joystick_mode_titles = [_("Nothing"), _("Mouse"), _("Joystick")]

joystick_values = ["none", "mouse", "keyboard"]

class InputGroup(fsui.Group):

    def __init__(self, parent, with_more_options=True,
                 parallel_ports=False):
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

        if parallel_ports:
            heading = _("Parallel Port Joysticks")
        else:
            heading = _("Joystick Ports")
        label = fsui.HeadingLabel(self, heading)
        self.layout2.add(label)

        self.selectors = []
        offset = 0
        if parallel_ports:
            offset = 2
        for i in range(2):
            self.layout2.add_spacer(10)
            selector = InputSelector(self, i + offset)
            self.layout2.add(selector, fill=True)

        """
        if with_more_options:
            self.layout2.add_spacer(10)
            self.layout3 = fsui.HorizontalLayout()
            self.layout2.add(self.layout3, fill=True)

            self.layout3.add_spacer(0, expand=True)

            self.refresh_button = IconButton(self, "refresh_button.png")
            self.refresh_button.set_tooltip(_("Refresh Joystick List"))
            self.refresh_button.on_activate = self.on_refresh_button
            self.refresh_button.disable()
            self.layout3.add(self.refresh_button, fill=True)

            self.layout3.add_spacer(10)

            self.gamepad_config_button = fsui.Button(self,
                    _("Configure Joysticks"))
            self.gamepad_config_button.set_tooltip(
                    _("Open Tool to Configure Button Mapping"))
            self.gamepad_config_button.on_activate = self.on_gamepad_config_button
            self.layout3.add(self.gamepad_config_button)

            self.layout3.add_spacer(10)

            self.more_button = fsui.Button(self, _("More..."))
            self.more_button.on_activate = self.on_more_button
            self.layout3.add(self.more_button)
        """

    #def on_gamepad_config_button(self):
    #    args = [sys.executable] + sys.argv[:]
    #    args.append("--joystick-config")
    #    print("start gamepad config, args =", args)
    #    subprocess.Popen(args, close_fds=True)

    #def on_more_button(self):
    #    from .ConfigDialog import ConfigDialog
    #    ConfigDialog.run(self.get_window(), ConfigDialog.INPUT)

    #def on_refresh_button(self):
    #    pass
