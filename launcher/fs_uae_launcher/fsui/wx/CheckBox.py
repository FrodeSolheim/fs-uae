from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class CheckBox(wx.CheckBox):
    def __init__(self, parent, text=""):
        wx.CheckBox.__init__(self, parent.get_container(), -1, text,
                wx.DefaultPosition, wx.DefaultSize)
        self.Bind(wx.EVT_CHECKBOX, self.__checkbox_event)

    def on_change(self):
        pass

    def is_checked(self):
        return self.GetValue()

    def check(self, checked=True):
        self.SetValue(checked)

    def __checkbox_event(self, event):
        self.on_change()

update_class(CheckBox)
