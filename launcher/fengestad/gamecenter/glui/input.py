from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import re
import pygame
import time
try:
    import pylirc
except ImportError:
    pylirc = None
from fengestad.gamecenter.gamecenter import GameCenter
from fengestad.gamecenter.notification import Notification
from fengestad.input.keyboard import Keyboard
from fengestad.input.joystick import Joystick
from fengestad.input.inputdevice import InputDevice
from fengestad.gamecenter.resources import resources, logger, ngettext, _
from fengestad import fs

REPEAT_TRESHOLD = 0.300
REPEAT_INTERVAL = 0.075

@fs.memoize
def get_controller_config(name, sdl_name, axes=0, hats=0, buttons=0, balls=0):
    try:
        # device id must end with #something (really a device number,
        # but can be anything
        device = InputDevice("menu", name + " #MENU", [], sdl_name=sdl_name,
                version=2, axes=axes, hats=hats, buttons=buttons, balls=balls)
        config = device.get_config()
    except Exception, e:
        print("error initializing device " + repr(name) + " for menu")
        print(repr(e))
        return None
    #config_inv = []
    for key, val in list(config.iteritems()):
        val = val.upper()
        config[key] = val
        config[val] = key
    return config

@fs.memoize
def get_controller_instance(name):
    print("get_controller_instance")
    name = name.upper()
    instances = {}
    for ext in pyapp.ext.ExtensionHook(
            "exthook:no.fengestad.input/device"):
        klass = ext.object
        print(klass)

        for ccname in ext.models:
            #print(name)
            if ccname.startswith("^"):
                #print("reg", ccname, "vs", name)
                if re.match(ccname, name) is not None:
                    #print("match")
                    #klass = cc
                    break
            elif name == ccname:
                #klass = cc
                break
        else:
            #print("no input device class found")
            continue
        try:
            instance = klass("menu", "", [])
        except Exception:
            logger.exception("Could not initialize controller for menu")
            continue
        #for name in ext.models:
        #    ctrlclasses[name.upper()] = klass
        #instance = klass()
        #for name in ext.models:
        #    print(name)
        #    instances[name.upper()] = instance
        return instance
    print("no input device class found")
    return None


