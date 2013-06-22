from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from .opengl import *
from .render import Render
from fengestad.gamecenter.resources import resources, logger, ngettext, _

class Dialog(object):

    def __init__(self):
        self.width = 1.6
        self.height = 1.0
        self.background_texture = None
        self.background_texture_offset = [0, 0]
        self.background_color = (0.4, 0.4, 0.4, 1.0)
        # make sure that the dialog is rendered at least once
        # after beeing created
        Render.dirty = True

    def render(self):
        Render.ortho_perspective()
        glDisable(GL_DEPTH_TEST)
        fs_emu_blending(True)
        glPushMatrix()
        glTranslatef(-self.width / 2, -self.height / 2, 0.0)
        self.render_background()
        self.render_content()
        glPopMatrix()
        glEnable(GL_DEPTH_TEST)
        pass

    def render_background(self):
        if self.background_texture:
            glBindTexture(GL_TEXTURE_2D, self.background_texture)
            glColor3f(1.0, 1.0, 1.0)
        else:
            glBindTexture(GL_TEXTURE_2D, 0)
            #r, g, b a = self.background_color
            glColor4f(*self.background_color)
        glBegin(GL_QUADS)
        bgx, bgy = self.background_texture_offset
        glTexCoord(bgx + 0.0, bgy + 1.0)
        glVertex2f(0, 0)
        glTexCoord(bgx + 1.0, bgy + 1.0)
        glVertex2f(self.width, 0)
        glTexCoord(bgx + 1.0, bgy + 0.0)
        glVertex2f(self.width, self.height)
        glTexCoord(bgx + 0.0, bgy + 0.0)
        glVertex2f(0, self.height)
        glEnd()

    def render_content(self):
        pass

    def destroy(self):
        pass

