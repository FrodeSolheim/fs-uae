from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
import pkg_resources

class Image:

    NEAREST = 0

    def __init__(self, name):
        index = name.find(":")
        if index > 1:
            package, file = name.split(":", 1)
            stream = pkg_resources.resource_stream(package, file)

            self.image = wx.ImageFromStream(stream)
            #self.bitmap = wx.BitmapFromImage(image)
        else:
            self.image = wx.Image(name)
            #self.bitmap = wx.Bitmap(name)

            #self.bitmap = wx.BitmapFromImage(image)

    @property
    def size(self):
        return (self.image.GetWidth(), self.image.GetHeight())

    @property
    def bitmap(self):
        return wx.BitmapFromImage(self.image)

    def resize(self, size, filter=1):
        if size == self.size:
            return
        if filter:
            q = wx.IMAGE_QUALITY_HIGH
        else:
            q = wx.IMAGE_QUALITY_NORMAL
        self.image.Rescale(size[0], size[1], q)

