from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mess import MessController


class MegaDriveController(MessController):

    def mess_init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='megadrive',
                    description='Gamepad'))

    def mess_get_input_mapping(self, port):
        return {
            'start': 'P#_START',
            'mode': 'P#_SELECT',
            'up': 'P#_JOYSTICK_UP',
            'down': 'P#_JOYSTICK_DOWN',
            'left': 'P#_JOYSTICK_LEFT',
            'right': 'P#_JOYSTICK_RIGHT',
            'a': 'P#_BUTTON1',
            'b': 'P#_BUTTON2',
            'c': 'P#_BUTTON3',
            'x': 'P#_BUTTON4',
            'y': 'P#_BUTTON5',
            'z': 'P#_BUTTON6',
        }

    def mess_configure(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        if self.get_game_refresh_rate() == 50:
            return 'megadriv'
        else:
            return 'genesis'

    def mess_get_firmware_name(self):
        return None

