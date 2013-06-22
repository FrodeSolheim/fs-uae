from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import hashlib
import fsui as fsui
from ..I18N import _, ngettext
from .SetupPanel import SetupPanel

class SetupDialog(fsui.Dialog):

    def __init__(self, parent):
        fsui.Dialog.__init__(self, parent, _("Import Kickstarts"))
        self.layout = fsui.VerticalLayout()
        self.layout.padding_top = 10
        self.layout.padding_bottom = 10
        self.layout.padding_left = 10
        self.layout.padding_right = 10

        self.panel = SetupPanel(self)
        self.panel.set_min_width(620)
        self.layout.add(self.panel)
        self.layout.add_spacer(10)

        hori_layout = fsui.HorizontalLayout()
        hori_layout.add_spacer(10, expand=True)
        self.layout.add(hori_layout, fill=True)
        self.close_button = fsui.Button(self, _("Close"))
        self.close_button.on_activate = self.on_close_button
        hori_layout.add(self.close_button)
        hori_layout.add_spacer(10)

        self.layout.add_spacer(10)
        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

    def on_close_button(self):
        self.end_modal(False)
