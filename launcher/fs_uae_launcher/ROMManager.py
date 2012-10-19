from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import hashlib
from .Archive import Archive
from .Database import Database

class ROMManager:

    @staticmethod
    def add_rom_to_database(path, database, log_function=None):
        if log_function is None:
            log_function = print
        file_name = os.path.basename(path)
        try:
            with open(path, "rb") as f:
                s = hashlib.sha1()
                while True:
                    data = f.read(65536)
                    if not data:
                        break
                    s.update(data)
                sha1 = s.hexdigest()
        except:
            log_function("Error calculating sha1 for {0}".format(repr(path)))
            import traceback
            traceback.print_exc()
            return
        s = hashlib.sha1()
        try:
            archive = Archive(path)
            ROMManager.decrypt_archive_rom(archive, path, sha1=s)
        except Exception:
            import traceback
            traceback.print_exc()
        else:
            sha1_dec = s.hexdigest()
            if sha1_dec != sha1:
                #log_function("Found encrypted rom {0} => {1}".format(
                #        sha1, sha1_dec))
                print("Found encrypted rom {0} => {1}".format(
                        sha1, sha1_dec))
                sha1 = sha1_dec
        try:
            st = os.stat(path)
        except:
            log_function("Error stat-ing file {0}".format(repr(path)))
            return
        size = st.st_size
        mtime = int(st.st_mtime)
        log_function("Adding ROM \"{0}\" to database".format(file_name))
        database.delete_file(path=path)
        database.add_file(path=path, sha1=sha1, mtime=mtime, size=size,
                scan=0, name=file_name)

    @staticmethod
    def get_decrypted_sha1(path):
        print("get_decrypted_sha1", path)
        with open(path, "rb") as f:
            s1 = hashlib.sha1()
            s2 = hashlib.sha1()
            data = f.read(len("AMIROMTYPE1"))
            s1.update(data)

            if data != b"AMIROMTYPE1":
                # not encrypted, return sha1 checksum for original file
                s1.update(f.read())
                return s1.hexdigest()

            key_file = os.path.join(os.path.dirname(path), "rom.key")
            if not os.path.exists(key_file):
                # key file missing, return checksum for original file
                print("WARNING: did not find", repr(key_file))
                s1.update(f.read())
                return s1.hexdigest()
            with open(key_file, "rb") as f2:
                key_data = f2.read()

            while True:
                data = f.read(len(key_data))
                if not data:
                    break
                dec = []
                for i in range(len(data)):
                    dec.append(chr(ord(data[i]) ^ ord(key_data[i])))
                dec_data = b"".join(dec)
                s1.update(data)
                s2.update(dec_data)
        return s2.hexdigest()

    @staticmethod
    def decrypt_rom(path, dest):
        print("decrypt_from", path)
        file = open(dest, "wb")
        with open(path, "rb") as f:
            data = f.read(len("AMIROMTYPE1"))
            if data != b"AMIROMTYPE1":
                # not encrypted, write raw data
                file.write(data)
                file.write(f.read())
                return

            key_file = os.path.join(os.path.dirname(path), "rom.key")
            if not os.path.exists(key_file):
                raise Exception("did not find rom.key to decrypt ROM with")
            with open(key_file, "rb") as f2:
                key_data = f2.read()

            while True:
                data = f.read(len(key_data))
                if not data:
                    break
                dec = []
                for i in range(len(data)):
                    dec.append(chr(ord(data[i]) ^ ord(key_data[i])))
                dec_data = b"".join(dec)
                file.write(dec_data)

    @staticmethod
    def decrypt_archive_rom(archive, path, sha1=None, file=None):
        print("decrypt_archive_rom", path)
        result = []
        f = archive.open(path)
        data = f.read(len("AMIROMTYPE1"))
        if data != b"AMIROMTYPE1":
            # not encrypted, write raw data
            if sha1 is not None:
                sha1.update(data)
            if file is not None:
                file.write(data)
            data = f.read()
            if sha1 is not None:
                sha1.update(data)
            if file is not None:
                file.write(data)
            return

        key_path = archive.join(archive.dirname(path), "rom.key")
        key_archive = Archive(key_path)
        try:
            f2 = key_archive.open(key_path)
        except Exception:
            raise Exception("did not find rom.key to decrypt ROM with")
        #if not os.path.exists(key_file):
        #    raise Exception("did not find rom.key to decrypt ROM with")
        key_data = f2.read()
        f2.close()

        while True:
            data = f.read(len(key_data))
            if not data:
                break
            dec = []
            for i in range(len(data)):
                dec.append(chr(ord(data[i]) ^ ord(key_data[i])))
            dec_data = b"".join(dec)
            if file is not None:
                file.write(dec_data)
            if sha1 is not None:
                sha1.update(dec_data)
