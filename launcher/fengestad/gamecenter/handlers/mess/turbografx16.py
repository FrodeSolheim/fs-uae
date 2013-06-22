from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mess import MessController


class TurboGrafx16Controller(MessController):

    def mess_init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='turbografx16',
                    description='Gamepad'))

    def mess_get_input_mapping(self, port):
        return {
            'run': 'P#_START',
            'select': 'P#_SELECT',
            'up': 'P#_JOYSTICK_UP',
            'down': 'P#_JOYSTICK_DOWN',
            'left': 'P#_JOYSTICK_LEFT',
            'right': 'P#_JOYSTICK_RIGHT',
            '1': 'P#_BUTTON2',
            '2': 'P#_BUTTON1',
        }

    def mess_configure(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        # FIXME: Japanese version is 'pce'
        return 'tg16'

    def mess_get_firmware_name(self):
        return None

    def get_game_refresh_rate(self):
        # refresh rate retrieved from MESS
        return 59.922743

