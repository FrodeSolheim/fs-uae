from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


#def xx_on_resize(self):
#    if self.layout:
#        self.layout.set_size(self.get_size())
#        self.layout.update()

class Element:

    def __init__(self, parent, delay_create=False):
        self.parent = parent
        self.layout = None
        self.position = (0, 0)
        self.size = (0, 0)
        #if not delay_create:
        #    self.on_create()

    #@classmethod
    #def init(cls, obj):
    #    obj.on_resize = xx_on_resize

    def create(self):
        self.on_create()
        return self

    def get_position_base(self):
        base = self.get_position_base()
        pos = self.get_position()
        return (base[0] + pos[0], base[1] + pos[1])

    def on_create(self):
        pass

    def get_real_parent(self):
        return self

    def get_size(self):
        return self.size

    def get_min_size(self):
        return (0, 0)

    def get_position(self):
        return self.position

    def set_position(self, position):
        self.position = position

    def set_position_and_size(self, position, size):
        self.set_position(position)
        self.set_size(size)

    def set_size(self, size):
        self.size = size
        if self.layout:
            self.layout.set_size(size)
        self.on_resize()

    def on_resize(self):
        if self.layout:
            self.layout.set_size(self.get_size())
            self.layout.update()

class LightElement(Element):

    def __init__(self, parent):
        Element.__init__(self, parent)

    def get_real_parent(self):
        return self.parent.get_real_parent()