class InputHandler():

    last_device = ""
    joystick_hash = None
    last_joystick_check = 0
    current_button = None
    lirc_socket = None
    repeat_info = None
    repeatable_buttons = ["UP", "DOWN", "LEFT", "RIGHT", "BACK"]
    #last_joystick_count = 0
    joysticks = []
    axis_status = {}
    first_init = True
    device_sdl_names = {}
    key_table = {}

    @classmethod
    def get_virtual_button(cls, event):
        if event.type in [pygame.KEYDOWN, pygame.KEYUP]:
            button = None
            try:
                key = Keyboard.key(event.key)
            except Exception, e:
                print(repr(e))
                return None, None
            try:
                button, device_id = cls.key_table[key.name]
            except Exception, e:
                #print(repr(e))
                return None, None

            #if button:
            #    #cls.last_device = "KEYBOARD #1"
            #    cls.last_device = device_id
            #print("get_virtual_button", button, cls.last_device)
            return button, device_id
        elif event.type in [pygame.JOYBUTTONDOWN, pygame.JOYBUTTONUP,
                pygame.JOYAXISMOTION, pygame.JOYHATMOTION]:
            if event.type == pygame.JOYAXISMOTION:
                #if event.value > -0.5 and event.value < 0.5:
                #    return None
                #sign = "-" if event.value < 0 else "+"
                sign = "neg" if event.value < 0 else "pos"
                #cfg_name = "JA%d%s" % (event.axis, sign)
                cfg_name = "axis_{0}_{1}".format(event.axis, sign)
            elif event.type == pygame.JOYHATMOTION:
                if event.value[0] == -1:
                    value = "left"
                elif event.value[0] == 1:
                    value = "right"
                elif event.value[1] == -1:
                    value = "down"
                elif event.value[1] == 1:
                    value = "up"
                else:
                    value = "0"
                #cfg_name = "JH" + str(event.hat) + str(value)
                cfg_name = "hat_{0}_{1}".format(event.hat, value)
            else:
                #cfg_name = "JB%02d" % (event.button,)
                cfg_name = "button_{0}".format(event.button)
            try:
                joystick = cls.joysticks[event.joy]
            except IndexError:
                return None, None
            #controller = get_controller_instance(joystick.name)
            #if controller is None:
            #    return None
            ##klass.get_
            #config = controller.get_config()
            #config_inv = controller.get_config_inverted()
            config = get_controller_config(joystick.name, joystick.sdl_name,
                    axes=joystick.axes, hats=joystick.hats,
                    buttons=joystick.buttons, balls=joystick.balls)
            #print(config_inv)
            if config is None:
                return None, None
            #print(cfg_name)
            try:
                button = config[cfg_name]
                #print(button)
                if button == "START":
                    combine = "SELECT"
                elif button == "SELECT":
                    combine = "START"
                else:
                    combine = None
                if combine:
                    try:
                        menu = False
                        #back = False
                        skip_left = False
                        skip_right = False
                        b = config[combine]
                        if b.startswith("button_"):
                            b = int(b[7:])
                            if joystick.joy_object.get_button(b):
                                menu = True
                        b = config["SKIP_LEFT"]
                        if b.startswith("button_"):
                            b = int(b[7:])
                            if joystick.joy_object.get_button(b):
                                skip_left = True
                        b = config["SKIP_RIGHT"]
                        if b.startswith("button_"):
                            b = int(b[7:])
                            if joystick.joy_object.get_button(b):
                                skip_right = True
                        if menu:
                            if skip_left and skip_right:
                                print("ABORT")
                                return "ABORT", joystick.id
                            print("MENU")
                            return "MENU", joystick.id
                    except KeyError:
                        pass
                #if button:
                #    #print("setting last device to", joystick.id, "event", event)
                #    #cls.last_device = joystick.id
                return button, joystick.id
            except KeyError:
                return None, None
            return None, None

    """
    @classmethod
    def handle_keydown_event(cls, event):
        #global current_button

        if event.key == pygame.K_LEFT or event.key == pygame.K_KP4:
            cls.current_button = "LEFT"
        elif event.key == pygame.K_RIGHT or event.key == pygame.K_KP6:
            cls.current_button = "RIGHT"
        elif event.key == pygame.K_PAGEUP or event.key == pygame.K_LSHIFT:
            cls.current_button = "FLEFT"
        elif event.key == pygame.K_PAGEDOWN or event.key == pygame.K_RSHIFT:
            cls.current_button = "FRIGHT"
        elif event.key == pygame.K_RETURN or event.key == pygame.K_SPACE:
            cls.current_button = "OK"
        elif event.key == pygame.K_BACKSPACE:
            cls.current_button = "BACK"
        elif event.key == pygame.K_ESCAPE:
            cls.current_button = "QUIT"
"""

    @classmethod
    def handle_event(cls, event):
        down_event = False
        up_event = False
        if event.type == pygame.KEYDOWN or event.type == pygame.JOYBUTTONDOWN:
            virtual_button, device_id = cls.get_virtual_button(event)
            down_event = True
        elif event.type == pygame.KEYUP or event.type == pygame.JOYBUTTONUP:
            virtual_button, device_id = cls.get_virtual_button(event)
            up_event = True
        elif event.type == pygame.JOYHATMOTION:
            axis_name = "%d_%d" % (event.joy, 1000 + event.hat)
            try:
                if cls.axis_status[axis_name] != event.value:
                    up_event = True
            except KeyError:
                pass
            cls.axis_status[axis_name] = event.value
            virtual_button, device_id = cls.get_virtual_button(event)
            if virtual_button:
                down_event = True
        elif event.type == pygame.JOYAXISMOTION:
            virtual_button, device_id = cls.get_virtual_button(event)
            axis_name = "%d_%d" % (event.joy, event.axis)
            if event.value < -0.66:
                if cls.axis_status.setdefault(axis_name, 0) != -1:
                    cls.axis_status[axis_name] = -1
                    down_event = True
            elif event.value > 0.66:
                if cls.axis_status.setdefault(axis_name, 0) != 1:
                    cls.axis_status[axis_name] = 1
                    down_event = True
            elif event.value > -0.33 and event.value < 0.33:
                if cls.axis_status.setdefault(axis_name, 0) != 0:
                    #print("axis status", cls.axis_status[axis_name])
                    cls.axis_status[axis_name] = 0
                    up_event = True

        if down_event:
            if virtual_button:
                cls.current_button = virtual_button
                if virtual_button in cls.repeatable_buttons:
                    t = time.time()
                    cls.repeat_info = [virtual_button, t, t]
                #cls.last_device_id = device_id
                cls.last_device = device_id
        elif up_event:
            if virtual_button:
                #print("up event", event)
                cls.repeat_info = None

    @classmethod
    def update(cls, event=None):
        cls.handle_lirc_events()
        t = time.time()
        if cls.repeat_info:
            #print(cls.repeat_info)
            if t - cls.repeat_info[1] > REPEAT_TRESHOLD:
                if t - cls.repeat_info[2] > REPEAT_INTERVAL:
                    cls.current_button = cls.repeat_info[0]
                    cls.repeat_info[2] = t

        if t > cls.last_joystick_check + 1.0:
            #print("checking joysticks")
            joystick_hash = Joystick.get_joystick_hash()
            if joystick_hash != cls.joystick_hash:
                print("joystick hash changed")
                cls.reinit_joysticks()
                cls.joystick_hash = joystick_hash
                GameCenter.register_user_activity()
            cls.last_joystick_check = t

    @classmethod
    def peek_button(cls):
        button = cls.current_button
        return button

    @classmethod
    def get_button(cls):
        button = cls.current_button
        cls.current_button = None
        return button

    @classmethod
    def clear_current_button(cls):
        current_button = None
        repeat_info = None

    @classmethod
    def handle_lirc_events(cls):
        if not cls.lirc_socket:
            return
        eventlist = pylirc.nextcode(1)
        while eventlist:
            print(eventlist)
            cls.current_button = eventlist[-1]["config"]
            eventlist = pylirc.nextcode(1)

    @classmethod
    def close(cls):
        cls._close_joysticks()
        if cls.lirc_socket:
            pylirc.exit()
            cls.lirc_socket = None

    @classmethod
    def open(cls):
        #last_joystick_count = 0
        #cls._open_joysticks()
        if pylirc:
            configuration = os.path.join(fs.get_home_dir(), ".lircrc")
            if os.path.exists(configuration):
                logger.info("Initializint lirc with config %s" % configuration)
                try:
                    lirc_socket = pylirc.init("ku-game-system", configuration)
                except Exception:
                    logger.exception("Could not connect to lirc")
            else:
                logger.info("Initializint lirc with lirc default config")
                try:
                    lirc_socket = pylirc.init("ku-game-system")
                except Exception:
                    logger.exception("Could not connect to lirc")

    @classmethod
    def _open_joysticks(cls):
        logger.debug("InputHandler._open_joysticks")
        cls.reinit_joysticks()

    @classmethod
    def reinit_joysticks(cls):
        print("\n" + "-" * 79 + "\n" + "INPUTHANDLER REINIT JOYSTICKS")
        #print("reinit_joysticks")
        cls.axis_status = {}
        for joystick in cls.joysticks:
            print("    uninitialize", joystick)
            joystick.joy_object.quit()
        # call get_devices to force InputDevice class to open/close
        # joysticks before we do here
        devices = InputDevice.get_devices()
        # build key table
        cls.key_table = {}
        for device in devices:
            print("device")
            if device.is_keyboard():
                print("is keyboard device")
            else:
                continue
            config = get_controller_config(device.name, device.sdl_name,
                    axes=device.axes, hats=device.hats,
                    buttons=device.buttons, balls=device.balls)
            print(config)
            if not config:
                continue
            for key, value in config.iteritems():
                if key.startswith("key_"):
                    cls.key_table["SDLK_" + key[4:].upper()] = \
                            (value.upper(), device.id)
        from pprint import pprint
        pprint(cls.key_table)
        # now we re-initialize all joysticks
        if pygame.joystick.get_init():
            print("joystick count", pygame.joystick.get_count())
            #print("joystick.quit")
            pygame.joystick.quit()
        #print("joystick.init")
        pygame.joystick.init()
        print("joystick count", pygame.joystick.get_count())

        old_devices = []
        for joystick in cls.joysticks:
            old_devices.append(joystick.id)
        old_devices.sort()
        old_devices_set = set(old_devices)

        cls.joysticks = []
        print("PYGAME JOYSTICK COUNT ==>", pygame.joystick.get_count())
        print("calling InputDevice.get_system_input_devices")
        joysticks = InputDevice.get_system_input_devices()
        print("InputHandler controllers:")
        new_devices = []
        for joystick in joysticks:
            print("  - {0}".format(joystick.id))
            joy_object = pygame.joystick.Joystick(joystick.index)
            joy_object.init()
            joystick.joy_object = joy_object
            #print(joy_object, joy_object.get_name())
            cls.joysticks.append(joystick)
            new_devices.append(joystick.id)
            cls.device_sdl_names[joystick.id] = joystick.sdl_name
        #print("OLD:", old_devices)
        #print("NEW:", new_devices)
        new_devices.sort()
        new_devices_set = set(new_devices)
        if cls.first_init:
            #print("first init..")
            cls.first_init = False
        else:
            #print("send notifications...")
            for device_id in new_devices:
                if not device_id in old_devices_set:
                    try:
                        device = InputDevice("menu", device_id, [],
                                sdl_name=cls.device_sdl_names[device_id],
                                version=2)
                    except Exception:
                        device_name = device_id
                        Notification("Unsupported device:\n" + device_name)
                    else:
                        device_name = device.name
                    Notification("Device connected:\n" + device_name)
            for device_id in old_devices:
                if not device_id in new_devices_set:
                    try:
                        device = InputDevice("menu", device_id, [],
                                sdl_name=cls.device_sdl_names[device_id],
                                version=2)
                    except Exception:
                        device_name = device_id
                    else:
                        device_name = device.name
                    Notification("Device disconnected:\n" + device_name)

    @classmethod
    def _close_joysticks(cls):
        pygame.joystick.quit()

