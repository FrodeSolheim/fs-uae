from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import pyapp
from .dolphin import DolphinController, InputMapper
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class WiiController(DolphinController):

    def init_input(self):
        self.inputs = []
        for i in range(4):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='wii_gamepad',
                    description='Controller'))

    def configure_input(self):
        input_mapping = {
            'a': 'Buttons/A',
            'b': 'Buttons/B',
            '1': 'Buttons/1',
            '2': 'Buttons/2',
            'ir_up': 'IR/Up',
            'ir_down': 'IR/Down',
            'ir_left': 'IR/Left',
            'ir_right': 'IR/Right',
            'shake': ('Shake/X', 'Shake/Y', 'Shake/Z'),
            'shake_x': 'Shake/X',
            'shake_y': 'Shake/Y',
            'shake_z': 'Shake/Z',
            'tilt_left': 'Tilt/Left',
            'tilt_right': 'Tilt/Right',
            'dpad_up': 'D-Pad/Up',
            'dpad_down': 'D-Pad/Down',
            'dpad_left': 'D-Pad/Left',
            'dpad_right': 'D-Pad/Right',
            '-': 'Buttons/-',
            '+': 'Buttons/+',
            'home': 'Buttons/Home',
            'nunchuk_up': 'Nunchuk/Stick/Up',
            'nunchuk_down': 'Nunchuk/Stick/Down',
            'nunchuk_left': 'Nunchuk/Stick/Left',
            'nunchuk_right': 'Nunchuk/Stick/Right',
            'nunchuk_modifier': 'Nunchuk/Stick/Modifier',
            'nunchuk_c': 'Nunchuk/Buttons/C',
            'nunchuk_z': 'Nunchuk/Buttons/Z',
            'nunchuk_shake': ('Nunchuk/Shake/X', 'Nunchuk/Shake/Y', 'Nunchuk/Shake/Z'),
            'nunchuk_shake_x': 'Nunchuk/Shake/X',
            'nunchuk_shake_y': 'Nunchuk/Shake/Y',
            'nunchuk_shake_z': 'Nunchuk/Shake/Z',
        }
        temp_dir = self.context.temp.dir('dolphin')
        input_config_file = os.path.join(temp_dir, 'user', 'Config',
                'WiimoteNew.ini')
        f = open(input_config_file, 'w')
        for i, input in enumerate(self.inputs):
            if not input.device:
                continue
            f.write('[Wiimote{num}]\n'.format(num=i + 1))

            # FIXME: needs more work...
            if input.device.name == 'Wiimote':
                f.write('Source = 2\n')
                continue

            f.write('Source = 1\n')
            #wiimote_config = self.context.game.config.get('wiimote', 'sideways')
            wiimote_config = self.context.game.config.get('wiimote', 'nunchuk')
            if 'nunchuk' in wiimote_config:
                f.write('Extension = Nunchuk\n')
            if 'sideways' in wiimote_config:
                f.write('Options/Sideways Wiimote = 1.000000\n')
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
        """
        with open(input_config_file, 'wb') as f:
            for i, device in enumerate(devices):
                
                wiimote_config = 'NUNCHUK'

                device_config = device.get_config()
                f.write('[Wiimote{num}]\n'.format(num=i + 1))
                # set type to emulated (Source = 1)
                f.write('Source = 1\n')
                if 'NUNCHUK' in wiimote_config:
                    f.write('Extension = Nunchuk\n')
                if 'SIDEWAYS' in wiimote_config:
                    f.write('Options/Sideways Wiimote = 1.000000\n')
                if device.get_type() == 'TYPE_KEYBOARD':
                    f.write('Device = DInput/0/Keyboard Mouse\n')
                else:
                    type_index = int(device.name[-1]) - 1
                    f.write('Device = SDL/{index}/{name}\n'.format(
                            index=type_index, name=device.sdl_name))
                for key, value in input_config.iteritems():
                    try:
                        button = calc_button(device_config[value])
                    except KeyError:
                        # button not configured
                        continue
                    f.write('{key} = {button}\n'.format(key=key,
                            button=button))
        """


