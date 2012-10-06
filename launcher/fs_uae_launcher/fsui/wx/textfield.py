from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class TextField(wx.TextCtrl):
    def __init__(self, parent, text="", read_only=False):
        style = wx.TE_PROCESS_ENTER
        if read_only:
            style = style | wx.TE_READONLY
        wx.TextCtrl.__init__(self, parent.get_container(), -1, text,
                wx.DefaultPosition, wx.DefaultSize, style)
        self.Bind(wx.EVT_KEY_DOWN, self.__key_down_event)
        self.Bind(wx.EVT_TEXT, self.__text_event)

    def get_min_width(self):
        return self.GetBestSize()[0]

    def get_min_height(self):
        return self.GetBestSize()[1]

    def set_position(self, position):
        self.SetPosition(position)

    def set_size(self, size):
        self.SetSize(size)

    def focus(self):
        self.SetFocus()

    def on_activate(self):
        pass

    def on_change(self):
        pass

    def get_text(self):
        return self.GetValue()

    def set_text(self, text):
        self.SetValue(text)

    def __key_down_event(self, event):
        if event.GetKeyCode() == wx.WXK_RETURN:
            self.on_activate()
        else:
            event.Skip()

    def __text_event(self, event):
        self.on_change()

update_class(TextField)
