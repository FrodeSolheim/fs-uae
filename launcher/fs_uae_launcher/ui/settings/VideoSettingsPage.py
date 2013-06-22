from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
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
        self.layout.add(label, margin=10, margin_bottom=20)

        def add_option(name):
            self.layout.add(OptionUI.create_group(self, name), fill=True,
                    margin=10)

        add_option("fullscreen")
        #add_option("zoom")
        #add_option("keep_aspect")
        #add_option("scanlines")
        #add_option("rtg_scanlines")
        add_option("video_format")
        add_option("video_sync")
        add_option("low_latency_vsync")

        label = fsui.HeadingLabel(self, _("OpenGL Settings"))
        self.layout.add(label, margin=10, margin_top=20, margin_bottom=20)

        add_option("fsaa")
        add_option("texture_filter")
        add_option("video_sync_method")
        add_option("texture_format")
