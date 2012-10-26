from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import time
import zlib
import hashlib
import traceback
from .Archive import Archive
from .Database import Database
from .Settings import Settings
from .ROMManager import ROMManager
from .I18N import _, ngettext
from .Util import get_real_case

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
            self.extensions.append(".lha")
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
        return self.paths

    def scan(self):
        database = Database()
        #database.clear()
        scan_dirs = self.get_scan_dirs()
        for dir in scan_dirs:
            self.scan_dir(database, dir)
        if self.stop_check():
            database.rollback()
        else:
            self.set_status(_("Scanning files"), _("Purging old entries..."))
            database.remove_unscanned_files(self.scan_version)
            self.set_status(_("Scanning files"), _("Committing data..."))
            print("FileScanner.scan - commiting data")
            database.commit()

    def scan_dir(self, database, dir):
        #print("scan_dir", repr(dir))
        if not os.path.exists(dir):
            return
        # this is important to make sure the database is portable across
        # operating systems
        dir = get_real_case(dir)

        for name in os.listdir(dir):
            if self.stop_check():
                return
            if name in [".git"]:
                continue
            path = os.path.join(dir, name)
            if os.path.isdir(path):
                self.scan_dir(database, path)
                continue
            dummy, ext = os.path.splitext(path)
            ext = ext.lower()
            if ext not in self.extensions:
                continue
            try:
                self.scan_file(database, path)
            except Exception:
                traceback.print_exc()

    def scan_file(self, database, path):
        #print("scan_file", repr(path))

        name = os.path.basename(path)
        #path = os.path.normcase(os.path.normpath(path))

        self.scan_count += 1
        self.set_status(_("Scanning files ({count} scanned)").format(
                count=self.scan_count), name)

        try:
            st = os.stat(path)
        except:
            print("error stat-ing file", repr(path))
            return
        size = st.st_size
        mtime = int(st.st_mtime)
        result = {}

        #print(path)
        database.find_file(path=path, result=result)
        #print(result)
        if result["path"]:
            if size == result["size"] and mtime == result["mtime"]:
                # file has not changed
                #print("ok")
                #print("not changed")
                database.update_file_scan(result["id"], self.scan_version)
                database.update_archive_scan(path, self.scan_version)
                #print("not changed - ok")
                return
        archive = Archive(path)
        #if archive.is_archive():
        for p in archive.list_files():
            #print(p)
            if self.stop_check():
                return
            #n = p.replace("\\", "/").replace("|", "/").split("/")[-1]
            n = os.path.basename(p)
            self.scan_count += 1
            self.scan_archive_stream(database, archive, p, n, size, mtime)
        if self.stop_check():
            return
        self.scan_archive_stream(database, archive, path, name, size, mtime)

    def scan_archive_stream(self, database, archive, path, name, size, mtime):
        #print(path)
        base_name, ext = os.path.splitext(name)
        ext = ext.lower()

        self.set_status(_("Scanning files ({count} scanned)").format(
                count=self.scan_count), name)

        f = archive.open(path)
        s = hashlib.sha1()
        #m = hashlib.md5()
        c = None
        #with open(path, "rb") as f:
        while True:
            if self.stop_check():
                return
            data = f.read(65536)
            if not data:
                break
            s.update(data)
            #m.update(data)
            #if c is None:
            #    c = zlib.crc32(data)
            #else:
            #    c = zlib.crc32(data, c)

        sha1 = s.hexdigest()
        #md5 = m.hexdigest()
        md5 = ""
        # will happen with 0-byte files
        #if c is None:
        #    c = 0
        #crc32 = c & 0xffffffff
        crc32 = ""
        if ext == ".rom":
            s = hashlib.sha1()
            try:
                ROMManager.decrypt_archive_rom(archive, path, sha1=s)
            except Exception:
                import traceback
                traceback.print_exc()
            else:
                sha1_dec = s.hexdigest()
                #sha1_dec = ROMManager.get_decrypted_sha1(path)
                if sha1_dec != sha1:
                    print("found encrypted rom {0} => {1}".format(sha1, sha1_dec))
                    # sha1 is now the decrypted sha1, not the actual sha1 of the file
                    # itself, a bit ugly, since md5 and crc32 are still encrypted
                    # hashes, but it works well with the kickstart lookup mechanism
                    sha1 = sha1_dec

        database.add_file(path=path, sha1=sha1, md5=md5, crc32=crc32,
                mtime=mtime, size=size, scan=self.scan_version, name=name)
        self.files_added += 1
        self.bytes_added += size
        #if self.bytes_added > 1024 ** 3:
        #    self.bytes_added = 0
        #    database.commit()
        #elif self.files_added % 500 == 0:
        #    database.commit()
        if ext == '.rom':
            if self.on_rom_found:
                self.on_rom_found(path, sha1)
