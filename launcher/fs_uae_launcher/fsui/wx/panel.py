from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class Panel(wx.Panel):
    def __init__(self, parent, paintable=False):
        # create it outside the visible area to avoid flickering on Windows
        # when the controls are displayed before they are moved to the
        # correct place
        wx.Panel.__init__(self, parent.get_container(), -1, (0, 2000),
                (0, 0), wx.FULL_REPAINT_ON_RESIZE)
        #p = parent.get_real_parent()
        #self._window = wx.Panel(p._container, -1)
        #self._container = self._window
        #self._window.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        self.Bind(wx.EVT_PAINT, self.__paint_event)
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.__erase_background_event)
        self.Bind(wx.EVT_LEFT_DOWN, self.__left_down_event)
        self.Bind(wx.EVT_LEFT_UP, self.__left_up_event)
        self.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        self.Bind(wx.EVT_ENTER_WINDOW, self.__enter_window_event)
        self.Bind(wx.EVT_LEAVE_WINDOW, self.__leave_window_event)
        self.position = (0, 2000)
        self._paintable = paintable
        self._paint_dc = None

    @property
    def size(self):
        return self.GetSizeTuple()

    def get_window(self):
        return self.GetParent().get_window()

    def get_container(self):
        return self

    def on_resize(self):
        print("Panel.on_resize")
        if self.layout:
            self.layout.set_size(self.get_size())
            self.layout.update()

    def on_destroy(self):
        pass

    def __destroy_event(self, event):
        self.on_destroy()

    def on_left_down(self):
        pass

    def __left_down_event(self, event):
        self.on_left_down()

    def on_left_up(self):
        pass

    def __left_up_event(self, event):
        self.on_left_up()

    def create_dc(self):
        from .dc import DrawingContext
        return DrawingContext(self._paint_dc)

    def on_paint(self):
        pass

    def __paint_event(self, event):
        if not self._paintable:
            #dc = wx.PaintDC(self)
            event.Skip()
            return
        self._paint_dc = wx.PaintDC(self)
        try:
            self.on_paint()
        finally:
            self._paint_dc = None

    def on_erase(self):
        return False

    def __erase_background_event(self, event):
        if not self._paintable:
            event.Skip()
        if self.on_erase():
            event.Skip()

    def on_mouse_enter(self):
        pass

    def __enter_window_event(self, event):
        self.on_mouse_enter()

    def on_mouse_leave(self):
        pass

    def __leave_window_event(self, event):
        self.on_mouse_leave()


update_class(Panel)
