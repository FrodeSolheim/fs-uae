from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import pygame
from .opengl import *
from .texture import Texture
from .state import State
from .render import Render
from .dialog import Dialog
from fengestad.gamecenter.resources import resources, logger, ngettext, _

class RunDialog(Dialog):

    def __init__(self):
        Dialog.__init__(self)
        #self.width = 32 / 9
        #self.height = 1.6
        #self.background_texture = Texture.from_resource("run_dialog.png",
        #        wrap_s=GL_REPEAT)
        #
        #liberation_sans_bold = resources.resource_filename(
        #        "LiberationSans-Regular.ttf")
        #self.title_font = pygame.font.Font(liberation_sans_bold,
        #        int(0.07 * Render.display_height))
        self.throbber_colors = [1.0, 0.8, 0.6, 0.2, 0.2, 0.2, 0.2, 0.2]
        self.throbber_start_time = 0
        self.throbber_progress = 0
        self.throbber_opacity = 1.0


    def render(self):
        Render.hd_perspective()
        if self.throbber_start_time == 0:
            self.throbber_start_time = State.time
        dt = State.time - self.throbber_start_time
        # run animation with 15 fps
        self.throbber_progress = int(dt * 15)
        
        bg_fade = (State.time - State.dialog_time) / 0.5
        if bg_fade > 1.0:
            bg_fade = 1.0
        elif bg_fade < 0.0:
            bg_fade = 0.0
        fs_emu_texturing(False)
        fs_emu_blending(True)
        glDisable(GL_DEPTH_TEST)
        glDepthMask(False)
        
        glBegin(GL_QUADS)
        glColor4f(0.0, 0.0, 0.0, bg_fade)
        glVertex2f(0, 0)
        glVertex2f(1920, 0)
        glVertex2f(1920, 1020)
        glVertex2f(0, 1020)
        glEnd()
        
        glBegin(GL_QUADS)
        glColor4f(0.0, 0.0, 0.0, bg_fade * 0.5)
        glVertex2f(0, 1020)
        glVertex2f(1920, 1020)
        glVertex2f(1920, 1080)
        glVertex2f(0, 1080)
        glEnd()
        
        #y = 0.0
        #tw, th = Render.text("LAUNCHING GAME", self.title_font,
        #        0.0, y, w=32 / 9, h=self.height,
        #        color=(1.0, 1.0, 1.0, 1.0), halign=0.0)
        #
        fs_emu_blending(True)
        #if bg_fade > 0.5:
        #    self.throbber_opacity = (bg_fade - 0.5) / 0.5 
        #    #self.throbber_opacity = bg_fade
        #    self.render_throbber()
        #if bg_fade == 1.0:
        if State.time - State.dialog_time > 1.0:
            # gradually show over 1/4 second
            self.throbber_opacity = (State.time - State.dialog_time - 1.0) * 4
            if self.throbber_opacity > 1.0:
                self.throbber_opacity = 1.0
            self.render_throbber()
        
        glEnable(GL_DEPTH_TEST)
        glDepthMask(True)
        #fs_emu_texturing(1)
        #fs_emu_blending(0)
        #print("Setting dirty..")
        Render.dirty = True

    def render_throbber(self):
        cell_width = 32
        cell_spacing = 16
        throbber_width = 3 * cell_width + 2 * cell_spacing
        throbber_height = throbber_width
        left = (1920 - throbber_width) / 2
        bottom = (1080 - throbber_width) / 2
        
        self.render_cell(0, left, bottom)
        self.render_cell(1, left + 1 * cell_width + 1 * cell_spacing, bottom)
        self.render_cell(2, left + 2 * cell_width + 2 * cell_spacing, bottom)
        self.render_cell(3, left + 2 * cell_width + 2 * cell_spacing,
                bottom + 1 * cell_width + 1 * cell_spacing)
        self.render_cell(4, left + 2 * cell_width + 2 * cell_spacing,
                bottom + 2 * cell_width + 2 * cell_spacing)
        self.render_cell(5, left + 1 * cell_width + 1 * cell_spacing,
                bottom + 2 * cell_width + 2 * cell_spacing)
        self.render_cell(6, left,
                bottom + 2 * cell_width + 2 * cell_spacing)
        self.render_cell(7, left,
                bottom + 1 * cell_width + 1 * cell_spacing)
        
    def render_cell(self, index, x, y):
        color = self.throbber_colors[(self.throbber_progress + index) % 8]
        color = color * self.throbber_opacity
        #print("render_cell", index, x, y, color)
        glColor4f(color, color, color, self.throbber_opacity)
        glBegin(GL_QUADS)
        glVertex2f(x, y)
        glVertex2f(x + 32, y)
        glVertex2f(x + 32, y + 32)
        glVertex2f(x, y + 32)
        glEnd()

    def render_background(self):
        pass

    def render_content(self):
        pass
