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
#from .Database import Database

#from .ROMManager import ROMManager
#from .I18N import _, ngettext
#from .Util import get_real_case

from .LocalFileDatabase import LocalFileDatabase

def _(msg):
    return msg

class FileScanner:

    def __init__(self, directories):
        self.directories = directories
        self.scan_for_roms = True # scan_for_roms
        self.scan_for_files = True # scan_for_files
        self.scan_for_configs = True # scan_for_configs
        self.on_status = None # on_status
        self.on_rom_found = None # on_rom_found
        self._stop_check = None # stop_check
        self.scan_count = 0
        self.files_added = 0
        self.bytes_added = 0
        self.scan_version = int(time.time() * 100)

        self.file_database = None

    def stop_check(self):
        if self._stop_check:
            return self._stop_check()

    def set_status(self, title, status):
        if self.on_status:
            self.on_status((title, status))

    def get_scan_dirs(self):
        return self.directories

    def scan(self):
        print("FileScanner starting...")
        self.set_status(_("Scanning files"), _("Scanning files"))
        self.file_database = LocalFileDatabase()

        scan_dirs = self.get_scan_dirs()
        for dir in scan_dirs:
            self.scan_dir(dir)
        if self.stop_check():
            self.file_database.rollback()
        else:
            self.set_status(_("Scanning files"), _("Purging old entries..."))
            self.file_database.remove_unscanned_files(
                    self.scan_version)
            self.set_status(_("Scanning files"), _("Committing data..."))
            print("FileScanner.scan - commiting data")
            self.file_database.commit()

            # FIXME: enable
            #ROMManager.patch_standard_roms(database)
        print("FileScanner stopping...")

    def scan_dir(self, dir):

        if not os.path.exists(dir):
            return
        # this is important to make sure the database is portable across
        # operating systems
        
        # FIXME: enable
        #dir = get_real_case(dir)

        for name in os.listdir(dir):
            try:
                # check that the file is actually unicode object (indirectly).
                # listdir will return str objects for file names on Linux
                # with invalid encoding.
                unicode(name)
            except UnicodeDecodeError:
                continue

            if self.stop_check():
                return
            if name in [".git"]:
                continue
            path = os.path.join(dir, name)
            if os.path.isdir(path):
                self.scan_dir(path)
                continue
            #dummy, ext = os.path.splitext(path)
            #ext = ext.lower()
            #if ext not in self.extensions:
            #    continue
            try:
                self.scan_file(path)
            except Exception:
                traceback.print_exc()

    def scan_file(self, path):
        name = os.path.basename(path)

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

        result = self.file_database.find_file(path=path)
        if result:
            if size == result["size"] and mtime == result["mtime"]:
                # file has not changed
                self.file_database.set_file_scanned(result["id"],
                        self.scan_version)
                self.file_database.set_archive_scanned(path,
                        self.scan_version)
                return

        archive = Archive(path)
        for p in archive.list_files():
            if self.stop_check():
                return
            n = os.path.basename(p)
            self.scan_count += 1
            self.scan_archive_stream(archive, p, n, size, mtime)
        if self.stop_check():
            return
        self.scan_archive_stream(archive, path, name, size, mtime)

    def scan_archive_stream(self, archive, path, name, size, mtime):

        base_name, ext = os.path.splitext(name)
        ext = ext.lower()

        self.set_status(_("Scanning files ({count} scanned)").format(
                count=self.scan_count), name)

        f = archive.open(path)
        s = hashlib.sha1()

        c = None

        while True:
            if self.stop_check():
                return
            data = f.read(65536)
            if not data:
                break
            s.update(data)

        sha1 = s.hexdigest()

        if ext == ".rom":
            # FIXME:

            #try:
            #    sha1_dec = ROMManager.decrypt_archive_rom(
            #            archive, path)["sha1"]
            #except Exception:
            #    import traceback
            #    traceback.print_exc()
            #    sha1_dec = None

            if sha1_dec != sha1:
                print("found encrypted rom {0} => {1}".format(sha1, sha1_dec))
                # sha1 is now the decrypted sha1, not the actual sha1 of the file
                # itself, a bit ugly, since md5 and crc32 are still encrypted
                # hashes, but it works well with the kickstart lookup mechanism
                sha1 = sha1_dec

        self.file_database.add_file(path=path, sha1=sha1, mtime=mtime,
                size=size, scanned=self.scan_version, name=name)

        self.files_added += 1
        self.bytes_added += size

        #if ext == '.rom':
        #    if self.on_rom_found:
        #        self.on_rom_found(path, sha1)
