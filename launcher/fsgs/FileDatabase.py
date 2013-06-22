from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import time
from binascii import unhexlify
import sqlite3

# FIXME: remove dependency on launcher, have the Launcher tell this class
# the path instead
from fs_uae_launcher.FSUAEDirectories import FSUAEDirectories
from .BaseDatabase import BaseDatabase

SENTINEL = "fae7671d-e232-4b71-b179-b3cd45995f92"
VERSION = 2


class File(dict):
    
    def __nonzero__(self):
        return bool(self["path"])

    def __bool__(self):
        return bool(self["path"])


class FileDatabase(BaseDatabase):

    def __init__(self, SENTINEL):
        BaseDatabase.__init__(self, SENTINEL)
        self.last_file_insert = None
        self.last_file_delete = None

    @classmethod
    def get_path(self):
        # FIXME: remove direct dependency on launcher
        cache_dir = FSUAEDirectories.get_cache_dir()
        path = os.path.join(cache_dir, "Files.sqlite")
        return path

    @classmethod
    def get_version(cls):
        return VERSION

    @classmethod
    def instance(cls, new=False):
        if new or not hasattr(cls.thread_local, "file_database"):
            cls.thread_local.file_database = cls(cls.SENTINEL)
        return cls.thread_local.file_database

    @classmethod
    def get_instance(cls):
        if not hasattr(cls.thread_local, "file_database"):
            cls.thread_local.file_database = cls(cls.SENTINEL)
        return cls.thread_local.file_database

    def get_file_ids(self):
        cursor = self.internal_cursor()
        cursor.execute("SELECT id FROM file WHERE parent IS NULL")
        return set([row[0] for row in cursor.fetchall()])

    def get_file_hierarchy_ids(self, path):
        path = self.encode_path(path)
        if path.endswith("/"):
            path = path[:-1]
        a_path = path + "\u002f"  # forward slash
        b_path = path + "\u0030"  # one more than forward slash
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT id FROM file WHERE path >= ? AND path < ?",
            (a_path, b_path))
        return set([row[0] for row in cursor.fetchall()])

    #def get_child_ids(self, id=id):
    #    cursor = self.internal_cursor()
    #    cursor.execute("SELECT id FROM file WHERE parent = ?", (id,))
    #    return set([row[0] for row in cursor.fetchall()])

#    def get_file_hierarchy(self, path):
#        self.init()
#        cursor = self.create_cursor()
#        #query = "SELECT id, path FROM configuration WHERE path like ?"
#        #args = ["$BASE/Configurations/%"]
#        #self._cursor.execute(query, args)
#        a_path = path + "\u002f" # forward slash
#        b_path = path + "\u0030" # one more than forward slash
#        query = "SELECT id, path FROM file WHERE " \
#                "path >= ? AND path < ?"
#        cursor.execute(query, (a_path, b_path))
#        result = {}
#        for row in cursor:
#            result[decode_path(row[1])] = row[0]
#        return result

    def encode_path(self, path):
        # this only works if both path and 
        # FSUAEDirectories.base_dir (etc) have been
        # normalized with get_real_case.
        path = path.replace("\\", "/")
        base_dir = FSUAEDirectories.get_base_dir()
        if path.startswith(base_dir):
            path = path[len(base_dir):]
            if path.startswith("/"):
                path = path[1:]
            path = "$/" + path
        return path
        #parts = path.split("/")
        #return "{0}{1}".format(len(parts), path)

    def decode_path(self, path):
        #path = path[1:]
        if not path or path[0] != "$":
            return path
        base_dir = FSUAEDirectories.get_base_dir() + "/"
        # FIXME: $base_dir is deprecated
        #path = path.replace("$base_dir/", base_dir)
        #path = path.replace("$BASE/", base_dir)
        #path = path.replace("$/", base_dir)
        if path.startswith("$/"):
            path = base_dir + path[2:]
        return path

    def find_local_roms(self):
        if not self.connection:
            self.init()

        a = "$/Kickstarts/"
        b = "$/Kickstarts" + "\u0030"  # one more than forward slash
        query = "SELECT id, path FROM file WHERE path >= ? AND path < ?"
        #args = ["$BASE/Kickstarts/%"]
        cursor = self.internal_cursor()
        cursor.execute(query, (a, b))
        result = {}
        for row in cursor.fetchall():
            result[self.decode_path(row[1])] = row[0]
        return result

    def delete_file(self, id=None, path=None):
        cursor = self.internal_cursor()
        delete_ids = []
        if id is not None:
            delete_ids.append(id)
        if path is not None:
            path = self.encode_path(path)
            cursor.execute("SELECT id FROM file WHERE path = ?", (path,))
            for row in cursor:
                delete_ids.append(row[0])            
        for id in delete_ids:
            cursor.execute(
                "DELETE FROM file WHERE id = ? OR parent = ?", (id, id))
        self.last_file_delete = int(time.time())

    def check_sha1(self, sha1):
        #if not hasattr(self, "check_sha1_cursor"):
        #    #self.check_sha1_cursor = cursor()
        #    
        #    self.init()
        #    self.check_sha1_cursor = self.create_cursor()
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT count(*) FROM file WHERE sha1 = ?", (
            sqlite3.Binary(unhexlify(sha1)),))
        return cursor.fetchone()[0]

    def find_file(self, name="", sha1="", path="", result=None):
        cursor = self.internal_cursor()
        if sha1:
            sha1 = unhexlify(sha1)
            cursor.execute(
                "SELECT id, path, sha1, mtime, size, parent "
                "FROM file WHERE sha1 = ? LIMIT 1",
                (sqlite3.Binary(sha1),))
        elif name:
            cursor.execute(
                "SELECT id, path, sha1, mtime, size, parent "
                "FROM file WHERE name = ? COLLATE NOCASE LIMIT 1",
                (name.lower(),))
        else:
            path = self.encode_path(path)
            cursor.execute(
                "SELECT id, path, sha1, mtime, size, parent "
                "FROM file WHERE path = ? LIMIT 1", (path,))
        row = cursor.fetchone()
        result = File()
        if row:
            if row[5]:
                # parent
                cursor.execute(
                    "SELECT path FROM file WHERE id = ?", (row[5],))
                path = cursor.fetchone()[0] + row[1]
            else:
                path = row[1]
            path = self.decode_path(path)
            #if result is not None:
            result["id"] = row[0]
            result["path"] = path
            result["sha1"] = row[2]
            result["mtime"] = row[3]
            result["size"] = row[4]
            #return result
        else:
            #if result is not None:
            result["id"] = None
            result["path"] = None
            result["sha1"] = None
            result["mtime"] = None
            result["size"] = None
            #return None
        return result

    def find_files(self, ext=None):
        cursor = self.internal_cursor()
        query = "SELECT path FROM file WHERE 1 = 1"
        args = []
        if ext is not None:
            # this is used (for now) to look up .fs-uae files, so..
            # we don't want files from archives
            # sqlite like is case insensitive by default
            query += " AND parent is NULL AND path LIKE ?"
            args.append("%" + ext)
        cursor.execute(query, args)
        results = []
        for row in cursor:
            data = {
                "path": self.decode_path(row[0]),
                #"name": row[1]
            }
            results.append(data)
        return results

    def add_file(self, path="", sha1=None, mtime=0, size=0, parent=None):
        self.init()
        #if not name:
        #    name = os.path.basename(path)
        path = self.encode_path(path)
        #parent_path, name = path.rsplit("/", 1)

        cursor = self.internal_cursor()
        #parent = self.ensure_dir(dir)
        if sha1:
            sha1 = unhexlify(sha1)
        cursor.execute(
            "INSERT INTO file (path, sha1, mtime, size, parent) "
            "VALUES (?, ?, ?, ?, ?)",
            (path, sqlite3.Binary(sha1), mtime, size, parent))
        self.last_file_insert = int(time.time())
        return cursor.lastrowid

