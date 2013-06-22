from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.six as six
import os
import json
import shutil
import urllib
import hashlib
import tempfile
import unittest
import zlib
#import pkg_resources

import fs_uae_launcher.fs as fs

from fsbc.Resources import Resources

from fsgs.Archive import Archive
from fsgs.Downloader import Downloader
from fsgs.GameChangeHandler import GameChangeHandler
from fsgs.GameNameUtil import GameNameUtil
from fsgs.GameDatabaseClient import GameDatabaseClient
from fsgs.Paths import Paths

from fsgs.amiga.ADFFileExtractor import ADFFileExtractor
from fsgs.amiga.Amiga import Amiga
from fsgs.amiga.ConfigWriter import ConfigWriter
from fsgs.amiga.FSUAE import FSUAE
from fsgs.amiga.ROMManager import ROMManager

# FIXME, remove dependency on fs_uae_launcher
from fs_uae_launcher.I18N import _, ngettext


class LaunchHandler:

    def __init__(self, fsgs, config_name, config, game_paths):
        self.fsgs = fsgs
        self.config_name = config_name
        self.config = config.copy()
        self.game_paths = game_paths
        self.hd_requirements = set()
        self.setpatch_installed = False
        self.stop_flag = False

    def stop(self):
        print("LauncherHandler - set stop flag")
        self.stop_flag = True

    def on_progress(self, progress):
        # method can be overriden / replaced in instances
        pass

    def on_complete(self):
        # method can be overriden / replaced in instances
        pass

    def prepare(self):
        print("LaunchHandler.prepare")
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
        if self.stop_flag:
            return
        self.copy_floppies()
        if self.stop_flag:
            return
        self.prepare_cdroms()
        if self.stop_flag:
            return
        self.prepare_hard_drives()
        if self.stop_flag:
            return
        self.copy_hd_files()
        if self.stop_flag:
            return
        self.init_changes()
        if self.stop_flag:
            return
        self.prepare_theme()
        if self.stop_flag:
            return
        self.prepare_extra_settings()

    def run_sequence(self, start=True, cleanup=True):
        print("LaunchHandler.run_sequence")
        self.prepare()

        if not self.stop_flag:
            # too late to stop now...
            if start:
                self.run()
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
            src = Paths.expand_path(src)

            archive = Archive(src)
            if not archive.exists(src):
                dirs = [self.fsgs.amiga.get_kickstarts_dir()]
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
        if "://" in src:
            return src, None
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

        src, archive = self.expand_default_path(src,
                self.fsgs.amiga.get_floppies_dir())
        ifs = self.fsgs.file.open(src)
        if not ifs:
            raise RuntimeError("Could not find floppy image "
                               "for {0}".format(src))

        dest_name = os.path.basename(src)
        dest = os.path.join(self.temp_dir, dest_name)
        dest_partial = dest + ".partial"
        with open(dest_partial, "wb") as ofs:
            ifs_sha1 = hashlib.sha1()
            while True:
                data = ifs.read()
                if not data:
                    break
                ifs_sha1.update(data)
                ofs.write(data)
        print("rename floppy from", dest_partial, "to", dest)
        if os.path.exists(dest):
            print("removing existing file", dest)
            os.remove(dest)
        os.rename(dest_partial, dest)
        #shutil.copy2(src, dest)
        #archive.copy(src, dest)
        self.config[key] = os.path.basename(dest)

    def copy_floppies(self):
        print("LaunchHandler.copy_floppies")
        self.on_progress(_("Preparing floppy images..."))

        floppies = []
        for i in range(Amiga.MAX_FLOPPY_DRIVES):
            key = "floppy_drive_{0}".format(i)
            if self.config.get(key, ""):
                floppies.append(self.config[key])
            self.copy_floppy(key)
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
            key = "floppy_image_{0}".format(i)
            if self.config.get(key, ""):
                break
        else:
            print("floppy image list is empty")
            for i, floppy in enumerate(floppies):
                self.config["floppy_image_{0}".format(i)] = floppy

        max_image = -1
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
            key = "floppy_image_{0}".format(i)
            self.copy_floppy(key)
            if self.config.get(key, ""):
                max_image = i

        save_image = max_image + 1
        
        #s = pkg_resources.resource_stream(str("fs_uae_launcher"),
        #        str("res/adf_save_disk.dat"))
        # FIXME: remove dependency on fs_uae_launcher package
        s = Resources("fs_uae_launcher", "res").stream("adf_save_disk.dat")

        data = s.read()
        data = zlib.decompress(data)
        save_disk = os.path.join(self.temp_dir, "Save Disk.adf")
        with open(save_disk, "wb") as f:
            f.write(data)
        key = "floppy_image_{0}".format(save_image)
        self.config[key] = "Save Disk.adf"

    def prepare_cdroms(self):
        print("LaunchHandler.prepare_cdroms")
        if not self.config.get("cdrom_drive_count", ""):
            if self.config.get("cdrom_drive_0", "") or \
                    self.config.get("cdrom_image_0", ""):
                self.config["cdrom_drive_count"] = "1"

        cdroms = []
        for i in range(Amiga.MAX_CDROM_DRIVES):
            key = "cdrom_drive_{0}".format(i)
            if self.config.get(key, ""):
                cdroms.append(self.config[key])
        for i in range(Amiga.MAX_CDROM_IMAGES):
            key = "cdrom_image_{0}".format(i)
            if self.config.get(key, ""):
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
                Downloader.install_file_from_url(src, dest)
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
        
        if self.config.get("unsafe_save_states") == "1":
            return
        self.config["save_states"] = "0"

    def prepare_workbench_hard_drive(self, i, src):
        #dir_name = "DH{0}".format(i)
        dir_name = src.rsplit("/", 1)[-1]
        dir_path = os.path.join(self.temp_dir, dir_name)
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)

        amiga_model = self.config.get("amiga_model", "A500")
        if amiga_model.startswith("A1200") or \
                amiga_model.startswith("A4000") or \
                amiga_model.startswith("A3000"):
            workbench = "Minimal Workbench v3.1"
        elif amiga_model == "A600":
            workbench = "Minimal Workbench v2.05"
        elif amiga_model == "A500+":
            workbench = "Minimal Workbench v2.04"
        else:
            workbench = "Minimal Workbench v1.3"
        
        print("Try to find pre-configured hard drive", workbench)
        src_dir = os.path.join(self.fsgs.amiga.get_hard_drives_dir(), workbench)
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

    def unpack_game_hard_drive(self, drive_index, src):
        print("unpack_game_hard_drive", drive_index, src)
        scheme, dummy, dummy, game_uuid, drive = src.split("/")
        drive_prefix = drive + "/"

        game_database = self.fsgs.get_game_database()
        game_database_client = GameDatabaseClient(game_database)
        game_id = game_database_client.get_game_id(game_uuid)
        values = game_database_client.get_final_game_values(game_id)
        file_list = json.loads(values["file_list"])

        dir_name = "DH{0}".format(drive_index)
        dir_path = os.path.join(self.temp_dir, dir_name)
        for file_entry in file_list:
            if self.stop_flag:
                return

            name = file_entry["name"]
            if not name.startswith(drive_prefix):
                continue

            # extract Amiga relative path and convert each path component
            # to host file name (where needed).

            amiga_rel_path = name[len(drive_prefix):]
            print("amiga_rel_path", amiga_rel_path)
            amiga_rel_parts = amiga_rel_path.split("/")
            for i, part in enumerate(amiga_rel_parts):
                # part can be blank if amiga_rel_parts is a directory
                # (ending with /)
                if part:
                    amiga_rel_parts[i] = amiga_filename_to_host_filename(part)
            amiga_rel_path = "/".join(amiga_rel_parts)

            dst_file = os.path.join(dir_path, amiga_rel_path)
            print(repr(dst_file))
            if name.endswith("/"):
                os.makedirs(fs.encode_path(dst_file))
                continue
            sha1 = file_entry["sha1"]
            src_file = self.fsgs.file.find_by_sha1(sha1)
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
                out_file.write("".join(metadata).encode("UTF-8"))
            
        self.config["hard_drive_{0}".format(drive_index)] = dir_path

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
                self.fsgs.amiga.get_hard_drives_dir())

        dir_name = "DH{0}".format(i)
        dir_path = os.path.join(self.temp_dir, dir_name)

        self.unpack_archive(src, dir_path)
        self.config["hard_drive_{0}".format(i)] = dir_path

    def copy_hd_files(self):
        whdload_args = self.config.get("x_whdload_args", "").strip()
        hdinst_args = self.config.get("x_hdinst_args", "").strip()
        hd_startup = self.config.get("x_hd_startup", "").strip()

        if not whdload_args and not hdinst_args and not hd_startup:
            return

        dest_dir = os.path.join(self.temp_dir, "DH0")
        if not self.config.get("hard_drive_0", ""):
            self.config["hard_drive_0"] = dest_dir

        print("copy_hd_files, dest_dir = ", dest_dir)

        s_dir = os.path.join(dest_dir, "S")
        if not os.path.exists(s_dir):
            os.makedirs(s_dir)
        libs_dir = os.path.join(dest_dir, "Libs")
        if not os.path.exists(libs_dir):
            os.makedirs(libs_dir)
        devs_dir = os.path.join(dest_dir, "Devs")
        if not os.path.exists(devs_dir):
            os.makedirs(devs_dir)
        fonts_dir = os.path.join(dest_dir, "Fonts")
        if not os.path.exists(fonts_dir):
            os.makedirs(fonts_dir)

        if hd_startup:
            self.config["hard_drive_0_priority"] = "6"
            # don't copy setpatch by default, at least not yet
            pass
        else:
            self.hd_requirements.add("setpatch")
            self.copy_setpatch(dest_dir)
        system_configuration_file = os.path.join(devs_dir,
                "system-configuration")
        if not os.path.exists(system_configuration_file):
            with open(system_configuration_file, "wb") as f:
                f.write(system_configuration)

        if whdload_args:
            self.copy_whdload_files(dest_dir, s_dir, libs_dir, devs_dir)
        elif hdinst_args:
            self.write_startup_sequence(s_dir, hdinst_args)
        elif hd_startup:
            self.write_startup_sequence(s_dir, hd_startup)

    def copy_whdload_files(self, dest_dir, s_dir, libs_dir, devs_dir):
        whdload_args = self.config.get("x_whdload_args", "").strip()
        if not whdload_args:
            return
        print("LaunchHandler.copy_whdload_files")
        self.on_progress(_("Preparing WHDLoad..."))
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

        self.copy_whdload_kickstart(dest_dir, "kick34005.A500",
                ["891e9a547772fe0c6c19b610baf8bc4ea7fcb785"])
        self.copy_whdload_kickstart(dest_dir, "kick40068.A1200",
                ["e21545723fe8374e91342617604f1b3d703094f1"])
        self.copy_whdload_kickstart(dest_dir, "kick40068.A4000",
                ["5fe04842d04a489720f0f4bb0e46948199406f49"])
        self.create_whdload_prefs_file(os.path.join(s_dir, "WHDLoad.prefs"))

        whdload_version = self.config["x_whdload_version"]

        for key, value in six.iteritems(whdload_files[whdload_version]):
            self.install_whdload_file(key, dest_dir, value)
        for key, value in six.iteritems(whdload_support_files):
            self.install_whdload_file(key, dest_dir, value)

        if self.config.get("__netplay_game", ""):
            print("WHDLoad key is not copied in net play mode")
        else:
            key_file = os.path.join(self.fsgs.amiga.get_base_dir(),
                    "WHDLoad.key")
            if os.path.exists(key_file):
                print("found WHDLoad.key at ", key_file)
                shutil.copy(key_file, os.path.join(s_dir, "WHDLoad.key"))
            else:
                print("WHDLoad key not found in base dir (FS-UAE dir)")

            # temporary feature, at least until it's possible to set more
            # WHDLoad settings directly in the Launcher
            prefs_file = os.path.join(self.fsgs.amiga.get_base_dir(),
                    "WHDLoad.prefs")
            if os.path.exists(prefs_file):
                print("found WHDLoad.prefs at ", prefs_file)
                shutil.copy(prefs_file, os.path.join(s_dir, "WHDLoad.prefs"))
            else:
                print("WHDLoad key not found in base dir (FS-UAE dir)")

        if self.config.get("__netplay_game", ""):
            print("WHDLoad base dir is not copied in net play mode")
        else:
            src_dir = self.fsgs.amiga.get_whdload_dir()
            if src_dir and os.path.exists(src_dir):
                print("WHDLoad base dir exists, copying resources...")
                self.copy_folder_tree(src_dir, dest_dir)

        icon = self.config.get("x_whdload_icon", "")
        # FIXME: currently disabled
        icon = ""
        if icon:
            shutil.copy(os.path.expanduser("~/kgiconload"), 
                    os.path.join(dest_dir, "C", "kgiconload"))
            icon_path = os.path.join(dest_dir, icon)
            print("create icon at ", icon_path)
            create_slave_icon(icon_path, whdload_args)
            self.write_startup_sequence(s_dir,
                    "cd \"{0}\"\n"
                    "kgiconload {1}\n"
                    "uae-configuration SPC_QUIT 1\n".format(
                    whdload_dir,
                    os.path.basename(icon)))
        else:
            self.write_startup_sequence(s_dir,
                    whdload_sequence.format(whdload_dir, whdload_args))

    def write_startup_sequence(self, s_dir, command):
        # FIXME: semi-colon is used in WHDLoad CONFIG options...
        command = "\n".join([x.strip() for x in command.split(";")])
        startup_sequence = os.path.join(s_dir, "Startup-Sequence")
        if not os.path.exists(startup_sequence):
            with open(startup_sequence, "wb") as f:
                if "setpatch" in self.hd_requirements:
                    if self.setpatch_installed:
                        f.write(setpatch_found_sequence.replace(
                                "\r\n", "\n").encode("ISO-8859-1"))
                    else:
                        f.write(setpatch_not_found_sequence.replace(
                                "\r\n", "\n").encode("ISO-8859-1"))
                f.write(command.replace(
                        "\r\n", "\n").encode("ISO-8859-1"))

        # The User-Startup file is useful if the user has provided a
        # base WHDLoad directory with an existing startup-sequence
        user_startup = os.path.join(s_dir, "User-Startup")
        with open(user_startup, "ab") as f:
            f.write(command.replace(
                    "\r\n", "\n").encode("ISO-8859-1"))

    def install_whdload_file(self, sha1, dest_dir, rel_path):
        abs_path = os.path.join(dest_dir, rel_path)
        name = os.path.basename(rel_path)
        self.on_progress(_("Downloading {0}...".format(name)))
        Downloader.install_file_by_sha1(sha1, name, abs_path)

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
            f.write(default_prefs.encode("UTF-8"))

    def copy_setpatch(self, base_dir):
        dest = os.path.join(base_dir, "C")
        if not os.path.exists(dest):
            os.makedirs(dest)
        dest = os.path.join(dest, "SetPatch")
        for checksum in workbench_disks_with_setpatch_39_6:
            path = self.fsgs.file.find_by_sha1(checksum)
            if path:
                print("found WB DISK with SetPatch 39.6 at", path)
                archive = Archive(path)
                if archive.exists(path):
                    f = archive.open(path)
                    wb_data = f.read()
                    f.close()
                    if self.extract_setpatch_39_6(wb_data, dest):
                        self.setpatch_installed = True
                    else:
                        print("WARNING: extract_setpatch_39_6 returned False")
        else:
            print("WARNING: did not find SetPatch 39.6")

    def extract_setpatch_39_6(self, wb_data, dest):
        extractor = ADFFileExtractor(wb_data)
        try:
            setpatch_data = extractor.extract_file("C/SetPatch")
        except KeyError:
            return False
        s = hashlib.sha1()
        s.update(setpatch_data)
        print(s.hexdigest())
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
            path = self.fsgs.file.find_by_sha1(checksum)
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
        return self.game_paths.get_state_dir()

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
        except OSError:
            # could not delete directories - ok - probably has content
            pass

    def prepare_theme(self):
        #path = self.game_paths.get_theme_path()
        #if path:
        #    self.config["theme"] = path
        pass

    def prepare_extra_settings(self):
        prefix = self.config.get("screenshots_output_prefix", "")
        if prefix:
            return
        #name = self.config.get("floppy_drive_0", "")
        #if not name:
        #    name = self.config.get("hard_drive_0", "")
        #if not name:
        #    name = self.config.get("cdrom_drive_0", "")
        #if not name:
        #    name = self.config.get("floppy_image_0", "")
        name = self.config_name
        if not name:
            name = "fs-uae"
        name, variant = GameNameUtil.extract_names(name)
        name = GameNameUtil.create_cmpname(name)
        self.config["screenshots_output_prefix"] = name

    def create_config(self):
        config = ConfigWriter(self.config).create_fsuae_config()
        return config

    def write_config(self, f):
        config_lines = self.create_config()
        for line in config_lines:
            f.write(line)
            f.write("\n")

    def write_config_to_file(self, path):
        with open(path, "wb") as f:
            self.write_config(f)

    def run(self):
        print("LaunchHandler.run")
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

    def unpack_archive(self, path, destination):
        print("unpack", path, "to", destination)
        archive = Archive(path)
        print(archive)
        print(archive.get_handler())
        for name in archive.list_files():
            if self.stop_flag:
                return

            print(name)
            n = name[len(path) + 2:]
            out_path = os.path.join(destination, n)
            print("out path", out_path)

            if name.endswith("/"):
                os.makedirs(out_path)
            else:
                if not os.path.exists(os.path.dirname(out_path)):
                    os.makedirs(os.path.dirname(out_path))
                f = archive.open(name)
                with open(out_path, "wb") as out_f:
                    while True:
                        data = f.read(65536)
                        if not data:
                            break
                        out_f.write(data)

    def copy_folder_tree(self, sourcepath, destpath, overwrite=False):
        for item in os.listdir(sourcepath):
            if self.stop_flag:
                return
            if item[0] == ".":
                continue
            itempath = os.path.join(sourcepath, item)
            destitempath = os.path.join(destpath, item)
            if os.path.isdir(itempath):
                if not os.path.exists(destitempath):
                    os.makedirs(destitempath)
                self.copy_folder_tree(itempath, destitempath)
                if self.stop_flag:
                    return
            else:
                if overwrite or not os.path.exists(destitempath):
                    print("copy", repr(itempath), "to", repr(destitempath))
                    shutil.copy(itempath, destitempath)

