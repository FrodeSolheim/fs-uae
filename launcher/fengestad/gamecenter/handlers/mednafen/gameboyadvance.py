from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
import shutil
import hashlib
import ConfigParser
from .mednafen import MednafenController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class GameBoyAdvanceController(MednafenController):

    def init_input(self):
        self.inputs = [
            self.create_input(name='Controller',
                    type='gameboyadvance',
                    description='Built-in Gamepad'),
        ]
        self.set_mednafen_input_order()

    def get_input_mapping(self, port):
        return {
                'a': 'gba.input.builtin.gamepad.a',
                'b': 'gba.input.builtin.gamepad.b',
                'l': 'gba.input.builtin.gamepad.shoulder_l',
                'r': 'gba.input.builtin.gamepad.shoulder_r',
                'up': 'gba.input.builtin.gamepad.up',
                'down': 'gba.input.builtin.gamepad.down',
                'left': 'gba.input.builtin.gamepad.left',
                'right': 'gba.input.builtin.gamepad.right',
                'select': 'gba.input.builtin.gamepad.select',
                'start': 'gba.input.builtin.gamepad.start',
            }

    def mednafen_post_configure(self):
        rom_file = self.context.game.file
        cfg_file = os.path.splitext(rom_file)[0] + '.cfg'
        if os.path.exists(cfg_file):
            config = ConfigParser.ConfigParser()
            config.read(cfg_file)
            if config.has_option('game', 'sram type'):
                type_file = os.path.join(self.context.game.state_dir,
                        os.path.splitext(os.path.basename(
                        rom_file))[0] + u'.type')
                with open(type_file, 'wb') as f:
                    f.write(config.get('game', 'sram type'))
                    f.write('\n')
        sav_file = os.path.splitext(rom_file)[0] + u'.sav'
        if os.path.exists(sav_file):
            m = hashlib.md5()
            with open(rom_file, 'rb') as f:
                while True:
                    data = f.read(16384)
                    if not data:
                        break
                    m.update(data)
                md5sum = str(m.hexdigest())
            save_name = os.path.splitext(os.path.basename(
                        rom_file))[0] + u'.' + md5sum + u'.sav'
            dest_path = os.path.join(self.context.game.state_dir, save_name)
            if not os.path.exists(dest_path):
                shutil.copy(sav_file, dest_path)

        colormap_file = self.context.temp.file('color.map')
        self.create_colormap(colormap_file, 1.3)
        self.args.insert(0, colormap_file)
        self.args.insert(0, '-gba.colormap')

    def create_colormap(self, path, gamma):
        with open(path, 'wb') as f:
            for x in range(32768):
                r = (x & 0x1f) << 3;
                g = ((x & 0x3e0) >> 5) << 3;
                b = ((x & 0x7c00) >> 10) << 3;

                r = r / 255.0
                g = g / 255.0
                b = b / 255.0
                
                #h, l, s = colorsys.rgb_to_hls(r, g, b)
                #l = l ** gamma
                #r, g, b = colorsys.hls_to_rgb(h, l, s)

                r = r ** gamma
                g = g ** gamma
                b = b ** gamma

                f.write(chr(int(r * 255)))
                f.write(chr(int(g * 255)))
                f.write(chr(int(b * 255)))

    def get_game_size(self):
        # FIXME
        return (240, 160)

    def force_aspect_ratio(self):
        #return 4/3
        return None

    def get_system_prefix(self):
        return 'gba'

    def get_rom_extensions(self):
        return ['.gba']

    def get_scanlines_setting(self):
        return 33

    def get_special_filter(self):
        return 'nn2x'

    def get_extra_graphics_options(self):
        options = []
        #if self.is_pal_game():
        #    options.extend(['-nes.pal', '1'])
        #else:
        #    options.extend(['-nes.pal', '0'])
        return []

    def get_game_refresh_rate(self):
        # all GBA games should use a refresh rate of 60.0 Hz
        # (or 30.0 Hz)
        return 60.0