#    def ensure_dir(self, path):
#        #if not path.endswith("/"):
#        #    path += "/"
#        assert path.endswith("/")
#        cursor = self.internal_cursor()
#        cursor.execute("SELECT id FROM file WHERE name = ?", (path,))
#        row = cursor.fetchone()
#        if row is not None:
#            return row[0]
#        cursor.execute("INSERT INTO file (name) VALUES (?)", (path,))
#        return cursor.lastrowid

    def get_last_event_stamps(self):
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT last_file_insert, last_file_delete FROM metadata")
        row = cursor.fetchone()
        result = {
            "last_file_insert": row[0],
            "last_file_delete": row[0],
        }
        return result

    def update_last_event_stamps(self):
        if self.last_file_insert:
            cursor = self.internal_cursor()
            cursor.execute(
                "UPDATE metadata set last_file_insert = ?",
                (self.last_file_insert,))
            self.last_file_insert = None
        if self.last_file_delete:
            cursor = self.internal_cursor()
            cursor.execute(
                "UPDATE metadata set last_file_delete = ?",
                (self.last_file_delete,))
            self.last_file_delete = None

    def clear(self):
        if not self.connection:
            self.init()
        cursor = self.internal_cursor()
        cursor.execute("DELETE FROM file")

    def update_database_to_version_1(self):
        cursor = self.internal_cursor()
        try:
            cursor.execute("SELECT count(*) FROM file")
        except sqlite3.OperationalError:
            cursor.execute("""CREATE TABLE file (
                    id INTEGER PRIMARY KEY,
                    sha1 BLOB,
                    path TEXT,
                    size INTEGER,
                    mtime INTEGER,
                    parent INTEGER
                    )""")
            cursor.execute("CREATE INDEX file_sha1 ON file(sha1)")
            cursor.execute("CREATE INDEX file_path ON file(path)")

        cursor.execute(
            "ALTER TABLE metadata ADD COLUMN last_file_insert INTEGER")
        cursor.execute(
            "ALTER TABLE metadata ADD COLUMN last_file_delete INTEGER")

    def update_database_to_version_2(self):
        cursor = self.internal_cursor()
        cursor.execute("CREATE INDEX file_parent ON file(parent)")

#    def update_database_to_version_1(self):
#        cursor = self.create_cursor()
#        try:
#            cursor.execute("SELECT count(*) FROM file")
#        except sqlite3.OperationalError:
#            cursor.execute("""CREATE TABLE file (
#id integer primary key,
#path text,
#name text,
#sha1 text,
#size integer,
#mtime integer
#)""")
#            cursor.execute("CREATE INDEX file_sha1 ON file(sha1)")
#            cursor.execute("CREATE INDEX file_path ON file(path)")
#            cursor.execute("CREATE INDEX file_name ON file("
#                    "name collate nocase)")
#
#    def update_database_to_version_2(self):
#        cursor = self.create_cursor()
#        cursor.execute("SELECT id, sha1 FROM file WHERE sha1 IS NOT NULL")
#        for row in cursor.fetchall():
#            id, sha1 = row
#            sha1 = unhexlify(sha1)
#            cursor.execute("UPDATE file SET sha1 = ? WHERE id = ?",
#                    (sqlite3.Binary(sha1), id))
#
#    def update_database_to_version_3(self):
#        cursor = self.create_cursor()
#        cursor.execute("ALTER TABLE metadata ADD COLUMN last_file_insert INT")
#        cursor.execute("ALTER TABLE metadata ADD COLUMN last_file_delete INT")
