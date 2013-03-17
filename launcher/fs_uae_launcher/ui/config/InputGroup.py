from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import subprocess
import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...DeviceManager import DeviceManager
from ...I18N import _, ngettext
from ...Settings import Settings
from ..IconButton import IconButton
from .InputSelector import InputSelector

class InputGroup(fsui.Group):

    def __init__(self, parent, with_more_options=True,
                 parallel_ports=False, custom_ports=False):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        if parallel_ports:
            heading = _("Parallel Port Joysticks")
        elif custom_ports:
            heading = _("Custom Joystick Port")
        else:
            heading = _("Mouse & Joystick Ports")
            if False:
                # Keeping the old string here to keep the translations alive
                _("Joystick Ports")


        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        heading_label = fsui.HeadingLabel(self, heading)
        hori_layout.add(heading_label, margin=10)
        hori_layout.add_spacer(0, expand=True)

        if with_more_options:
            self.refresh_button = IconButton(self, "refresh_button.png")
            self.refresh_button.set_tooltip(
                    _("Refresh list of connected joystick devices"))
            self.refresh_button.on_activate = self.on_refresh_button
            hori_layout.add(self.refresh_button, margin_right=10)

        self.layout.add_spacer(0)

        self.selectors = []
        offset = 0
        count = 2
        if parallel_ports:
            offset = 2
        elif custom_ports:
            offset = 4
            count = 1
        for i in range(count):
            #self.layout.add_spacer(10)
            selector = InputSelector(self, i + offset)
            self.layout.add(selector, fill=True, margin=10)

    def on_refresh_button(selfself):
        DeviceManager.refresh()
