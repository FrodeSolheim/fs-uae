from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
import math
import tempfile
import subprocess
import wx
import pyapp
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from fengestad.input.sdlkeycodes import sdl_key_codes
from fengestad.input.dinputkeycodes import dinput_key_codes
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class MegaDriveController(GameController):

    def configure_emulator(self):
        self.args = []
        with open(self.get_gens_cfg_path(), 'wb') as f:
            self._configure_emulator(f)

    def _configure_emulator(self, f):
        # sound options
        #f.write(sound_config)

        state_dir = self.context.game.state_dir
        f.write('[Directories]\n')
        f.write('Save Path={state_dir}\n'.format(state_dir=state_dir))
        f.write('BRAM Path={state_dir}\n'.format(state_dir=state_dir))
        f.write('SRAM Path={state_dir}\n'.format(state_dir=state_dir))


        f.write('[Graphics]\n')
        if fs.windows:
            pass
        else:
           f.write('Backend=SDL+OpenGL\n')
           f.write('OpenGL Filter=1\n')
           if self.context.config.fullscreen:
               f.write('OpenGL Width={0}\n'.format(
                       self.context.system.screen_width))
               f.write('OpenGL Height={0}\n'.format(
                       self.context.system.screen_height))
        #graphics_config = """
        #[Graphics]
        #Full Screen=%(full_screen)s
        #OpenGL Filter=%(opengl_filter)s
        #OpenGL Height=%(opengl_height)s
        #OpenGL Width=%(opengl_width)s
        #Render Fullscreen=Double
        #Render OpenGL=1
        #Render Windowed=Double
        #Sprite Limit=1
        #Stretch=3
        #"""

        # graphics options
        #screenw, screenh = self.context.system.screen_size
        if self.context.config.fullscreen:
            f.write('Full Screen=1\n')
            """
            if False: #self.options["stretch"]:
                opengl_width = screenw
                opengl_height = screenh
            else:
                aw = 4/3 # self.force_aspect_ratio()
                ah = 1.0
                scale = min(screenw / aw, screenh / ah)
                opengl_width = aw * scale
                opengl_height = ah * scale
            opengl_filter=1
            full_screen = 1
            """
        else:
            f.write('Full Screen=0\n')
            """
            opengl_width = 800
            opengl_height = 600
            opengl_filter=1
            full_screen = 0
            """
        if self.context.config.vsync:
            try:
                game_refresh = float(self.context.game.config['refreshrate'])
            except Exception:
                game_refresh = self.get_game_refresh_rate()
            rrtool = RefreshRateTool(game_platform=self.context.game.platform,
                    game_refresh=game_refresh)
            rrtool.set_best_mode()
            if rrtool.allow_vsync():
                vsync = True
        effect = 'Interpolated'
        filter_mapping = {
            'auto': 'Blargg NTSC',
            'ntsc': 'Blargg NTSC',
            'nearest': 'Double',
        }
        for filter in self.context.config.filters:
            try:
                effect = filter_mapping[filter]
            except KeyError:
                continue
            break
        f.write('Render Fullscreen={effect}\n'.format(effect=effect))
        f.write('Render Windowed={effect}\n'.format(effect=effect))
        if vsync:
            f.write('Full Screen VSync=1\n')
            f.write('Windows VSync=1\n')
            f.write('Frame Skip=0\n')
            self.set_vsync(True)
        else:
            f.write('Full Screen VSync=0\n')
            f.write('Windows VSync=0\n')
            f.write('Frame Skip=-1\n')
            self.set_vsync(False)
        # looks nicer without border color, but can it have some
        # negative side-effects??
        f.write('Border Color Emulation=0\n')
        f.write('Stretch=0\n')

        f.write('[Sound]\n')
        f.write('YM2612 Improvement=1\n')

        # input options
        f.write('[Input]\n')
        #device_list = []
        if fs.windows:
            device_order = 'DINPUT8'
        else:
            device_order = 'SDL'
        devices = self.context.input.get_devices(2,
                device_order=device_order)
                #device_list=device_list)
        #buttonsbefore = {}
        #numaxes = {}
        #buttonscount = 0
        #for sc in device_list:
        #    numaxes[sc.id] = sc.axes
        #    buttonsbefore[sc.id] = buttonscount
        #    buttonscount += sc.buttons + 2 * sc.axes
        #print "Buttons before:", buttonsbefore
        #print controllers[0].get_config()
        #print controllers[1].get_config()
        input_configs = [{
                'P1.Mode': 'MODE',
                'P1.Start': 'START',
                'P1.X': 'X',
                'P1.Y': 'Y',
                'P1.Z': 'Z',
                'P1.A': 'A',
                'P1.B': 'B',
                'P1.C': 'C',
                'P1.Up': 'UP',
                'P1.Down': 'DOWN',
                'P1.Left': 'LEFT',
                'P1.Right': 'RIGHT',
            }, {
                'P2.Mode': 'MODE',
                'P2.Start': 'START',
                'P2.X': 'X',
                'P2.Y': 'Y',
                'P2.Z': 'Z',
                'P2.A': 'A',
                'P2.B': 'B',
                'P2.C': 'C',
                'P2.Up': 'UP',
                'P2.Down': 'DOWN',
                'P2.Left': 'LEFT',
                'P2.Right': 'RIGHT',
            }]
        def calc_button(value, joy_num):
            if len(value) == 4 and value[:2] == "JA":
                axis = int(value[2:])
                offset = 1 if value[-1] == '+' else 0
                return 0x8000 + 0x100 * joy_num + axis * 2 + offset
            #    num = bb + axis * 2
            #    if value[-1] == "+":
            #        pass
            #    elif value[-1] == "-":
            #        num += 1
            #    else:
            #        raise Exception("Invalid last character in axis config")
            #    return 256 + num
            elif len(value) == 4 and value[:2] == "JH":
                hat = int(value[2])
                offset = [0, 0, 1, 0, 2, 0, 0, 0, 3][int(value[3], 16)]
                return 0xa000 + 0x100 * joy_num + hat * 4 + offset
            elif len(value) == 4 and value[:2] == "JB":
                button = int(value[2:])
                return 0x9000 + 0x100 * joy_num + button
                #value = int(value[2:])
                #return 256 + bb + ab + value - 1
            else:
                if fs.windows:
                    return dinput_key_codes[value]
                else:
                    return sdl_key_codes[value]
        for i, device in enumerate(devices):
            input_config = input_configs[i]
            #cname = devices[i].name
            #write_player_options(i + 1, devices[i],
            #        buttonsbefore[cname], numaxes[cname])
            #if controller.get_type() == "TYPE_JOYSTICK":
            #bb = buttonsbefore
            #ab = numaxes * 2
            device_config = device.get_config()
            joy_num = device.index
            #j = (controller.get_type() == "TYPE_JOYSTICK")
            f.write('P%d.Type=1\n' % (i + 1,))
            for key, value in input_config.iteritems():
                button = calc_button(device_config[value], device.index)
                f.write('{key}=0x{button:X}\n'.format(key=key,
                        button=button))

        rom_path = self.context.game.file
        self.args.extend([rom_path])

