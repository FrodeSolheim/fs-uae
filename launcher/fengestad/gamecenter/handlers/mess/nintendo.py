from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from .mess import MessController


class NintendoController(MessController):

    def mess_init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='nintendo',
                    description='Gamepad'))

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
        }

    def mess_configure_media(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        if self.get_game_refresh_rate() == 50:
            return 'nespal'
        else:
            return 'nes'

    def mess_get_firmware_name(self):
        return None

    def mess_get_offset_and_scale(self):
        #if self.get_romset() == 'nespal':
        #    return 0.0, 0.0, 1.082, 1.250
        #return 0.0, 0.0, 1.072, 1.164
        return 0.0, 0.0, 1.072, 1.100

