from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division

import threading
#from OpenGL.GL import *
#from OpenGL.GLU import *
from .opengl import *
from .texture import Texture
from .render import Render

CACHE_SIZE = 50

class TextureManager():

    __instance = None

    @classmethod
    def get(cls):
        if not cls.__instance:
            cls.__instance = cls()
        return cls.__instance

    def __init__(self):
        self.lock = threading.Lock()
        self.image_list = []
        self.delay_set = set()
        self.image_dict = {}
        self.texture_dict = {}
        self.transparent = "\x00" * 512 * 512 * 4

    def set_image(self, image_path, image, size):
        with self.lock:
            if not image_path in self.image_dict:
                # No longer interested
                return
            self.image_dict[image_path] = (image, size)

    def load_images(self, image_paths):
        #print "load_images", image_paths
        with self.lock:
            for ip in reversed(image_paths):
                if ip in self.image_dict:
                    self.image_list.remove(ip)
                    #self.image_list.insert(0, ip)
                else:
                    # texture = False means not loaded
                    # texture = None means failure loading
                    self.image_dict[ip] = False, None
                    self.texture_dict[ip] = None
                self.image_list.insert(0, ip)

            for ip in self.image_list[CACHE_SIZE:]:
                texture = self.texture_dict[ip]
                if texture is not None:
                    #glDeleteTextures([texture.texture])
                    if texture is not Texture.default_item:
                        Render.delete_texture_list.append(texture.texture)
                del self.image_dict[ip]
                del self.texture_dict[ip]
            self.image_list = self.image_list[:CACHE_SIZE]

    """
    def bind_texture(self, ip):
        if ip is None:
            #glBindTexture(GL_TEXTURE_2D, 0)
            return False
        with self.lock:
            texture = self.texture_dict[ip]
            if texture is None:
                #texture = 0
                return False
            #print "glBindTexture", texture
            glBindTexture(GL_TEXTURE_2D, texture)
            return True
    """

    def get_texture(self, ip):
        if ip is None:
            return None
        with self.lock:
            texture = self.texture_dict[ip]
            if texture:
                if ip in self.delay_set:
                    # do not return the texture the first time we ask for it,
                    # this is to allow the GPU driver time to upload the texture
                    # in the background (DMA transfer) without blocking for it
                    self.delay_set.remove(ip)
                    return None
            #return texture, size
            return texture

    def load_textures(self, max_num=10):
        loaded = 0
        target = GL_TEXTURE_2D
        with self.lock:
            try:
                for ip in self.image_list:
                    if loaded == max_num:
                        break
                    texture = self.texture_dict[ip]
                    if texture is not None:
                        continue
                    pixels, size = self.image_dict[ip]
                    if pixels is False:
                        # queued
                        #self.texture_dict[ip] = Texture.default_item
                        continue
                    if pixels is None:
                        # could not load
                        #self.texture_dict[ip] = Texture(0, size=(0, 0))
                        #self.texture_dict[ip] = Texture.default_item
                        self.texture_dict[ip] = Texture.default_item
                        continue

                    #assert size == (512, 512) or size == (1024, 1024)

                    texture = glGenTextures(1)
                    #print(size)
                    self.texture_dict[ip] = Texture(texture,
                            size=(size[0], size[1]))
                    self.delay_set.add(ip)
                    #if size == (293, 256):
                    #    #with open("c:\\test.raw", "wb") as f:
                    #    #    f.write(pixels)
                    #    from PIL import Image
                    #    im = Image.fromstring("RGB", size, pixels)
                    #    im.save("c:\\test.png")
                    self.lock.release()
                    #glBindTexture(GL_TEXTURE_2D, texture)
                    glBindTexture(target, texture)
                    #glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0,
                    #        GL_RGBA, GL_UNSIGNED_BYTE, self.transparent)
                    #glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0,
                    #        GL_RGB, GL_UNSIGNED_BYTE, None)
                    #print(">>>a<<<", id(pixels), size[0], size[1])
                    #print(size[0] * size[1] * 3, len(pixels))
                    internal_format = GL_RGBA
                    #internal_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
                    glTexImage2D(target, 0, internal_format, size[0],
                            size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels)
                    #print(">>>b<<<")
                    """
                    width, height = size
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
                    #glPixelStorei(GL_UNPACK_ROW_LENGTH, width)
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels)
                    # duplicate last column to avoid bleed
                    if width < 512:
                        glPixelStorei(GL_UNPACK_ROW_LENGTH, width)
                        glPixelStorei(GL_UNPACK_SKIP_PIXELS, width)
                        glTexSubImage2D(GL_TEXTURE_2D, 0, width, 0,
                                1, height, GL_RGB, GL_UNSIGNED_BYTE, pixels)
                        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0)
                        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0)
                    if height < 512:
                        # FIXME:
                        pass
                    """
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR)
                    glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR)
                    glTexParameteri(target, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE)
                    glTexParameteri(target, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE)
                    loaded += 1
                    self.lock.acquire()
            except Exception, e:
                print(e)

    def unload_textures(self):
        with self.lock:
            for ip in self.image_list:
                texture = self.texture_dict[ip]
                if texture is not None:
                    #glDeleteTextures([texture])
                    if texture is not Texture.default_item:
                        Render.delete_texture_list.append(texture.texture)
                self.texture_dict[ip] = None

