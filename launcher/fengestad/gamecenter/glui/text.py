from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

# cutout.py
#
# Having recently discovered Python, this looked like a
# fun way to learn the language...
#
# Uses 5x5 convolution matrices to do Emboss, Cutout, Blur & Edge effects.
# Generates a somewhat random circular gradient texture.
#
# Even using Numpy this is pretty slow.
#
# Bits of code blatantly stolen from the Pygame site.
#
# Author: Sherwyn Ramkissoon (bei@sympatico.ca)


# Import some useful (or not) stuff
import os
import sys
import pygame
import pygame.image
#import pygame.transform
#import pygame.font
#import Numeric
import math as math
import pygame.surfarray as surfarray
from pygame.locals import *
import numpy
import Image

# FILTER STUFF

# Convolution Kernels
BLUR_FILTER = ([[0,1,2,1,0],[1,2,4,2,1],[2,4,8,4,2],[1,2,4,2,1],[0,1,2,1,0]],
        48)


# 5x5 Convolution filter
def convolution_filter(inarray,filter,divisor):
    size = len(filter)
    out = Numeric.zeros(inarray.shape)
    (R,C,D) = out.shape
    for i in range((1-size)/2,(size+1)/2):
        for j in range((1-size)/2,(size+1)/2):
            if filter[i+2][j+2] != 0:
                out[2:R-2,2:C-2] += inarray[2+i:R-2+i,2+j:C-2+j]*filter[i+2][j+2]
    out = Numeric.clip(out / divisor, 0, 255)
    return out


def convolution_filter(inarray,filter,divisor):
    size = len(filter)
    out = Numeric.zeros(inarray.shape)
    (R,C) = out.shape
    for i in range((1-size)/2,(size+1)/2):
        for j in range((1-size)/2,(size+1)/2):
            if filter[i+2][j+2] != 0:
                out[2:R-2,2:C-2] += inarray[2+i:R-2+i,2+j:C-2+j]*filter[i+2][j+2]
    out = Numeric.clip(out / divisor, 0, 255)
    return out

# Blur Filter

def apply_filter(surf,filter_func):
    #pix = pygame.surfarray.array3d(surf)
    pix = pygame.surfarray.pixels_alpha(surf)
    #surf = pygame.Surface((pix.shape[0], pix.shape[1]), 32)
    #surfarray.blit_array(surf, filter_func(pix))
    surfarray.blit_array(surf, filter_func(pix))
    return surf


class TextRenderer():
    def __init__(self, font):
        self.font = font

    @staticmethod
    def BLUR(inarray):
        return convolution_filter(inarray, BLUR_FILTER[0], BLUR_FILTER[1])

    def render_text(self, text, color, filter=None):
        #color = (255, 255, 255)
        rendered = self.font.render(text, True, color)
        if filter:
            rendered = apply_filter(rendered, filter)
        txtsize = rendered.get_size()
        txtdata = pygame.image.tostring(rendered, "RGBA", 1)
        
        # convert to premultipled alpha
        a = numpy.fromstring(txtdata, dtype=numpy.uint8)
        alphaLayer = a[3::4] / 255.0
        a[::4]  *= alphaLayer
        a[1::4] *= alphaLayer
        a[2::4] *= alphaLayer
        #im = Image.fromstring("RGBA", im.size, a.tostring())
        
        #return txtdata, txtsize
        return a.tostring(), txtsize

    def get_size(self, text):
        return self.font.size(text)

