from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import numpy
import Image
# import .opengl first
from .opengl import *
#from OpenGL.GL import *
from fengestad.gamecenter.resources import resources, logger, ngettext, _

class Texture:

    shadow = None
    shadow2 = None
    gloss = None
    screen_gloss = None
    static = None
    default_item = None
    missing_screenshot = None
    missing_cover = None
    logo = None
    top = None
    top_logo = None
    top_logo_selected = None

    add = None
    add_selected = None
    home = None
    home_selected = None
    minimize = None
    minimize_selected = None
    close = None
    close_selected = None
    shutdown = None
    shutdown_selected = None

    bottom_bar = None
    screen_border_1 = None
    screen_border_2 = None
    top_background = None
    top_item = None
    top_item_selected = None
    top_item_left = None
    top_item_left_selected = None
    top_item_right = None
    top_item_arrow = None
    top_item_arrow_selected = None
    
    glow_top = None
    glow_top_left = None
    glow_left = None
    
    sidebar_background_shadow = None

    def __init__(self, name, target=GL_TEXTURE_2D, **kwargs):
        #print(repr(type(name)))
        if isinstance(name, (int, long)):
            self.size = kwargs["size"]
            self.texture = name
        else:
            self.size = [0, 0]
            #print(name, kwargs)
            out_data = {}
            self.texture = self.from_resource(name, target=target,
                    size=self.size, out_data=out_data, **kwargs)
            self.data = out_data["imdata"]
            self.gl_type = out_data["type"]
        self.w, self.h = self.size
        self.target = target

    def bind(self):
        glBindTexture(self.target, self.texture)

    def render(self, x, y, w, h, z=0.0, opacity=1.0):
        self.bind()
        fs_emu_texturing(True)
        if self.gl_type == GL_RGBA or opacity < 1.0:
            fs_emu_blending(True)
        else:
            fs_emu_blending(False)
        glBegin(GL_QUADS)
        if opacity < 1.0:
            glColor4f(opacity, opacity, opacity, opacity)
        else:
            glColor3f(1.0, 1.0, 1.0)
        glTexCoord(0.0, 1.0)
        glVertex3f(x, y, z)
        glTexCoord(1.0, 1.0)
        glVertex3f(x + w, y, z)
        glTexCoord(1.0, 0.0)
        glVertex3f(x + w, y + h, z)
        glTexCoord(0.0, 0.0)
        glVertex3f(x, y + h, z)
        glEnd()

    @classmethod
    def load(cls, im, mipmap=False, min_filter=None,
            wrap_s=GL_CLAMP_TO_EDGE, wrap_t=GL_CLAMP_TO_EDGE,
            target=GL_TEXTURE_2D, size=[0, 0], out_data=None):
        type = "RGB"
        gl_type = GL_RGB
        if im.mode == "RGBA":
            # convert to premultiplied alpha
            #pix = im.load()
            #for x in range(im.size[0]):
            #    for y in range(im.size[1]):
            #        r, g, b, a = pix[x, y]
            #        if a:
            #            pix[x, y] = r * 255 // a, g * 255 // a, b * 255 // a, a
            #        else:
            #            pix[x, y] = 0, 0, 0, 0
            a = numpy.fromstring(im.tostring(), dtype=numpy.uint8)
            alphaLayer = a[3::4] / 255.0
            a[::4]  *= alphaLayer
            a[1::4] *= alphaLayer
            a[2::4] *= alphaLayer
            #im = Image.fromstring("RGBA", im.size, a.tostring())
            imdata = a.tostring()
            type = "RGBA"
            gl_type = GL_RGBA
        else:
            imdata = im.tostring("raw", type)
        size[0] = im.size[0]
        size[1] = im.size[1]
        #texture = glGenTextures(1)
        from .render import Render
        texture = Render.create_texture()
        glBindTexture(target, texture)
        glTexImage2D(target, 0, gl_type, im.size[0], im.size[1], 0,
                gl_type, GL_UNSIGNED_BYTE, imdata)
        if mipmap:
            glGenerateMipmap(target)
            if min_filter is None:
                min_filter = GL_LINEAR_MIPMAP_LINEAR
        else:
            if min_filter is None:
                min_filter = GL_LINEAR
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter)
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s)
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t)
        if out_data is not None:
            out_data["imdata"] = imdata
            out_data["type"] = gl_type
        return texture

    @classmethod
    def from_resource(cls, name, size=[0, 0], **kwargs):
        im = resources.resource_pil_image(name)
        return cls.load(im, size=size, **kwargs)

