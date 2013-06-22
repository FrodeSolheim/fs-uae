from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
import hashlib
import subprocess
import pyapp
from fengestad.gamecenter.gamecontroller import GameController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class MednafenController(GameController):

    def configure(self):
        with open(self.mednafen_cfg_path(), 'wb') as f:
            self._configure_emulator(f)

    def run(self):
        self.env['HOME'] = pyapp.fs.encode(self.context.temp.dir('mednafen'))
        plugin = pyapp.plugins.get_plugin('no.fengestad.emulator.mednafen')
        args = self.args[:]
        if fs.windows:
            temp_dir = self.context.temp.dir('mednafen')
            #data_path = os.path.join(pyapp.fs.data_dir_user_app(),
            #        "mednafen")
            # Dir should have been created by config file writing
            for item in os.listdir(plugin.get_bin_dir()):
                shutil.copyfile(os.path.join(plugin.get_bin_dir(), item),
                        os.path.join(temp_dir, item))
            args.insert(0, os.path.join(temp_dir, 'mednafen.exe'))
            return subprocess.Popen(args, cwd=temp_dir, env=self.env)
        return plugin.mednafen(args, env=self.env)

    def get_supported_filters(self):
        supported = [{
                'name': '2x',
                'special': 'nn2x',
            }, {
                'name': 'none',
                'special': 'none',
            }, {
                'name': 'scale2x',
                'special': 'scale2x',
            }, {
                'name': 'hq2x',
                'special': 'hq2x',
            }
        ]
        return supported

    def _configure_emulator(self, f):
        self.args = []
        pfx = self.get_system_prefix()
        screenw, screenh = self.context.system.screen_size
        destw, desth = screenw, screenh
        aratio = self.force_aspect_ratio()
        gamew, gameh = self.get_game_size()
        real_game_size = (gamew, gameh)

        if aratio:
            logger.info("Forcing aspect ratio %f", aratio)
            gameh = gamew / aratio
        xscale = min(destw / gamew, desth / gameh)
        if aratio:
            yscale = xscale * real_game_size[0] / real_game_size[1] / aratio
        else:
            yscale = xscale
        logger.debug("Fullscreen scale factors: %f %f", xscale, yscale)
        self.args.extend(["-%s.xscalefs" % pfx, str(xscale)])
        self.args.extend(["-%s.yscalefs" % pfx, str(yscale)])

        if self.get_option('fullscreen'):
            self.args.extend(["-fs", "1"])
            #self.args.extend(["-%s.scanlines" % pfx,
            #        str(self.get_scanlines_setting())])
        else:
            self.args.extend(["-fs", "0"])
            #self.args.extend(["-%s.scanlines" % pfx, "0"])
            #if gamew < 200:
            #    self.args.extend(["-%s.xscale" % pfx, "4"])
            #    self.args.extend(["-%s.yscale" % pfx, "4"])
            #else:
            #    self.args.extend(["-%s.xscale" % pfx, "3"])
            #    self.args.extend(["-%s.yscale" % pfx, "3"])
            #self.args.extend(["-%s.videoip" % pfx, "0"])
            #self.args.extend(["-%s.special" % pfx, "none"])
            self.args.extend(["-%s.xscale" % pfx, '2'])
            self.args.extend(["-%s.yscale" % pfx, '2'])

            # FIXME:
            #self.options.smooth = False
            #self.options.filter = '2x'

        self.args.extend(["-%s.xres" % pfx, str(screenw)])
        self.args.extend(["-%s.yres" % pfx, str(screenh)])
        if self.get_option('stretch'):
            self.args.extend(["-%s.stretch" % pfx, "1"])
        else:
            self.args.extend(["-%s.stretch" % pfx, "0"])

        self.args.extend(self.get_extra_graphics_options())

        #xscale = 3
        #if aratio:
        #    yscale = xscale * real_game_size[0] / real_game_size[1] / aratio
        #else:
        #    yscale = xscale
        #self.args.extend(["-%s.xscale" % pfx, str(xscale)])
        #self.args.extend(["-%s.yscale" % pfx, str(yscale)])

        filter_data = self.configure_filter()
        if filter_data['name'] == 'ntsc':
            self.args.extend(['-{0}.ntscblitter'.format(pfx), '1'])
            self.args.extend(['-{0}.ntsc.preset'.format(pfx), 'composite'])
            self.args.extend(['-{0}.ntsc.saturation'.format(pfx), '0.5'])
        else:
            self.args.extend(['-{0}.special'.format(pfx),
                    filter_data['special']])
        if self.get_option('smooth'):
            self.args.extend(["-%s.videoip" % pfx, "1"])

        #self.args.extend(["-%s.special" % pfx, self.get_special_filter()])

        if self.configure_vsync():
            self.args.extend(['-glvsync', '1'])
        else:
            self.args.extend(['-glvsync', '0'])

        #input_platform, input_configs = self.get_mednafen_input_config()
        #if fs.windows:
        #    device_order = 'DINPUT8'
        #else:
        #    device_order = 'SDL'
        #device_list = []
        #devices = self.context.input.get_devices(len(input_configs),
        #        device_order=device_order, device_list=device_list)

        #unique_ids = self.get_joystick_unique_ids(device_list)
        #for i, device in enumerate(devices):
        #    input_config = input_configs[i]
        #    device_config = device.get_config()
        #    print("MEDNAFEN INPUT", i, device)
        #    if device.get_type() == "TYPE_JOYSTICK":
        #        joy_id = unique_ids[device.get_name()]
        #        for key, value in input_config.iteritems():
        #            f.write("%s joystick %x %08x\n" % (key,
        #                    joy_id, calc_button(device_config[value])))
        #            #f.write("%s joystick %x %08x\n" % (key,
        #            #        joy_id, ctrl_config[value]))
        #            pass
        #        #joy_id = self.get_joystick_unique_id(
        #        #        scdict[controller.get_name()])
        #    else:
        #        for key, value in input_config.iteritems():
        #            f.write("%s keyboard %d\n" % (key,
        #                    calc_button(device_config[value])))

        for i, input in enumerate(self.inputs):
            input_mapping = self.get_input_mapping(i)
            mapper = InputMapper(input, input_mapping)
            for key, value in mapper.iteritems():
                f.write('{key} {value}\n'.format(
                        key=key, value=value))

        state_dir = self.context.game.state_dir
        docdir = pyapp.user.documents_dir()
        self.args.extend(["-path_sav", state_dir, "-path_state", state_dir,
                "-path_movie", docdir, "-path_cheat", docdir,
                "-path_palette", docdir, "-path_snap", state_dir])
        #self.args.extend([self.find_game_rom(self.get_rom_extensions())])
        self.args.append(self.context.game.file)
        self.mednafen_post_configure()

    def set_mednafen_input_order(self):
        if fs.windows:
            self.input_device_order = 'DINPUT8'
        self.input_mapping_multiple = False


    def get_extra_graphics_options(self):
        return []

    def mednafen_post_configure(self):
        # can be overriden by subclasses
        pass

    #def get_game_refresh_rate(self):
    #    # can be overriden by subclasses
    #    return None

    def mednafen_cfg_path(self):
        #config_path = os.path.join(os.environ['HOME'], '.mednafen')
        # the SDL version (even on Windows) seems to read HOME, so
        # use environment instead of pyapp.user.home_dir
        #try:
        #    config_path = os.path.join(os.environ['HOME'], '.mednafen')
        #except KeyError:
        #    config_path = os.path.join(pyapp.user.home_dir(), '.mednafen')
        config_path = os.path.join(self.context.temp.dir('mednafen'), '.mednafen')
        if not os.path.isdir(config_path):
            os.makedirs(config_path)
        return os.path.join(config_path, "mednafen.cfg")
        #elif fs.windows:
        #    config_path = os.path.join(pyapp.fs.data_dir_user_app(),
        #            "mednafen")
        #    os.putenv("HOME", pyapp.fs.encode_path(config_path))
        #    if not os.path.exists(config_path):
        #        os.makedirs(config_path)
        #    config_path = os.path.join(config_path, ".mednafen")
        #    if not os.path.exists(config_path):
        #        os.makedirs(config_path)
        #    return os.path.join(config_path, "mednafen.cfg")
        #elif fs.macosx:
        #    config_path = os.path.join(pyapp.user.home_dir(), "Library",
        #            "Application Support", "ZSNES")
        #    if not os.path.isdir(config_path):
        #        os.makedirs(config_path)
        #    return os.path.join(config_path, "zsnesl.cfg")

    #def get_joystick_unique_ids(self, controllers):
    #    unique_ids = {}
    #    for i in range(len(controllers)):
    #        controller = controllers[i]
    #        unique_id = self.get_joystick_unique_id(controller)
    #        plusplus = unique_ids.values().count(unique_id)
    #        unique_id += plusplus
    #        unique_ids[controller.id] = unique_id
    #    return unique_ids

