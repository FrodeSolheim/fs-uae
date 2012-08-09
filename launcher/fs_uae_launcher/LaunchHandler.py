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
from .Archive import Archive
from .ConfigWriter import ConfigWriter
from .DownloadService import DownloadService
from .GameHandler import GameHandler
from .Settings import Settings
from .Database import Database
from .ROMManager import ROMManager
from .games.GameChangeHandler import GameChangeHandler
from .I18N import _, ngettext

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
        self.prepare_roms()
        self.copy_floppies()
        self.unpack_hard_drives()
        self.copy_whdload_files()
        self.init_changes()

        self.prepare_theme()

        self.start()
        self.update_changes()
        self.cleanup()
        if self.on_complete:
            print("calling LaunchHandler.on_complete")
            self.on_complete()

    def prepare_roms(self):
        print("LaunchHandler.prepare_roms")
        roms = []
        roms.append(("kickstart_file", self.config.get("kickstart_file")))
        if self.config.get("kickstart_ext_file"):
            # not all Amigas have extended ROMs
            roms.append(("kickstart_ext_file",
                    self.config.get("kickstart_ext_file")))
        for config_key, src in roms:
            if not src:
                continue
            if src == "internal":
                continue
            
            archive = Archive(src)
            if not archive.exists(src):
                dirs = [Settings.get_kickstarts_dir()]
                for dir in dirs:
                    path = os.path.join(dir, src)
                    print("checking", repr(path))
                    archive = Archive(path)
                    if archive.exists(path):
                        src = path
                        break
                else:
                    raise Exception("Cannot find kickstart " + repr(src))
            dest = os.path.join(self.temp_dir, os.path.basename(src))
            
            with open(dest, "wb") as f:
                ROMManager.decrypt_archive_rom(archive, src, file=f)
                self.config[config_key] = os.path.basename(src)
            self.config["kickstarts_dir"] = self.temp_dir

    def copy_floppy(self, key):
        src = self.config.get(key, "").strip()
        if not src:
            return
        if src.startswith("http://") or src.startswith("https://"):
            url, name = src.rsplit("/", 1)
            self.on_progress(_("Downloading {0}...".format(name)))
            dest = os.path.join(self.temp_dir, name)
            DownloadService.install_file_from_url(url, dest)
            self.config[key] = name
            return

        archive = Archive(src)
        if not archive.exists(src):
            dirs = [Settings.get_floppies_dir()]
            for dir in dirs:
                path = os.path.join(dir, src)
                print("checking", repr(path))
                archive = Archive(path)
                if archive.exists(path):
                #if os.path.exists(path):
                    src = path
                    break
            else:
                raise Exception("Cannot find floppy " + repr(src))
        dest = os.path.join(self.temp_dir, os.path.basename(src))
        #shutil.copy2(src, dest)
        archive.copy(src, dest)
        self.config[key] = os.path.basename(dest)

    def copy_floppies(self):
        print("LaunchHandler.copy_floppies")
        self.on_progress(_("Preparing floppy images..."))

        for i in range(4):
            key = "floppy_drive_{0}".format(i)
            self.copy_floppy(key)
        max_image = -1
        for i in range(20):
            key = "floppy_image_{0}".format(i)
            self.copy_floppy(key)
            if self.config.get(key):
                max_image = i

        save_image = max_image + 1
        s = pkg_resources.resource_stream("fs_uae_launcher",
                "res/zipped_save_disk.dat")
        data = s.read()
        data = zlib.decompress(data)
        save_disk = os.path.join(self.temp_dir, u"Save Disk.adf")
        with open(save_disk, "wb") as f:
            f.write(data)
        key = "floppy_image_{0}".format(save_image)
        self.config[key] = "Save Disk.adf"

    def unpack_hard_drives(self):
        print("LaunchHandler.unpack_hard_drives")
        self.on_progress(_("Preparing hard drives..."))
        for i in range(0, 10):
            key = "hard_drive_{0}".format(i)
            value = self.config.get(key, "")
            if value.endswith(".zip"):
                print("zipped hard drive", value)
                self.unpack_hard_drive(i, value)

    def unpack_hard_drive(self, i, zip_path):
        dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)
        #self.unpack_zip(zip_path, dir_path)
        self.unpack_archive(zip_path, dir_path)
        key = "hard_drive_{0}".format(i)
        self.config[key] = dir_path

    def copy_whdload_files(self):
        whdload_args = self.config.get("x_whdload_args", "").strip()
        if not whdload_args:
            return
        print("LaunchHandler.copy_whdload_files")
        self.on_progress(_("Preparing WHDLoad..."))
        dest_dir = os.path.join(self.temp_dir, "DH0")
        #src_dir = os.path.join(fs.get_home_dir(), "Games", "Amiga", "WHDLoad")

        whdload_dir = ""
        slave = whdload_args.split(" ", 1)[0]
        slave = slave.lower()
        for dir_path, dir_names, file_names in os.walk(dest_dir):
            for name in file_names:
                #print(name, slave)
                if name.lower() == slave:
                    print("found", name)
                    whdload_dir = dir_path[len(dest_dir):]
                    whdload_dir = whdload_dir.replace("\\", "/")
                    if whdload_dir[0] == "/":
                        whdload_dir = whdload_dir[1:]
                    break
        print("WHDLoad dir:", repr(whdload_dir))
        print("WHDLoad args:", whdload_args)

        s_dir = os.path.join(dest_dir, "S")
        if not os.path.exists(s_dir):
            os.makedirs(s_dir)

        libs_dir = os.path.join(dest_dir, "Libs")
        if not os.path.exists(libs_dir):
            os.makedirs(libs_dir)

        self.copy_whdload_kickstart(dest_dir, "kick34005.A500",
                ["891e9a547772fe0c6c19b610baf8bc4ea7fcb785"])
        self.copy_whdload_kickstart(dest_dir, "kick40068.A1200",
                ["e21545723fe8374e91342617604f1b3d703094f1"])
        self.copy_whdload_kickstart(dest_dir, "kick40068.A4000",
                ["5fe04842d04a489720f0f4bb0e46948199406f49"])
        self.create_whdload_prefs_file(os.path.join(s_dir, "WHDLoad.prefs"))

        self.install_whdload_file("1ad1b55e7226bd5cd66def8370a69f19244da796",
                dest_dir, "Devs/Kickstarts/kick40068.A1200.RTB")
        self.install_whdload_file("209c109855f94c935439b60950d049527d2f2484",
                dest_dir, "Devs/Kickstarts/kick34005.A500.RTB")
        self.install_whdload_file("973b42dcaf8d6cb111484b3c4d3b719b15f6792d",
                dest_dir, "Devs/Kickstarts/kick40068.A4000.RTB")
        self.install_whdload_file("09e4d8a055b4a9801c6b011e7a3de42bafaf070d",
                dest_dir, "C/uae-configuration")
        self.install_whdload_file("100d80eead41511dfef6086508b1f77d3c1672a8",
                dest_dir, "C/RawDIC")
        self.install_whdload_file("e1b6c3871d8327f771874b17258167c2a454029a",
                dest_dir, "C/Patcher")
        self.install_whdload_file("0ec213a8c62beb3eb3b3509aaa44f21405929fce",
                dest_dir, "C/WHDLoad")
        self.install_whdload_file("57f29b23cff0107eec81b98159ce304ccd69441a",
                dest_dir, "C/WHDLoadCD32")
        self.install_whdload_file("2fcb5934019133ed7a2069e333cdbc349ecaa7ee",
                dest_dir, "C/DIC")

        startup_sequence = os.path.join(s_dir, "startup-sequence")
        with open(startup_sequence, "wb") as f:
            f.write("cd {0}\n".format(whdload_dir))
            f.write("WHDLoad {0}\n".format(whdload_args))
            #f.write("WHDLoad DH0:{0}/{1}\n".format(whdload_dir, whdload_args))
            f.write("uae-configuration SPC_QUIT 1\n")

        if self.config["__netplay_game"]:
            print("WHDLoad base dir is not copied in net play mode ")
        else:
            src_dir = Settings.get_whdload_dir()
            if src_dir and os.path.exists(src_dir):
                print("WHDLoad base dir exists, copying resources...")
                self.copy_folder_tree(src_dir, dest_dir)

        # The User-Startup file is useful if the user has provided a
        # base WHDLoad directory with an existing startup-sequence
        user_startup = os.path.join(s_dir, "User-Startup")
        with open(user_startup, "ab") as f:
            f.write("cd {0}\n".format(whdload_dir))
            f.write("WHDLoad {0}\n".format(whdload_args))
            f.write("uae-configuration SPC_QUIT 1\n")

        # FIXME:
        self.config["fast_memory"] = "8192"

    def install_whdload_file(self, sha1, dest_dir, rel_path):
        abs_path = os.path.join(dest_dir, rel_path)
        name = os.path.basename(rel_path)
        self.on_progress(_("Downloading {0}...".format(name)))
        DownloadService.install_file_by_sha1(sha1, abs_path)

    def create_whdload_prefs_file(self, path):
        default_prefs = """
;ButtonWait            ;wait for button pressed (slave must support this)
;ChipNoCache            ;disable cachebility of Chip-Memory
;CoreDumpPath=T:        ;path for coredump files
;DebugKey=$5b            ;rawkey code to quit with coredump (debug)
;ExecuteStartup=rx offline.rexx    ;command to execute on WHDLoad startup
;ExecuteCleanup=rx online.rexx    ;command to execute on WHDLoad exit
;Expert                ;selects expert mode
;FreezeKey=$5d            ;rawkey code to enter HrtMon/TK
;MMU                ;use MMU (for 68030)
;NoAutoVec            ;ignore unwanted autovector interrupts
;NoFilter            ;disable audio filter
;NoFlushMem            ;do not flush memory
;NoMemReverse            ;do not allocate memory reverse
;NoWriteCache            ;disable the disk write cache
QuitKey=$45
;ReadDelay=150            ;wait after reading from disk (1/50 seconds)
;RestartKey=$5c            ;rawkey code to restart
;ShowRegs=SYS:Utilities/MuchMore W WL=80 WT=80 WW=582 WH=700    ;command for Show Regs
SplashDelay=0        ;time to display splash window (1/50 seconds)
;WriteDelay=150            ;wait after saving something to disk (1/50 seconds)
"""
        # make sure the data is CRLF line terminated
        default_prefs = default_prefs.replace("\r\n", "\n")
        default_prefs = default_prefs.replace("\n", "\r\n")
        with open(path, "wb") as f:
            f.write(default_prefs)

    def copy_whdload_kickstart(self, base_dir, name, checksums):
        dest = os.path.join(base_dir, "Devs", "Kickstarts")
        if not os.path.exists(dest):
            os.makedirs(dest)
        dest = os.path.join(dest, name)
        for checksum in checksums:
            #print("find kickstart with sha1", checksum)
            path = Database().find_file(sha1=checksum)
            if path:# and os.path.exists(path):
                print("found kickstart for", name, "at", path)
                #ROMManager.decrypt_rom(path, dest)
                #break
                archive = Archive(path)
                if archive.exists(path):
                    with open(dest, "wb") as f:
                        ROMManager.decrypt_archive_rom(archive, path, file=f)
                    print(repr(dest))
                    break
        else:
            print("did not find kickstart for", name)

    def get_state_dir(self):
        return self.game_handler.get_state_dir()

    def init_changes(self):
        print("LaunchHandler.init_changes")
        self.on_progress(_("Restoring changes..."))
        self.change_handler.init(self.get_state_dir(),
                ignore=["*.uss", "*.sdf"])

    def update_changes(self):
        print("LaunchHandler.update_changes")
        self.on_progress(_("Saving changes..."))
        self.change_handler.update(self.get_state_dir())

    def cleanup(self):
        print("LaunchHandler.cleanup")
        self.on_progress(_("Cleaning up..."))
        #self.delete_tree(self.temp_dir)
        shutil.rmtree(self.temp_dir, ignore_errors=True)
        state_dir = self.get_state_dir()
        try:
            # this will only succeed if the directory is empty -we don't
            # want to leave unnecessary empty state directories
            os.rmdir(state_dir)
            print("removed", repr(state_dir))
            # also try to remove the parent (letter dir)
            os.rmdir(os.path.dirname(state_dir))
            print("removed", repr(os.path.dirname(state_dir)))
        except OSError, e:
            # could not delete directories - ok - probably has content
            pass

    def prepare_theme(self):
        path = self.game_handler.get_theme_path()
        if path:
            self.config["theme"] = path

    def start(self):
        print("LaunchHandler.start")
        self.on_progress(_("Starting FS-UAE..."))
        config = ConfigWriter(self.config).create_fsuae_config()
        process, config_file = FSUAE.start_with_config(config)
        process.wait()
        print("LaunchHandler.start is done")
        print("removing", config_file)
        try:
            os.remove(config_file)
        except Exception:
            print("could not remove config file", config_file)

    def unpack_archive(cls, path, destination):
        #try:
        print("unpack", path, "to", destination)
        archive = Archive(path)
        print(archive)
        print(archive.get_handler())
        for name in archive.list_files():
            print(name)
            f = archive.open(name)
            n = name[len(path) + 1:]
            out_path = os.path.join(destination, n)
            print("out path", out_path)
            if not os.path.exists(os.path.dirname(out_path)):
                os.makedirs(os.path.dirname(out_path))
            with open(out_path, "wb") as out_f:
                while True:
                    data = f.read(65536)
                    if not data:
                        break
                    out_f.write(data)
        #raise Exception("unpack")
            #zip = zipfile.ZipFile(archive, "r")
            #   
            #def extract_members(zip):
            #    for name in zip.namelist():
            #        if ".." in name:
            #            continue
            #       if not os.path.normpath(os.path.join(destination, name)
            #               ).startswith(destination):
            #            raise Exception("Invalid path")
            #        yield name
            #zip.extractall(path=destination, members=extract_members(zip))
            #zip.close()
        #except Exception, e:
        #    print(e)
        #    raise fs.cause(Exception("Could not unpack game"), e)

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
