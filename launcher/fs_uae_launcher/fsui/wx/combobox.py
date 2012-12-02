from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

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

    def get_index(self):
        return self.GetSelection()

    def set_index(self, index):
        self.SetSelection(index)

    def set_items(self, items):
        print("setting items", items)
        self.Clear()
        for item in items:
            self.Append(item)

    def set_item_text(self, index, text):
        self.SetString(index, text)

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
