from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import subprocess
import pyapp
import shutil
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class DolphinController(GameController):

    def configure(self):
        # configure dolphin.ini
        temp_dir = self.context.temp.dir('dolphin')
        dolphin_config_file = os.path.join(temp_dir, 'user', 'Config',
                'Dolphin.ini')
        if not os.path.exists(os.path.dirname(dolphin_config_file)):
            os.makedirs(os.path.dirname(dolphin_config_file))

        #dest = os.path.join(temp_dir, 'User', 'Wii', 'title')
        dest = os.path.join(temp_dir, 'user')
        if not os.path.exists(dest):
            os.makedirs(dest)
        plugin = pyapp.plugins.get_plugin('no.fengestad.emulator.dolphin')
        GameCenterUtil.copy_folder_tree(os.path.join(plugin.get_bin_dir(),
                'user'), dest)
        self.changes = ChangeHandler(dest)
        self.changes.init(self.context.game.state_dir)

        with open(dolphin_config_file, 'wb') as f:
            f.write('[Interface]\n')
            f.write('HideCursor = True\n')
            f.write('[Display]\n')
            f.write('RenderToMain = True\n')
            f.write('FullscreenResolution = {w}x{h}\n'.format(
                    w=self.context.system.screen_width,
                    h=self.context.system.screen_height))
            if self.get_option('fullscreen'):
                f.write('Fullscreen = True\n')
            else:
                f.write('Fullscreen = False\n')
            f.write('[Core]\n')
            if fs.windows:
                f.write('GFXPlugin = Plugin_VideoDX9.dll\n')
            self.configure_core(f)
 
        self.configure_input()

        # graphics options
        if self.configure_vsync():
            vsync = True
        else:
            vsync = False
        if fs.windows:
            graphics_config_file = os.path.join(temp_dir, 'user', 'Config',
                    'gfx_dx9.ini')
            with open(graphics_config_file, 'wb') as f:
                f.write('[Hardware]\n')
                if vsync:
                    f.write('VSync = True\n')
                else:
                    f.write('VSync = False\n')
                f.write('[Settings]\n')
                f.write('DisableFog = True\n')
                f.write('[Hacks]\n')
                f.write('EFBToTextureEnable = True\n')
        else:
            graphics_config_file = os.path.join(temp_dir, 'user', 'Config',
                    'gfx_opengl.ini')
            with open(graphics_config_file, 'wb') as f:
                f.write('[Hardware]\n')
                if vsync:
                    f.write('VSync = True\n')
                else:
                    f.write('VSync = False\n')
                f.write('[Settings]\n')

        # media options
        rom_path = self.context.game.file
        self.args.extend(['--exec=' + rom_path])

    def run(self):
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.dolphin')
        if fs.windows:
            temp_dir = self.context.temp.dir('dolphin')
            GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
            args = self.args[:]
            args.insert(0, os.path.join(temp_dir, 'Dolphin.exe'))
            return subprocess.Popen(args, env=self.env, cwd=temp_dir,
                    close_fds=True)
        else:
            temp_dir = self.context.temp.dir('dolphin')
            GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
            args = self.args[:]
            args.insert(0, os.path.join(temp_dir, 'dolphin-emu'))
            return subprocess.Popen(args, env=self.env, cwd=temp_dir,
                    close_fds=True)

    def cleanup(self):
        # remove some dirs and recopy config files, etc to
        # clean up the list of changed files
        temp_dir = self.context.temp.dir('dolphin')
        paths = [
            os.path.join(temp_dir, 'user', 'Config'),
            os.path.join(temp_dir, 'user', 'GC'),
            os.path.join(temp_dir, 'user', 'Logs'),
        ]
        for p in paths:
            if os.path.exists(p):
                shutil.rmtree(p)
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.dolphin')
        GameCenterUtil.copy_folder_tree(os.path.join(plugin.get_bin_dir(),
                'user'), os.path.join(temp_dir, 'user'))
        # now take backup of the real changes
        self.changes.update(self.context.game.state_dir)

    def configure_input(self):
        pass

    def configure_core(self, f):
        pass


class InputMapper(GameController.InputMapper):
    
    def axis(self, axis, positive):
        dir_str = '+' if positive else '-'
        return 'Axis ' + str(axis) + dir_str

    def hat(self, hat, direction):
        dir_str = {
            'left': 'W',
            'right': 'E',
            'up': 'N',
            'down': 'S',
        }[direction]
        return 'Hat ' + str(hat) + ' ' + dir_str

    def button(self, button):
        return 'Button ' + str(button)

    # Mouse values
    #elif value.startswith('M/'):
    #    if value == 'M/UP': return 'Cursor Y-'
    #    if value == 'M/DOWN': return 'Cursor Y+'
    #    if value == 'M/LEFT': return 'Cursor X-'
    #    if value == 'M/RIGHT': return 'Cursor X+'
    #    if value == 'M/00': return 'Click 0'
    #    if value == 'M/01': return 'Click 1'
    #    if value == 'M/02': return 'Click 2'
    #    raise Exception("unknown mouse value " + value)
    #else:
    #    #if fs.windows:
    #    #    return key_mapping[value]
    #    #else:
    #    #    return value
    #    return ''

    def key(self, key):
        if fs.windows:
            return key.dinput_name[4:]
        else:
            return key.sdl_name[5:]

    def mouse(self, button, axis, positive):
        if button:
            return 'Click ' + str(button - 1)
        else:
            if axis == 0:
                return 'Cursor X+' if positive else 'Cursor X-'
            if axis == 1:
                return 'Cursor Y+' if positive else 'Cursor Y-'

