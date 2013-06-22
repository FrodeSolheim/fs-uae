from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import subprocess
from fengestad import fs
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class NintendoDSController(GameController):

    def configure(self):
        temp_dir = self.context.temp.dir('desmume')
        config_file = os.path.join(temp_dir, 'desmume.ini')
        with open(config_file, 'w') as f:
            self._configure(f)
        self.args.append(self.context.game.file)

    def _configure(self, f):
        f.write('[3D]\n')
        f.write('Renderer=2\n')
        f.write('[Display]\n')
        f.write('ScreenGap=64\n')
        f.write('ScreenGapColor=0\n')
        f.write('Show Toolbar=0\n')
        if self.options.fullscreen:
            self.env['DESMUME_FULLSCREEN'] = '1'
        #    f.write('Display Method=1\n')
        #else:
        #    f.write('Display Method=0\n')
        f.write('[Emulation]\n')
        # FIXME: Decreases compatibility but increases performance
        f.write('AdvancedTiming=0\n')

    def create_emulator_process(self, env):
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.desmume')
        if fs.windows:
            temp_dir = self.context.temp.dir('desmume')
            GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
            args = self.args[:]
            args.insert(0, os.path.join(temp_dir, 'desmume.exe'))
            return subprocess.Popen(args, env=env, close_fds=True)
        return plugin.desmume(self.args, env=env)

