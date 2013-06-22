from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .Label import Label


class HeadingLabel(Label):

    def __init__(self, parent, label):
        Label.__init__(self, parent, label)
        font = self.get_font()
        font.set_bold(True)
        self.set_font(font)
