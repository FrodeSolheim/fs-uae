from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree, fromstring
from zipfile import ZipFile
from .Database import Database

archive_extensions = [u".zip", u".xml"] 

class ZipHandler:
    
    def __init__(self, path):
        self.path = path
        self.zip = ZipFile(self.path, "r")

    #def is_archive(self):
    #    return True

    def list_files(self, sub_path):
        if sub_path:
            return
        for name in self.zip.namelist():
            if name.endswith("/"):
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
        name = name.replace(u"%5f", u"\\")
        name = name.replace(u"%25", u"%")
        name = name.encode("CP437")
        return name

    def decode_name(self, name):
        name = name.replace("%", "%25")
        name = name.replace("\\", "%5f")
        name = name.replace("/", os.sep)
        name = name.decode("CP437")
        return name

class XMLHandler:
    
    def __init__(self, path):
        self.path = path
        self.tree = None

    #def is_archive(self):
    #    return False

    def get_tree(self):
        if self.tree is None:
            tree = ElementTree()
            tree.parse(self.path)
            self.tree = tree
        return self.tree

    def list_files(self, sub_path):
        print("XMLHandler.list_files, sub_path =", sub_path)
        result = []
        if not sub_path:
            return result
        archive_name = sub_path.split("/", 1)[0]
        tree = self.get_tree()
        root = tree.getroot()
        for file_node in root.findall("file"):
            n = file_node.find("name").text.strip()
            if archive_name != n:
                continue
            archive_node = file_node.find("archive")
            sub_file_nodes = archive_node.findall("file")
            for sub_file_node in sub_file_nodes:
                result.append(u"{0}/{1}".format(archive_name,
                    sub_file_node.find("name").text.strip()))
            break
        return result

    def get_real_path(self, sub_path):
        archive_name, name = sub_path.split("/", 1)
        tree = self.get_tree()
        root = tree.getroot()
        for file_node in root.findall("file"):
            n = file_node.find("name").text.strip()
            if archive_name != n:
                continue
            archive_node = file_node.find("archive")
            sub_file_nodes = archive_node.findall("file")
            for sub_file_node in sub_file_nodes:
                n = sub_file_node.find("name").text.strip()
                if name != n:
                    continue
                sha1 = sub_file_node.find("sha1").text.strip()
                database = Database()
                return database.find_file(sha1=sha1)
            break
        return None

    def open(self, name):
        path = self.get_real_path(name)
        #return open(path, "rb")
        #archive = Archive(path)
        #return archive.open
        return Archive(path).open(path)

    def exists(self, name):
        path = self.get_real_path(name)
        #if not path:
        #    return False
        #return os.path.exists(path)
        return Archive(path).exists(path)

class NullHandler:

    def __init__(self, path):
        self.path = path

    #def is_archive(self):
    #    return False
        
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
        parts = path.replace(u"\\", u"/").split(u"/")

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
        if ext == ".zip":
            self._handler = ZipHandler(self.path)
        #elif ext == ".lha":
        #    self._handler = LHAHandler(self.path)
        elif ext == ".xml":
            self._handler = XMLHandler(self.path)
        else:
            self._handler = NullHandler(self.path)
        return self._handler

    def list_files(self):
        result = []
        for item in self.get_handler().list_files(self.sub_path):
            #result.append(u"{0}|{1}".format(self.path, item))
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
