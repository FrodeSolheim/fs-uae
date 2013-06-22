from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sqlite3
from fengestad.base.Application import Application
from fengestad.base.Paths import Paths

class LocalFileDatabase:

    VERSION = 1
    instance = None

    @classmethod
    def get(cls):
        if not cls.instance:
            cls.instance = cls()
        return cls.instance

    @classmethod
    def get_database_path(cls):
        path = Application.get().get_data_path("local-file-database-beta1.sqlite")
        return path

    def __init__(self):
        self._database = None
        self.init()

    def init(self):
        if self._database is not None:
            return
        self._database = sqlite3.connect(self.get_database_path())
        self._cursor = self._database.cursor()
        self.ensure_updated_database()

    def cursor(self):
        self.init()
        return self._database.cursor()

    def encode_path(self, path):
        # this only works if both path and Settings.base_dir (etc) have been
        # normalized with get_real_case.

        #path = path.replace("\\", "/")
        #base_dir = Settings.get_base_dir()
        #if path.startswith(base_dir):
        #    path = path[len(base_dir):]
        #    if path.startswith("/"):
        #        path = path[1:]
        #    path = "$BASE/" + path
        return path

    def decode_path(self, path):
        #if not path or path[0] != "$":
        #    return path
        #base_dir = Settings.get_base_dir() + "/"
        #path = path.replace("$BASE/", base_dir)
        return path

    def find_file(self, sha1="", name="", path="", result=None):
        self.init()
        if sha1:
            #print("xxx", repr(sha1))
            #import traceback
            #traceback.print_stack()
            #print("check sha1")
            self._cursor.execute("SELECT id, path, sha1, mtime, size "
                    "FROM file WHERE sha1 = ? LIMIT 1", (sha1,))
        elif name:
            #print("check name")
            self._cursor.execute("SELECT id, path, sha1, mtime, size "
                    "FROM file WHERE name = ? COLLATE NOCASE LIMIT 1",
                    (name.lower(),))
        else:
            path = self.encode_path(path)
            #print(path)
            #path = unicode(path)
            #print("SELECT path, sha1, mtime, size FROM file "
            #        "WHERE path = '{0}' LIMIT 1".format(path))
            #self._cursor.execute("SELECT count(*) FROM file "
            #        "WHERE lower(path) = ?", (path.lower(),))

            #self._cursor.execute("SELECT * FROM file LIMIT 1 OFFSET 100")
            #print(self.cursor.fetchall())

            self._cursor.execute("SELECT id, path, sha1, mtime, size "
                    "FROM file WHERE path = ? LIMIT 1", (path,))
        row = self._cursor.fetchone()
        #print("---------", row)
        if row:
            return {
                "id": row[0],
                "path": self.decode_path(row[1]),
                "sha1": row[2],
                "mtime": row[3],
                "size": row[4],
            }
        return None

    def add_file(self, path="", sha1=None, mtime=0, size=0, scanned=0,
            name=""):
        self.init()
        path = self.encode_path(path)
        print("adding file", path)
        self._cursor.execute("INSERT INTO file (path, sha1, mtime, size, "
                "name, scanned) VALUES (?, ?, ?, ?, ?, ?)",
                (path, sha1, mtime, size, name, scanned))
        print("ok")

    def set_file_scanned(self, id, scanned):
        self.init()
        self._cursor.execute("UPDATE file SET scanned = ? WHERE id = ?",
                (scanned, id))

    def set_archive_scanned(self, path, scanned):
        self.init()
        path = self.encode_path(path)
        a = path + "\u002f" # forward slash
        b = path + "\u0030" # one more than forward slash
        self._cursor.execute("UPDATE file SET scanned = ? WHERE "
                "path >= ? AND path < ?", (scanned, a, b))

    def remove_unscanned_files(self, scanned):
        self.init()
        self._cursor.execute("DELETE FROM file WHERE scanned != ?", (scanned,))

    def rollback(self):
        print("Database.rollback")
        self.init()
        self._database.rollback()

    def commit(self):
        print("LocalFileDatabase.commit")
        self.init()
        self._database.commit()

    def clear(self):
        self.init()
        self._cursor.execute("DELETE FROM file")

    def ensure_updated_database(self):
        try:
            self._cursor.execute("SELECT version FROM metadata")
            version = self._cursor.fetchone()[0]
        except sqlite3.OperationalError:
            self._cursor.execute("CREATE TABLE metadata (version INTEGER)")
            self._cursor.execute("INSERT INTO metadata (version) VALUES (?)", (0,))
            version = 0
        if self.VERSION < version:
            raise Exception("GameDatabase: Installed database schema is "
                    "newer. Have you started an old version?")
        if self.VERSION > version:
            self.update_database(version, self.VERSION)

    def update_database(self, old, new):
        for i in range(old + 1, new + 1):
            print("upgrading game database to version", i)
            getattr(self, "update_database_to_version_{0}".format(i))()
            self._cursor.execute("UPDATE metadata SET version = ?", (i,))
            self.commit()

    def update_database_to_version_1(self):
        self._cursor.execute("""CREATE TABLE file (
                id INTEGER PRIMARY KEY,
                path TEXT,
                name TEXT,
                sha1 TEXT,
                mtime INTEGER,
                size INTEGER,
                scanned INT
        )""")
        self._cursor.execute("CREATE INDEX file_sha1 ON file(sha1)")
        self._cursor.execute("CREATE INDEX file_path ON file(path)")
        self._cursor.execute("CREATE INDEX file_scanned ON file(scanned)")
        self._cursor.execute("CREATE INDEX file_name ON file(name collate nocase)")
