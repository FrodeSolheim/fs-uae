from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mess import MessController


class GameBoyAdvanceController(MessController):

    def mess_init_input(self):
        self.inputs = [self.create_input(
                name='Controller',
                type='gameboyadvance',
                description='Built-in Gamepad')]

    def mess_get_input_mapping(self, port):
        return {
            'start': 'P#_START',
            'select': 'P#_SELECT',
            'up': 'P#_JOYSTICK_UP',
            'down': 'P#_JOYSTICK_DOWN',
            'left': 'P#_JOYSTICK_LEFT',
            'right': 'P#_JOYSTICK_RIGHT',
            'a': 'P#_BUTTON1',
            'b': 'P#_BUTTON2',
            'l': 'P#_BUTTON3',
            'r': 'P#_BUTTON4',
        }

    def mess_configure(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        return 'gba'

    def get_game_refresh_rate(self):
        # refresh rate retrieved from MESS
        return 59.727501

