from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mednafen import MednafenController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class GameBoyColorController(MednafenController):

    def init_input(self):
        self.inputs = [
            self.create_input(name='Controller',
                    type='gameboycolor',
                    description='Built-in Gamepad'),
        ]
        self.set_mednafen_input_order()

    def get_game_size(self):
        # FIXME
        return (160, 144)

    def force_aspect_ratio(self):
        return None
        #return 4/3

    def get_system_prefix(self):
        return 'gb'

    def get_rom_extensions(self):
        return ['.gbc']

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
        # FIXME: Is this correct for GBC?
        return 60.0

    def get_input_mapping(self, port):
        return {
                'a': 'gb.input.builtin.gamepad.a',
                'b': 'gb.input.builtin.gamepad.b',
                'up': 'gb.input.builtin.gamepad.up',
                'down': 'gb.input.builtin.gamepad.down',
                'left': 'gb.input.builtin.gamepad.left',
                'right': 'gb.input.builtin.gamepad.right',
                'select': 'gb.input.builtin.gamepad.select',
                'start': 'gb.input.builtin.gamepad.start',
            }

