from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import hashlib
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Separator import Separator
from ..I18N import _, ngettext
from .WHDLoadGroup import WHDLoadGroup
from .HardDriveGroup import HardDriveGroup

class HardDrivesPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)

        self.whdload_group = WHDLoadGroup(self)
        self.hard_drive_groups = []
        for i in range(4):
            self.hard_drive_groups.append(HardDriveGroup(self, i))

        self.layout = fsui.VerticalLayout()
        self.layout.add_spacer(20)
        for i in range(4):
            self.layout.add(self.hard_drive_groups[i], fill=True)
            self.layout.add(Separator(self), fill=True)
        self.layout.add(self.whdload_group, fill=True)

        self.layout.add_spacer(20)

