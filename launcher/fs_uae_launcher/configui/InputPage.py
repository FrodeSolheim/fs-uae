from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..Separator import Separator
from ..I18N import _, ngettext

class InputPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()
        self.layout.padding_top = 20
        #self.layout.padding_right = 20
        self.layout.padding_bottom = 20

        from .InputGroup import InputGroup
        self.input_group = InputGroup(self, with_more_options=False)
        self.layout.add(self.input_group, fill=True)

        self.layout.add(Separator(self), fill=True)

        self.input_group = InputGroup(self, with_more_options=False,
                parallel_ports=True)
        self.layout.add(self.input_group, fill=True)

        #self.layout.add(Separator(self), fill=True)

        #from ..PreferredJoysticksGroup import PreferredJoysticksGroup
        #self.pref_group = PreferredJoysticksGroup(self)
        #self.layout.add(self.pref_group, fill=True)
