from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from .ConfigCheckBox import ConfigCheckBox
from ..HelpButton import HelpButton

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

        hor2_layout = fsui.HorizontalLayout()
        widget = ConfigCheckBox(self, _("Picasso96 Support"),
                "uaegfx_card")
        widget.set_tooltip(_("Picasso96 Support (uaegfx.card)"))
        hor2_layout.add(widget, expand=True)
        widget = HelpButton(self,
                "http://fengestad.no/fs-uae/options#uaegfx-card")
        hor2_layout.add(widget, margin_left=10)
        vert_layout.add(hor2_layout, fill=True, margin=10)

        #vert_layout = fsui.VerticalLayout()
        #hori_layout.add(vert_layout, fill=True, expand=True)

        hor2_layout = fsui.HorizontalLayout()
        widget = ConfigCheckBox(self, _("Built-in TCP/IP Stack"),
                "bsdsocket_library")
        widget.set_tooltip(_("Built-in TCP/IP Stack (bsdsocket.library)"))
        hor2_layout.add(widget, expand=True)
        widget = HelpButton(self,
                "http://fengestad.no/fs-uae/options#bsdsocket-library")
        hor2_layout.add(widget, margin_left=10)
        vert_layout.add(hor2_layout, fill=True, margin=10)
