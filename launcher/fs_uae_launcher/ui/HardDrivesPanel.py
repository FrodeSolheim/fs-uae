from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext
from .config.WHDLoadGroup import WHDLoadGroup
from .config.HardDriveGroup import HardDriveGroup
from .Skin import Skin

class HardDrivesPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        self.hard_drive_groups = []
        for i in range(4):
            self.hard_drive_groups.append(HardDriveGroup(self, i))
        self.whdload_group = WHDLoadGroup(self)

        self.layout = fsui.VerticalLayout()
        for i in range(4):
            self.layout.add(self.hard_drive_groups[i], fill=True,
                    margin=10)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        self.layout.add(self.whdload_group, fill=True)
