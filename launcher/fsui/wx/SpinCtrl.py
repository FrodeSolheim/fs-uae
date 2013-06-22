from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class SpinCtrl(wx.SpinCtrl):
    def __init__(self, parent, min_value, max_value, initial_value):
        wx.SpinCtrl.__init__(self, parent.get_container(), -1, "",
                wx.DefaultPosition, wx.DefaultSize, wx.SP_ARROW_KEYS,
                min_value, max_value, initial_value)
        self.Bind(wx.EVT_SPINCTRL, self.__spin_event)

    def on_change(self):
        pass

    def get_value(self):
        return self.GetValue()

    def set_value(self, value):
        self.SetValue(value)

    def __spin_event(self, event):
        self.on_change()

update_class(SpinCtrl)
