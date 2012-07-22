from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import time
import zlib
import hashlib
from .Database import Database
from .Settings import Settings
from .ROMManager import ROMManager
from .I18N import _, ngettext

#SCAN_EXTENSIONS = [".adf", ".ipf", ".dms", ".rom", ".fs-uae"]

class FileScanner:

    def __init__(self, paths, scan_for_roms, scan_for_files, scan_for_configs,
                 on_status=None, on_rom_found=None, stop_check=None):
        self.paths = paths
        self.scan_for_roms = scan_for_roms
        self.scan_for_files = scan_for_files
        self.scan_for_configs = scan_for_configs
        self.on_status = on_status
        self.on_rom_found = on_rom_found
        self._stop_check = stop_check
        self.scan_count = 0
        self.files_added = 0
        self.bytes_added = 0
        self.scan_version = int(time.time() * 100)

        self.extensions = []
        if self.scan_for_roms:
            self.extensions.append(".rom")
        if self.scan_for_files:
            self.extensions.append(".adf")
            self.extensions.append(".adz")
            self.extensions.append(".ipf")
            self.extensions.append(".dms")
            self.extensions.append(".bin")
            self.extensions.append(".cue")
            self.extensions.append(".iso")
            self.extensions.append(".wav")
            self.extensions.append(".zip")
        if self.scan_for_configs:
            self.extensions.append(".fs-uae")
            #self.extensions.append(".xml")

    def stop_check(self):
        if self._stop_check:
            return self._stop_check()

    def set_status(self, title, status):
        if self.on_status:
            self.on_status((title, status))

    def get_scan_dirs(self):
        #dirs = []
        #if self.scan_for_roms:
        #    dirs.append(Settings.get_kickstarts_dir())
        #if self.scan_for_files:
        #    dirs.extend(Settings.get_files_dirs())
        #if self.scan_for_configs:
        #    dirs.append(Settings.get_configurations_dir())
        #return dirs
        return self.paths

    def scan(self):
        database = Database()
        #database.clear()
        scan_dirs = self.get_scan_dirs()
        for dir in scan_dirs:
            self.scan_dir(database, dir)
        if self.stop_check():
            # aborted
            database.rollback()
        else:
            #print("FIXME: not removing old files currently")
            self.set_status(_("Scanning files"), _("Purging old entries..."))
            database.remove_unscanned_files(self.scan_version)
            self.set_status(_("Scanning files"), _("Commiting data..."))
            database.commit()

    def scan_dir(self, database, dir):
        if not os.path.exists(dir):
            return
        for name in os.listdir(dir):
            if self.stop_check():
                return
            path = os.path.join(dir, name)
            if os.path.isdir(path):
                self.scan_dir(database, path)
                continue
            dummy, ext = os.path.splitext(path)
            ext = ext.lower()
            if ext not in self.extensions:
                continue

            self.scan_count += 1
            self.set_status(_("Scanning files ({count} scanned)").format(
                    count=self.scan_count), name)

            try:
                st = os.stat(path)
            except:
                print("error stat-ing file", repr(path))
                continue
            size = st.st_size
            mtime = int(st.st_mtime)
            result = {}
            database.find_file(path=path, result=result)
            #print(result)
            if result["path"]:
                if size == result["size"] and mtime == result["mtime"]:
                    # file has not changed
                    #print("ok")
                    database.update_file_scan(result["id"], self.scan_version)
                    continue
            s = hashlib.sha1()
            m = hashlib.md5()
            c = None
            with open(path, "rb") as f:
                while True:
                    data = f.read(65536)
                    if not data:
                        break
                    s.update(data)
                    m.update(data)
                    if c is None:
                        c = zlib.crc32(data)
                    else:
                        c = zlib.crc32(data, c)
            sha1 = s.hexdigest()
            md5 = m.hexdigest()
            # will happen with 0-byte files
            if c is None:
                c = 0
            crc32 = c & 0xffffffff
            if ext == ".rom":
                sha1_dec = ROMManager.get_decrypted_sha1(path)
                if sha1_dec != sha1:
                    print("found encrypted rom {0} => {1}".format(sha1, sha1_dec))
                    # sha1 is now the decrypted sha1, not the actual sha1 of the file
                    # itself, a bit ugly, since md5 and crc32 are still encrypted
                    # hashes, but it works well with the kickstart lookup mechanism
                    sha1 = sha1_dec

            database.add_file(path=path, sha1=sha1, md5=md5, crc32=crc32,
                    mtime=mtime, size=size, scan=self.scan_version)
            self.files_added += 1
            self.bytes_added += size
            if self.bytes_added > 100 * 1000 * 1000:
                self.bytes_added = 0
                database.commit()
            #elif self.files_added % 500 == 0:
            #    database.commit()
            if ext == '.rom':
                if self.on_rom_found:
                    self.on_rom_found(path, sha1)