EVILCHARS = '%\\*?\"/|<>'

def amiga_filename_to_host_filename(amiga_filename, ascii=False):
    """
    Converted from FS-UAE C code (src/od-fs/fsdb-host.py)
    @author: TheCyberDruid
    """
    length = len(amiga_filename)

    repl_1 = -1
    repl_2 = -1

    check = amiga_filename[:3].upper()
    dot_pos = -1
    if check in ["AUX", "CON", "PRN", "NUL"]:
        dot_pos = 4
    elif check in ["LPT", "COM"] and length >= 4 and amiga_filename[3].isdigit():        
        dot_pos = 5
    if (dot_pos > -1 and (length == (dot_pos - 1) or (length > dot_pos and \
            amiga_filename[dot_pos] == "."))):
        repl_1 = 2

    if (amiga_filename[-1] == "." or amiga_filename[-1] == " "):
        repl_2 = length - 1

    i = 0
    filename = ""
    for char in amiga_filename:
        x = ord(char)
        repl = False
        if (i == repl_1):
            repl = True
        elif (i == repl_2):
            repl = True
        elif (x < 32):
            repl = True
        elif (ascii and x > 127):
            repl = True

        if (not repl):
            for evil in EVILCHARS:
                if (evil == char):
                    repl = True
                    break
        if (i == length - 1) and amiga_filename[-5:] == ".uaem":
            repl = True

        if (repl):
            filename += "%" + "%02x" % ord(char)
        else:
            filename += char

        i += 1

    return filename

