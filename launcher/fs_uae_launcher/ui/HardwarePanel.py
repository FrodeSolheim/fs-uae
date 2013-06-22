from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext
from .config.KickstartGroup import KickstartGroup
from .config.MemoryGroup import MemoryGroup
from .config.ExpansionsGroup import ExpansionsGroup
from .Skin import Skin

class HardwarePanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        self.kickstart_group = KickstartGroup(self)
        self.memory_group = MemoryGroup(self)
        self.expansions_group = ExpansionsGroup(self)

        self.layout = fsui.VerticalLayout()
        self.layout.add(self.kickstart_group, fill=True)
        self.layout.add_spacer(10)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        self.layout.add(self.memory_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        self.layout.add(self.expansions_group, fill=True)