class InputMapper(GameController.InputMapper):

    def axis(self, axis, positive):
        if positive:
            offset = 0x8000
        else:
            offset = 0xc000
        joystick_id = self.get_unique_id(self.device, self.device.id)
        return 'joystick {0:x} {1:08x}'.format(
                joystick_id, axis + offset)

    def hat(self, hat, direction):
        offset = {
            'left': 8,
            'right': 2,
            'up': 1,
            'down': 4,
        }[direction]
        joystick_id = self.get_unique_id(self.device, self.device.id)
        return 'joystick {0:x} {1:08x}'.format(
                joystick_id, 0x2000 + offset)

    def button(self, button):
        joystick_id = self.get_unique_id(self.device, self.device.id)
        return 'joystick {0:x} {1:08x}'.format(
                joystick_id, int(button))

    def key(self, key):
        # FIXME: Need other key codes on Windows ... ?
        return 'keyboard {0}'.format(key.sdl_code)

    @pyapp.func.memoize
    def get_unique_id(self, device, device_id):
        print("get_unique_id for", device.id)
        # Implemented the algorithm in mednafen
        # src/drivers/joystick.cpp:GetJoystickUniqueID

        m = hashlib.md5()
        # FIXME: This assumes mednafen is compiled for little-endian
        print(device.axes, device.balls, device.hats, device.buttons)
        # processors
        buffer = chr(device.axes) + \
                "\0\0\0" + chr(device.balls) + \
                "\0\0\0" + chr(device.hats) + \
                "\0\0\0" + chr(device.buttons) + "\0\0\0"
        #print(repr(buffer))
        m.update(buffer)
        #digest = int(m.hexdigest(), 16)
        digest = m.digest()
        #print("digest len", len(digest))
        ret = 0
        for x in range(16):
            ret ^= ord(digest[x]) << ((x & 7) * 8)
        print("%x" % ret)
        return ret

