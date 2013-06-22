from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import re
#import wx
import time
import pygame
import traceback
from ConfigParser import ConfigParser
import subprocess
import pkg_resources
from fengestad import fs
from .joystick import Joystick
from fsbc.Application import Application
from fsgs import fsgs
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class InputDeviceNotFoundException(Exception):
    pass


class MissingPlatformSupportException(Exception):
    pass


class InputDeviceInfo:
    id = ""
    generic_id = ""
    sdl_name = ""
    name = ""
    buttons = 0
    axes = 0
    hats = 0
    balls = 0
    index = 0

    def get_type(self):
        if self.sdl_name:
            return 'TYPE_JOYSTICK'
        else:
            return 'TYPE_KEYBOARD'


class InputDevice:

    MissingPlatformSupportException = MissingPlatformSupportException 

    def __init__(self, platform, name, sclist, sdl_name="", sdl_joystick_id=-1,
            version=1, buttons=0, axes=0, hats=0, balls=0):
        """
        sclist -- system controller list
        """
        self.type = ""
        self.config = None
        self.config_inv = None
        self.id = name
        #self._name = name
        if "#" in self.id:
            self.name, dummy = self.id.rsplit('#', 1)
        else:
            self.name = self.id
        self.name = self.name.strip()
        self.decorate_name_with_number()
        #print("********** InputDevice Constructor", self.name, "ID is",
        #        self.id)
        self.platform = platform
        self.sdl_name = sdl_name
        self.sdl_joystick_id = sdl_joystick_id
        self.index = -1
        self.version = version
        self.buttons = buttons
        self.axes = axes
        self.hats = hats
        self.balls = balls
        for sc in sclist:
            #print("---", name, "---", sc.id, sc.index)
            if name == sc.id:
                self.index = sc.index
                break

        if version == 1:
            #if platform == "Amiga":
            method_name = "configure_for_" + platform.lower().replace(
                    " ", "_").replace("-", "_")
            #if hasattr(self, method_name):
            #    if self.get_type() == "TYPE_KEYBOARD":
            #        num = self.count_keyboard_input_devices(input_devices) + 1
            #    elif self.get_type() == "TYPE_JOYSTICK":
            #        num = self.count_joystick_input_devices(input_devices) + 1
            #    getattr(self, method_name)(num)
            #else:
            #    raise Exception("No controller support for platform")
            if hasattr(self, method_name):
                getattr(self, method_name)(name, sclist=sclist)
            else:
                logger.info("{0} does not support platform {1}".format(
                        name, platform))
                raise MissingPlatformSupportException(
                        "No input device support for platform")
        elif version == 2:
            #if self.platform:
            old_name = self.name
            self.configure_version_2()
            #else:
            #    self.configure_version_2()
            if self.name != old_name:
                #print("****** name is now", self.name)
                print(self.id, "=>", repr(self.name))

    def decorate_name_with_number(self):
        #print("decorate_name_with_number, ID is", self.id)
        if "#" in self.id:
            dummy, number_str = self.id.rsplit('#', 1)
        else:
            number_str = "1"
        if number_str != '1':
            self.name = "{0} #{1}".format(self.name, number_str)

    #def get_name(self):
    #    return self._name
    #def set_name(self, name):
    #    import traceback
    #    traceback.print_stack()
    #    print("NEW NAME:", name)
    #    self._name = name
    #name = property(get_name, set_name)

    #@property
    #def id(self):
    #    return self.name

    def is_joystick(self):
        return self.get_type() == 'TYPE_JOYSTICK'

    def is_keyboard(self):
        return self.get_type() == 'TYPE_KEYBOARD'

    def get_type(self):
        if self.type.startswith('joystick'):
            return 'TYPE_JOYSTICK'
        elif self.type.startswith('keyboard'):
            return 'TYPE_KEYBOARD'
        else:
            raise Exception("unknown input device type " + repr(self.type))


    @staticmethod
    @fs.memoize
    def get_config_files():
        application = Application.instance()
        search_dirs = []
        for dir in application.data_dirs():
            search_dirs.append(os.path.join(dir, "input"))
            search_dirs.append(os.path.join(dir, "fengestad.input", "res"))
        # files from latter dirs overwrite earlier dirs
        search_dirs.sort(reverse=True)

        try:
            base_dir = pkg_resources.resource_filename('fengestad.input', 'res')
            search_dirs.append(base_dir)
        except Exception:
            pass

        configs = {}
        for base_dir in search_dirs:
            print("read input configs from", base_dir)
            if os.path.exists(base_dir):
                for dir_path, dir_names, file_names in os.walk(base_dir):
                    for file_name in file_names:
                        name, ext = os.path.splitext(file_name)
                        if ext in [".ini", ".conf"]:
                            folder = os.path.basename(dir_path)
                            path = os.path.join(dir_path, file_name)
                            print(" -", path)
                            configs[folder + '_' + name] = path
                            configs[name] = path

        # FIXME: fix dependency
        controllers_dir = fsgs.amiga.get_controllers_dir()
        print("read configs from controllers_dir at", controllers_dir)
        if os.path.exists(controllers_dir):
            for file_name in os.listdir(controllers_dir):
                if file_name.endswith(".conf"):
                    name, ext = os.path.splitext(file_name)
                    path = os.path.join(controllers_dir, file_name)
                    print(" -", path)
                    configs[name] = path
        # print("input config files:", configs)
        return configs

    def read_config(self, config_name, config, platform, multiple):
        #print("read_config ", config_name)
        configs = self.get_config_files()
        try:
            path = configs[config_name]
        except KeyError:
            #traceback.print_stack()
            print("no config file found for", repr(self.sdl_name), "=",
                    config_name)
            #raise InputDeviceNotFoundException(
            #        "no config found for " + repr(self.sdl_name))
            if platform:
                raise MissingPlatformSupportException(
                        "no config found for " + repr(self.sdl_name))
            else:
                return
        cp = ConfigParser()
        #print(path, os.path.exists(path))
        cp.read(path)
        if cp.has_option('device', 'type'):
            self.type = cp.get('device', 'type')
        if cp.has_option('device', 'name'):
            #print("HAD NAME", self.name, self.sdl_name)
            self.name = cp.get('device', 'name')
            self.decorate_name_with_number()
            #print("HAS NAME", self.name, self.sdl_name)
            #try:
            #    dummy, num = self.name.rsplit('#', 1)
            #except ValueError:
            #    self.name = name
            #else:
            #    self.name = "{0} #{1}".format(name, num)
        if cp.has_section(platform):
            section = platform
        elif cp.has_section('default'):
            #print("has default section")
            section = 'default'
        else:
            if platform:
                raise MissingPlatformSupportException(
                        "no config found for platform " + repr(
                        platform))
            else:
                return
        #config = {}
        #if section == 'gamepad':
        #    
            #for option in cp.options('gamepad'):
            #    value = cp.get('gamepad', option)
            #    print("gamepad", option, value)

        if cp.has_option(section, 'include'):
            include_config = cp.get(section, 'include')
            include_config = include_config.replace('/', '_')
            self.read_config(include_config, config, platform, multiple)

        #iconfig = {}
        #for key, value in config.iteritems():
        #    iconfig[value] = key

        #for key in cp.options(section):
        #    value = cp.get(section, key)
        for key, value in cp.items(section):
            value = value.strip()
            if value.startswith('('):
                if not multiple:
                    continue
                assert value.endswith(')')
                value = value[1:-1]
            #print(key, "===>", value)
            #print("key, value is", key, value)
            #if value in iconfig:
            try:
                #config[key] = iconfig[value]
                #config[key] = config[value]
                #del config[value]
                #config[config[value]] = value
                
                #if key in iconfig:
                config[key] = config[value]
                #del iconfig[key]
                #iconfig[config[value]] = key
                #if not config[value] in iconfig:
                #    iconfig[config[value]] = key
                del config[value]
            except KeyError:
                config[key] = value
                #if not value in iconfig:
                #    iconfig[value] = key
                #config_order.append(key)

            #config[option] = cp.get(section, option)

        #if cp.has_section('gamepad'):
        #    for key, value in list(config.iteritems()):
        #        if cp.has_option('gamepad', value):
        #            config[key] = cp.get('gamepad', value)

    def configure_version_2(self):
        self.config = self.configure(self.platform)

    def configure(self, platform, multiple=True):
        #print("CONFIGURE", platform, self.name, self.sdl_name)
        #print("InputDevice.configure")
        
        name_lower = self.sdl_name.lower()
        name = ""
        for c in name_lower:
            if c in "abcdefghijklmnopqrstuvwxyz0123456789":
                name = name + c
            else:
                if not name.endswith("_"):
                    name = name + "_"
        name = name.strip("_")
        if fs.windows:
            host_platform = "windows"
        elif fs.macosx:
            host_platform = "macosx"
        elif fs.linux:
            host_platform = "linux"
        else:
            host_platform = "other"
        config_name = "{0}_{1}_{2}_{3}_{4}_{5}".format(name, self.buttons,
                self.axes, self.hats, self.balls, host_platform)

        config = {}
        try:
            self.read_config(config_name, config, platform, multiple)
        except Exception:
            pass
        else:
            return config

        config_name = ""
        for c in self.sdl_name.lower():
            if c in 'abcdefghijklmnopqrstuvwxyz0123456789':
                config_name += c
            elif len(config_name) == 0 or config_name[-1] != '_':
                config_name += '_'
        if config_name.endswith('_usb'):
            config_name = config_name[:-4]
        while config_name.endswith('_'):
            config_name = config_name[:-1]
        #print("config_name =", config_name, "sdl_name", repr(self.sdl_name))
        self.read_config(config_name, config, platform, multiple)
        return config

    @staticmethod
    def supported_keyboard_input_devices():
        return "", 0

    def get_name(self):
        return self.name

    def get_sdl_joystick_id(self):
        return self.sdl_joystick_id

    def get_config(self):
        if self.config is None:
            raise Exception("InputDevice is not configured")
        return self.config

    def get_config_inverted(self):
        if self.config_inv is not None:
            return self.config_inv
        if self.config is None:
            raise Exception("InputDevice is not configured")
        self.config_inv = {}
        for k, v in self.config.iteritems():
            self.config_inv[v] = k
        return self.config_inv

    @classmethod
    def get_system_input_devices(cls):
        #import traceback
        #traceback.print_stack()
        try:
            input_devices = cls.get_system_input_devices_pygame()
        except ImportError:
            input_devices = cls.get_system_input_devices_wx()
        # Add enumerators to controller names
        ccount = {}
        #retval = []
        #for name, buttons, axes, joy_num, real_name in input_devices:

        #k = 1
        #for controller in input_devices[:]:
        #    #if controller.get_type() == 'TYPE_JOYSTICK':
        #    input_devices.append("JOYSTICK #%d" % (k,))
        #    k += 1

        for i, controller in enumerate(input_devices):
            count = ccount.setdefault(controller.name, 0)
            controller.generic_id = "JOYSTICK #%d" % (i + 1,)
            #retval.append(["%s #%d"  % (name, count + 1),
            #        name, buttons, axes, real_name])
            if count == 0:
                controller.id = "%s"  % (controller.name)
            else:
                controller.id = "%s #%d"  % (controller.name, count + 1)
            
            ccount[controller.name] = ccount[controller.name] + 1

        return input_devices
        #return retval

    @classmethod
    def get_system_input_devices_pygame(cls):
        #print("\n\n\nGET SYSTEM INPUT DEVICES PYGAME")
        print("************** GET SYSTEM INPUT DEVICES PYGAME *******************")
        input_devices = []
        manufacturers = cls.get_controller_manufacturers()
        #print("\n\n")
        #print("\n\n")
        #print("\n\n")
        #must_init_joystick = not pygame.joystick.get_init()
        #if must_init_joystick:
        #pygame.joystick.
        #pygame.joystick.init()
        #if pygame.joystick.get_init():

        pygame.joystick.quit()
        pygame.joystick.init()

        # for some reason (pygame / SDL bug?), must call get_count, and
        # then quit / re-init to be sure to detect joysticks properly
        count = pygame.joystick.get_count()
        print("... joystick count is", count, "activating pygame workaround")
        pygame.joystick.quit()
        pygame.joystick.init()
        count = pygame.joystick.get_count()
        print("... joystick count is", count)

        for i in range(0, count):
            joy = pygame.joystick.Joystick(i)
            joy.init()
            controller = InputDeviceInfo()
            controller.sdl_name = joy.get_name()
            name = joy.get_name().strip().upper()
            # Quick hack to remove superficious spaces
            #name = name.replace("    ", " ")
            #for dummy in range(0, 10):
            #    name = name.replace("  ", " ")

            try:
                name = "0x%04x %s" % (manufacturers[joy.get_name()], name)
            except KeyError:
                pass
            controller.name = name
            controller.buttons = joy.get_numbuttons()
            controller.axes = joy.get_numaxes()
            controller.hats = joy.get_numhats()
            controller.balls = joy.get_numballs()
            controller.index = i
            #input_devices.append([name, buttons, axes, i, real_name])
            input_devices.append(controller)
            print("[J] {0}: {1} btns {2} axes {3} hats {4} balls".format(
                    joy.get_name(), controller.buttons, controller.axes, 
                    controller.hats, controller.balls))
            joy.quit()
        return input_devices

    @classmethod
    def get_system_input_devices_wx(cls):
        print("GET SYSTEM INPUT DEVICES WX")
        input_devices = []
        joy_count = wx.Joystick.GetNumberJoysticks(wx.Joystick(wx.JOYSTICK1))
        for i in range(joy_count):
            joy = wx.Joystick(wx.JOYSTICK1 + i)
            controller = InputDeviceInfo()
            real_name = joy.GetProductName()
            print("  SYSTEM CONTROLLER (WX)", real_name)
            name = joy.GetProductName().strip().upper()
            # Quick hack to remove superficious spaces
            name = name.replace("    ", " ")
            for dummy in range(0, 10):
                name = name.replace("  ", " ")
            name = "0x%04x %s" % (joy.GetManufacturerId(), name)
            controller.name = name
            controller.buttons = joy.GetNumberButtons()
            controller.axes = joy.GetNumberAxes()
            controller.hats = 0
            controller.balls = 0
            controller.index = i            #pov = joy.HasZ()
            input_devices.append(controller)
            print(" ", controller.name)
            print("  with %d buttons" % controller.buttons)
            print("  with %d axes" % controller.axes)
            print("  with %d hats" % controller.hats)
            print("  with %d balls" % controller.balls)
        return input_devices

    @classmethod
    def get_controller_manufacturers(cls):
        if not fs.windows:
            # Information not needed on Linux. Manufacturer name is part
            # of joystick name already.
            return {}

        # this does not work on Windows 7 ?
        return {}
        """
        pyapp.app.require_wx_app()
        input_devices = {}
        joy_count = wx.Joystick.GetNumberJoysticks(wx.Joystick(wx.JOYSTICK1))
        print("joy count", joy_count)
        for i in range(joy_count):
            joy = wx.Joystick(wx.JOYSTICK1 + i)
            print(joy.GetButtonState())
            print(joy.GetProductName(), joy.GetProductId(), joy.GetManufacturerId())
            input_devices[joy.GetProductName()] = joy.GetManufacturerId()
        print("get_controller_manufacturers", input_devices)
        return input_devices
        """

    @classmethod
    def get_preferred_input_devices(cls, platform):
        input_devices = []
        k = 0
        while True:
            c = fs.config_string("InputDevice/%d" % k)
            k += 1
            if c is None:
                break
            c = c.upper()
            if ":" in c:
                p, c = c.split(":")
                if p.strip() == platform.upper():
                    input_devices.append(c.strip())
            else:
                input_devices.append(c.strip())

        # prefer Amiga/C64-like controllers for those platforms
        if platform in ['Amiga', 'Atari 2600', 'Commodore 64']:
            input_devices.append('MOSIC SPEED-LINK COMPETITION PRO')
            input_devices.append('MOSIC SPEED-LINK COMPETITION PRO')
            input_devices.append('SPEED-LINK COMPETITION PRO')
            input_devices.append('SPEED-LINK COMPETITION PRO')

        # match up to 10 recognized and connected joysticks
        for i in range(10):
            input_devices.append("JOYSTICK")

        # always append two keybord options last
        #for i in range(2):
        #    input_devices.append("KEYBOARD")
        input_devices.append("KEYBOARD")
        input_devices.append("NONE")

        # add enumerators to controller names
        ccount = {}
        retval = []
        already_seen = set()
        for name in input_devices:
            if "#" in name:
                basename = name.split("#")[0].strip()
                number = int(name.split("#")[1].strip())
                fullname = "%s #%d"  % (basename, number)
                already_seen.add(fullname)
                retval.append(fullname)
                continue
            count = ccount.setdefault(name, 0)
            while True:
                fullname = "%s #%d"  % (name, count + 1)
                if fullname in already_seen:
                    count = count + 1
                    continue
                retval.append(fullname)
                ccount[name] = ccount[name] + 1
                break
        return retval

    cached_input_devices = []
    device_list_version = None

    @classmethod
    def get_input_devices(cls, platform, min_count, max_count, version=1):
        print("get %d controller(s) for %s, version %d" % (min_count, platform, version))
        if version == 2:
            joystick_hash = Joystick.get_joystick_hash()
            if joystick_hash == cls.device_list_version:
                #print(" -- cached --", cls.device_list_version, cls.cached_input_devices)
                return cls.cached_input_devices, []

            sclist = cls.get_system_input_devices()
            # FIXME: WHY IS THIS NEEDED TWICE?
            # When starting game center without joysticks connected, and
            # then connecting one, the first call to get_syste_input_devices
            # does not detect any... strange..
            # DONE: added workaround in get_system_input_devices
            #sclist = cls.get_system_input_devices()
            print("system input_devices:", sclist)
            input_devices = []
            #klass = JoystickInputDevice
            for sc in sclist:
                try:
                    controller = JoystickInputDevice(platform, sc.id, sclist,
                            sdl_name=sc.sdl_name, sdl_joystick_id=sc.index,
                            version=version, hats=sc.hats, axes=sc.axes,
                            buttons=sc.buttons, balls=sc.balls)
                    #if controller.get_type() == 'TYPE_JOYSTICK':
                    #    #controller.sdl_joystick_id = sc.index
                    #    #controller.sdl_name = sc.sdl_name
                    input_devices.append(controller)
                except MissingPlatformSupportException:
                    print(" (missing platform support, skipping)")
                except InputDeviceNotFoundException:
                    print(" (input device not found, skipping)")
            k = 0
            while True:
                c = fs.config_string("InputDevice/%d" % k)
                k += 1
                if c is None:
                    break
                id = c
                if not '#' in id:
                    id = id + " #1"
                controller = KeyboardInputDevice(platform, id, sclist,
                        sdl_name=c, sdl_joystick_id=-1, version=version)
                input_devices.append(controller)
            controller = KeyboardInputDevice(platform, "KEYBOARD", sclist,
                    sdl_name="Generic Keyboard",
                    sdl_joystick_id=-1, version=version)
            input_devices.append(controller)
            controller = MouseInputDevice(platform, "MOUSE", sclist,
                    sdl_name="Generic Mouse",
                    sdl_joystick_id=-1, version=version)
            input_devices.append(controller)
            #if len(input_devices) < min_count:
            #    raise Exception("Did not find enough input_devices")
            print("found {0} input devices(s)".format((len(input_devices), input_devices)))
            cls.cached_input_devices = input_devices
            cls.device_list_version = joystick_hash
            #return input_devices, sclist
            return input_devices, []

        pclist = cls.get_preferred_input_devices(platform)
        print("Preferred input_devices:", pclist)
        sclist = cls.get_system_input_devices()
        print("System input_devices:", sclist)
        input_devices = []
        already_used_sc_ids = set()
        aliases = {}
        ctrlclasses = {}
        print("Resolving controller extensions")
        for ext in pyapp.ext.ExtensionHook(
                "exthook:no.fengestad.input/device"):
            klass = ext.object
            for name in ext.models:
                ctrlclasses[name.upper()] = klass

            primary_name = ext.models[0]
            for name in ext.models:
                for i in range(1, 10):
                    aliases["%s #%d" % (name.upper(), i)] = \
                            "%s #%d" % (primary_name.upper(), i)
            # FIXME!!
            #sclist.extend(klass.get_keyboard_input_devices())
            kname, kcount = klass.supported_keyboard_input_devices()
            for i in range(kcount):
                controller = InputDeviceInfo()
                controller.name = kname
                # forge sdl_name to get config file support
                controller.sdl_name = 'generic keyboard'
                controller.id = "%s #%d" % (kname, i + 1)
                sclist.append(controller)

        print("Find input_devices:")

        for prefctrl in pclist:
            if len(input_devices) == max_count:
                break
            for sc in sclist:
                if prefctrl.upper() == sc.id or prefctrl == sc.generic_id:
                    if sc.id in already_used_sc_ids:
                        continue
                    for ccname, cc in ctrlclasses.iteritems():
                        print("--- ccname", ccname)
                        if ccname.startswith('^'):
                            print("reg")
                            if re.match(ccname, sc.name.upper()) is not None:
                                print("match")
                                klass = cc
                                break
                        elif sc.name.upper() == ccname:
                            klass = cc
                            break
                    else:
                        print(" (did not find controller class for %s)" % (
                                sc.name,))
                        break
                    print(" --> Using controller", prefctrl)
                    try:
                        controller = klass(platform, sc.id, sclist,
                                sdl_name=sc.sdl_name,
                                sdl_joystick_id=sc.index,
                                version=version, hats=sc.hats, axes=sc.axes,
                                buttons=sc.buttons, balls=sc.balls)
                        #controller.hats = sc.hats
                        #controller.axes = sc.axes
                        #controller.buttons = sc.buttons
                        #if controller.get_type() == 'TYPE_JOYSTICK':
                        #    #controller.sdl_joystick_id = sc.index
                        #    #controller.sdl_name = sc.sdl_name
                        input_devices.append(controller)
                        already_used_sc_ids.add(sc.id)
                        break
                    except MissingPlatformSupportException:
                        print(" (missing platform support, skipping)")
                    except InputDeviceNotFoundException:
                        print(" (input device not found, skipping)")
        else:
            if len(input_devices) < min_count:
                raise Exception("did not find enough input_devices")
        print("found %d input devices(s)" % (len(input_devices),))
        return input_devices, sclist

    @classmethod
    def get_devices(cls, platform=None, min_count=0, max_count=None,
            device_order='SDL', device_list=None, version=2):
        print("InputDevice.get_devices, device_order = ", device_order)
        if max_count is None:
            max_count = min_count
        devices, info = InputDevice.get_input_devices(platform,
                min_count, max_count, version)
        if device_list is not None:
            device_list[:] = info
        if device_order == 'DINPUT8':
            #plugin = pyapp.plugins.get_plugin('no.fengestad.input.platform')
            #path = os.path.join(plugin.get_bin_dir(), 'dinput8.exe')
            path = pkg_resources.resource_filename('fengestad.input', 
                    os.path.join('res', 'dinput8.exe'))
            # Must specify all files (stdin/stdout/stderr) when running in GUI mode
            # on Windows
            p = subprocess.Popen([path], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT)
            p.stdin.close()
            data = p.stdout.read()
            #print(data)
            order = []
            number = {}
            sdl_mapping = []
            #for device_info in info:
            for device_info in devices:
                print(" * {0} ({1})".format(device_info.id, device_info.sdl_name))
                sdl_mapping.append((device_info.sdl_name.strip(),
                        device_info.id))
            for i, line in enumerate(data.split('\n')):
                if not line.strip():
                    continue
                if i % 2 == 0:
                    line = line.strip()
                    #number[line] = number.get(line, 0) + 1
                    #order.append(line + " #" + str(number[line]))
                    for key, value in sdl_mapping:
                        #print(key, " -- vs --", line)
                        if key == line:
                            order.append(value)
                            sdl_mapping.remove((key, value))
                            break
                    else:
                        order.append("UNSUPPORTED " + line)
        elif device_order == 'ALPHABETICAL':
            joystick_list = []
            #for device_info in info:
            for device_info in devices:
                if device_info.get_type() == 'TYPE_JOYSTICK':
                    joystick_list.append((device_info.sdl_name,
                            device_info.id))
            order = [x[1] for x in sorted(joystick_list)]
            #order = sorted([(x.sdl_name, x.id) for x in info])
            #order = [x[1] for x in order]
            #order = sorted([x.name for x in devices])
        else:
            joystick_list = []
            #for device_info in info:
            for device_info in devices:
                if device_info.get_type() == 'TYPE_JOYSTICK':
                    joystick_list.append((device_info.sdl_name,
                            device_info.id))
            order = [x[1] for x in joystick_list]
            #order = [x.id for x in info]
            #order = [x.name for x in devices]
        print("device order is " + repr(order))
        for device in devices:
            #print("--->", device.name)
            if device.get_type() == 'TYPE_JOYSTICK':
                device.index = order.index(device.id)
        # print list of devices sorted by device index
        for device in [x[1] for x in sorted([(x.index, x) for x in devices])]:
            if device.get_type() == 'TYPE_JOYSTICK':
                print("device index {0:2d}: {1}".format(device.index, device.id))
        return devices


