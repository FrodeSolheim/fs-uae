from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from fengestad.gamecenter_mednafen.controller import MednafenController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class NintendoController(MednafenController):

    def get_supported_filters(self):
        supported = MednafenController.get_supported_filters(self)
        supported.append({
            'name': 'ntsc',
        })
        return supported

    #def __init__(self):
    #    pass

    def is_pal_game(self):
        # FIXME
        return False

    def is_ntsc_game(self):
        if 'USA' in self.context.game.file:
            return True

    def get_game_size(self):
        # FIXME
        if self.is_ntsc_game():
            size = (256, 224)
        else:
            size = (256, 240)
        if self.nes_clip_sides():
            size = (size[0] - 16, size[1])
        return size

    def force_aspect_ratio(self):
        return 4/3

    def get_system_prefix(self):
        return 'nes'

    def get_rom_extensions(self):
        return ['.nes']

    def get_extra_graphics_options(self):
        options = []
        if self.is_pal_game():
            options.extend(['-nes.pal', '1'])
        else:
            options.extend(['-nes.pal', '0'])
        if self.nes_clip_sides():
            options.extend(['-nes.clipsides', '1'])
        return options

    def nes_clip_sides(self):
        # FIXME: Sane default? -Or enable this in a per-game config
        # instead? SMB3 looks better with this
        return True

    def get_game_refresh_rate(self):
        if self.is_ntsc_game():
            return 60.0
        elif self.is_pal_game():
            return 50.0
        return None

    def get_mednafen_input_config(self):
        return 'Nintendo', [{
                'nes.input.port1.gamepad.a': 'A',
                'nes.input.port1.gamepad.b': 'B',
                'nes.input.port1.gamepad.up': 'UP',
                'nes.input.port1.gamepad.down': 'DOWN',
                'nes.input.port1.gamepad.left': 'LEFT',
                'nes.input.port1.gamepad.right': 'RIGHT',
                'nes.input.port1.gamepad.select': 'SELECT',
                'nes.input.port1.gamepad.start': 'START',
            }, {
                'nes.input.port2.gamepad.a': 'A',
                'nes.input.port2.gamepad.b': 'B',
                'nes.input.port2.gamepad.up': 'UP',
                'nes.input.port2.gamepad.down': 'DOWN',
                'nes.input.port2.gamepad.left': 'LEFT',
                'nes.input.port2.gamepad.right': 'RIGHT',
                'nes.input.port2.gamepad.select': 'SELECT',
                'nes.input.port2.gamepad.start': 'START',
            }]

