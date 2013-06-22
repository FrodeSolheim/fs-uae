from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import pyapp
from .mess import MessController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class Atari5200Controller(MessController):

    def mess_init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='atari5200',
                    description='Gamepad'))

    def mess_get_input_mapping(self, port):
        return {
            'start': 'type="P#_START"',
            'pause': 'tag="keypad_2" type="KEYPAD" mask="1" defvalue="0"',
            'reset': 'tag="keypad_1" type="KEYPAD" mask="1" defvalue="0"',
            'left': ('type="P#_AD_STICK_X"', 'increment'),
            'right': ('type="P#_AD_STICK_X"', 'decrement'),
            'up': ('type="P#_AD_STICK_Y"', 'decrement'),
            'down': ('type="P#_AD_STICK_Y"', 'increment'),
            '1': 'P#_BUTTON1',
            '2': 'P#_BUTTON2',
            'pad#': 'tag="keypad_0" type="KEYPAD" mask="2" defvalue="0"',
            'pad0': 'tag="keypad_0" type="KEYPAD" mask="4" defvalue="0"',
            'pad*': 'tag="keypad_0" type="KEYPAD" mask="8" defvalue="0"',
            'pad9': 'tag="keypad_1" type="KEYPAD" mask="2" defvalue="0"',
            'pad8': 'tag="keypad_1" type="KEYPAD" mask="4" defvalue="0"',
            'pad7': 'tag="keypad_1" type="KEYPAD" mask="8" defvalue="0"',
            'pad6': 'tag="keypad_2" type="KEYPAD" mask="2" defvalue="0"',
            'pad5': 'tag="keypad_2" type="KEYPAD" mask="4" defvalue="0"',
            'pad4': 'tag="keypad_2" type="KEYPAD" mask="8" defvalue="0"',
            'pad3': 'tag="keypad_3" type="KEYPAD" mask="2" defvalue="0"',
            'pad2': 'tag="keypad_3" type="KEYPAD" mask="4" defvalue="0"',
            'pad1': 'tag="keypad_3" type="KEYPAD" mask="8" defvalue="0"',
            #'start': 'P#_START/keypad_3',
            #'pause': 'KEYPAD/keypad_2',
            #'reset': 'KEYPAD/keypad_1',
            #'left': 'P#_AD_STICK_X:increment',
            #'right': 'P#_AD_STICK_X:decrement',
            #'up': 'P#_AD_STICK_Y:decrement',
            #'down': 'P#_AD_STICK_Y:increment',
            #'1': 'P#_BUTTON1',
            #'2': 'P#_BUTTON2',
        }

    def mess_configure(self):
        self.mess_configure_cartridge()

    def mess_get_romset(self):
        #if self.get_game_refresh_rate() == 50:
        #    return 'a7800p'
        #else:
        # FIXME: NO PAL VERSION?
        return 'a5200'

