from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
from fengestad import fs
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from fengestad.gamecenter.resources import resources, logger, ngettext, _


# constants
SDLK_LAST = 323
# values from stella/src/emucore/EventHandler.hxx
kNumJoysticks = 8
kNumJoyButtons = 24
kNumJoyAxis = 16
kNumJoyHats = 16
kNumModes = 2


class Atari2600Controller(GameController):

    def init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i),
                    type='atari2600',
                    description='Joystick'))

    def configure(self):
        with open(self.get_stella_cfg_file(), 'wb') as f:
            self._configure_emulator(f)

    def run(self):
        plugin = pyapp.plugins.get_plugin('no.fengestad.emulator.stella')
        return plugin.stella(self.args, env=self.env)

    def _configure_emulator(self, f):
        # configure graphics
        f.write('video = gl\n')
        f.write('gl_filter = linear\n')
        f.write('gl_fsmax = 1\n')
        #if self.context.config.vsync:
        #    try:
        #        game_refresh = float(self.context.game.config['refreshrate'])
        #    except Exception:
        #        game_refresh = self.get_game_refresh_rate()
        #    rrtool = RefreshRateTool(game_platform=self.context.game.platform,
        #            game_refresh=game_refresh)
        #    rrtool.set_best_mode()
        #    if rrtool.allow_vsync():
        #        vsync = True
        #if vsync:
        if self.configure_vsync():
            f.write('gl_vsync = 1\n')
            #self.set_vsync(True)
        else:
            f.write('gl_vsync = 0\n')
            #self.set_vsync(False)
        if self.get_option('fullscreen'):
            f.write('fullscreen = 1\n')
        else:
            f.write('fullscreen = 0\n')
        f.write('fullres = {w}x{h}\n'.format(
                w=self.context.system.screen_width,
                h=self.context.system.screen_height))
        f.write('tv_tex = off\n')
        f.write('tv_bleed = low\n')
        f.write('tv_noise = medium\n')
        f.write('tv_phos = 1\n')
        #f.write('uselauncher = false\n')

        # configure input for emulator
        input_mapping = [
            {
                'select': 9,
                'reset': 10,
                'up': 11,
                'down': 12,
                'left': 13,
                'right': 14,
                '1': 15,
                '2': 16,
                '3': 17,
            },
            {
                'select': 9,
                'reset': 10,
                'up': 18,
                'down': 19,
                'left': 20,
                'right': 21,
                '1': 22,
                '2': 23,
                '3': 24,
            },
        ]

        keymap = InputMapper.default_keymap.replace(
                '\n', '').replace('\r', '').replace(' ', '')
        keymap = [int(x) for x in keymap.split(':')]
        joymap = [0] * kNumModes * kNumJoysticks * kNumJoyButtons
        joyaxismap = [0] * kNumModes * kNumJoysticks * kNumJoyAxis * 2
        joyhatmap = [0] * kNumModes * kNumJoysticks * kNumJoyHats * 4
        maps = [joyaxismap, joyhatmap, joymap, keymap]

        #def calc_button(value, action, joy_num):
        for i, input in enumerate(self.inputs):
            #print(i, input)
            #if not input.device_config:
            #    print(input.device, "no device config")
            #    continue

            mapper = InputMapper(input, input_mapping[i])
            for key, value in mapper.iteritems():
                print("keymap ------->", value, key)
                maps[value[0]][value[1]] = key

        #for i, device in enumerate(devices):
        #    input_config = input_configs[i]
        #    device_config = device.get_config()
        #    for key, value in input_config.iteritems():
        #        try:
        #            calc_button(device_config[value], key, device.index)
        #        except KeyError:
        #            pass
        f.write('keymap = 115:{map}\n'.format(
                map=':'.join([str(x) for x in keymap])))
        f.write('joymap = 115:{map}\n'.format(
                map=':'.join([str(x) for x in joymap])))
        f.write('joyaxismap = 115:{map}\n'.format(
                map=':'.join([str(x) for x in joyaxismap])))
        f.write('joyhatmap = 115:{map}\n'.format(
                map=':'.join([str(x) for x in joyhatmap])))

        # configure media and save states
        state_dir = self.context.game.state_dir + os.sep
        f.write('romdir = {state_dir}\n'.format(state_dir=os.path.dirname(
                self.context.game.file)))
        f.write('statedir = {state_dir}\n'.format(state_dir=state_dir))
        f.write('eepromdir = {state_dir}\n'.format(state_dir=state_dir))
        self.args.append(self.context.game.file)

    def get_stella_cfg_file(self):
        if fs.windows:
            config_path = os.path.join(pyapp.fs.data_dir_user(), 'Stella')
            if not os.path.exists(config_path):
                os.makedirs(config_path)
            return os.path.join(config_path, "stella.ini")
        elif fs.linux:
            config_path = os.path.join(pyapp.user.home_dir(), ".stella")
            if not os.path.isdir(config_path):
                os.makedirs(config_path)
            return os.path.join(config_path, "stellarc")
        #elif fs.macosx:
        #    config_path = os.path.join(pyapp.user.home_dir(), "Library",
        #            "Application Support", "ZSNES")
        #    if not os.path.isdir(config_path):
        #        os.makedirs(config_path)
        #    return os.path.join(config_path, "zsnesl.cfg")

    #def get_game_refresh_rate(self):
    #    refresh = GameController.get_game_refresh_rate(self)
    #    if not refresh:
    #        # default to NTSC
    #        return 60.0
    #    NTSC_FRAMERATE = 60.0
    #    PAL_FRAMERATE = 50.0
    #    config_name = self.context.game.config['name'].upper()
    #    if 'USA' in config_name:
    #        return NTSC_FRAMERATE
    #    if 'WORLD' in config_name:
    #        return NTSC_FRAMERATE
    #    if 'EUROPE' in config_name:
    #        return PAL_FRAMERATE
    #    if 'PAL' in config_name:
    #        return PAL_FRAMERATE
    #    if 'EU' in config_name:
    #        return PAL_FRAMERATE
    #    # fall back to NTSC frame rate, and hope the game is actually
    #    # an NTSC game...
    #    return NTSC_FRAMERATE


