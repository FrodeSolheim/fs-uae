from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext
from .config.KickstartGroup import KickstartGroup
from .Skin import Skin

class HardwarePanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        self.layout = fsui.VerticalLayout()

        self.kickstart_group = KickstartGroup(self)

        self.layout.add(self.kickstart_group, fill=True)
