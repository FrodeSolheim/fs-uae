from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from .common import update_class

class Panel(wx.Panel):
    def __init__(self, parent):
        # create it outside the visible area to avoid flickering on Windows
        # when the controls are displayed before they are moved to the
        # correct place
        wx.Panel.__init__(self, parent.get_container(), -1, (0, 2000))
        #p = parent.get_real_parent()
        #self._window = wx.Panel(p._container, -1)
        #self._container = self._window
        #self._window.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        self.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        self.position = (0, 2000)
        self.size = (0, 0)

    def get_window(self):
        return self.GetParent().get_window()

    def get_container(self):
        return self

    #def get_min_width(self):
    #    if self.layout:
    #        return self.layout.get_min_width()
    #    return self.GetBestSize()[0]

    #def get_min_height(self):
    #    if self.layout:
    #        return self.layout.get_min_height()
    #    return self.GetBestSize()[1]

    #def set_position_and_size(self, position, size):
    #    self.SetDimensions(position[0], position[1], size[0], size[1])
    #    if self.layout:
    #        #self.layout.set_position_and_size((0, 0))
    #        self.layout.set_size(size)

    def on_resize(self):
        print("Panel.on_resize")
        if self.layout:
            self.layout.set_size(self.get_size())
            self.layout.update()

    def on_destroy(self):
        pass

    def __destroy_event(self, event):
        self.on_destroy()

update_class(Panel)
