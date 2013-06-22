from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mess import MessController


class GameGearController(MessController):

    def mess_init_input(self):
        self.inputs = [self.create_input(
                name='Controller',
                type='gamegear',
                description='Built-in Gamepad')]

    def mess_get_input_mapping(self, port):
        return {
            'start': 'P#_START',
            'up': 'P#_JOYSTICK_UP',
            'down': 'P#_JOYSTICK_DOWN',
            'left': 'P#_JOYSTICK_LEFT',
            'right': 'P#_JOYSTICK_RIGHT',
            '1': 'P#_BUTTON1',
            '2': 'P#_BUTTON2',
        }

    def mess_configure(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        return 'gamegear'

    def get_game_refresh_rate(self):
        # refresh rate retrieved from MESS
        return 59.922743

