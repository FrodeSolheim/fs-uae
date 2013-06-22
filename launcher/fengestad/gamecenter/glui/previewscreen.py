from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import random
import pygame
from .opengl import *
from .render import Render
from fengestad.gamecenter.resources import resources, logger, ngettext, _

class PreviewScreen(object):

    def __init__(self):
        self.screen = None
        self.brightness = 1.0
        self.texture = Texture.from_resource("preview_screen.png", mipmap=True,
                min_filter=GL_LINEAR_MIPMAP_NEAREST)
        self.static = []
        for i in range(10):
            texture = Texture.from_resource("preview_static{0}.png".format(i),
                    mipmap=True)
            self.static.append(texture)
        #self.counter = -1

    def set_screen(self, texture):
        self.screen = texture

    def get_onscreen_texture(self):
        if self.screen:
            return self.screen
        #return random.choice(self.static)
        #self.counter += 1
        #index = self.counter // 6
        #if index >= len(self.static):
        #    index = 0
        #    self.counter = 0
        index = int(pygame.time.get_ticks() / (1000/30)) % len(self.static)
        texture = self.static[index]
        return texture

    def render(self):
        Render.ortho_perspective()

        glPushMatrix()
        glTranslatef(0.078, -0.127, 0.0)

        glDisable(GL_DEPTH_TEST)
        fs_emu_texturing(True)
        fs_emu_blending(True)
        glBindTexture(GL_TEXTURE_2D, self.texture)
        x1, x2, y1, y2 = 0.7, Render.gl_right + 0.05, -1.08, -0.28
        glColor3f(1.0, 1.0, 1.0)
        glBegin(GL_QUADS)
        glTexCoord2f(0.0, 1.0)
        glVertex2f(x1, y1)
        glTexCoord2f(1.0, 1.0)
        glVertex2f(x2, y1)
        glTexCoord2f(1.0, 0.0)
        glVertex2f(x2, y2)
        glTexCoord2f(0.0, 0.0)
        glVertex2f(x1, y2)
        glEnd()

        glColor3f(self.brightness, self.brightness, self.brightness)

        glBindTexture(GL_TEXTURE_2D, self.get_onscreen_texture())
        x1, x2, y1, y2 = 0.825, Render.gl_right - 0.078, -0.87, -0.355
        glBegin(GL_QUADS)
        glTexCoord2f(0.0, 1.0)
        glVertex2f(x1, y1)
        glTexCoord2f(1.0, 1.0)
        glVertex2f(x2, y1)
        glTexCoord2f(1.0, 0.0)
        glVertex2f(x2, y2)
        glTexCoord2f(0.0, 0.0)
        glVertex2f(x1, y2)
        glEnd()

        glBindTexture(GL_TEXTURE_2D, Texture.gloss)
        glColor4f(self.brightness * 0.75, self.brightness * 0.75,
                  self.brightness * 0.75, 0.75)
        glBegin(GL_QUADS)
        glTexCoord2f(0.0, 1.0)
        glVertex2f(x1, y1)
        glTexCoord2f(1.0, 1.0)
        glVertex2f(x2, y1)
        glTexCoord2f(1.0, 0.0)
        glVertex2f(x2, y2)
        glTexCoord2f(0.0, 0.0)
        glVertex2f(x1, y2)
        glEnd()

        fs_emu_texturing(False)
        #fs_emu_blending(False)
        glEnable(GL_DEPTH_TEST)

        glPopMatrix()