class InputMapper(GameController.InputMapper):

    def axis(self, axis, positive):
        offset = 1 if positive else 0
        index = 0 * (kNumJoysticks * kNumJoyAxis * 2) + \
                self.device.index * (kNumJoyAxis * 2) + \
                axis * 2 + \
                offset
        return 0, index
        #joyaxismap[index] = action

    def hat(self, hat, direction):
        offset = {
                'up': 3, 
                'down': 2,
                'left': 1,
                'right': 0,
                }[direction]
        index = 0 * (kNumJoysticks * kNumJoyHats * 4) + \
                self.device.index * (kNumJoyHats * 4) + \
                hat * 4 + \
                offset
        return 1, index
        #joyhatmap[index] = action

    def button(self, button):
        index = 0 * (kNumJoysticks * kNumJoyButtons) + \
                self.device.index * kNumJoyButtons + \
                button
        return 2, index
        #joymap[index] = action

    def key(self, key):
        #keymap[key.sdl_code] = action
        return 3, key.sdl_code

    default_keymap = """
0:0:0:0:0:0:0:0:98:94:0:0:0:0:0:0:0:0:0:93:0:0:0:0:0:0:0:97:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:62:0:63:64:55:41:42:43:0:0:0:0:53:54:0:61:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:95:0:0:0:96:47:0:52:49:46:0:0:
0:56:0:59:60:0:0:57:58:44:0:48:0:0:0:45:51:0:50:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:9:10:3:4:5:6:
7:8:90:88:89:91:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:114:0:0:0:0:109:0:0:0:0:0:0:0:0:0:0:0:0:0:113:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:101:102:104:103:0:105:106:107:108:0:0:0:0:0:0:0:0:0:0:0:0:
0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
"""

