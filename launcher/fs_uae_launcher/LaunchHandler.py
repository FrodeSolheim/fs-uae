from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import zlib
import json
import shutil
import urllib
import zipfile
import hashlib
import tempfile
import pkg_resources
import fs_uae_launcher.fs as fs
import fs_uae_launcher.fsui as fsui
from fs_uae_launcher.fsgs.GameNameUtil import GameNameUtil
from .FSUAE import FSUAE
from .Archive import Archive
from .Amiga import Amiga
from .Config import Config
from .ConfigWriter import ConfigWriter
from .DownloadService import DownloadService
from .GameHandler import GameHandler
from .Settings import Settings
from .Database import Database
from .Paths import Paths
from .ROMManager import ROMManager
from .fsgs.GameChangeHandler import GameChangeHandler
from .fsgs.GameDatabase import GameDatabase
from .fsgs.GameDatabaseClient import GameDatabaseClient
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
                    "Use the 'Import Kickstarts' function from the menu."))
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

    def on_progress(self, progress):
        # method can be overriden / replaced in instances
        pass

    def on_complete(self):
        # method can be overriden / replaced in instances
        pass

    def run(self, start=True, cleanup=True):
        print("LaunchHandler.run")
        self.temp_dir = tempfile.mkdtemp(prefix="fs-uae-")
        print("temp dir", self.temp_dir)
        self.config["floppies_dir"] = self.temp_dir

        print("state dir", self.get_state_dir())
        self.config["state_dir"] = self.get_state_dir()
        self.config["save_states_dir"] = ""
        self.config["floppy_overlays_dir"] = ""
        self.config["flash_memory_dir"] = ""

        self.change_handler = GameChangeHandler(self.temp_dir)
        self.prepare_roms()
        self.copy_floppies()
        self.prepare_cdroms()
        self.prepare_hard_drives()
        self.copy_whdload_files()
        self.init_changes()

        self.prepare_theme()
        self.prepare_extra_settings()

        if start:
            self.start()
        self.update_changes()
        if cleanup:
            self.cleanup()

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
        src = Paths.expand_path(src, default_dir)
        archive = Archive(src)
        #if not archive.exists(src):
        #    dirs = [default_dir]
        #    for dir in dirs:
        #        path = os.path.join(dir, src)
        #        print("checking", repr(path))
        #        archive = Archive(path)
        #        if archive.exists(path):
        #        #if os.path.exists(path):
        #            src = path
        #            break
        #    else:
        #        raise Exception("Cannot find path for " + repr(src))
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
        s = pkg_resources.resource_stream(str("fs_uae_launcher"),
                str("res/adf_save_disk.dat"))
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


    def prepare_hard_drives(self):
        print("LaunchHandler.prepare_hard_drives")
        self.on_progress(_("Preparing hard drives..."))
        for i in range(0, 10):
            key = "hard_drive_{0}".format(i)
            src = self.config.get(key, "")
            dummy, ext = os.path.splitext(src)
            ext = ext.lower()

            if src.startswith("http://") or src.startswith("https://"):
                name = src.rsplit("/", 1)[-1]
                name = urllib.unquote(name)
                self.on_progress(_("Downloading {0}...".format(name)))
                dest = os.path.join(self.temp_dir, name)
                DownloadService.install_file_from_url(src, dest)
                src = dest
            elif src.startswith("hd://game/"):
                self.unpack_game_hard_drive(i, src)
                self.disable_save_states()
                return
            elif src.startswith("hd://template/workbench/"):
                self.prepare_workbench_hard_drive(i, src)
                self.disable_save_states()
                return
            elif src.startswith("hd://template/empty/"):
                self.prepare_empty_hard_drive(i, src)
                self.disable_save_states()
                return

            if ext in [".zip", ".lha"]:
                print("zipped hard drive", src)
                self.unpack_hard_drive(i, src)
                self.disable_save_states()

            elif src.endswith("HardDrive"):
                print("XML-described hard drive", src)
                self.unpack_hard_drive(i, src)
                self.disable_save_states()
            else:
                src = Paths.expand_path(src)
                self.config[key] = src

    def disable_save_states(self):
        # Save states cannot currently be used with temporarily created
        # hard drives, as HD paths are embedded into the save states, and
        # restoring the save state causes problems.
        self.config["save_states"] = "0"

    def prepare_workbench_hard_drive(self, i, src):
        #dir_name = "DH{0}".format(i)
        dir_name = src.rsplit("/", 1)[-1]
        dir_path = os.path.join(self.temp_dir, dir_name)
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)

        amiga_model = self.config.get("amiga_model", "A500")
        if amiga_model.startswith("A1200") or amiga_model.startswith("A4000"):
            workbench = "Minimal Workbench v3.1"
        elif amiga_model == "A600":
            workbench = "Minimal Workbench v2.05"
        elif amiga_model == "A500+":
            workbench = "Minimal Workbench v2.04"
        else:
            workbench = "Minimal Workbench v1.3"
        
        print("Try to find pre-configured hard drive", workbench)
        src_dir = os.path.join(Settings.get_hard_drives_dir(), workbench)
        if src_dir and os.path.exists(src_dir):
            print("found", src_dir)
            self.copy_folder_tree(src_dir, dir_path)
        else:
            print(" - not found -")
            raise Exception("Did not found pre-configured hard drive " +
                    repr(workbench))
            
        self.config["hard_drive_{0}".format(i)] = dir_path

    def prepare_empty_hard_drive(self, i, src):
        dir_name = src.rsplit("/", 1)[-1]
        #dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)
        self.config["hard_drive_{0}".format(i)] = dir_path

    def unpack_game_hard_drive(self, i, src):
        scheme, dummy, dummy, game_uuid, drive = src.split("/")
        drive_prefix = drive + "/"
        database = Database()
        game_database = GameDatabase.get_instance()
        game_database_client = GameDatabaseClient(game_database)
        game_id = game_database_client.get_game_id(game_uuid)
        values = game_database_client.get_final_game_values(game_id)
        file_list = json.loads(values["file_list"])

        dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)
        for file_entry in file_list:
            name = file_entry["name"]
            if not name.startswith(drive_prefix):
                continue
            dst_file = os.path.join(dir_path, name[len(drive_prefix):])
            print(repr(dst_file))
            if name.endswith("/"):
                os.makedirs(fs.encode_path(dst_file))
                continue
            sha1 = file_entry["sha1"]
            src_file = database.find_file(sha1=sha1)
            if not os.path.exists(os.path.dirname(dst_file)):
                os.makedirs(os.path.dirname(dst_file))
            archive = Archive(src_file)
            f = archive.open(src_file)
            data = f.read()
            with open(dst_file, "wb") as out_file:
                out_file.write(data)
            metadata = ["----rwed", " ", "2000-01-01 00:00:00.00", " ", "",
                    "\n"]
            if "comment" in file_entry:
                metadata[4] = self.encode_file_comment(file_entry["comment"])
            with open(dst_file + ".uaem", "wb") as out_file:
                out_file.write("".join(metadata))
            
        self.config["hard_drive_{0}".format(i)] = dir_path

    def encode_file_comment(self, comment):
        result = []
        #raw = 0
        for c in comment:
        #    if c == '%':
        #        result.append("%")
        #        raw = 2
        #    elif raw:
        #        result.append(c)
        #        raw = raw - 1
        #    else:
        #        result.append("%{0:x}".format(ord(c)))
            result.append("%{0:x}".format(ord(c)))
        return "".join(result)

    def unpack_hard_drive(self, i, src):
        src, archive = self.expand_default_path(src,
                Settings.get_hard_drives_dir())

        dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)
        #self.unpack_zip(zip_path, dir_path)
        self.unpack_archive(src, dir_path)
        self.config["hard_drive_{0}".format(i)] = dir_path

    def copy_whdload_files(self):
        whdload_args = self.config.get("x_whdload_args", "").strip()
        if not whdload_args:
            return
        print("LaunchHandler.copy_whdload_files")
        self.on_progress(_("Preparing WHDLoad..."))
        dest_dir = os.path.join(self.temp_dir, "DH0")
        print("copy_whdload_files, dest_dir = ", dest_dir)
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

        devs_dir = os.path.join(dest_dir, "Devs")
        if not os.path.exists(devs_dir):
            os.makedirs(devs_dir)
        with open(os.path.join(devs_dir, "system-configuration"), "wb") as f:
            f.write(system_configuration)

        self.copy_whdload_kickstart(dest_dir, "kick34005.A500",
                ["891e9a547772fe0c6c19b610baf8bc4ea7fcb785"])
        self.copy_whdload_kickstart(dest_dir, "kick40068.A1200",
                ["e21545723fe8374e91342617604f1b3d703094f1"])
        self.copy_whdload_kickstart(dest_dir, "kick40068.A4000",
                ["5fe04842d04a489720f0f4bb0e46948199406f49"])
        self.create_whdload_prefs_file(os.path.join(s_dir, "WHDLoad.prefs"))
        self.copy_setpatch(dest_dir)

        whdload_files = whdload_17_1_files
        for key, value in whdload_files.iteritems():
            self.install_whdload_file(key, dest_dir, value)

        if self.config["__netplay_game"]:
            print("WHDLoad key is not copied in net play mode")
        else:
            key_file = os.path.join(Settings.get_base_dir(), "WHDLoad.key")
            if os.path.exists(key_file):
                print("found WHDLoad key at ", key_file)
                shutil.copy(key_file, os.path.join(s_dir, "WHDLoad.key"))
            else:
                print("WHDLoad key not found in base dir (FS-UAE dir)")

        if self.config["__netplay_game"]:
            print("WHDLoad base dir is not copied in net play mode")
        else:
            src_dir = Settings.get_whdload_dir()
            if src_dir and os.path.exists(src_dir):
                print("WHDLoad base dir exists, copying resources...")
                self.copy_folder_tree(src_dir, dest_dir)

        startup_sequence = os.path.join(s_dir, "Startup-Sequence")
        if not os.path.exists(startup_sequence):
            with open(startup_sequence, "wb") as f:
                f.write(setpatch_sequence.replace(
                        "\r\n", "\n").encode("ISO-8859-1"))
                f.write(whdload_sequence.format(whdload_dir,
                        whdload_args).replace(
                        "\r\n", "\n").encode("ISO-8859-1"))

        # The User-Startup file is useful if the user has provided a
        # base WHDLoad directory with an existing startup-sequence
        user_startup = os.path.join(s_dir, "User-Startup")
        with open(user_startup, "ab") as f:
            f.write(whdload_sequence.format(whdload_dir,
                    whdload_args).replace(
                    "\r\n", "\n").encode("ISO-8859-1"))

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
;QuitKey=$45
;ReadDelay=150            ;wait after reading from disk (1/50 seconds)
;RestartKey=$5c            ;rawkey code to restart
;ShowRegs=SYS:Utilities/MuchMore W WL=80 WT=80 WW=582 WH=700    ;command for Show Regs
;SplashDelay=0        ;time to display splash window (1/50 seconds)
;WriteDelay=150            ;wait after saving something to disk (1/50 seconds)
"""
        # make sure the data is CRLF line terminated
        default_prefs = default_prefs.replace("\r\n", "\n")
        default_prefs = default_prefs.replace("\n", "\r\n")
        with open(path, "wb") as f:
            f.write(default_prefs)

    def copy_setpatch(self, base_dir):
        dest = os.path.join(base_dir, "C")
        if not os.path.exists(dest):
            os.makedirs(dest)
        dest = os.path.join(dest, "SetPatch")
        for checksum in workbench_disks_with_setpatch_39_6:
            path = Database().find_file(sha1=checksum)
            if path:
                print("found WB DISK with SetPatch 39.6 at", path)
                archive = Archive(path)
                if archive.exists(path):
                    f = archive.open(path)
                    wb_data = f.read()
                    f.close()
                    if self.extract_setpatch_39_6(wb_data, dest):
                        return
                    else:
                        print("WARNING: extract_setpatch_39_6 returned False")
        else:
            print("WARNING: did not find SetPatch 39.6")

    def extract_setpatch_39_6(self, wb_data, dest):
        offset = wb_data.find(b"$VER: setpatch 39.6 (8.9.92)") - 0x845
        if offset < 0:
            return False
        setpatch_data = wb_data[offset:offset+7364]
        s = hashlib.sha1()
        s.update(setpatch_data)
        if s.hexdigest() != "4d4aae988310b07726329e436b2250c0f769ddff":
            return False
        with open(dest, "wb") as f:
            f.write(setpatch_data)
        return True

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

    def prepare_extra_settings(self):
        prefix = self.config.get("screenshots_output_prefix", "")
        if prefix:
            return
        name = self.config.get("floppy_drive_0", "")
        #if not name:
        #    name = self.config.get("hard_drive_0", "")
        if not name:
            name = self.config.get("cdrom_drive_0", "")
        #if not name:
        #    name = self.config.get("floppy_image_0", "")
        if not name:
            name = "fs-uae"
        name, variant = GameNameUtil.extract_names(name)
        name = GameNameUtil.create_cmpname(name)
        self.config["screenshots_output_prefix"] = name

    def create_config(self):
        config = ConfigWriter(self.config).create_fsuae_config()
        return config

    def start(self):
        print("LaunchHandler.start")
        self.on_progress(_("Starting FS-UAE..."))
        config = self.create_config()
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
                    print("copy", repr(itempath), "to", repr(destitempath))
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

system_configuration = b"\x08\x00\x00\x05\x00\x00\x00\x00\x00\x00" \
b"\xc3P\x00\x00\x00\x00\x00\t'\xc0\x00\x00\x00\x01\x00\x00N \x00\x00\x00\x00" \
b"\xc0\x00@\x00p\x00\xb0\x00<\x00L\x00?\x00C\x00\x1f\xc0 \xc0\x1f\xc0 \x00" \
b"\x0f\x00\x11\x00\r\x80\x12\x80\x04\xc0\t@\x04`\x08\xa0\x00 \x00@\x00\x00" \
b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
b"\x00\x00\x00\x00\xff\x00\x0eD\x00\x00\x0e\xec\x00\x01\n\xaa\x00\x00\x0f" \
b"\xff\x06\x8b\x00\x00\x00\x81\x00,\x00\x00\x00\x00generic\x00\x00\x00\x00" \
b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
b"\x00\x00\x00\x00\x00\x00\x00\x00\x05\x00K\x00\x00\x00\x00\x00\x00\x00\x07" \
b"\x00 \x00B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"

workbench_disks_with_setpatch_39_6 = [
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 1 of 6)(Install).adf
"ba24b4172339b9198e4f724a6804d0c6eb5e394b",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 1 of 6)(Install)[a].adf
"c0781dece2486b54e15ce54a9b24dec6d9429421",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 1 of 6)(Install)[m drive definitions].adf
"7eeb2511ce34f8d3f09efe82b290bddeb899d237",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 1 of 6)(Install)[m2].adf
"7271d7db4472e10fbe4b266278e16f03336c14e3",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 1 of 6)(Install)[m3].adf
"92c2f33bb73e1bdee5d9a0dc0f5b09a15524f684",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[!].adf
"e663c92a9c88fa38d02bbb299bea8ce70c56b417",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[a2].adf
"65ab988e597b456ac40320f88a502fc016d590aa",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[a].adf
"9496daa66e6b2f4ddde4fa2580bb3983a25e3cd2",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[m2].adf
"cf2f24cf5f5065479476a38ec8f1016b1f746884",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[m3].adf
"0e7f30223af254df0e2b91ea409f35c56d6164a6",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[m4].adf
"08c4afde7a67e6aaee1f07af96e95e9bed897947",
# amiga-os-300-workbench.adf
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[m5].adf
"4f4770caae5950eca4a2720e0424df052ced6a32",
# Workbench v3.0 rev 39.29 (1992)(Commodore)(A1200-A4000)(M10)(Disk 2 of 6)(Workbench)[m].adf
"53086c3e44ec2d34e60ab65af71fb11941f4e0af",
]

setpatch_sequence = """
IF EXISTS C:SetPatch
C:SetPatch
ELSE
echo "Warning: SetPatch (39.6) not found."
echo "Make sure a WB 3.0 disk is scanned in FS-UAE Launcher"
echo "and the file will automatically be copied from the disk."
EndIF
"""

whdload_sequence = """
cd "{0}"
WHDLoad {1}
uae-configuration SPC_QUIT 1
"""
