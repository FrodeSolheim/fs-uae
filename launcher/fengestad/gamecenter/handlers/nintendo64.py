from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
import subprocess
from fengestad import fs
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.input.sdlkeycodes import sdl_key_codes
from fengestad.input.dinputkeycodes import dinput_key_codes
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class Nintendo64Controller(GameController):

    def init_input(self):
        self.inputs = []
        for i in range(4):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='nintendo64',
                    description='Controller'))

    def configure(self):
        temp_dir = self.context.temp.dir('mupen64plus')
        self.args.extend(['--configdir', temp_dir])
        self.args.extend(['--datadir', temp_dir])
        config_file = os.path.join(temp_dir, 'mupen64plus.cfg')
        with open(config_file, 'w') as f:
            self.write_config(f)
            self.configure_input(f)
        input_config_file = os.path.join(temp_dir, 'InputAutoCfg.ini')
        with open(input_config_file, 'wb') as f:
            pass
        rom_path = self.context.game.file
        self.args.extend([rom_path])

    def run(self):
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.mupen64plus')
        if fs.windows:
            temp_dir = self.context.temp.dir('mupen64plus')
            GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
            args = self.args[:]
            args.insert(0, os.path.join(temp_dir, 'mupen64plus-ui-console.exe'))
            return subprocess.Popen(args, env=self.env, cwd=temp_dir,
                    close_fds=True)
        return plugin.mupen64plus(self.args, env=self.env,
                cwd=plugin.get_bin_dir(), close_fds=True)

    def write_config(self, f):
        f.write('[Core]\n')
        f.write('OnScreenDisplay = False\n')
        # FIXME: Save state path does not seem to work
        # -stuff are saved to default dir in AppData/Roaming/Mupen64Plus/save
        # instead...
        # FIXME: Might work now with updated mupen64plus
        f.write('SaveStatePath = "{path}"\n'.format(
                path=self.context.game.state_dir + os.sep))
        f.write('[Video-General]\n')
        if self.get_option('fullscreen'):
            f.write('Fullscreen = True\n')
            f.write('ScreenWidth = {0}\n'.format(
                    self.context.system.screen_width))
            f.write('ScreenHeight = {0}\n'.format(
                    self.context.system.screen_height))
        else:
            f.write('Fullscreen = False\n')
            f.write('ScreenWidth = 640\n')
            f.write('ScreenHeight = 480\n')
        f.write('[Video-Rice]\n')
        f.write('AccurateTextureMapping = True\n')
        #f.write('ScreenUpdateSetting = 2\n')
        #f.write('MultiSampling = 2\n')
        #f.write('InN64Resolution = True\n')
        #f.write('AccurateTextureMapping = False\n')
        #f.write('WinFrameMode = True\n')
        #f.write('InN64Resolution = False\n')
        #f.write('ForceTextureFilter = 0\n')
        #f.write('TextureFilteringMethod = 1\n')
        #f.write('TextureFilteringMethod = 0\n')
        # automatically choose fog generation
        #f.write('FogMethod = 1\n')
        #f.write('EnableVertexShader = True\n')
        #f.write('NormalAlphaBlender = True\n')
        #f.write('ShowFPS = True\n')
        #f.write('EnableMipmaping = False\n')

        #self.args.extend(['--set', 'Video-Rice[InN64resolution]=True'])
        #self.args.extend(['--set', 'Video-Rice[ForceTextureFilter]=0'])

        if self.configure_vsync():
            # cannot find config for vsync in rice video plugin,
            # but should work for linux/nvidia due to env. variable
            # being set
            pass

    def configure_input(self, f):
        input_mapping = {
            'dpad_right': 'DPad R',
            'dpad_left': 'DPad L',
            'dpad_down': 'DPad D',
            'dpad_up': 'DPad U',
            'start': 'Start',
            'z': 'Z Trig',
            'b': 'B Button',
            'a': 'A Button',
            'c_right': 'C Button R',
            'c_left': 'C Button L',
            'c_down': 'C Button D',
            'c_up': 'C Button U',
            'r': 'R Trig',
            'l': 'L Trig',
            'mempak': 'Mempak switch',
            'rumplepak': 'Rumblepak switch',
            'stick_left': ('X Axis', 0),
            'stick_right': ('X Axis', 1),
            'stick_up': ('Y Axis', 0),
            'stick_down': ('Y Axis', 1),
        }
        for i, input in enumerate(self.inputs):
            if not input.device:
                continue
            mapper = InputMapper(input, input_mapping)
            config = {}
            for key, value in mapper.iteritems():
                print("---->", key, value)
                if isinstance(key, tuple):
                    key, index = key
                else:
                    index = 0
                config.setdefault(key, {})[index] = value
            f.write('[Input-SDL-Control{0}]\n'.format(i + 1))
            f.write('plugged = True\n')
            f.write('plugin = 2\n')
            f.write('mouse = False\n')
            if input.device.is_joystick():
                f.write('device = {0}\n'.format(input.device.index))
                f.write('AnalogDeadZone = 512,512\n')
                f.write('AnalogPeak = 32767,32767\n')
            else:
                # -2 means keyboard/mouse
                f.write('device = -2\n')

            for key, value in config.iteritems():
                type = value[0][0]
                values = [x[1][1] for x in sorted(list(value.iteritems()))]
                values_str = ','.join(values)
                f.write('{key} = {type}({values})\n'.format(key=key,
                        type=type, values=values_str))


class InputMapper(GameController.InputMapper):

    def axis(self, axis, positive):
        dir_str = '+' if positive else '-'
        return 'axis', '{0}{1}'.format(axis, dir_str)

    def hat(self, hat, direction):
        dir_str = {
            'left': 'Left',
            'right': 'Right',
            'up': 'Up',
            'down': 'Down',
        }[direction]
        return 'hat',  '{0} {1}'.format(hat, dir_str)

    def button(self, button):
        return 'button', str(button)

    def key(self, key):
        return 'key', str(key.sdl_code)

