from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import pyapp
from .mess import MessController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class Atari2600Controller(MessController):

    def mess_init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='atari2600',
                    description='Joystick'))

    def mess_get_input_mapping(self, port):
        return {
            'select': 'tag="SWB" type="P#_START" mask="2" defvalue="2"',
            'reset': 'tag="SWB" type="OTHER" mask="1" defvalue="1"',
            'up': 'P#_JOYSTICK_UP',
            'down': 'P#_JOYSTICK_DOWN',
            'left': 'P#_JOYSTICK_LEFT',
            'right': 'P#_JOYSTICK_RIGHT',
            '1': 'P#_BUTTON1',
            '2': 'P#_BUTTON2',
            '3': 'P#_BUTTON3',
        }

    def mess_configure(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        if self.get_game_refresh_rate() == 50:
            return 'a2600p'
        else:
            return 'a2600'

    def mess_get_firmware_name(self):
        return None

