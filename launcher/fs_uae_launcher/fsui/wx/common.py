#import sys
#macosx = (sys.platform == "darwin")

def get_parent(self):
    return self.GetParent()

def enable(self, enable=True):
    self.Enable(enable)

def disable(self):
    self.Enable(False)

def show(self, show=True):
    self.Show(show)

def hide(self):
    self.Show(False)

def set_position_and_size(self, position, size):
    self.SetDimensions(position[0], position[1], size[0], size[1])
    #self.SetPosition(position)
    #self.SetSize(size)
    #self.set_position(position)
    #self.set_size(size)
    if hasattr(self, "layout"):
        #self.layout.set_position_and_size((0, 0))
        #self.layout.set_position_and_size(size)
        self.layout.set_size(size)

def set_size(self, size):
    self.SetSize(size)

#def get_min_width(self):
#    return self.GetBestSize()[0]

#def get_min_height(self):
#    return self.GetBestSize()[1]

#def set_fixed_width(self, width):
#    self.min_width = width
#
#def set_fixed_height(self, height):
#    self.min_height = height

def set_min_width(self, width):
    self.min_width = width

def set_min_height(self, height):
    self.min_height = height

def get_min_width(self):
    width = 0
    if hasattr(self, "min_width"):
        if self.min_width:
            width = max(self.min_width, width)
    if hasattr(self, "layout"):
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
    if hasattr(self, "layout"):
        height = max(self.layout.get_min_height(), height)
        return height
    return max(height, self.GetBestSize()[1])

def focus(self):
    self.SetFocus()

def get_window(self):
    while self.parent:
        parent = self.parent
    return parent

def set_background_color(self, color):
    import wx
    self.SetBackgroundColour(wx.Colour(*color))

def set_tooltip(self, text):
    import wx
    self.SetToolTip(wx.ToolTip(text))

names = [
    "disable",
    "enable",
    "focus",
    "get_parent",
    "get_window",
    "get_min_height",
    "get_min_width",
    "hide",
    "set_background_color",
    "set_min_height",
    "set_min_width",
    "set_position_and_size",
    "set_size",
    "set_tooltip",
    "show",
]

def update_class(klass):
    for name in names:
        if not hasattr(klass, name):
            setattr(klass, name, globals()[name])
