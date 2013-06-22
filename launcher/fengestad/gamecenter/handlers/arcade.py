from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
from fengestad import fs
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from .mame import MameController
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class ArcadeController(MameController):

    def mame_init(self):
        #for i in range(4):
        #    option = InputOption(i)
        #    self.options.append(option)
        pass

    def mame_init_input(self):
        self.inputs = []
        for i in range(4):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='arcade',
                    description='Joystick'))

    #def prepare(self):
    #    print("ArcadeController.prepare")

    #def configure(self):
    #    self.configure_mame()

    #def run(self):
    #    self.run_mame()

    def mame_get_romset(self):
        #try:
        #    romset = self.context.game.config['romset']
        #    print("romset from config:", romset)
        #except Exception:
        #    config_name = self.context.game.config['name']
        #    romset = config_name.rsplit(',', 1)[-1].strip()
        #    print("romset from filename:", romset)
        romset, ext = os.path.splitext(os.path.basename(
                self.context.game.file))
        return romset

    def mame_get_input_mapping(self, port):
        mapping = {
            'start': 'START#',
            'select': 'COIN#',
            #'start': "P#_START",
            #'select': "P#_SELECT",
            'up': "P#_JOYSTICK_UP",
            'down': "P#_JOYSTICK_DOWN",
            'left': "P#_JOYSTICK_LEFT",
            'right': "P#_JOYSTICK_RIGHT",
        }
        button_order = self.context.game.config.get("buttonorder",
                "1 2 3 4 5 6 7 8 9 10 11 12")
        b = 0
        for button in button_order.split(' '):
            button = button.strip()
            if button:
                b = b + 1
                mapping[str(b)] = "P#_BUTTON" + button
        return mapping


class InputOption(GameController.Option):

    def __init__(self, number):
        self.priority = 0.1 * number
        self.title = "Joystick"
        self.subtitle = "Controller #{0}".format(number)
        self.value = 'joystick'

    def activate(self):
        print("InputItem.activate")
        return [
            {'title': 'Nothing', 'subtitle': '', 'value': ''},
            {'title': 'Joystick', 'subtitle': '', 'value': 'joystick'}
        ]

