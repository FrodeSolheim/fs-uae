from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from fsbc.Resources import Resources


class Image:

    NEAREST = 0

    def __init__(self, name=None, wx_image=None):
        self._bitmap = None
        if wx_image:
            self._image = wx_image
            #self.bitmap = wx.BitmapFromImage(image)
        else:
            logger = wx.LogNull()
            index = name.find(":")
            if index > 1:
                package, file_ = name.split(":", 1)
                stream = Resources(package).stream(file_)
                self._image = wx.ImageFromStream(stream)
                #self.bitmap = wx.BitmapFromImage(image)
            elif name:
                self._image = wx.Image(name)
                #self.bitmap = wx.Bitmap(name)
            # not really needed, but included to make the point that the
            # object must be destroyed here
            del logger

    def grey_scale(self):
        return Image(wx_image=self._image.ConvertToGreyscale())

    @property
    def size(self):
        return self._image.GetWidth(), self._image.GetHeight()

    @property
    def bitmap(self):
        if self._bitmap is None:
            self._bitmap = wx.BitmapFromImage(self._image, -1)
        return self._bitmap

    def resize(self, size, filter=1):
        if size == self.size:
            return
        if filter:
            q = wx.IMAGE_QUALITY_HIGH
        else:
            q = wx.IMAGE_QUALITY_NORMAL
        #print(self._image.GetPalette())
        self._image.Rescale(size[0], size[1], q)
        self._bitmap = None
