from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx

class DirDialog():
    def __init__(self, parent, message="", directory=""):
        style = wx.DD_DEFAULT_STYLE
        #if multiple:
        #    style |= wx.FD_MULTIPLE
        self._window = wx.DirDialog(parent, message, directory, style)
        self._window.CenterOnParent()

    def get_path(self):
        return self._window.GetPath()

    def show_modal(self):
        if self._window.ShowModal() == wx.ID_OK:
            return True
        return False

    def destroy(self):
        self._window.Destroy()
