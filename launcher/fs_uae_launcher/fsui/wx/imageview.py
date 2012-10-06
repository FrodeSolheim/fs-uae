from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class

class ImageView(wx.StaticBitmap):

    def __init__(self, parent, image=None):
        if image is not None:
            bitmap = image.bitmap
        else:
            bitmap = wx.NullBitmap
        wx.StaticBitmap.__init__(self, parent.get_container(), -1,
                bitmap)

    def set_image(self, image):
        if image is None:
            #self.SetBitmap(wx.NullBitmap)
            #print("self.SetBitmap(wx.NullBitmap)")
            self.SetBitmap(wx.BitmapFromBufferRGBA(1, 1, "\0\0\0\0"))
        else:
            self.SetBitmap(image.bitmap)

update_class(ImageView)
