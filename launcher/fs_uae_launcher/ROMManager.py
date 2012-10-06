from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import hashlib
from .Archive import Archive

class ROMManager:

    @staticmethod
    def get_decrypted_sha1(path):
        print("get_decrypted_sha1", path)
        with open(path, "rb") as f:
            s1 = hashlib.sha1()
            s2 = hashlib.sha1()
            data = f.read(len("AMIROMTYPE1"))
            s1.update(data)

            if data != "AMIROMTYPE1":
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
            if data != "AMIROMTYPE1":
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
        if data != "AMIROMTYPE1":
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
