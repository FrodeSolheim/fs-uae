from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import zlib
import shutil
import zipfile
import tempfile
import pkg_resources
import fs_uae_launcher.fs as fs
from .FSUAE import FSUAE
from .ConfigWriter import ConfigWriter
from .GameHandler import GameHandler
from .Settings import Settings
from .Database import Database
from .ROMManager import ROMManager
from .games.GameChangeHandler import GameChangeHandler

class LaunchHandler:

    def __init__(self, config_name, config, game_handler):
        self.config_name = config_name
        self.config = config.copy()
        self.game_handler = game_handler
        self.on_progress = None
        self.on_complete = None

    def run(self):
        print("LaunchHandler.run")
        self.temp_dir = tempfile.mkdtemp(prefix="fs-uae-")
        print("temp dir", self.temp_dir)
        self.config["floppies_dir"] = self.temp_dir

        print("state dir", self.get_state_dir())
        self.config["save_states_dir"] = self.get_state_dir()
        self.config["floppy_overlays_dir"] = self.get_state_dir()
        self.config["flash_memory_dir"] = self.get_state_dir()

        self.change_handler = GameChangeHandler(self.temp_dir)
        self.copy_floppies()
        self.unpack_hard_drives()
        self.copy_whdload_files()
        self.init_changes()

        self.prepare_theme()

        self.start()
        if self.on_complete:
            print("calling LaunchHandler.on_complete")
            self.on_complete()
        self.update_changes()
        self.cleanup()

    def copy_floppies(self):
        print("LaunchHandler.copy_floppies")
        def copy_floppy(key):
            src = self.config.get(key, "").strip()
            if not src:
                return
            if not os.path.exists(src):
                dirs = [Settings.get_floppies_dir()]
                for dir in dirs:
                    path = os.path.join(dir, src)
                    print("checking", repr(path))
                    if os.path.exists(path):
                        src = path
                        break
                else:
                    raise Exception("Cannot find floppy " + repr(src))
            dst = os.path.join(self.temp_dir, os.path.basename(src))
            shutil.copy2(src, dst)
            self.config[key] = os.path.basename(dst)
        for i in range(4):
            key = "floppy_drive_{0}".format(i)
            copy_floppy(key)
        max_image = -1
        for i in range(20):
            key = "floppy_image_{0}".format(i)
            copy_floppy(key)
            if self.config.get(key):
                max_image = i

        save_image = max_image + 1
        s = pkg_resources.resource_stream("fs_uae_launcher.res",
                "zipped_save_disk.dat")
        data = s.read()
        data = zlib.decompress(data)
        save_disk = os.path.join(self.temp_dir, u"Save Disk.adf")
        with open(save_disk, "wb") as f:
            f.write(data)
        key = "floppy_image_{0}".format(save_image)
        self.config[key] = "Save Disk.adf"

    def unpack_hard_drives(self):
        print("LaunchHandler.unpack_hard_drives")
        for i in range(0, 10):
            key = "hard_drive_{0}".format(i)
            value = self.config.get(key, "")
            if value.endswith(".zip"):
                print("zipped hard drive", value)
                self.unpack_hard_drive(i, value)

    def unpack_hard_drive(self, i, zip_path):
        dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)
        self.unpack_zip(zip_path, dir_path)
        key = "hard_drive_{0}".format(i)
        self.config[key] = dir_path

    def copy_whdload_files(self):
        whdload_args = self.config.get("x_whdload_args", "").strip()
        if not whdload_args:
            return
        print("LaunchHandler.copy_whdload_files")
        dst_dir = os.path.join(self.temp_dir, "DH0")
        #src_dir = os.path.join(fs.get_home_dir(), "Games", "Amiga", "WHDLoad")
        src_dir = Settings.get_whdload_dir()
        self.copy_folder_tree(src_dir, dst_dir)

        whdload_dir = ""
        slave = whdload_args.split(" ", 1)[0]
        slave = slave.lower()
        for dir_path, dir_names, file_names in os.walk(dst_dir):
            for name in file_names:
                #print(name, slave)
                if name.lower() == slave:
                    print("found", name)
                    whdload_dir = dir_path[len(dst_dir):]
                    whdload_dir = whdload_dir.replace("\\", "/")
                    if whdload_dir[0] == "/":
                        whdload_dir = whdload_dir[1:]
                    break
        print("WHDLoad dir:", whdload_dir)
        print("WHDLoad args:", whdload_args)

        user_startup = os.path.join(dst_dir, "S")
        if not os.path.exists(user_startup):
            os.makedirs(user_startup)
        user_startup = os.path.join(user_startup, "User-Startup")
        with open(user_startup, "ab") as f:
            f.write("cd {0}\n".format(whdload_dir))
            f.write("WHDLoad {0}\n".format(whdload_args))
            f.write("uae-configuration SPC_QUIT 1\n")

        # FIXME:
        self.config["fast_memory"] = "8192"

        #from .Amiga import A500_KICKSTARTS
        self.copy_whdload_kickstart(dst_dir, "kick34005.A500",
                ["891e9a547772fe0c6c19b610baf8bc4ea7fcb785"])

        #from .Amiga import A1200_KICKSTARTS
        self.copy_whdload_kickstart(dst_dir, "kick40068.A1200",
                ["e21545723fe8374e91342617604f1b3d703094f1"])

        #from .Amiga import A4000_KICKSTARTS
        self.copy_whdload_kickstart(dst_dir, "kick40068.A4000",
                ["5fe04842d04a489720f0f4bb0e46948199406f49"])

    def copy_whdload_kickstart(self, base_dir, name, checksums):
        dest = os.path.join(base_dir, "Devs", "Kickstarts")
        if not os.path.exists(dest):
            os.makedirs(dest)
        dest = os.path.join(dest, name)
        for checksum in checksums:
            path = Database().find_file(sha1=checksum)
            if path and os.path.exists(path):
                print("found kickstart for", name, "at", path)
                ROMManager.decrypt_rom(path, dest)
                break
        else:
            print("did not find kickstart for", name)

    def get_state_dir(self):
        return self.game_handler.get_state_dir()

    def init_changes(self):
        print("LaunchHandler.init_changes")
        self.change_handler.init(self.get_state_dir(), ignore=["*.uss", "*.sdf"])

    def update_changes(self):
        print("LaunchHandler.update_changes")
        self.change_handler.update(self.get_state_dir())

    def cleanup(self):
        print("LaunchHandler.cleanup")
        #self.delete_tree(self.temp_dir)
        shutil.rmtree(self.temp_dir, ignore_errors=True)
        state_dir = self.get_state_dir()
        try:
            # this will only succeed if the directory is empty -we don't
            # want to leave unnecessary empty state directories
            print("trying to remove", state_dir)
            os.rmdir(state_dir)
            # also try to remove the parent (letter dir)
            print("trying to remove", os.path.dirname(state_dir))
            os.rmdir(os.path.dirname(state_dir))
        except Exception, e:
            print(repr(e))

    def prepare_theme(self):
        path = self.game_handler.get_theme_path()
        if path:
            self.config["theme"] = path

    def start(self):
        print("LaunchHandler.start")
        config = ConfigWriter(self.config).create_fsuae_config()
        process, config_file = FSUAE.start_with_config(config)
        process.wait()
        print("LaunchHandler.start is done")
        print("removing", config_file)
        try:
            os.remove(config_file)
        except Exception:
            print("could not remove config file", config_file)

    def unpack_zip(cls, archive, destination):
        try:
            print("unpack", archive, "to", destination)
            zip = zipfile.ZipFile(archive, "r")

            def extract_members(zip):
                for name in zip.namelist():
                    if ".." in name:
                        continue
                    if not os.path.normpath(os.path.join(destination, name)
                            ).startswith(destination):
                        raise Exception("Invalid path")
                    yield name
            zip.extractall(path=destination, members=extract_members(zip))
            zip.close()
        except Exception, e:
            print(e)
            raise fs.cause(Exception("Could not unpack game"), e)

    def copy_folder_tree(cls, sourcepath, destpath, overwrite=False):
        for item in os.listdir(sourcepath):
            if item[0] == ".":
                continue
            itempath = os.path.join(sourcepath, item)
            destitempath = os.path.join(destpath, item)
            if os.path.isdir(itempath):
                if not os.path.exists(destitempath):
                    os.makedirs(destitempath)
                cls.copy_folder_tree(itempath, destitempath)
            else:
                if overwrite or not os.path.exists(destitempath):
                    shutil.copy(itempath, destitempath)

