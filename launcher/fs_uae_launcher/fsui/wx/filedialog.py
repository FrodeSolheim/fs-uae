from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx

class FileDialog():
    def __init__(self, parent, message="", directory="", file="",
            pattern="*.*", multiple=False):
        #if parent:
        #    p = parent.get_real_parent()
        #    parent = p._container
        style = wx.FD_DEFAULT_STYLE
        if multiple:
            style |= wx.FD_MULTIPLE
        self._window = wx.FileDialog(parent, message, directory, file,
                pattern, style)
        self._window.CenterOnParent()

    def get_path(self):
        return self._window.GetPath()

    def get_paths(self):
        return self._window.GetPaths()

    def show_modal(self):
        if self._window.ShowModal() == wx.ID_OK:
            return True
        return False

    def destroy(self):
        self._window.Destroy()

    # FIXME: remove
    def show(self):
        return self.show_modal()
