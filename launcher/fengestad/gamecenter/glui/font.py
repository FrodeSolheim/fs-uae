from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import json
import ctypes
from .opengl import *
from .texture import Texture
import fengestad.fs as fs
from fengestad.gamecenter.resources import resources, logger, ngettext, _

CACHE_SIZE = 100

text_cache = []

# initialize cache
for i in range(CACHE_SIZE):
    item = {
        "text": None,
        "font": None,
        "texture": None,
    }
    text_cache.append(item)


class Font:
    title_font = None
    subtitle_font = None
    small_font = None
    main_path_font = None
    list_subtitle_font = None
    header_font = None


class BitmappedFont:
    
    title_font = None
    menu_font = None
    
    def __init__(self, name):
        self.w = [0 for x in range(256)]
        self.h = 0
        self.load(name)

    @fs.memoize
    def measure(self, text):
        required_width = 0
        required_height = self.h
        for c in text:
            required_width += self.w[ord(c)]
        return required_width, required_height

    def render(self, text, x, y, r=1.0, g=1.0, b=1.0, alpha=1.0):
        global text_cache
        if not text:
            return 0
        #if (!g_initialized) {
        #    initialize();
        #}
        #if (g_fs_emu_video_version != g_video_version) {
        #    GList* list = g_cache;
        #    while (list) {
        #        cache_item *item = (cache_item *) list->data;
        #        g_free(item->text);
        #        g_free(item);
        #        list = list->next;
        #    }
        #    g_list_free(g_cache);
        #    g_cache = NULL;
        #    initialize_cache();
        #}
        
        # find cached text entry, if any
        
        for i, item in enumerate(text_cache):
            if item["text"] == text and item["font"] == self:
                text_cache.pop(i)
                break
        else:
            item = None
        
        if item:
            # FIXME: REMOVE
            #fs_emu_blending(False)
            #fs_emu_texturing(False)
            
            fs_emu_blending(True)
            fs_emu_texturing(True)
            
            #//fs_emu_ortho();
            #fs_emu_set_texture(NULL);
            w = item["w"]
            h = item["h"]
            glBindTexture(GL_TEXTURE_2D, item["texture"])
            glBegin(GL_QUADS)
            glColor4f(r * alpha, g * alpha, b * alpha, alpha)
            glTexCoord2d(0.0, 0.0)
            glVertex2d(x, y)
            glTexCoord2d(1.0, 0.0)
            glVertex2d(x + w, y)
            glTexCoord2d(1.0, 1.0)
            glVertex2d(x + w, y + h)
            glTexCoord2d(0.0, 1.0)
            glVertex2d(x, y + h)
            glEnd()
            
            # re-insert item at front
            text_cache.insert(0, item)
            return w, h
    
        # calculate size of text
        required_width, required_height = self.measure(text)
    
        # setup fbo
    
        mip_mapping = 0
    
        render_texture = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, render_texture)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, required_width,
                     required_height, 0, GL_RGBA, GL_UNSIGNED_INT, None)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    
        # FIXME: Mipmapping?
        if mip_mapping:
            glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE)
            glGenerateMipmapEXT(GL_TEXTURE_2D)
        else:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)

        glBindTexture(GL_TEXTURE_2D, 0)
    
        # Set up some renderbuffer state
    
        frame_buffer = GLuint()
        #glGenFramebuffersEXT(1, frame_buffer)
        #glGenFramebuffersEXT(1, ctypes.byref(frame_buffer))
        glGenFramebuffersEXT(1, frame_buffer)
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frame_buffer)
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D, render_texture, 0)
    
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)
        if status != GL_FRAMEBUFFER_COMPLETE_EXT:
            print ("glCheckFramebufferStatusEXT error", status)
    
        glPushMatrix()
        glLoadIdentity()
        glPushAttrib(int(GL_VIEWPORT_BIT) | int(GL_ENABLE_BIT))
        glViewport(0, 0, required_width, required_height)
        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        glLoadIdentity()
        gluOrtho2D(0, required_width, 0, required_height)
    
        glClearColor(0.0, 0.0, 0.0, 0.0)
        glClear(GL_COLOR_BUFFER_BIT)

        # FIXME: DUPLICATE ASSIGNMENT HERE?
        # glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
        # GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, render_texture, 0)
    
        # FIXME:
        #fs_emu_blending(False)
        #fs_emu_blending(True)
        
        glEnable(GL_BLEND)
        glEnable(GL_TEXTURE_2D)
    
        # FIXME:
        # fs_emu_set_texture(NULL);
        # fs_emu_set_texture(font->texture);
        #glBindTexture(GL_TEXTURE_2D, )
        self.texture.bind()
    
        # fs_emu_texturing(0);
        tw = self.texture.w
        th = self.texture.h
    
        # int length = strlen(text);
        # for (int i = 0; i < length; i++) {
        
        glBegin(GL_QUADS)
        glColor4f(1.0, 1.0, 1.0, 1.0)
        x2 = 0
        h = self.h
        for ch in text:
            c = ord(ch)
            w = self.w[c]
            #printf("%d %d %d %d\n", font->x[c], font->y[c], w, h)
            s1 = self.x[c] / tw
            s2 = (self.x[c] + w) / tw
            # double t1 = 1.0 - (font->y[c]) / th
            # double t2 = 1.0 - (font->y[c] + h) / th
            t1 = (self.y[c]) / th
            t2 = (self.y[c] + h) / th
            # printf("%d %d %d %d\n", x, y, w, h)
            glTexCoord2d(s1, t2)
            # glVertex2d(x, y)
            glVertex2d(x2, 0)
            glTexCoord2d(s2, t2)
            # glVertex2d(x + w, y)
            glVertex2d(x2 + w, 0)
            glTexCoord2d(s2, t1)
            # glVertex2d(x + w, y + h)
            glVertex2d(x2 + w, h)
            glTexCoord2d(s1, t1)
            # glVertex2d(x, y + h)
            glVertex2d(x2, h)
            x2 += w
        glEnd()
    
        glPopMatrix()
        glMatrixMode(GL_MODELVIEW)
        glPopAttrib()
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0)
        glPopMatrix()
    
        #glDeleteFramebuffersEXT(1, ctypes.byref(frame_buffer))
        glDeleteFramebuffersEXT(1, frame_buffer)
        # glDeleteRenderbuffersEXT(1, &depth_buffer)

        # FIXME:
        #fs_emu_set_texture(NULL);

        if mip_mapping:
            glBindTexture(GL_TEXTURE_2D, render_texture)
            glGenerateMipmapEXT(GL_TEXTURE_2D)
            glBindTexture(GL_TEXTURE_2D, 0)

        new_item = {
            "font": self,
            "text": text,
            "w": required_width,
            "h": required_height,
            "texture": render_texture,
        }
        text_cache.insert(0, new_item)

        item = text_cache.pop()
        #print(repr(item))
        if item["texture"]:
            glDeleteTextures([item["texture"]])

        # now the text is in the cache, so call function again
        return self.render(text, x, y, r, g, b, alpha)
    
    def load(self, name):
        print ("load font", name)
        self.texture = Texture(name + ".png")
        f = resources.resource_stream(name + ".json")
        doc = json.load(f)
        self.x = doc["x"]
        self.y = doc["y"]
        self.w = doc["w"]
        self.h = doc["h"][0]