#    def run_emulator(self):
#        self.config_file.close()
#        print "run_emulator"
#        plugin = pyapp.plug.get_plugin('net.fsolheim.emulator.gens')
#        emulator_path = plugin.get_executable_path()
#        args = [emulator_path]
#        args += self.args
#        print "args =", args
#        self._run_emulator(args)

    def get_gens_cfg_path(self):
        if fs.windows:
            temp_dir = self.context.temp.dir('gens')
            #config_path = os.path.join(pyapp.fs.data_dir_user(), "gens")
            #if not os.path.exists(config_path):
            #    os.makedirs(config_path)
            return os.path.join(temp_dir, 'gens.cfg')
        elif fs.linux:
            config_path = os.path.join(pyapp.user.home_dir(), ".gens")
            if not os.path.isdir(config_path):
                os.makedirs(config_path)
            return os.path.join(config_path, "gens.cfg")
            #self.config_file = open(config_path, "wb")
        #elif fs.macosx:
        #    config_path = os.path.join(pyapp.user.home_dir(), "Library",
        #            "Application Support", "ZSNES")
        #    if not os.path.isdir(config_path):
        #        os.makedirs(config_path)
        #    return os.path.join(config_path, "zsnesl.cfg")

    def create_emulator_process(self, env):
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.gens')
        if fs.windows:
            temp_dir = self.context.temp.dir('gens')
            GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
            args = self.args[:]
            args.insert(0, os.path.join(temp_dir, 'gens.exe'))
            return subprocess.Popen(args, env=env, close_fds=True)
        return plugin.gens(self.args, env=env, close_fds=True)

