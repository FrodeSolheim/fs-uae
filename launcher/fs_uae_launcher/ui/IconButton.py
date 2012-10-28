from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui

class IconButton(fsui.ImageButton):
    def __init__(self, parent, name):
        image = fsui.Image("fs_uae_launcher:res/" + name)
        fsui.ImageButton.__init__(self, parent, image)
        self.set_min_width(40)

    def set_icon_name(self, name):
        image = fsui.Image("fs_uae_launcher:res/" + name)
        self.set_image(image)
