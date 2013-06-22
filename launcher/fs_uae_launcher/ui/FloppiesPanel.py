from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from .FloppiesGroup import FloppiesGroup
from .MediaListGroup import MediaListGroup
from .Skin import Skin


class FloppiesPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        self.floppies_group = FloppiesGroup(self, 4)
        self.media_list_group = MediaListGroup(self, False)

        self.layout = fsui.VerticalLayout()
        self.layout.add(self.floppies_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        self.layout.add(self.media_list_group, expand=True, fill=True)
