from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class Separator(wx.StaticLine):

    def __init__(self, parent):
        wx.StaticLine.__init__(self, parent.get_container(), -1,
                wx.DefaultPosition, wx.DefaultSize, wx.LI_HORIZONTAL)

update_class(Separator)