whdload_support_files = {
    "1ad1b55e7226bd5cd66def8370a69f19244da796": "Devs/Kickstarts/kick40068.A1200.RTB",
    "209c109855f94c935439b60950d049527d2f2484": "Devs/Kickstarts/kick34005.A500.RTB",
    "973b42dcaf8d6cb111484b3c4d3b719b15f6792d": "Devs/Kickstarts/kick40068.A4000.RTB",
    "09e4d8a055b4a9801c6b011e7a3de42bafaf070d": "C/uae-configuration",
}

# Other 17.0 files
# "100d80eead41511dfef6086508b1f77d3c1672a8": "C/RawDIC",
# "e1b6c3871d8327f771874b17258167c2a454029a": "C/Patcher",
# "57f29b23cff0107eec81b98159ce304ccd69441a": "C/WHDLoadCD32",
# "2fcb5934019133ed7a2069e333cdbc349ecaa7ee": "C/DIC",

# Other 17.1 files
# "100d80eead41511dfef6086508b1f77d3c1672a8": "C/RawDIC",
# "e1b6c3871d8327f771874b17258167c2a454029a": "C/Patcher",
# "a4f425b2c7e29600a970abd90f70a4dd4804c01c": "C/WHDLoadCD32",
# "2fcb5934019133ed7a2069e333cdbc349ecaa7ee": "C/DIC",

