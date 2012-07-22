from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import traceback
import subprocess
from .Config import Config
from .FSUAE import FSUAE

class DeviceManager:

    initialized = False
    device_ids = []
    device_names = []
    device_name_count = {}

    @classmethod
    def init(cls):
        if cls.initialized:
            return
        #cls.init_pygame()
        cls.init_fsuae()
        cls.initialized = True

    @classmethod
    def init_wx(cls):
        import wx
        #count = wx.Joystick(wx.JOYSTICK1).GetNumberJoysticks()
        count = 1
        print(count)

        for i in range(count):
            joy = wx.Joystick(wx.JOYSTICK1 + i)
            print(joy, joy.IsOk())
            name = joy.GetProductName()
            print("a")
            name_count = cls.device_name_count.get(name, 0) + 1
            cls.device_name_count[name] = name_count
            if name_count > 1:
                name = name + u" #" + str(name_count)
            cls.device_ids.append(name)
            for i in range(3):
                name = name.replace("  ", " ")
            cls.device_names.append(name)
            print("a")

    @classmethod
    def init_pygame(cls):
        import pygame
        #pygame.init()
        pygame.joystick.init()
        count = pygame.joystick.get_count()

        for i in range(count):
            joy = pygame.joystick.Joystick(i)
            name = joy.get_name()
            name_count = cls.device_name_count.get(name, 0) + 1
            cls.device_name_count[name] = name_count
            if name_count > 1:
                name = name + u" #" + str(name_count)
            cls.device_ids.append(name)
            for i in range(3):
                name = name.replace("  ", " ")
            cls.device_names.append(name)

    @classmethod
    def init_fsuae(cls):
        try:
            p = FSUAE.start_with_args(["--list-joysticks"],
                    stdout=subprocess.PIPE)
            joysticks = p.stdout.read()
            p.wait()
        except Exception:
            print("exception while listing joysticks")
            traceback.print_exc()
            return
        joysticks = [x.strip() for x in joysticks.split("\n") if x.strip()]
        for name in joysticks:
            name_count = cls.device_name_count.get(name, 0) + 1
            cls.device_name_count[name] = name_count
            if name_count > 1:
                name = name + u" #" + str(name_count)
            cls.device_ids.append(name)
            for i in range(3):
                name = name.replace("  ", " ")
            cls.device_names.append(name)

    @classmethod
    def get_joystick_names(cls):
        cls.init()
        return cls.device_names[:]
