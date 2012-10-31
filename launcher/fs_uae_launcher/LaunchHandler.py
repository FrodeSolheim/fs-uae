from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import zlib
import shutil
import urllib
import zipfile
import tempfile
import pkg_resources
import fs_uae_launcher.fs as fs
import fs_uae_launcher.fsui as fsui
from .FSUAE import FSUAE
from .Archive import Archive
from .Amiga import Amiga
from .Config import Config
from .ConfigWriter import ConfigWriter
from .DownloadService import DownloadService
from .GameHandler import GameHandler
from .Settings import Settings
from .Database import Database
from .ROMManager import ROMManager
from .games.GameChangeHandler import GameChangeHandler
from .I18N import _, ngettext
from .Util import expand_path

class LaunchHandler:

    @classmethod
    def start_game(self):
        from .netplay.Netplay import Netplay
        if Netplay.game_channel:
            Netplay.start_netplay_game()
        else:
            LaunchHandler.start_local_game()

    @classmethod
    def start_local_game(self):
        if not Config.get("x_kickstart_file"):# or not \
                #os.path.exists(Config.get("kickstart_file")):
            fsui.show_error(_("No kickstart found for this model. " +
                    "Try 'scan' function."))
            return
        cs = Amiga.get_model_config(Config.get("amiga_model"))["ext_roms"]
        if len(cs) > 0:
            # extended kickstart ROM is needed
            if not Config.get("x_kickstart_ext_file"):
                fsui.show_error(_("No extended kickstart found for this "
                        "model. Try 'scan' function."))
                return

        #from .LaunchHandler import LaunchHandler
        handler = LaunchHandler(Settings.get("config_name"), Config,
                GameHandler.current())
        from .ui.LaunchDialog import LaunchDialog
        from .ui.MainWindow import MainWindow
        dialog = LaunchDialog(MainWindow.instance, handler)
        dialog.run()
        dialog.show_modal()
        dialog.close()

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
        self.prepare_cdroms()
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
            src = expand_path(src)

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

    def expand_default_path(self, src, default_dir):
        src = expand_path(src)
        archive = Archive(src)
        if not archive.exists(src):
            dirs = [default_dir]
            for dir in dirs:
                path = os.path.join(dir, src)
                print("checking", repr(path))
                archive = Archive(path)
                if archive.exists(path):
                #if os.path.exists(path):
                    src = path
                    break
            else:
                raise Exception("Cannot find path for " + repr(src))
        return src, archive

    def copy_floppy(self, key):
        src = self.config.get(key, "").strip()
        if not src:
            return
        if src.startswith("http://") or src.startswith("https://"):
            name = src.rsplit("/", 1)[-1]
            name = urllib.unquote(name)
            self.on_progress(_("Downloading {0}...".format(name)))
            dest = os.path.join(self.temp_dir, name)
            DownloadService.install_file_from_url(src, dest)
            self.config[key] = name
            return

        src, archive = self.expand_default_path(src,
                Settings.get_floppies_dir())

        dest = os.path.join(self.temp_dir, os.path.basename(src))
        #shutil.copy2(src, dest)
        archive.copy(src, dest)
        self.config[key] = os.path.basename(dest)

    def copy_floppies(self):
        print("LaunchHandler.copy_floppies")
        self.on_progress(_("Preparing floppy images..."))

        floppies = []
        for i in range(Amiga.MAX_FLOPPY_DRIVES):
            key = "floppy_drive_{0}".format(i)
            if self.config[key]:
                floppies.append(self.config[key])
            self.copy_floppy(key)
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
            key = "floppy_image_{0}".format(i)
            if self.config[key]:
                break
        else:
            print("floppy image list is empty")
            for i, floppy in enumerate(floppies):
                self.config["floppy_image_{0}".format(i)] = floppy

        max_image = -1
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
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

    def prepare_cdroms(self):
        print("LaunchHandler.prepare_cdroms")
        if not self.config.get("cdrom_drive_count"):
            if self.config.get("cdrom_drive_0") or \
                    self.config.get("cdrom_image_0"):
                self.config["cdrom_drive_count"] = "1"

        cdroms = []
        for i in range(Amiga.MAX_CDROM_DRIVES):
            key = "cdrom_drive_{0}".format(i)
            if self.config[key]:
                cdroms.append(self.config[key])
        for i in range(Amiga.MAX_CDROM_IMAGES):
            key = "cdrom_image_{0}".format(i)
            if self.config[key]:
                break
        else:
            print("CD-ROM image list is empty")
            for i, cdrom in enumerate(cdroms):
                self.config["cdrom_image_{0}".format(i)] = cdrom


    def unpack_hard_drives(self):
        print("LaunchHandler.unpack_hard_drives")
        self.on_progress(_("Preparing hard drives..."))
        for i in range(0, 10):
            key = "hard_drive_{0}".format(i)
            src = self.config.get(key, "")

            if src.startswith("http://") or src.startswith("https://"):
                name = src.rsplit("/", 1)[-1]
                name = urllib.unquote(name)
                self.on_progress(_("Downloading {0}...".format(name)))
                dest = os.path.join(self.temp_dir, name)
                DownloadService.install_file_from_url(src, dest)
                src = dest

            if src.endswith(".zip"):
                print("zipped hard drive", src)
                self.unpack_hard_drive(i, src)
            elif src.endswith("HardDrive"):
                print("XML-described hard drive", src)
                self.unpack_hard_drive(i, src)

    def unpack_hard_drive(self, i, src):
        src, archive = self.expand_default_path(src,
                Settings.get_hard_drives_dir())

        dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)
        #self.unpack_zip(zip_path, dir_path)
        self.unpack_archive(src, dir_path)
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
        found_slave = False
        for dir_path, dir_names, file_names in os.walk(dest_dir):
            for name in file_names:
                #print(name, slave)
                if name.lower() == slave:
                    print("found", name)
                    found_slave = True
                    whdload_dir = dir_path[len(dest_dir):]
                    whdload_dir = whdload_dir.replace("\\", "/")
                    if whdload_dir[0] == "/":
                        whdload_dir = whdload_dir[1:]
                    break
            if found_slave:
                break
        if not found_slave:
            raise Exception("Did not find the specified WHDLoad slave. "
                    "Check the WHDLoad arguments")
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

        whdload_files = whdload_17_1_files
        for key, value in whdload_files.iteritems():
            self.install_whdload_file(key, dest_dir, value)

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

    def install_whdload_file(self, sha1, dest_dir, rel_path):
        abs_path = os.path.join(dest_dir, rel_path)
        name = os.path.basename(rel_path)
        self.on_progress(_("Downloading {0}...".format(name)))
        DownloadService.install_file_by_sha1(sha1, name, abs_path)

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

