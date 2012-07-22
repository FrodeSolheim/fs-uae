from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from .Config import Config
from .IconButton import IconButton
from .GroupsDialog import GroupsDialog
from .I18N import _, ngettext

class HardDrivesDialog(GroupsDialog):

    def __init__(self, parent):
        GroupsDialog.__init__(self, parent, _("Configure Hard Drives"), 600)

        """
        from .FloppiesGroup import FloppiesGroup
        self.cd_group = FloppiesGroup(self, 1, cd_mode=True)

        from .MediaListGroup import MediaListGroup
        self.media_list_group = MediaListGroup(self, True)

        self.add_group(self.cd_group)
        self.add_separator()
        self.add_group(self.media_list_group)

        # FIXME: remove
        _("Select Multiple CD-ROMs")
        self.multi_select_button = fsui.Button(self, _("Select Files..."))
        self.multi_select_button.on_activate = self.on_multi_select_button
        self.add_button(self.multi_select_button)
        """

        self.add_close_button()
        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

    #def on_multi_select_button(self):
    #    from .CDManager import CDManager
    #    CDManager.multiselect(self.get_window())
