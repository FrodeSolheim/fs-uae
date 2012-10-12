from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from .RemovableMediaGroup import RemovableMediaGroup
from .config.InputGroup import InputGroup
from .config.ModelGroup import ModelGroup
from .ConfigGroup import ConfigGroup
from .Skin import Skin

class MainPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)
        self.layout = fsui.VerticalLayout()

        self.config_group = ConfigGroup(self)
        self.model_group = ModelGroup(self)
        self.removable_media_group = RemovableMediaGroup(self, 2)
        self.input_group = InputGroup(self)

        self.layout.add(self.config_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)

        self.layout.add(self.model_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)

        self.layout.add(self.removable_media_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)

        self.layout.add(self.input_group, fill=True)
