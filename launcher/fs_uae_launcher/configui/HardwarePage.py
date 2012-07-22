from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from ..Separator import Separator
from ..I18N import _, ngettext

from .ModelGroup import ModelGroup
from .KickstartGroup import KickstartGroup

class HardwarePage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)

        self.model_group = ModelGroup(self, with_more_button=False)
        self.kickstart_group = KickstartGroup(self)

        self.layout = fsui.VerticalLayout()
        self.layout.add_spacer(20)
        self.layout.add(self.model_group, fill=True)
        self.layout.add(Separator(self), fill=True)
        self.layout.add(self.kickstart_group, fill=True)

        #self.add_group(self.model_group)
        #self.add_separator()
        #self.add_group(self.kickstart_group)

        self.layout.add_spacer(20)

