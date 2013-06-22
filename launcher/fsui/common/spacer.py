from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .element import LightElement

class Spacer(LightElement):

    def __init__(self, size, size2=None):
        LightElement.__init__(self, None)
        self.width = size
        if size2 is None:
            self.height = size
        else:
            self.height = size2

    def get_min_size(self):
        return (self.width, self.height)

    def get_min_width(self):
        return self.width

    def get_min_height(self):
        return self.height
