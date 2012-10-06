from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class Choice(wx.Choice):

    def __init__(self, parent, items=[]):
        wx.Choice.__init__(self, parent.get_container(), -1,
                wx.DefaultPosition, wx.DefaultSize, items)
        if len(items) > 0:
            self.SetSelection(0)
        self.Bind(wx.EVT_CHOICE, self.__choice_event)
        #self.min_height = 26

    #def get_min_width(self):
    #    return self.GetBestSize()[0]

    #def get_min_height(self):
    #    return self.GetBestSize()[1]

    def set_position(self, position):
        self.SetPosition(position)

    def set_size(self, size):
        self.SetSize(size)

    def get_index(self):
        return self.GetSelection()

    def set_index(self, index):
        self.SetSelection(index)

    def on_change(self):
        print("Choice.on_change")

    def __choice_event(self, event):
        self.on_change()

update_class(Choice)
