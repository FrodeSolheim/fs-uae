from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import weakref
import pygame
from .bezier import Bezier
from .render import State

class AnimationSystem:
    animation_list = []

    @classmethod
    def is_active(cls):
        #for refobj in cls.animation_list:
        #    obj = refobj()
        #    if obj == None:
        #        deleted.append(refobj)
        #print "is_active"
        #print cls.animation_list
        return len(cls.animation_list) > 0

    @classmethod
    def update(cls, t=None):
        t = t or State.time
        deleted = []
        for refobj in cls.animation_list:
            obj = refobj()
            if obj == None:
                deleted.append(refobj)
            else:
                if not obj.update(t):
                    deleted.append(refobj)
        for d in deleted:
            cls.animation_list.remove(d)


class AnimateValueLinear():
    def __init__(self, what, from_value, from_time, to_value, to_time,
            run=True):
        self.what = what
        self.from_value = from_value
        self.to_value = to_value
        self.from_time = from_time
        self.to_time = to_time
        if run:
            AnimationSystem.animation_list.append(weakref.ref(self))

    @property
    def max_time(self):
        return self.to_time

    def update(self, t=None):
        t = t or State.time
        #print self.from_value, self.to_value
        progress = (t - self.from_time) / (
                self.to_time - self.from_time)
        progress = max(0.0, min(1.0, progress))
        setattr(self.what[0], self.what[1],
                self.from_value + progress * (self.to_value - self.from_value))
        return t < self.to_time


class AnimateValueBezier():
    def __init__(self, what, x0, t0, x1, t1, x2, t2, x3, t3, points=50,
            run=True):
        self.what = what
        #self.from_value = from_value
        #self.to_value = to_value
        #self.from_time = from_time
        #self.to_time = to_time
        self.params = (x0, t0, x1, t1, x2, t2, x3, t3)
        self.curve = Bezier.bezier((t0, x0), (t1, x1), (t2, x2), (t3, x3))
        if run:
            AnimationSystem.animation_list.append(weakref.ref(self))

    @property
    def max_time(self):
        return self.curve[-1][0]

    def update(self, t=None):
        #print "AnimateValue.update"
        #print self.from_value, self.to_value

        #progress = pygame.get_ticks() - from_time
        #totaltime = to_time - from_time

        x0, t0, x1, t1, x2, t2, x3, t3 = self.params
        #t = t or max(t0, min(t3, pygame.time.get_ticks()))
        t = t or State.time
        #t = max(t0, min(t3, t))
        if t < t0:
            t = t0
        elif t > t3:
            t = t3
        #print t0, t, t3
        #print type(t0), type(t), type(t3)

        #progress = (pygame.time.get_ticks() - self.from_time) / (
        #        self.to_time - self.from_time)
        #progress = max(0.0, min(1.0, progress))
        #pygame.time.get_ticks() - t0

        x = Bezier.interpolate(self.curve, t)
        #print x
        setattr(self.what[0], self.what[1], x)
        #print t, t3, t < t3
        return t < t3


class AnimationSequence(object):
    def __init__(self, animseq):
        self.animseq = animseq
        #for anim in self.animseq:
        AnimationSystem.animation_list.append(weakref.ref(self))

    def update(self, t=None):
        t = t or State.time
        for anim in self.animseq:
            if t > anim.max_time:
                continue
            anim.update(t)
            break
        else:
            return self.animseq[-1].update()
        return True

