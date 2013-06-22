from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsui import ImageButton, Image


class IconButton(ImageButton):

    BUTTON_WIDTH = 40

    def __init__(self, parent, name):
        image = Image("fs_uae_launcher:res/" + name)
        ImageButton.__init__(self, parent, image)
        self.set_min_width(self.BUTTON_WIDTH)

    def set_icon_name(self, name):
        image = Image("fs_uae_launcher:res/" + name)
        self.set_image(image)
