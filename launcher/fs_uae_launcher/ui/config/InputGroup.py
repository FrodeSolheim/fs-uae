from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sys
import subprocess
import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...Settings import Settings
from ...IconButton import IconButton
from ...DeviceManager import DeviceManager
from ...I18N import _, ngettext
from .InputSelector import InputSelector

joystick_mode_values = ["nothing", "mouse", "joystick"]
joystick_mode_titles = [_("Nothing"), _("Mouse"), _("Joystick")]

joystick_values = ["none", "mouse", "keyboard"]

class InputGroup(fsui.Group):

    def __init__(self, parent, with_more_options=True,
                 parallel_ports=False):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        if parallel_ports:
            heading = _("Parallel Port Joysticks")
        else:
            heading = _("Joystick Ports")
        heading_label = fsui.HeadingLabel(self, heading)
        self.layout.add(heading_label, margin=10)
        self.layout.add_spacer(0)

        self.selectors = []
        offset = 0
        if parallel_ports:
            offset = 2
        for i in range(2):
            #self.layout.add_spacer(10)
            selector = InputSelector(self, i + offset)
            self.layout.add(selector, fill=True, margin=10)
