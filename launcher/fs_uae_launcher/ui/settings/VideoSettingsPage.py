from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from .OptionUI import OptionUI

class VideoSettingsPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()
        self.layout.padding_left = 10
        self.layout.padding_right = 10
        self.layout.padding_top = 10
        self.layout.padding_bottom = 20

        label = fsui.HeadingLabel(self, _("Video Settings"))
        self.layout.add(label, margin=10)

        def add_option(name):
            self.layout.add(OptionUI.create_group(self, name), fill=True,
                    margin=10)

        add_option("fullscreen")
        add_option("fsaa")
        add_option("keep_aspect")
        add_option("zoom")
        add_option("video_sync")
        add_option("video_sync_method")