whdload_files = {
    "10.0": {
        "3096b2f41dfebf490aac015bdf0e91a80045c2c0": "C/WHDLoad",
    },
    "13.0": {
        "4bcb393e820d68b0520da9131e0d529018e303d1": "C/WHDLoad",
    },
    "16.0": {
        "883b9e37bc81fc081f78a3f278b732f97bdddf5c": "C/WHDLoad",
    },
    "16.1": {
        "250506c2444d9fb89b711b4fba5d70dd554e6f0e": "C/WHDLoad",
    },
    "16.2": {
        "a8bc2828c7da88f6236a8e82c763c71582f66cfd": "C/WHDLoad",
    },
    "16.3": {
        "5d636899fa9332b7dfccd49df3447238b5b71e49": "C/WHDLoad",
    },
    "16.4": {
        "1bb42fc83ee9237a6cfffdf15a3eb730504c9f65": "C/WHDLoad",
    },
    "16.5": {
        "8974e6c828ac18ff1cc29e56a31da0775ddeb0f0": "C/WHDLoad",
    },
    "16.6": {
        "b268bf7a05630d5b2bbf99616b32f282bac997bf": "C/WHDLoad",
    },
    "16.7": {
        "be94bc3d70d5980fac7fd04df996120e8220c1c0": "C/WHDLoad",
    },
    "16.8": {
        "a3286827c821386ac6e0bb519a7df807550d6a70": "C/WHDLoad",
    },
    "16.9": {
        "b4267a21918d6375e1bbdcaee0bc8b812e366802": "C/WHDLoad",
    },
    "17.0": {
        "0ec213a8c62beb3eb3b3509aaa44f21405929fce": "C/WHDLoad",
    },
    "17.1": {
        "1a907ca4539806b42ad5b6f7aeebacb3720e840d": "C/WHDLoad",
    },
    "2013-03-01": {
        "7ee8516eceb9e799295f1b16909749d08f13d26c": "C/WHDLoad",
    }
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

setpatch_found_sequence = """
C:SetPatch
"""

setpatch_not_found_sequence = """
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

base_icon = b"\xe3\x10\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x006\x00" \
        b"\x17\x00\x04\x00\x01\x00\x01\x00\x02\x1a\x98\x00\x00\x00\x00\x00" \
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
        b"\x00\x04\x00\x00\t\x01l\x00\x01\xa5\x9c\x80\x00\x00\x00\x80\x00" \
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x10\x00\x00\x00" \
        b"\x00\x00\x006\x00\x16\x00\x02\x00\x08\xce \x03\x00\x00\x00\x00\x00" \
        b"\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x0c\x00" \
        b"\x00\x00\x00\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x0c\x00" \
        b"\x00\x00\x00\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x0c\x00" \
        b"\x03\xf0\x0f\xff\xe0\x00\x0c\x00\x02\x080\x00\x1c\x00\x0c\x00\x02" \
        b"\x07\xc0\x00\x03\x80\x0c\x00\x02\x00\x00\x00\x00`\x0c\x00\x02\x00" \
        b"\x00\x00\x00\x10\x0c\x00\x02\x00\x00\x00\x00\x08\x0c\x00\x02\x07" \
        b"\xc0\x00\x1f\xc4\x0c\x00\x02\x08 \x00 2\x0c\x00\x03\xf0\x18\x00" \
        b"\xc0\r\x0c\x00\x00\x00\x06\x03\x00\x03\x0c\x00\x00\x00\x02\x02" \
        b"\x00\x00\x0c\x00\x00\x00\x02\x02\x00\x00\x0c\x00\x00\x00\x02\x02" \
        b"\x00\x00\x0c\x00\x00\x00\x03\xfe\x00\x00\x0c\x00\x00\x00\x00\x00" \
        b"\x00\x00\x0c\x00\x7f\xff\xff\xff\xff\xff\xfc\x00\xff\xff\xff\xff" \
        b"\xff\xff\xf8\x00\xd5UUUUUP\x00\xd5UUUUUP\x00\xd5UUUUUP\x00\xd5UUU" \
        b"UUP\x00\xd5UUUUUP\x00\xd4\x05P\x00\x15UP\x00\xd4\x05@\x00\x01UP" \
        b"\x00\xd4\x00\x00\x00\x00UP\x00\xd4\x00\x00\x00\x00\x15P\x00\xd4" \
        b"\x00\x00\x00\x00\x05P\x00\xd4\x00\x00\x00\x00\x05P\x00\xd4\x00" \
        b"\x00\x00\x00\x01P\x00\xd4\x05@\x00\x15AP\x00\xd4\x05@\x00\x15PP" \
        b"\x00\xd5UP\x00UTP\x00\xd5UT\x01UUP\x00\xd5UT\x01UUP\x00\xd5UT" \
        b"\x01UUP\x00\xd5UT\x01UUP\x00\xd5UUUUUP\x00\x80\x00\x00\x00\x00" \
        b"\x00\x00\x00"

def char(v):
    return chr(v)

def write_number(f, n):
    f.write(char((n & 0xff000000) >> 3))
    f.write(char((n & 0x00ff0000) >> 2))
    f.write(char((n & 0x0000ff00) >> 1))
    f.write(char((n & 0x000000ff) >> 0))

def write_string(f, s):
    write_number(f, len(s) + 1)
    f.write(s)
    f.write("\0")

def create_slave_icon(path, whdload_args):
    default_tool = "DH0:/C/WHDLoad"
    # FIXME: handle "" around slave name?
    #args = whdload_args.split(" ")
    tool_types = whdload_args.split(" ")
    tool_types[0] = "SLAVE=" + tool_types[0]

    with open(path, "wb") as f:
        f.write(base_icon)
        write_string(f, default_tool)
        write_number(f, (len(tool_types) + 1) * 4)
        for tool_type in tool_types:
            write_string(f, tool_type)
        f.close()

class TestCase(unittest.TestCase):

    def test_convert_amiga_file_name(self):
        result = amiga_filename_to_host_filename("pro.i*riska")
        self.assertEquals(result, "pro.i%2ariska")

    def test_convert_amiga_file_name_2(self):
        result = amiga_filename_to_host_filename("mypony.uaem")
        self.assertEquals(result, "mypony.uae%6d")

    def test_convert_amiga_file_name_short(self):
        result = amiga_filename_to_host_filename("t")
        self.assertEquals(result, "t")

    def test_convert_amiga_file_name_short_2(self):
        result = amiga_filename_to_host_filename("t ")
        self.assertEquals(result, "t%20")

    def test_convert_amiga_file_name_lpt1(self):
        result = amiga_filename_to_host_filename("LPT1")
        self.assertEquals(result, "LP%541")

    def test_convert_amiga_file_name_aux(self):
        result = amiga_filename_to_host_filename("AUX")
        self.assertEquals(result, "AU%58")

if __name__ == '__main__':
    unittest.main()
