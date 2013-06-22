from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
import math
import shutil
import tempfile
import subprocess
import zipfile
import wx
from fengestad import fs
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class AtariSTController(GameController):

    def configure_emulator(self):
        self.args = []
        self.env = {}
        temp_dir = self.context.temp.dir('hatari')
        config_file = os.path.join(temp_dir, 'hatari.cfg')
        with open(config_file, 'w') as f:
            self._configure_emulator(f)
        self.args.extend(['--configfile', config_file])

    def _configure_emulator(self,f ):

        floppies_dir = os.path.join(self.context.temp.dir('uae'), 'Disks')
        os.makedirs(floppies_dir)
        original_floppies = self.prepare_floppies()
        floppies = []
        for p in original_floppies:
            dest_path = os.path.join(floppies_dir, os.path.basename(p))
            shutil.copy(p, dest_path)
            floppies.append(dest_path)
        self.changes = ChangeHandler(floppies_dir)
        self.changes.init(os.path.join(self.context.game.state_dir, 'Disks'))
        num_floppy_drives = 2
        sorted_floppies = self.sort_floppies(floppies)
        inserted_floppies = sorted_floppies[:num_floppy_drives]

        f.write('[Floppy]\n')
        f.write('szDiskAFileName = {path}\n'.format(
                path=inserted_floppies[0]))
        if len(inserted_floppies) > 1:
            f.write('szDiskBFileName = {path}\n'.format(
                    path=inserted_floppies[1]))

        bios_path = self.find_tos('tos102us.img')
        f.write('[ROM]\n')
        f.write('szTosImageFileName = {path}\n'.format(path=bios_path))

        ATARI_WIDTH = 832
        ATARI_HEIGHT = 576
        try:
            viewportstr = self.context.game.config['viewport']
            sx, sy, sw, sh = viewportstr.split(",")
            sx, sy, sw, sh = int(sx), int(sy), int(sw), int(sh)
        except Exception:
            logger.exception("Could not get viewport information")
            sx, sy, sw, sh = 0, 0, ATARI_WIDTH, ATARI_HEIGHT

        if self.context.config.fullscreen:
            screenw, screenh = self.context.system.screen_size
            print("viewport is", sx, sy, sw, sh)
            scale = min(screenw / sw, screenh / sh)
            targetw = sw * scale
            targeth = sh * scale
            scale_x = targetw / sw
            scale_y = targeth / sh
            print("scale", scale_x, scale_y)
            org_scale_x = min(screenw / ATARI_WIDTH, screenh / ATARI_HEIGHT)
            org_scale_y = org_scale_x
            print("org_scale = ", org_scale_x, org_scale_y)
            offset_x = -(sx + (sw / 2.0) - ATARI_WIDTH / 2.0) / \
                    (ATARI_WIDTH / 2.0)
            offset_y = -(sy + (sh / 2.0) - ATARI_HEIGHT / 2.0) / \
                    (ATARI_HEIGHT / 2.0)

            hz = scale_x / org_scale_x
            vz = scale_y / org_scale_y
            #hz = int(round(scale_x / org_scale_x * 100000000))
            #vz = int(round(scale_y / org_scale_y * 100000000))
            print("horizontal zoom:", hz / 100000000.0);
            print("vertical zoom:", vz / 100000000.0);
            #ho = int(round(offset_x * 100000000))
            #vo = int(round(offset_y * 100000000))
            #print("horizontal offset:", ho / 100000000.0);
            #print("vertical offset:", vo / 100000000.0);
            self.env['FILTER_VERT_OFFSET'] = str(offset_y)
            self.env['FILTER_HORIZ_OFFSET'] = str(offset_x)
            self.env['FILTER_VERT_ZOOM'] = str(vz)
            self.env['FILTER_HORIZ_ZOOM'] = str(hz)

    def extract_match_name(self, name):
        name = os.path.normcase(name)
        pos_list = []
        pos_list.append(name.find('('))
        pos_list.append(name.find('['))
        pos_list.append(name.rfind('.'))
        pos_list.append(len(name))
        for pos in sorted(pos_list):
            if pos > -1:
                return name[:pos].strip().lower()

    def sort_floppies(self, floppies):
        return sorted(floppies)

    def prepare_floppies(self):
        floppies = []
        media_dir = os.path.dirname(self.context.game.file)
        base_match = self.extract_match_name(os.path.basename(
                self.context.game.file))
        for name in os.listdir(media_dir):
            dummy, ext = os.path.splitext(name)
            if ext.lower() not in ['.st']:
                continue
            match = self.extract_match_name(name)
            if base_match == match:
                floppies.append(os.path.join(media_dir, name))
                #floppies.append(name)
        return floppies

    def find_tos(self, bios):
        for dir in self.resources_dirs():
            bios_dir = os.path.join(dir, self.platform)
            for name in os.listdir(bios_dir):
                if bios in name:
                    return os.path.join(bios_dir, name)
        raise Exception("Could not find Atari TOS " + repr(bios))

    def create_emulator_process(self, env):
        #cwd = os.path.join(self.context.temp.dir('uae'))
        env.update(self.env)
        cwd = None
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.hatari')
        #if fs.windows:
        #    GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), cwd)
        #    args = self.args[:]
        #    args.insert(0, os.path.join(cwd, 'winuae.exe'))
        #    return subprocess.Popen(args, env=env, cwd=cwd, close_fds=True)
        return plugin.hatari(self.args, env=env, cwd=cwd, close_fds=True)

    def cleanup(self):
        self.changes.update(os.path.join(self.context.game.state_dir, 'Disks'))

