from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

def call_after(function):
    import wx
    wx.CallAfter(function)

def call_later(ms, function):
    import wx
    wx.CallLater(ms, function)

def show_error(message):
    import wx
    wx.MessageBox(message, "Error", wx.ICON_ERROR)

def get_screen_size():
    import wx
    return wx.GetDisplaySize()

def get_background_color():
    import wx
    c = wx.SystemSettings_GetColour(wx.SYS_COLOUR_BTNFACE)
    from .Color import Color
    return Color(c)
