from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from .common import update_class

class ComboBox(wx.ComboBox):

    def __init__(self, parent, items=[], read_only=False):
        style = 0
        if read_only:
            style = style | wx.CB_READONLY
        wx.ComboBox.__init__(self, parent.get_container(), -1, "",
                wx.DefaultPosition, wx.DefaultSize, items, style)
        if len(items) > 0:
            self.SetSelection(0)
        self.Bind(wx.EVT_COMBOBOX, self.__combobox_event)
        self.Bind(wx.EVT_TEXT, self.__text_event)
        #self.min_height = 26

    #def get_min_width(self):
    #    return self.GetBestSize()[0]

    #def get_min_height(self):
    #    return self.GetBestSize()[1]

    #def set_position(self, position):
    #    self.SetPosition(position)

    #def set_size(self, size):
    #    self.SetSize(size)

    def get_index(self):
        return self.GetSelection()

    def set_index(self, index):
        self.SetSelection(index)

    def get_text(self):
        return self.GetValue()

    def set_text(self, text):
        self.SetValue(text)

    def on_change(self):
        print("ComboBox.on_change")

    def __combobox_event(self, event):
        self.on_change()

    def __text_event(self, event):
        self.on_change()

update_class(ComboBox)
