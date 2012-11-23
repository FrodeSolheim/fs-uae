from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import re
import traceback
import subprocess
from .Config import Config
from .Settings import Settings
from .FSUAE import FSUAE
from .I18N import _

def create_cmp_id(id):
    return id.lower().replace(" ", "")

class Device:

    def __init__(self, id, name, type):
        self.id = id
        self.name = name
        self.type = type
        self.port = None
        self.cmp_id = create_cmp_id(id)

class DeviceManager:

    initialized = False
    device_ids = []
    device_names = []
    device_name_count = {}

    devices = []

    @classmethod
    def init(cls):
        if cls.initialized:
            return
        #cls.init_pygame()

        cls.devices.append(Device("none", _("No Device"), "none"))
        cls.devices.append(Device("mouse", _("Mouse"), "mouse"))
        cls.init_fsuae()
        for id, name in zip(cls.device_ids, cls.device_names):
            cls.devices.append(Device(id, name, "joystick"))
        cls.devices.append(Device("keyboard",
                _("Cursor Keys and Right Ctrl/Alt"), "joystick"))
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
                name = name + " #" + str(name_count)
            cls.device_ids.append(name)
            name = re.sub("[ ]+", " ", name)
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
                name = name + " #" + str(name_count)
            cls.device_ids.append(name)
            name = re.sub("[ ]+", " ", name)
            cls.device_names.append(name)

    @classmethod
    def init_fsuae(cls):
        print("finding connected joysticks")
        try:
            p = FSUAE.start_with_args(["--list-joysticks"],
                    stdout=subprocess.PIPE)
            joysticks = p.stdout.read()
            p.wait()
        except Exception:
            print("exception while listing joysticks")
            traceback.print_exc()
            return
        print(repr(joysticks))
        # If the character conversion failes, replace will ensure that
        # as much as possible succeeds. The joystick in question will
        # not be pre-selectable in the launcher, but the other ones will
        # work at least.
        joysticks = joysticks.decode("UTF-8", "replace")
        joysticks = [x.strip() for x in joysticks.split("\n") if x.strip()]
        for name in joysticks:
            name_count = cls.device_name_count.get(name, 0) + 1
            cls.device_name_count[name] = name_count
            if name_count > 1:
                name = name + " #" + str(name_count)
            cls.device_ids.append(name)
            name = re.sub("[ ]+", " ", name)
            cls.device_names.append(name)

    @classmethod
    def get_joystick_names(cls):
        cls.init()
        return cls.device_names[:]

    @classmethod
    def get_joystick_ids(cls):
        cls.init()
        return cls.device_ids[:]

    @classmethod
    def get_preferred_joysticks(cls):
        prefs = []
        if Settings.get("primary_joystick"):
            prefs.append(create_cmp_id(Settings.get("primary_joystick")))
        if Settings.get("secondary_joystick"):
            prefs.append(create_cmp_id(Settings.get("secondary_joystick")))
        return prefs

    @classmethod
    def get_preferred_gamepads(cls):
        return cls.get_preferred_joysticks()

    @classmethod
    def get_devices_for_ports(cls, config):
        cls.init()
        ports = [cls.devices[0] for x in range(4)]
        for device in cls.devices:
            device.port = None
        for p in range(4):
            key = "joystick_port_{0}".format(p)
            value = config.get(key)
            for device in cls.devices:
                if device.id == value:
                    device.port = p
                    break
        #print("-")
        #for device in cls.devices:
        #    print(device.port, device.id)
        #print("-")
        def autofill(port, type):
            mode = config.get("joystick_port_{0}_mode".format(port))
            value = config.get("joystick_port_{0}".format(port))
            if value:
                # specific device chosen
                for device in cls.devices:
                    if device.id == value:
                        ports[port] = device
                        break
                return
            if type == "mouse":
                #print("a", mode)
                if mode != "mouse":
                    return
                #print("b")
                for device in cls.devices:
                    #print("c")
                    if device.type == "mouse" and device.port is None:
                        #print("d")
                        ports[port] = device
                        device.port = port
                        return
            if type == "joystick":
                if mode == "cd32 gamepad":
                    prefs = cls.get_preferred_joysticks()
                elif mode == "joystick":
                    prefs = cls.get_preferred_gamepads()
                else:
                    return
                # try to find an available preferred device first
                for pref in prefs:
                    for device in cls.devices:
                        if device.cmp_id == pref and device.port is None:
                            ports[port] = device
                            device.port = port
                            return
                # find first suitable device
                for device in cls.devices:
                    if device.type == "joystick" and device.port is None:
                        ports[port] = device
                        device.port = port
                        return
        for p in [0, 1, 2, 3]:
            autofill(p, "mouse")
        for p in [1, 0, 3, 2]:
            autofill(p, "joystick")
        return ports

    @classmethod
    def get_device_for_port(cls, config, port):
        return cls.get_devices_for_ports(config)[port]