class JoystickInputDevice(InputDevice):

    @staticmethod
    def get_type():
        return "TYPE_JOYSTICK"

    def configure_for_amiga(self, ctrlid, sclist=[], **options):
        self.config = {}


class MouseInputDevice(InputDevice):

    @staticmethod
    def get_type():
        return "TYPE_MOUSE"


class KeyboardInputDevice(InputDevice):

    @staticmethod
    def get_type():
        return "TYPE_KEYBOARD"

    def configure_for_amiga(self, ctrlid, **options):
        self.config = {}
        # FIXME: CORRECT NAMES HERE
        if ctrlid[-2:] == "#1":
            self.config['UP'] = 'UP'
            self.config['DOWN'] = 'DOWN'
            self.config['LEFT'] = 'LEFT'
            self.config['RIGHT'] = 'RIGHT'
            self.config['FIRE'] = 'RCTRL'
        elif ctrlid[-2:] == "#2":
            self.config['UP'] = 'KP8'
            self.config['DOWN'] = 'KP2'
            self.config['LEFT'] = 'KP4'
            self.config['RIGHT'] = 'KP6'
            self.config['FIRE'] = 'KP0'
        else:
            raise InputDeviceNotFoundException("3 Player input not support "
                    "on the Amiga")

class CompatibleInputDevice(JoystickInputDevice):

    def get_compatible_config(self):
        raise InputDeviceNotFoundException("get_compatible_config not impl.")

    def configure_for_compatible(self, ctrlid, **options):
        self.config = self.get_compatible_config()

    def configure_for_menu(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']                = compat['D_PAD_UP']
        cfg['DOWN']              = compat['D_PAD_DOWN']
        cfg['LEFT']              = compat['D_PAD_LEFT']
        cfg['RIGHT']             = compat['D_PAD_RIGHT']
        cfg['SELECT']            = compat['SELECT_BUTTON']
        cfg['START']             = compat['START_BUTTON']
        cfg['PRIMARY']           = compat['SOUTH_BUTTON']
        cfg['SECONDARY']         = compat['WEST_BUTTON']
        cfg['TERTIARY']          = compat['NORTH_BUTTON']
        cfg['BACK']              = compat['EAST_BUTTON']
        cfg['SKIP_LEFT']         = compat['LEFT_SHOULDER']
        cfg['SKIP_RIGHT']        = compat['RIGHT_SHOULDER']
        cfg['MENU']              = compat['MENU_BUTTON']
        cfg['POINTER_SCROLL']    = compat['R_STICK_UP'], compat['R_STICK_DOWN']
        cfg['POINTER_UPDOWN']    = compat['L_STICK_UP'], compat['L_STICK_DOWN']
        cfg['POINTER_LEFTRIGHT'] = compat['L_STICK_LEFT'], \
                                   compat['L_STICK_RIGHT']
        self.config = cfg

    def configure_for_amiga(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']                = compat['D_PAD_UP']
        cfg['DOWN']              = compat['D_PAD_DOWN']
        cfg['LEFT']              = compat['D_PAD_LEFT']
        cfg['RIGHT']             = compat['D_PAD_RIGHT']
        #cfg['UP']                = compat['L_STICK_UP']
        #cfg['DOWN']              = compat['L_STICK_DOWN']
        #cfg['LEFT']              = compat['L_STICK_LEFT']
        #cfg['RIGHT']             = compat['L_STICK_RIGHT']
        cfg['FIRE']              = compat['SOUTH_BUTTON']
        self.config = cfg

    def configure_for_nintendo_64(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['START']             = compat['START_BUTTON']
        cfg['STICK_UP']          = compat['L_STICK_UP']
        cfg['STICK_DOWN']        = compat['L_STICK_DOWN']
        cfg['STICK_LEFT']        = compat['L_STICK_LEFT']
        cfg['STICK_RIGHT']       = compat['L_STICK_RIGHT']
        cfg['D_PAD_UP']          = compat['D_PAD_UP']
        cfg['D_PAD_DOWN']        = compat['D_PAD_DOWN']
        cfg['D_PAD_LEFT']        = compat['D_PAD_LEFT']
        cfg['D_PAD_RIGHT']       = compat['D_PAD_RIGHT']
        cfg['A']                 = compat['SOUTH_BUTTON']
        cfg['B']                 = compat['WEST_BUTTON']
        cfg['C_UP']              = compat['R_STICK_UP']
        cfg['C_DOWN']            = compat['R_STICK_DOWN']
        cfg['C_LEFT']            = compat['R_STICK_LEFT']
        cfg['C_RIGHT']           = compat['R_STICK_RIGHT']
        cfg['L']                 = compat['LEFT_SHOULDER']
        cfg['R']                 = compat['RIGHT_SHOULDER']
        cfg['Z']                 = compat['RIGHT_TRIGGER']
        cfg['RUMBLEPAK']         = compat['SELECT_BUTTON']
        cfg['MEMPAK']            = compat['R_STICK_BUTTON']
        self.config = cfg

    def configure_for_gamecube(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['START']             = compat['START_BUTTON']
        cfg['STICK_UP']          = compat['L_STICK_UP']
        cfg['STICK_DOWN']        = compat['L_STICK_DOWN']
        cfg['STICK_LEFT']        = compat['L_STICK_LEFT']
        cfg['STICK_RIGHT']       = compat['L_STICK_RIGHT']
        cfg['Z_STICK_UP']        = compat['R_STICK_UP']
        cfg['Z_STICK_DOWN']      = compat['R_STICK_DOWN']
        cfg['Z_STICK_LEFT']      = compat['R_STICK_LEFT']
        cfg['Z_STICK_RIGHT']     = compat['R_STICK_RIGHT']
        cfg['D_PAD_UP']          = compat['D_PAD_UP']
        cfg['D_PAD_DOWN']        = compat['D_PAD_DOWN']
        cfg['D_PAD_LEFT']        = compat['D_PAD_LEFT']
        cfg['D_PAD_RIGHT']       = compat['D_PAD_RIGHT']
        cfg['A']                 = compat['SOUTH_BUTTON']
        cfg['B']                 = compat['WEST_BUTTON']
        cfg['X']                 = compat['EAST_BUTTON']
        cfg['Y']                 = compat['NORTH_BUTTON']
        cfg['Z']                 = compat['SELECT_BUTTON']
        cfg['L']                 = compat['LEFT_SHOULDER']
        cfg['R']                 = compat['RIGHT_SHOULDER']
        cfg['L_ANALOG']          = compat['LEFT_TRIGGER']
        cfg['R_ANALOG']          = compat['RIGHT_TRIGGER']
        self.config = cfg

    def configure_for_4button_analog(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']                = compat['L_STICK_UP']
        cfg['DOWN']              = compat['L_STICK_DOWN']
        cfg['LEFT']              = compat['L_STICK_LEFT']
        cfg['RIGHT']             = compat['L_STICK_RIGHT']
        cfg['1']                 = compat['SOUTH_BUTTON']
        cfg['2']                 = compat['EAST_BUTTON']
        cfg['3']                 = compat['WEST_BUTTON']
        cfg['4']                 = compat['NORTH_BUTTON']
        self.config = cfg

    def configure_for_arcade(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']     = compat['D_PAD_UP']
        cfg['DOWN']   = compat['D_PAD_DOWN']
        cfg['LEFT']   = compat['D_PAD_LEFT']
        cfg['RIGHT']  = compat['D_PAD_RIGHT']
        cfg['SELECT'] = compat['SELECT_BUTTON']
        cfg['START']  = compat['START_BUTTON']
        cfg['1']      = compat['WEST_BUTTON']
        cfg['2']      = compat['SOUTH_BUTTON']
        cfg['3']      = compat['EAST_BUTTON']
        cfg['4']      = compat['NORTH_BUTTON']
        cfg['5']      = compat['LEFT_SHOULDER']
        cfg['6']      = compat['RIGHT_SHOULDER']
        cfg['7']      = compat['LEFT_TRIGGER']
        cfg['8']      = compat['RIGHT_TRIGGER']
        self.config = cfg

    def configure_for_atari_2600(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']      = compat['D_PAD_UP']
        cfg['DOWN']    = compat['D_PAD_DOWN']
        cfg['LEFT']    = compat['D_PAD_LEFT']
        cfg['RIGHT']   = compat['D_PAD_RIGHT']
        cfg['FIRE']    = compat['SOUTH_BUTTON']
        cfg['RESET']   = compat['START_BUTTON']
        cfg['SELECT']  = compat['SELECT_BUTTON']
        self.config = cfg

    def configure_for_atari_7800(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']      = compat['D_PAD_UP']
        cfg['DOWN']    = compat['D_PAD_DOWN']
        cfg['LEFT']    = compat['D_PAD_LEFT']
        cfg['RIGHT']   = compat['D_PAD_RIGHT']
        cfg['1']       = compat['SOUTH_BUTTON']
        cfg['2']       = compat['EAST_BUTTON']
        #cfg['SELECT']  = compat['SELECT_BUTTON']
        self.config = cfg

    def configure_for_atari_lynx(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']      = compat['D_PAD_UP']
        cfg['DOWN']    = compat['D_PAD_DOWN']
        cfg['LEFT']    = compat['D_PAD_LEFT']
        cfg['RIGHT']   = compat['D_PAD_RIGHT']
        cfg['OPTION1'] = compat['START_BUTTON']
        cfg['OPTION2'] = compat['SELECT_BUTTON']
        cfg['PAUSE']   = compat['RIGHT_SHOULDER']
        cfg['A']       = compat['EAST_BUTTON']
        cfg['B']       = compat['SOUTH_BUTTON']
        self.config = cfg

    def configure_for_commodore_64(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['SELECT']  = compat['SELECT_BUTTON']
        cfg['RESET']   = compat['START_BUTTON']
        cfg['UP']      = compat['D_PAD_UP']
        cfg['DOWN']    = compat['D_PAD_DOWN']
        cfg['LEFT']    = compat['D_PAD_LEFT']
        cfg['RIGHT']   = compat['D_PAD_RIGHT']
        cfg['FIRE']    = compat['SOUTH_BUTTON']
        cfg['TRIGGER'] = compat['WEST_BUTTON']
        cfg['BOOSTER'] = compat['NORTH_BUTTON']
        self.config = cfg

    def configure_for_turbografx_16(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['SELECT'] = compat['SELECT_BUTTON']
        cfg['RUN']    = compat['START_BUTTON']
        cfg['UP']     = compat['D_PAD_UP']
        cfg['DOWN']   = compat['D_PAD_DOWN']
        cfg['LEFT']   = compat['D_PAD_LEFT']
        cfg['RIGHT']  = compat['D_PAD_RIGHT']
        cfg['I']      = compat['EAST_BUTTON']
        cfg['II']     = compat['SOUTH_BUTTON']
        #cfg['X']      = compat['NORTH_BUTTON']
        #cfg['Y']      = compat['WEST_BUTTON']
        #cfg['L']      = compat['LEFT_SHOULDER']
        #cfg['R']      = compat['RIGHT_SHOULDER']
        self.config = cfg

    def configure_for_super_nintendo(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['SELECT'] = compat['SELECT_BUTTON']
        cfg['START']  = compat['START_BUTTON']
        cfg['UP']     = compat['D_PAD_UP']
        cfg['DOWN']   = compat['D_PAD_DOWN']
        cfg['LEFT']   = compat['D_PAD_LEFT']
        cfg['RIGHT']  = compat['D_PAD_RIGHT']
        cfg['A']      = compat['EAST_BUTTON']
        cfg['B']      = compat['SOUTH_BUTTON']
        cfg['X']      = compat['NORTH_BUTTON']
        cfg['Y']      = compat['WEST_BUTTON']
        cfg['L']      = compat['LEFT_SHOULDER']
        cfg['R']      = compat['RIGHT_SHOULDER']
        self.config = cfg

    def configure_for_nintendo(self, ctrlid, **options):
        self.configure_for_super_nintendo(ctrlid, **options)

    def configure_for_game_boy(self, ctrlid, **options):
        self.configure_for_super_nintendo(ctrlid, **options)

    def configure_for_game_boy_color(self, ctrlid, **options):
        self.configure_for_super_nintendo(ctrlid, **options)

    def configure_for_game_boy_advance(self, ctrlid, **options):
        self.configure_for_super_nintendo(ctrlid, **options)

    def configure_for_master_system(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']     = compat['D_PAD_UP']
        cfg['DOWN']   = compat['D_PAD_DOWN']
        cfg['LEFT']   = compat['D_PAD_LEFT']
        cfg['RIGHT']  = compat['D_PAD_RIGHT']
        cfg['1']      = compat['SOUTH_BUTTON']
        cfg['2']      = compat['EAST_BUTTON']
        self.config = cfg

    def configure_for_game_gear(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['UP']      = compat['D_PAD_UP']
        cfg['DOWN']    = compat['D_PAD_DOWN']
        cfg['LEFT']    = compat['D_PAD_LEFT']
        cfg['RIGHT']   = compat['D_PAD_RIGHT']
        cfg['START']   = compat['START_BUTTON']
        cfg['1']       = compat['SOUTH_BUTTON']
        cfg['2']       = compat['EAST_BUTTON']
        self.config = cfg

    def configure_for_mega_drive(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['MODE']   = compat['SELECT_BUTTON']
        cfg['START']  = compat['START_BUTTON']
        cfg['UP']     = compat['D_PAD_UP']
        cfg['UP']     = compat['D_PAD_UP']
        cfg['DOWN']   = compat['D_PAD_DOWN']
        cfg['LEFT']   = compat['D_PAD_LEFT']
        cfg['RIGHT']  = compat['D_PAD_RIGHT']
        cfg['A']      = compat['WEST_BUTTON']
        cfg['B']      = compat['SOUTH_BUTTON']
        cfg['C']      = compat['EAST_BUTTON']
        cfg['X']      = compat['NORTH_BUTTON']
        cfg['Y']      = compat['LEFT_SHOULDER']
        cfg['Z']      = compat['RIGHT_SHOULDER']
        self.config = cfg

    """
    def configure_for_nintendo_wii(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['HOME']              = compat['MENU_BUTTON']
        cfg['+']                 = compat['START_BUTTON']
        cfg['-']                 = compat['SELECT_BUTTON']
        cfg['NUNCHUK_UP']        = compat['L_STICK_UP']
        cfg['NUNCHUK_DOWN']      = compat['L_STICK_DOWN']
        cfg['NUNCHUK_LEFT']      = compat['L_STICK_LEFT']
        cfg['NUNCHUK_RIGHT']     = compat['L_STICK_RIGHT']
        cfg['NUNCHUK_SHAKE_X']   = compat['LEFT_TRIGGER']
        cfg['NUNCHUK_SHAKE_Y']   = compat['LEFT_TRIGGER']
        cfg['NUNCHUK_SHAKE_Z']   = compat['LEFT_TRIGGER']
        #cfg['NUNCHUK_MODIFIER']  = compat['']
        cfg['NUNCHUK_C']         = compat['NORTH_BUTTON']
        cfg['NUNCHUK_Z']         = compat['EAST_BUTTON']
        cfg['SHAKE_X']           = compat['RIGHT_TRIGGER']
        cfg['SHAKE_Y']           = compat['RIGHT_TRIGGER']
        cfg['SHAKE_Z']           = compat['RIGHT_TRIGGER']
        cfg['D_PAD_UP']          = compat['D_PAD_UP']
        cfg['D_PAD_DOWN']        = compat['D_PAD_DOWN']
        cfg['D_PAD_LEFT']        = compat['D_PAD_LEFT']
        cfg['D_PAD_RIGHT']       = compat['D_PAD_RIGHT']
        cfg['A']                 = compat['LEFT_SHOULDER']
        cfg['B']                 = compat['RIGHT_SHOULDER']
        cfg['1']                 = compat['WEST_BUTTON']
        cfg['2']                 = compat['SOUTH_BUTTON']
        cfg['IR_UP']             = compat['R_STICK_UP']
        cfg['IR_DOWN']           = compat['R_STICK_DOWN']
        cfg['IR_LEFT']           = compat['R_STICK_LEFT']
        cfg['IR_RIGHT']          = compat['R_STICK_RIGHT']
        self.config = cfg
"""

    def configure_for_wii(self, ctrlid, **options):
        compat = self.get_compatible_config()
        cfg = {}
        cfg['HOME']              = compat['MENU_BUTTON']
        cfg['+']                 = compat['START_BUTTON']
        cfg['-']                 = compat['SELECT_BUTTON']
        cfg['NUNCHUK_UP']        = compat['L_STICK_UP']
        cfg['NUNCHUK_DOWN']      = compat['L_STICK_DOWN']
        cfg['NUNCHUK_LEFT']      = compat['L_STICK_LEFT']
        cfg['NUNCHUK_RIGHT']     = compat['L_STICK_RIGHT']
        cfg['NUNCHUK_SHAKE_X']   = compat['NORTH_BUTTON']
        cfg['NUNCHUK_SHAKE_Y']   = compat['NORTH_BUTTON']
        cfg['NUNCHUK_SHAKE_Z']   = compat['NORTH_BUTTON']
        cfg['NUNCHUK_C']         = compat['LEFT_SHOULDER']
        cfg['NUNCHUK_Z']         = compat['LEFT_TRIGGER']
        cfg['SHAKE_X']           = compat['EAST_BUTTON']
        cfg['SHAKE_Y']           = compat['EAST_BUTTON']
        cfg['SHAKE_Z']           = compat['EAST_BUTTON']
        cfg['D_PAD_UP']          = compat['D_PAD_UP']
        cfg['D_PAD_DOWN']        = compat['D_PAD_DOWN']
        cfg['D_PAD_LEFT']        = compat['D_PAD_LEFT']
        cfg['D_PAD_RIGHT']       = compat['D_PAD_RIGHT']
        cfg['A']                 = compat['RIGHT_SHOULDER']
        cfg['B']                 = compat['RIGHT_TRIGGER']
        cfg['1']                 = compat['WEST_BUTTON']
        cfg['2']                 = compat['SOUTH_BUTTON']
        cfg['IR_UP']             = compat['R_STICK_UP']
        cfg['IR_DOWN']           = compat['R_STICK_DOWN']
        cfg['IR_LEFT']           = compat['R_STICK_LEFT']
        cfg['IR_RIGHT']          = compat['R_STICK_RIGHT']
        self.config = cfg

