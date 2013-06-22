from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import pygame

class BarcodeSupport:
    animation_list = []

    @classmethod
    def update(cls):
        deleted = []
        for refobj in cls.animation_list:
            obj = refobj()
            if obj == None:
                deleted.append(refobj)
            else:
                obj.update()
        for d in deleted:
            cls.animation_list.remove(d)
