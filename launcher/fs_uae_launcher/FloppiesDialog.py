from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from .Config import Config
from .IconButton import IconButton
from .GroupsDialog import GroupsDialog
from .I18N import _, ngettext

class FloppiesDialog(GroupsDialog):

    def __init__(self, parent):
        GroupsDialog.__init__(self, parent, _("Configure Floppies"), 600)

        from .FloppiesGroup import FloppiesGroup
        self.floppies_group = FloppiesGroup(self, 4)

        from .MediaListGroup import MediaListGroup
        self.media_list_group = MediaListGroup(self, False)

        self.add_group(self.floppies_group)
        self.add_separator()
        self.add_group(self.media_list_group)

        self.multi_select_button = fsui.Button(self, _("Select Files..."))
        self.multi_select_button.set_tooltip(
                _("Add Multiple Floppies at Once"))
        self.multi_select_button.on_activate = self.on_multi_select_button
        self.add_button(self.multi_select_button)

        self.add_close_button()
        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

    def on_multi_select_button(self):
        from .FloppyManager import FloppyManager
        FloppyManager.multiselect(self.get_window())
