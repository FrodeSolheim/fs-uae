import fsui

class Separator(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20
        self.layout.padding_top = 14
        self.layout.padding_bottom = 14
        self.separator = fsui.Separator(self)
        self.layout.add(self.separator, expand=True)
