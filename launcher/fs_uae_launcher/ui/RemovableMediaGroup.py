from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fsui as fsui
from fsgs.amiga.Amiga import Amiga
from ..Config import Config
from ..CDManager import CDManager
from ..FloppyManager import FloppyManager
from ..I18N import _, ngettext
from ..Settings import Settings
from .IconButton import IconButton
from .FloppiesGroup import FloppiesGroup

class RemovableMediaGroup(FloppiesGroup):

    def __init__(self, parent, drives):
        FloppiesGroup.__init__(self, parent, drives)
        self.layout3 = fsui.HorizontalLayout()
        self.layout.add(self.layout3, fill=True)

        self.layout3.add_spacer(0, expand=True)

        self.cd_mode = False

        self.update_media_type()
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def on_config(self, key, value):
        if key == "amiga_model":
            self.update_media_type()

    def update_media_type(self):
        self.set_cd_mode(Amiga.is_cd_based(Config))

    def set_cd_mode(self, cd_mode):
        if self.cd_mode == cd_mode:
            return
        self.cd_mode = cd_mode
        for selector in self.selectors:
            selector.set_cd_mode(cd_mode)
        self.update_heading_label()
        self.selectors[1].enable(not self.cd_mode)
