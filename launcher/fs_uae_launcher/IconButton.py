import fsui

class IconButton(fsui.ImageButton):
    def __init__(self, parent, name):
        image = fsui.Image("fs_uae_launcher:res/" + name)
        fsui.ImageButton.__init__(self, parent, image)
        self.set_min_width(40)
