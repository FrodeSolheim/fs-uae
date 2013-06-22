from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import pyapp
from .dolphin import DolphinController, InputMapper
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class GameCubeController(DolphinController):

    def init_input(self):
        self.inputs = []
        for i in range(4):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='gamecube',
                    description='Controller'))

    def configure_core(self, f):
        # find devices now (need to know how many devices to
        # specify in dolphin.ini)
        #devices = self.context.input.get_devices(1, 4)
        # configure dolphin.ini
        device_count = 0
        for i, input in enumerate(self.inputs):
            if input.device:
                device_count += 1
            
        for i in range(device_count):
            f.write('SIDevice{0} = 150994944\n'.format(i))

        # FIXME:
        memcard_region = 'USA'

        f.write('MemcardA = {dir}/MemoryCardA.{region}.raw\n'.format(
                dir=self.context.game.state_dir,
                region=memcard_region))
        f.write('MemcardB = {dir}/MemoryCardB.{region}.raw\n'.format(
                dir=self.context.game.state_dir,
                region=memcard_region))

    def configure_input(self):
        #devices = self.context.input.get_devices(1, 4)
        input_mapping = {
            'a': 'Buttons/A',
            'b': 'Buttons/B',
            'x': 'Buttons/X',
            'y': 'Buttons/Y',
            'z': 'Buttons/Z',
            'start': 'Buttons/Start',
            'stick_up': 'Main Stick/Up',
            'stick_down': 'Main Stick/Down',
            'stick_left': 'Main Stick/Left',
            'stick_right': 'Main Stick/Right',
            'c_up': 'C-Stick/Up', # or z?
            'c_down': 'C-Stick/Down',
            'c_left': 'C-Stick/Left',
            'c_right': 'C-Stick/Right',
            'l': 'Triggers/L',
            'r': 'Triggers/R',
            'dpad_up': 'D-Pad/Up',
            'dpad_down': 'D-Pad/Down',
            'dpad_left': 'D-Pad/Left',
            'dpad_right': 'D-Pad/Right',
            'l_analog': 'Triggers/L-Analog',
            'r_analog': 'Triggers/R-Analog',
            # FIXME: ADD MODIFIER?
        }
        temp_dir = self.context.temp.dir('dolphin')
        input_config_file = os.path.join(temp_dir, 'user', 'Config',
                'GCPadNew.ini')
        f = open(input_config_file, 'w')
        for i, input in enumerate(self.inputs):
            if not input.device:
                continue
            f.write('[GCPad{num}]\n'.format(num=i + 1))
            if input.device.is_keyboard():
                f.write('Device = DInput/0/Keyboard Mouse\n')
            else:
                type_index = int(input.device.id.rsplit('#', 1)[1]) - 1
                f.write('Device = SDL/{index}/{name}\n'.format(
                        index=type_index, name=input.device.sdl_name))
            mapper = InputMapper(input, input_mapping)
            for key, value in mapper.iteritems():
                if isinstance(key, tuple):
                    for key in key:
                        f.write('{key} = {value}\n'.format(key=key,
                                value=value))
                else:
                    f.write('{key} = {value}\n'.format(key=key,
                            value=value))

