from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import io
import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree, fromstring
from zipfile import ZipFile

archive_extensions = [".zip", ".xml"]
supports_lha = False

try:
    from lhafile import Lhafile
except ImportError:
    print("lhafile module not found")
else:
    archive_extensions.append(".lha")
    supports_lha = True

class ZipHandler:

    def __init__(self, path):
        self.path = path
        self.zip = ZipFile(self.path, "r")

    def list_files(self, sub_path):
        if sub_path:
            return
        for name in self.zip.namelist():
            if name.endswith(str("/")):
                continue
            yield self.decode_name(name)

    def open(self, name):
        name = self.encode_name(name)
        return self.zip.open(name)

    def exists(self, name):
        name = self.encode_name(name)
        try:
            self.zip.getinfo(name)
        except KeyError:
            return False
        else:
            return True

    def encode_name(self, name):
        name = name.replace("\\", "/")
        name = name.replace("%5f", "\\")
        name = name.replace("%25", "%")
        #name = name.encode("CP437")
        name = name.encode("ISO-8859-1")
        return name

    def decode_name(self, name):
        #name = name.decode("CP437")
        name = name.decode("ISO-8859-1")
        name = name.replace("%", "%25")
        name = name.replace("\\", "%5f")
        name = name.replace("/", os.sep)
        return name

class LhaHandler:

    def __init__(self, path):
        self.path = path
        self.zip = Lhafile(self.path, "r")

    def list_files(self, sub_path):
        if sub_path:
            return
        for name in self.zip.namelist():
            if name.endswith(str("/")):
                continue
            yield self.decode_name(name)

    def open(self, name):
        name = self.encode_name(name)
        # Lhafile does not have open method
        data = self.zip.read(name)
        return io.BytesIO(data)

    def exists(self, name):
        name = self.encode_name(name)
        try:
            self.zip.getinfo(name)
        except KeyError:
            return False
        else:
            return True

    def encode_name(self, name):
        name = name.replace("\\", "/")
        name = name.replace("%5f", "\\")
        name = name.replace("%25", "%")

        # FIXME: a little hack here, Lhafile uses os.sep
        # as path separator
        name = name.replace("/", os.sep)

        name = name.encode("ISO-8859-1")
        return name

    def decode_name(self, name):
        #print("decode_name", name)

        name = name.decode("ISO-8859-1")
        # FIXME: a little hack here, Lhafile uses os.sep
        # as path separator, normalizing to /
        name = name.replace(os.sep, "/")

        name = name.replace("%", "%25")
        name = name.replace("\\", "%5f")
        name = name.replace("/", os.sep)
        return name

class NullHandler:

    def __init__(self, path):
        self.path = path

    def list_files(self, sub_path):
        return []

class Archive:

    def __init__(self, path):
        self.path, self.sub_path = self.split_path(path)

    #@staticmethod
    def join(self, base, *args):
        return os.path.join(base, *args)

    #@staticmethod
    def dirname(self, path):
        return os.path.dirname(path)

    def split_path(self, path):
        parts = path.replace("\\", "/").split("/")

        for i, part in enumerate(parts):
            n, ext = os.path.splitext(part)
            ext = ext.lower()
            if ext in archive_extensions:
                # FIXME: should also check that it isn't a dir
                path = unicode(os.sep).join(parts[:i + 1])
                sub_path = unicode(os.sep).join(parts[i + 1:])
                return path, sub_path
        return path, ""

    def get_handler(self):
        if hasattr(self, "_handler"):
            return self._handler
        name, ext = os.path.splitext(self.path)
        ext = ext.lower()
        print(name, ext)
        if ext == ".zip":
            self._handler = ZipHandler(self.path)
        elif ext == ".lha" and supports_lha:
            self._handler = LhaHandler(self.path)
        else:
            self._handler = NullHandler(self.path)
        return self._handler

    def list_files(self):
        result = []
        print(self.get_handler())
        for item in self.get_handler().list_files(self.sub_path):
            result.append(os.path.join(self.path, item))
        return result

    #def is_archive(self):
    #    return self.get_handler().is_archive()

    def exists(self, path):
        path, sub_path = self.split_path(path)
        #print(path, self.path)
        assert path == self.path
        if not sub_path:
            #print("os.path.exists", path)
            return os.path.exists(path)
        return self.get_handler().exists(sub_path)

    def open(self, path):
        #print("open", repr(path))
        path, sub_path = self.split_path(path)
        #print(path, self.path)
        assert path == self.path
        if not sub_path:
            return open(path, "rb")
        return self.get_handler().open(sub_path)

    def copy(self, path, dest):
        ifo = self.open(path)
        with open(dest, "wb") as ofo:
            while True:
                data = ifo.read(65536)
                if not data:
                    break
                ofo.write(data)
