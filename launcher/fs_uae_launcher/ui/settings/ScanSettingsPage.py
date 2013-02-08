from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...I18N import _, ngettext
from ...Settings import Settings
from .ScanPathsGroup import ScanPathsGroup
from .OptionUI import OptionUI

class ScanSettingsPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()
        self.layout.padding_left = 10
        self.layout.padding_right = 10
        self.layout.padding_top = 10
        self.layout.padding_bottom = 20

        label = fsui.HeadingLabel(self,
                _("List of Folders to Index"))
        self.layout.add(label, margin=10)

        self.scan_paths_group = ScanPathsGroup(self)
        self.layout.add(self.scan_paths_group, fill=True)

        label = fsui.HeadingLabel(self, _("Additional Options"))
        self.layout.add(label, margin=10, margin_bottom=20)

        def add_option(name):
            self.layout.add(OptionUI.create_group(self, name), fill=True,
                    margin=10)

        add_option("builtin_configs")
        #add_option("kickstart_setup")
