from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
from .mednafen import MednafenController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class AtariLynxController(MednafenController):

    def init_input(self):
        self.inputs = [
            self.create_input(name='Controller',
                    type='atarilynx',
                    description='Built-in Gamepad'),
        ]
        self.set_mednafen_input_order()

    def prepare(self):
        self.firmware_dir = self.prepare_dir('Atari Lynx Firmware')
        if not os.path.exists(os.path.join(self.context.temp.dir('mednafen'),
                '.mednafen')):
            os.makedirs(os.path.join(self.context.temp.dir('mednafen'),
                    '.mednafen'))
        shutil.copy(os.path.join(self.firmware_dir, 'lynxboot.img'),
                os.path.join(self.context.temp.dir('mednafen'),
                '.mednafen', 'lynxboot.img'))
        MednafenController.prepare(self)

    def get_input_mapping(self, port):
        return {
            'a': 'lynx.input.builtin.gamepad.a',
            'b': 'lynx.input.builtin.gamepad.b',
            'up': 'lynx.input.builtin.gamepad.up',
            'down': 'lynx.input.builtin.gamepad.down',
            'left': 'lynx.input.builtin.gamepad.left',
            'right': 'lynx.input.builtin.gamepad.right',
            'option1': 'lynx.input.builtin.gamepad.option_1',
            'option2': 'lynx.input.builtin.gamepad.option_2',
            'pause': 'lynx.input.builtin.gamepad.pause',
        }

    def get_game_size(self):
        return (160, 102)

    def force_aspect_ratio(self):
        return 160.0 / 102.0

    def get_system_prefix(self):
        return 'lynx'

    def get_rom_extensions(self):
        return ['.lnx']

    def get_scanlines_setting(self):
        return 33

    def get_special_filter(self):
        return 'nn2x'

    def get_extra_graphics_options(self):
        options = []
        return options

    def get_game_refresh_rate(self):
        # FIXME: Is this correct for Atari Lynx?
        return 60.0

