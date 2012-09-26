from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from .common import update_class

#try:
#    from agw import ultimatelistctrl as ulc
#except ImportError: # if it's not there locally, try the wxPython lib.
#    from wx.lib.agw import ultimatelistctrl as ulc

from .lib import ultimatelistctrl as ulc

"""
class UltimateListLineData(ulc.UltimateListLineData):

    #def DrawVistaRectangle(self, dc, rect, hasfocus):
    #    #if rect[0] == 1:
    #    #    rect[0] = 0
    #    print(rect)
    #    dc.DrawRectangle(*rect)
    pass
"""
ulc.HEADER_OFFSET_X = 0
"""
ulc.UltimateListLineData = UltimateListLineData

def fix_renderer():

    class RendererNative(wx.RendererNative):

        @staticmethod
        def Get():
            renderer = wx.RendererNativeOrg.Get()
            if hasattr(renderer, "DrawItemSelectionRect_org"):
                return
            renderer.DrawItemSelectionRect_org = renderer.DrawItemSelectionRect

            def DrawItemSelectionRect(win, dc, rect, flags):
                #print("...")
                renderer.DrawItemSelectionRect_org(win, dc, rect,
                        flags & ~wx.CONTROL_CURRENT)

            renderer.DrawItemSelectionRect = DrawItemSelectionRect
            return renderer

    wx.RendererNativeOrg = wx.RendererNative
    wx.RendererNative = RendererNative
"""

class ListView(ulc.UltimateListCtrl):

    def __init__(self, parent):
        #fix_renderer()

        awg_style = wx.LC_REPORT | ulc.ULC_VIRTUAL
        awg_style = awg_style | wx.LC_SINGLE_SEL
        awg_style = awg_style | ulc.ULC_NO_HEADER

        style = 0
        style = style | wx.SUNKEN_BORDER

        #style = style | wx.LC_NO_HEADER
        ulc.UltimateListCtrl.__init__(self, parent.get_container(), -1,
                wx.DefaultPosition, wx.DefaultSize, style=style,
                agwStyle=awg_style)
        #self.Bind(wx.EVT_KEY_DOWN, self.__key_down_event)
        #self.Bind(wx.EVT_TEXT, self.__text_event)
        self.InsertColumn(0, "")
        self.SetColumnWidth(0, ulc.ULC_AUTOSIZE_FILL)

        self.Bind(ulc.EVT_LIST_ITEM_SELECTED, self.__list_item_selected)
        self.Bind(ulc.EVT_LIST_ITEM_ACTIVATED, self.__list_item_activated)
        #self.EnableSelectionVista()
        self.__index = 0
        self.__items = []

    def __list_item_selected(self, event):
        self.__index = event.GetIndex()
        self.on_select_item()

    def __list_item_activated(self, event):
        self.__index = event.GetIndex()
        self.on_activate_item()

    def on_select_item(self):
        pass

    def on_activate_item(self):
        pass

    def get_index(self):
        if self.__index >= self.GetItemCount():
            return -1
        if self.GetItemState(self.__index, ulc.ULC_STATE_SELECTED) & \
                ulc.ULC_STATE_SELECTED:
            return self.__index
        return -1

    def set_index(self, index):
        self.SetItemState(index, ulc.ULC_STATE_SELECTED, ulc.ULC_STATE_SELECTED)

    def get_item(self, index):
        return self.__items[index]

    def get_item_count(self):
        return self.GetItemCount()

    def set_items(self, items):
        self.__items = list(items)
        self.set_item_count(len(self.__items))

    def set_item_count(self, count):
        self.SetItemCount(count)

    def on_get_item_text(self, row, column):
        return self.__items[row]

    def on_get_item_tooltip(self, row, column):
        return ""

    #def OnGetItemToolTip(self, row, column):
    #    pass

    def OnGetItemText(self, row, column):
        #return u"   " + self.on_get_item_text(row, column)
        #print("f", id(wx.RendererNative_Get()))
        return self.on_get_item_text(row, column)

    def OnGetItemTextColour(self, item, col):
        return None

    def OnGetItemToolTip(self, item, col):
        return self.on_get_item_tooltip(item, col)

    def OnGetItemImage(self, item):
        return []

update_class(ListView)