whdload_17_0_files = {
    "1ad1b55e7226bd5cd66def8370a69f19244da796": "Devs/Kickstarts/kick40068.A1200.RTB",
    "209c109855f94c935439b60950d049527d2f2484": "Devs/Kickstarts/kick34005.A500.RTB",
    "973b42dcaf8d6cb111484b3c4d3b719b15f6792d": "Devs/Kickstarts/kick40068.A4000.RTB",
    "09e4d8a055b4a9801c6b011e7a3de42bafaf070d": "C/uae-configuration",
    "100d80eead41511dfef6086508b1f77d3c1672a8": "C/RawDIC",
    "e1b6c3871d8327f771874b17258167c2a454029a": "C/Patcher",
    "0ec213a8c62beb3eb3b3509aaa44f21405929fce": "C/WHDLoad",
    "57f29b23cff0107eec81b98159ce304ccd69441a": "C/WHDLoadCD32",
    "2fcb5934019133ed7a2069e333cdbc349ecaa7ee": "C/DIC",
}

whdload_17_1_files = {
    "1ad1b55e7226bd5cd66def8370a69f19244da796": "Devs/Kickstarts/kick40068.A1200.RTB",
    "209c109855f94c935439b60950d049527d2f2484": "Devs/Kickstarts/kick34005.A500.RTB",
    "973b42dcaf8d6cb111484b3c4d3b719b15f6792d": "Devs/Kickstarts/kick40068.A4000.RTB",
    "09e4d8a055b4a9801c6b011e7a3de42bafaf070d": "C/uae-configuration",
    "100d80eead41511dfef6086508b1f77d3c1672a8": "C/RawDIC",
    "e1b6c3871d8327f771874b17258167c2a454029a": "C/Patcher",
    "1a907ca4539806b42ad5b6f7aeebacb3720e840d": "C/WHDLoad",
    "a4f425b2c7e29600a970abd90f70a4dd4804c01c": "C/WHDLoadCD32",
    "2fcb5934019133ed7a2069e333cdbc349ecaa7ee": "C/DIC",
}
