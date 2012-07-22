from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .FloppyManager import FloppyManager
from .I18N import _, ngettext

class FloppiesGroup(fsui.Group):

    def __init__(self, parent, drives=2, cd_mode=False):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20

        if cd_mode:
            image = fsui.Image("fs_uae_launcher:res/cd_group.png")
        else:
            image = fsui.Image("fs_uae_launcher:res/floppy_group.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        if cd_mode:
            self.label = fsui.HeadingLabel(self, _("CD-ROM Drive"))
        else:
            self.label = fsui.HeadingLabel(self, _("Floppy Drives"))
        self.layout2.add(self.label)

        self.selectors = []
        for i in range(drives):
            self.layout2.add_spacer(10)
            from .FloppySelector import FloppySelector
            selector = FloppySelector(parent, i)
            if cd_mode:
                selector.set_cd_mode(True)
            self.selectors.append(selector)
            self.layout2.add(selector, fill=True)
