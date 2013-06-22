from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import locale
# we must set the LC_NUMERIC locale to C, because PyOpenGL has
# a problem if comma is used as a decimal separator (problem
# parsing the OpenGL version)
locale.setlocale(locale.LC_NUMERIC, "C")
del locale
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GL.ARB.shader_objects import *
from OpenGL.GL.ARB.vertex_shader import *
from OpenGL.GL.ARB.multitexture import *
from OpenGL.GL.ARB.fragment_shader import *
from OpenGL.GL.ARB.framebuffer_object import *
from OpenGL.raw.GL.EXT.framebuffer_object import *
from OpenGL.arrays.vbo import VBO
#from OpenGLContext.arrays import *

# FIXME: cache

def fs_emu_blending(enable):
    if enable:
        glEnable( GL_BLEND )
    else:
        glDisable( GL_BLEND )


def fs_emu_texturing(enable):
    if enable:
        glEnable( GL_TEXTURE_2D )
    else:
        glDisable( GL_TEXTURE_2D )
