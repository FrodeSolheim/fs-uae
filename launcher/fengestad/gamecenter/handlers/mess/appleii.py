from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
from fengestad.gamecenter.diskutil import DiskUtil
from .mess import MessController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class AppleIIController(MessController):

    def configure_mess(self):
        self.args.append(self.get_apple_system())
        path = self.context.game.file
        #self.args.extend(['-s6d1', path])
        #self.args.extend(['-flop1', path])

        disks_dir = os.path.join(self.context.temp.dir('mess'), 'Disks')
        os.makedirs(disks_dir)
        found_disks = DiskUtil.find_disks(self.context.game.file,
                script=self.context.game.script)
        disks = []
        for p in found_disks:
            dest_path = os.path.join(disks_dir, os.path.basename(p))
            print("copy", p, "=>", dest_path)
            shutil.copy(p, dest_path)
            disks.append(dest_path)

        self.args.extend(['-{0}'.format(self.get_drive_name(0)), disks[0]])
        if len(disks) > 1:
            self.args.extend(['-{0}'.format(self.get_drive_name(1)), disks[1]])

    def get_apple_system(self):
        return 'apple2e'

    def get_drive_name(self, index):
        return 'flop{0}'.format(index + 1)


class AppleIIGSController(AppleIIController):

    def get_apple_system(self):
        return 'apple2gs'

    def get_drive_name(self, index):
        return 'flop{0}'.format(index + 3)

