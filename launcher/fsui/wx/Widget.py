from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .dc import Font


class Widget(object):

    def __init__(self):
        self.layout = None
        self.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)

    def on_destroy(self):
        pass

    def __destroy_event(self, event):
        # for function in self.destroy_listeners:
        #     function()
        self.on_destroy()

    def get_font(self):
        return Font(self.GetFont())

    def get_parent(self):
        return self.GetParent()

    def is_enabled(self):
        return self.IsEnabled()

    def enable(self, enable=True):
        self.Enable(enable)

    def disable(self):
        self.Enable(False)

    def show(self, show=True):
        self.Show(show)

    def show_or_hide(self, show):
        self.Show(show)

    def hide(self):
        self.Show(False)

    def is_visible(self):
        return self.IsShown()

    def set_hand_cursor(self):
        import wx
        self.SetCursor(wx.StockCursor(wx.CURSOR_HAND))

    def set_default_cursor(self):
        import wx
        self.SetCursor(wx.StockCursor(wx.CURSOR_DEFAULT))

    def set_position_and_size(self, position, size):
        self.SetDimensions(position[0], position[1], size[0], size[1])
        #self.SetPosition(position)
        #self.SetSize(size)
        #self.set_position(position)
        #self.set_size(size)
        if hasattr(self, "layout") and self.layout is not None:
            #self.layout.set_position_and_size((0, 0))
            #self.layout.set_position_and_size(size)
            self.layout.set_size(size)

    def measure_text(self, text):
        return self.GetTextExtent(text)

    def set_size(self, size):
        self.SetSize(size)

    def set_min_width(self, width):
        self.min_width = width

    def set_min_height(self, height):
        self.min_height = height

    def get_min_width(self):
        width = 0
        if hasattr(self, "min_width"):
            if self.min_width:
                width = max(self.min_width, width)
        if hasattr(self, "layout") and self.layout is not None:
            #return self.layout.get_min_width()
            width = max(self.layout.get_min_width(), width)
            return width
        return max(width, self.GetBestSize()[0])
        #return self.GetBestSize()[0]

    def get_min_height(self):
        height = 0
        if hasattr(self, "min_height"):
            if self.min_height:
                height = max(self.min_height, height)
        if hasattr(self, "layout") and self.layout is not None:
            height = max(self.layout.get_min_height(), height)
            return height
        return max(height, self.GetBestSize()[1])

    def focus(self):
        self.SetFocus()

    def get_window(self):
        parent = None
        while self.parent:
            parent = self.parent
        return parent

    def refresh(self):
        self.Refresh()

    def get_background_color(self):
        from .Color import Color
        c= self.GetBackgroundColour()
        return Color(c.Red(), c.Blue(), c.Green())

    def set_background_color(self, color):
        import wx
        self.SetBackgroundColour(wx.Colour(*color))

    def set_tooltip(self, text):
        import wx
        self.SetToolTip(wx.ToolTip(text))

    def popup_menu(self, menu, pos=(0, 0)):
        self.PopupMenu(menu._menu, pos)
