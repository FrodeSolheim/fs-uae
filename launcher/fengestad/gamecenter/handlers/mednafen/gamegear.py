from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mednafen import MednafenController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class GameGearController(MednafenController):

    def init_input(self):
        self.inputs = [
            self.create_input(name='Controller',
                    type='gamegear',
                    description='Built-in Gamepad'),
        ]
        self.set_mednafen_input_order()

    def get_input_mapping(self, port):
        return {
                '1': 'gg.input.builtin.gamepad.button1',
                '2': 'gg.input.builtin.gamepad.button2',
                'up': 'gg.input.builtin.gamepad.up',
                'down': 'gg.input.builtin.gamepad.down',
                'left': 'gg.input.builtin.gamepad.left',
                'right': 'gg.input.builtin.gamepad.right',
                'start': 'gg.input.builtin.gamepad.start',
            }

    def get_game_size(self):
        return (160, 146)

    def force_aspect_ratio(self):
        #return 160 / 102
        # FIXME: or 160 / 120
        return 4 / 3 

    def get_system_prefix(self):
        return 'gg'

    def get_rom_extensions(self):
        return ['.gg']

    def get_scanlines_setting(self):
        return 33

    def get_special_filter(self):
        return 'nn2x'

    def get_extra_graphics_options(self):
        options = []
        return options

    def get_game_refresh_rate(self):
        # FIXME: refresh rate for Sega Game Gear?
        return None

