from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import subprocess
import shutil
import pyapp
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from ..mame import MameController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class MessController(MameController):

    def mame_init_input(self):
        self.mess_init_input()

    def mame_prepare(self):
        firmware_name = self.mess_get_firmware_name()
        if firmware_name:
            self.mess_firmware_dir = self.prepare_firmware(firmware_name)
        else:
            self.mess_firmware_dir = None

    def mame_configure(self):
        self.args.append(self.mess_get_romset())
        #self.configure_mame()
        #with open(self.mednafen_cfg_path(), 'wb') as f:
        #    self._configure_emulator(f)
        #bios_dir = self.get_bios_dir()
        #if bios_dir:
        #    self.args.extend(['-rompath', bios_dir])
        #self.args.extend(['-joystick_deadzone', '0.15'])

        self.mess_configure()
        #self.mess_configure_media()

    def run(self):
        plugin = pyapp.plugins.get_plugin('no.fengestad.emulator.mess')
        args = self.args[:]
        print("starting mess with args", args)
        return plugin.mess(args, env=self.env)

    def mame_get_input_mapping(self, port):
        return self.mess_get_input_mapping(port)

    def mame_get_romset(self):
        return self.mess_get_romset()

    def mame_get_bios_dir(self):
        print("xxxxxxxxx", self.mess_firmware_dir)
        return self.mess_firmware_dir

    def mame_get_offset_and_scale(self):
        return self.mess_get_offset_and_scale()

    def mess_get_offset_and_scale(self):
        return 0.0, 0.0, 1.0, 1.0

    def mess_get_firmware_name(self):
        return self.context.game.platform + ' Firmware'

#    def mess_configure_media(self):
#        pass

    def mess_configure_cartridge(self):
        self.args.extend(['-cart', self.context.game.file])

    def mess_configure(self):
        """override in subclasses to provide custom configuration"""
        pass

