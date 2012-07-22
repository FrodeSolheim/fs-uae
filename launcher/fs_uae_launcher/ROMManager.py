from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import hashlib

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
                dec_data = "".join(dec)
                s1.update(data)
                s2.update(dec_data)
        return s2.hexdigest()

    @staticmethod
    def decrypt_rom(path, dest):
        print("decrypt_from", path)
        outf = open(dest, "wb")
        with open(path, "rb") as f:
            data = f.read(len("AMIROMTYPE1"))
            if data != "AMIROMTYPE1":
                # not encrypted, write raw data
                outf.write(data)
                outf.write(f.read())
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
                dec_data = "".join(dec)
                outf.write(dec_data)
