from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from ..common.element import Element

class WXElement(Element):
    def __init__(self, parent, title=""):
        Element.__init__(self, parent)
        self._window = None
        #self._container = None

    def get_position_base(self):
        return (0, 0)

    def get_position(self):
        return self._window.GetPos()

    def set_position(self, position):
        print("--- set position", position)
        return self._window.SetPosition(position)

    def get_size(self):
        return self._window.GetSize()

    def get_min_size(self):
        print("returning", self._window.GetBestSize())
        return self._window.GetBestSize()

    def set_size(self, size):
        self._window.SetSize(size)
        Element.set_size(self, size)
