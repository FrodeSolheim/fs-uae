from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from .common import update_class

class TextArea(wx.TextCtrl):
    def __init__(self, parent, text="", read_only=False, horizontal_scroll=False):
        style = wx.TE_MULTILINE
        if read_only:
            style = style | wx.TE_READONLY
        if horizontal_scroll:
            style = style | wx.HSCROLL
        wx.TextCtrl.__init__(self, parent.get_container(), -1, text,
                wx.DefaultPosition, wx.DefaultSize, style)

    #def get_min_width(self):
    #    return self.GetBestSize()[0]

    #def get_min_height(self):
    #    return self.GetBestSize()[1]

    def set_position(self, position):
        self.SetPosition(position)

    def set_size(self, size):
        self.SetSize(size)

    def get_text(self):
        return self.GetValue()

    def set_text(self, text):
        self.SetValue(text)

    def append_text(self, text):
        self.AppendText(text)

update_class(TextArea)
