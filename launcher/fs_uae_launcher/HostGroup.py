from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .I18N import _, ngettext

class HostGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20

        image = fsui.Image("fs_uae_launcher:res/model.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        heading = _("Host Options")
        label = fsui.HeadingLabel(self, heading)
        self.layout2.add(label)

        #self.selectors = []
        #for i in range(2):
        #    self.layout2.add_spacer(10)
        #    selector = InputSelector(self, i)
        #    self.layout2.add(selector, fill=True)

        self.layout2.add_spacer(10)
        self.layout3 = fsui.HorizontalLayout()
        self.layout2.add(self.layout3, fill=True)

        #self.gamepad_config_button = fsui.Button(self,
        #        "Configure Joysticks")
        #self.gamepad_config_button.on_activate = self.on_gamepad_config_button
        #self.layout3.add(self.gamepad_config_button)
        #self.layout3.add_spacer(10)

        self.scan_button = fsui.Button(parent, _("Scan"))
        self.scan_button.on_activate = self.on_scan_button
        self.layout3.add(self.scan_button)
        self.layout3.add_spacer(10)

        self.layout3.add_spacer(0, expand=True)

        self.more_button = fsui.Button(self, _("More..."))
        self.more_button.disable()
        self.layout3.add(self.more_button)

    def on_gamepad_config_button(self):
        from .JoystickConfigDialog import JoystickConfigDialog
        dialog = JoystickConfigDialog(self.get_window())
        dialog.ShowModal()

    def on_scan_button(self):
        from .ScanDialog import ScanDialog
        dialog = ScanDialog(self.get_window())
        dialog.show_modal()
        dialog.destroy()
        Config.update_kickstart()
