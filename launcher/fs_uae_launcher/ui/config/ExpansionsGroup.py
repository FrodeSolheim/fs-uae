from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from .ConfigCheckBox import ConfigCheckBox

class ExpansionsGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        heading_label = fsui.HeadingLabel(self, _("Expansions"))
        self.layout.add(heading_label, margin=10)
        self.layout.add_spacer(0)
        
        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        vert_layout = fsui.VerticalLayout()
        hori_layout.add(vert_layout, fill=True, expand=True)

        widget = ConfigCheckBox(self, "uaegfx.card (Picasso 96)",
                "uaegfx_card")
        vert_layout.add(widget, fill=True, margin=10)

        vert_layout = fsui.VerticalLayout()
        hori_layout.add(vert_layout, fill=True, expand=True)

        widget = ConfigCheckBox(self, "bsdsocket.library (Networking)",
                "bsdsocket_library")
        if fsui.System.windows:
            widget.disable()
        vert_layout.add(widget, fill=True, margin=10)
