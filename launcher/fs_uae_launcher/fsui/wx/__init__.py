from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

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
