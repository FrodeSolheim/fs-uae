from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..FloppiesGroup import FloppiesGroup
from ..MediaListGroup import MediaListGroup
from ..I18N import _, ngettext
from .Skin import Skin

class FloppiesPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        self.layout = fsui.VerticalLayout()

        self.floppies_group = FloppiesGroup(self, 4)

        self.media_list_group = MediaListGroup(self, False)

        self.layout.add(self.floppies_group, fill=True)
        #self.add_separator()
        self.layout.add(self.media_list_group, expand=True, fill=True)

#        hori_layout = fsui.HorizontalLayout()
#        hori_layout.add_spacer(0, expand=True)
#
#        self.multi_select_button = fsui.Button(self, _("Select Files..."))
#        self.multi_select_button.set_tooltip(
#                _("Add Multiple Floppies at Once"))
#        self.multi_select_button.on_activate = self.on_multi_select_button
#        #self.add_button(self.multi_select_button)
#
#        self.layout.add(hori_layout, fill=True)
#        hori_layout.add(self.multi_select_button, margin=10)
#
#    def on_multi_select_button(self):
#        from ..FloppyManager import FloppyManager
#        FloppyManager.multiselect(self.get_window())
