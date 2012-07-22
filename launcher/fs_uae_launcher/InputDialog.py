from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .Separator import Separator
from .I18N import _, ngettext

class InputDialog(fsui.Dialog):

    def __init__(self, parent):
        fsui.Dialog.__init__(self, parent, _("Configure Input"))
        self.layout = fsui.VerticalLayout()
        self.layout.add_spacer(540, 0)
        self.layout.padding_top = 20
        self.layout.padding_right = 20
        self.layout.padding_bottom = 20

        from .InputGroup import InputGroup
        self.input_group = InputGroup(self, with_more_options=False)
        self.layout.add(self.input_group, fill=True)

        self.layout.add(Separator(self), fill=True)

        self.input_group = InputGroup(self, with_more_options=False,
                parallel_ports=True)
        self.layout.add(self.input_group, fill=True)

        self.layout.add(Separator(self), fill=True)

        from .PreferredJoysticksGroup import PreferredJoysticksGroup
        self.pref_group = PreferredJoysticksGroup(self)
        self.layout.add(self.pref_group, fill=True)

        self.layout.add_spacer(20)
        self.layout.add_spacer(20)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True)

        hor_layout.add_spacer(0, expand=True)

        self.close_button = fsui.Button(self, _("Close"))
        self.close_button.on_activate = self.on_close_button
        hor_layout.add(self.close_button)
        hor_layout.add_spacer(20)

        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

    def on_close_button(self):
        self.end_modal(False)
