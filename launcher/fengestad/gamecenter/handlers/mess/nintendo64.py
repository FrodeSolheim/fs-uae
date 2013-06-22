from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import pyapp
from .mess import MessController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class Nintendo64Controller(MessController):

    def init_input(self):
        self.inputs = []
        for i in range(4):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='nintendo64',
                    description='Gamepad'))
        if fs.windows:
            self.input_device_order = 'DINPUT8'

    def get_input_mapping(self):
        return {
            #'start': 'P#_START',
            #'select': 'P#_SELECT',
            #'up': 'P#_JOYSTICK_UP',
            #'down': 'P#_JOYSTICK_DOWN',
            #'left': 'P#_JOYSTICK_LEFT',
            #'right': 'P#_JOYSTICK_RIGHT',
            #'a': 'P#_BUTTON1',
            #'b': 'P#_BUTTON2',
        }

    def configure_media(self):
        self.configure_cartridge()

    def get_romset(self):
        #if self.get_game_refresh_rate() == 50:
        #    return 'nespal'
        #else:
        #    return 'nes'
        return 'n64'

    #def get_firmware_name(self):
    #    return None

    #def get_offset_and_scale(self):
    #    #if self.get_romset() == 'nespal':
    #    #    return 0.0, 0.0, 1.082, 1.250
    #    #return 0.0, 0.0, 1.072, 1.164
    #    #return 0.0, 0.0, 1.072, 1.100

