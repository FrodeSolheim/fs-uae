from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class ItemView(wx.ScrolledWindow):
    def __init__(self, parent):
        wx.ScrolledWindow.__init__(self, parent.get_container(), -1,
                wx.DefaultPosition, wx.DefaultSize, wx.SUNKEN_BORDER)
        self.SetBackgroundColour(wx.WHITE)
        self.SetScrollbars(1, 1, 1, 1)
        self.SetVirtualSize((0, 1000))

update_class(ItemView)
