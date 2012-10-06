from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ..RemovableMediaGroup import RemovableMediaGroup
from .config.InputGroup import InputGroup
from .config.ModelGroup import ModelGroup
from .ConfigGroup import ConfigGroup
from .Skin import Skin

class MainPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        #self.set_background_color((0xcc, 0x00, 0x00))
        self.layout = fsui.VerticalLayout()

        self.config_group = ConfigGroup(self)
        self.model_group = ModelGroup(self)
        self.removable_media_group = RemovableMediaGroup(self, 2)
        self.input_group = InputGroup(self)

        self.layout.add(self.config_group, fill=True)

        self.layout.add(self.model_group, fill=True)
        #self.separator(parent)
        self.layout.add(self.removable_media_group, fill=True)
        #self.separator(parent)
        self.layout.add(self.input_group, fill=True)
        #self.separator(parent)
        #parent.layout.add_spacer(20)
        #self.separator(parent)

        #self.layout.add_spacer(20)
        #parent.layout.add_spacer(20, expand=1)
        #self.layout.add_spacer(0, expand=1)

        #layout = fsui.HorizontalLayout()
        #parent.layout.add(layout, fill=True)
