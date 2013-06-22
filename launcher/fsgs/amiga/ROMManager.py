from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.six as six
import os
import hashlib
from fsgs.Archive import Archive

#if b"\0"[0] == 0: # Python 3
#    def byte_value(x):
#        return x
#else:
#    def byte_value(x):
#        return ord(x) 

class ROMManager:

    @classmethod
    def add_rom_to_database(cls, path, database, log_function=None):
        if log_function is None:
            log_function = print
        try:
            archive = Archive(path)
            rom = ROMManager.decrypt_archive_rom(archive, path)
        except Exception:
            import traceback
            traceback.print_exc()
            return
        try:
            st = os.stat(path)
        except:
            log_function("Error stat-ing file {0}".format(repr(path)))
            return
        size = st.st_size
        mtime = int(st.st_mtime)
        log_function("Adding ROM \"{0}\" to database (SHA-1: {1})".format(
                path, rom["sha1"]))
        database.delete_file(path=path)
        database.add_file(path=path, sha1=rom["sha1"], mtime=mtime, size=size)

    @classmethod
    def decrypt_archive_rom(cls, archive, path, file=None):
        print("decrypt_archive_rom", path)
        result = {}
        f = archive.open(path)
        data = f.read(len("AMIROMTYPE1"))
        out_data = b""
        sha1 = hashlib.sha1()
        if data != b"AMIROMTYPE1":
            # not encrypted, write raw data
            sha1.update(data)
            out_data += data
            data = f.read()
            sha1.update(data)
            out_data += data
            result["data"] = out_data
            result["sha1"] = sha1.hexdigest()
            cls.patch_rom(result)
            if file is not None:
                file.write(result["data"])
            return result

        key_path = archive.join(archive.dirname(path), "rom.key")
        key_archive = Archive(key_path)
        try:
            f2 = key_archive.open(key_path)
        except Exception:
            raise Exception("did not find rom.key to decrypt ROM with")
        print("using key file", key_path)
        #if not os.path.exists(key_file):
        #    raise Exception("did not find rom.key to decrypt ROM with")
        key_data = f2.read()
        f2.close()

        while True:
            data = f.read(len(key_data))
            if not data:
                break
            dec = []
            if six.PY3:
                for i in range(len(data)):
                    dec.append(data[i] ^ key_data[i])
                dec_data = bytes(dec)
            else:
                for i in range(len(data)):
                    dec.append(chr(ord(data[i]) ^ ord(key_data[i])))
                dec_data = b"".join(dec)
            #if file is not None:
            #    file.write(dec_data)
            out_data += dec_data
            if sha1 is not None:
                sha1.update(dec_data)
        result["data"] = out_data
        result["sha1"] = sha1.hexdigest()
        cls.patch_rom(result)
        if file is not None:
            file.write(result["data"])
        return result

    @classmethod
    def patch_rom(cls, rom):
        src_data = rom["data"]
        dst_data = b""

        if rom["sha1"] == "c39bd9094d4e5f4e28c1411f3086950406062e87":
            dst_data += src_data[:413]
            dst_data += b"\x08"
            dst_data += src_data[414:176029]
            dst_data += b"\xb9"
            dst_data += src_data[176030:262121]
            dst_data += b"\x26"
            dst_data += src_data[262122:]
            rom["sha1"] = "891e9a547772fe0c6c19b610baf8bc4ea7fcb785"

        elif rom["sha1"] == "90933936cce43ca9bc6bf375662c076b27e3c458":
            # from Kickstart v1.3 rev 34.5 (1987)(Commodore)
            # (A500-A1000-A2000-CDTV)[o].rom
            dst_data += src_data[:262144]
            rom["sha1"] = "891e9a547772fe0c6c19b610baf8bc4ea7fcb785"

        elif rom["sha1"] == "c3c481160866e60d085e436a24db3617ff60b5f9":
            # from Kickstart v3.1 r40.68 (1993)(Commodore)(A4000)[h Cloanto]
            # to Kickstart v3.1 r40.68 (1993)(Commodore)(A4000)
            dst_data += src_data[:220]
            dst_data += b"\x74"
            dst_data += src_data[221:222]
            dst_data += b"\x7a"
            dst_data += src_data[223:326]
            dst_data += b"\x70"
            dst_data += src_data[327:434]
            dst_data += b"\x7c"
            dst_data += src_data[435:524264]
            dst_data += b"\x45"
            dst_data += src_data[524265:524266]
            dst_data += b"\x14"
            dst_data += src_data[524267:]
            rom["sha1"] = "5fe04842d04a489720f0f4bb0e46948199406f49"

        if dst_data:
            rom["data"] = dst_data
        sha1 = hashlib.sha1()
        sha1.update(rom["data"])
        assert sha1.hexdigest() == rom["sha1"]
