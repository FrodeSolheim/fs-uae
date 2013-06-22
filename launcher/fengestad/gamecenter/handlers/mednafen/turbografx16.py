from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mednafen import MednafenController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class TurboGrafx16Controller(MednafenController):

    def init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i),
                    type='turbografx16',
                    description='Gamepad'))
        self.set_mednafen_input_order()

    def get_input_mapping(self, port):
        return [{
                '1': 'pce.input.port1.gamepad.i',
                '2': 'pce.input.port1.gamepad.ii',
                'up': 'pce.input.port1.gamepad.up',
                'down': 'pce.input.port1.gamepad.down',
                'left': 'pce.input.port1.gamepad.left',
                'right': 'pce.input.port1.gamepad.right',
                'select': 'pce.input.port1.gamepad.select',
                'run': 'pce.input.port1.gamepad.run',
            }, {
                '1': 'pce.input.port2.gamepad.i',
                '2': 'pce.input.port2.gamepad.ii',
                'up': 'pce.input.port2.gamepad.up',
                'down': 'pce.input.port2.gamepad.down',
                'left': 'pce.input.port2.gamepad.left',
                'right': 'pce.input.port2.gamepad.right',
                'select': 'pce.input.port2.gamepad.select',
                'run': 'pce.input.port2.gamepad.run',
            }][port]

    def is_pal_game(self):
        # FIXME
        return False

    def get_game_size(self):
        # FIXME
        return (256, 240)

    def force_aspect_ratio(self):
        #return 4/3
        return 0

    def get_system_prefix(self):
        return 'pce'

    def get_rom_extensions(self):
        return ['.pce']

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

