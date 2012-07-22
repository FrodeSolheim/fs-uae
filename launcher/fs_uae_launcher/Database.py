from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import sqlite3
from .Settings import Settings

class Database:

    instance = None

    @classmethod
    def get_database_path(self):
        path = Settings.get_launcher_dir()
        path = os.path.join(path, "Database6.sqlite")
        return path

    @classmethod
    def get_instance(cls):
        if not cls.instance:
            cls.instance = Database()
        return cls.instance

    def __init__(self):
        self.connection = None
        self.cursor = None

    def init(self):
        if self.connection:
            return
        self.connection = sqlite3.connect(self.get_database_path())
        self.cursor = self.connection.cursor()

        try:
            self.cursor.execute("SELECT count(*) FROM file")
        except sqlite3.OperationalError:
            self.cursor.execute("""CREATE TABLE file (
id integer primary key,
path text,
name text,
sha1 text,
md5 text,
crc32 text,
size integer,
mtime integer,
scan int
)""")
            self.cursor.execute("CREATE INDEX file_sha1 ON file(sha1)")
            self.cursor.execute("CREATE INDEX file_path ON file(path)")
            self.cursor.execute("CREATE INDEX file_name ON file(name collate nocase)")

        try:
            self.cursor.execute("SELECT count(*) FROM configuration")
        except sqlite3.OperationalError:
            self.cursor.execute("""CREATE TABLE configuration (
id integer primary key,
path text,
name text,
search text,
scan int
)""")
            self.cursor.execute("CREATE INDEX configuration_name "
                    "ON configuration(name)")
            self.cursor.execute("CREATE INDEX configuration_search "
                    "ON configuration(search)")
            self.cursor.execute("CREATE INDEX configuration_path "
                    "ON configuration(path)")

        try:
            self.cursor.execute("SELECT count(*) FROM game")
        except sqlite3.OperationalError:
            self.cursor.execute("""CREATE TABLE game (
id integer primary key,
uuid text,
path text,
name text,
search text,
scan int
)""")
            self.cursor.execute("CREATE INDEX game_uuid "
                    "ON game(uuid)")

    def get_files(self, ext=None):
        self.init()
        query = "SELECT path FROM file WHERE 1 = 1 "
        args = []
        if ext is not None:
            query = query + " AND path like ?"
            args.append("%" + ext)
        self.cursor.execute(query, args)
        results = []
        for row in self.cursor:
            data = {
                'path': row[0]
            }
            results.append(data)
        return results

    def get_configuration_path(self, id):
        self.init()
        query = "SELECT path FROM configuration WHERE id = ?"
        self.cursor.execute(query, (id,))
        return self.cursor.fetchone()[0]

    def get_game_info(self, id):
        self.init()
        query = "SELECT name, uuid, path FROM game WHERE id = ?"
        self.cursor.execute(query, (id,))
        row = self.cursor.fetchone()
        return {
            "name": row[0],
            "uuid": row[1],
            "path": row[2],
        }

    def search_configurations(self, search):
        self.init()
        query = "SELECT id, name FROM configuration"
        args = []
        for word in search.split(" "):
            word = word.strip().lower()
            if word:
                if len(args) == 0:
                    query = query + " WHERE search like ?"
                else:
                    query = query + " AND search like ?"
                args.append(u"%{0}%".format(word))
        query = query + " ORDER BY name"
        self.cursor.execute(query, args)
        return self.cursor.fetchall()

    def search_games(self, search):
        self.init()
        query = "SELECT id, name FROM game"
        args = []
        for word in search.split(" "):
            word = word.strip().lower()
            if word:
                if len(args) == 0:
                    query = query + " WHERE search like ?"
                else:
                    query = query + " AND search like ?"
                args.append(u"%{0}%".format(word))
        query = query + " ORDER BY name"
        self.cursor.execute(query, args)
        return self.cursor.fetchall()

    def find_game(self, uuid="", result=None):
        self.init()
        if uuid:
            self.cursor.execute("SELECT path FROM game "
                    "WHERE uuid = ? LIMIT 1", (uuid,))
        row = self.cursor.fetchone()
        if row:
            if result is not None:
                result["path"] = row[0]
            return row[0]
        else:
            if result is not None:
                result["path"] = None
            return None

    def find_file(self, name="", sha1="", path="", result=None):
        self.init()
        if sha1:
            #print("xxx", repr(sha1))
            #import traceback
            #traceback.print_stack()
            print("check sha1")
            self.cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE sha1 = ? LIMIT 1", (sha1,))
        elif name:
            #print("check name")
            self.cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE name = ? COLLATE NOCASE LIMIT 1", (name.lower(),))
        else:
            #print(path)
            #path = unicode(path)
            #print("SELECT path, sha1, mtime, size FROM file "
            #        "WHERE path = '{0}' LIMIT 1".format(path))
            #self.cursor.execute("SELECT count(*) FROM file "
            #        "WHERE lower(path) = ?", (path.lower(),))

            #self.cursor.execute("SELECT * FROM file LIMIT 1 OFFSET 100")
            #print(self.cursor.fetchall())

            self.cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE path = ? LIMIT 1", (path,))
        row = self.cursor.fetchone()
        #print("---------", row)
        if row:
            #print(row)
            if result is not None:
                result["id"] = row[0]
                result["path"] = row[1]
                result["sha1"] = row[2]
                result["mtime"] = row[3]
                result["size"] = row[4]
            return row[1]
        else:
            if result is not None:
                result["id"] = None
                result["path"] = None
                result["sha1"] = None
                result["mtime"] = None
                result["size"] = None
            return None

    def add_file(self, path="", sha1=None, md5=None, crc32=None, mtime=0,
            size=0, scan=0):
        self.init()
        #print("adding path", path)
        p, name = os.path.split(path)
        #name = name.lower()
        self.cursor.execute("INSERT INTO file (path, sha1, mtime, size, "
                "md5, crc32, name, scan) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                (path, sha1, mtime, size, md5, crc32, name, scan))

    def add_configuration(self, path="", name="", search="", scan=0):
        self.init()
        self.cursor.execute("INSERT INTO configuration (path, name, scan, "
                "search) VALUES (?, ?, ?, ?)", (path, name, scan, search))

    def add_game(self, uuid="", path="", name="", search="", scan=0):
        self.init()
        self.cursor.execute("INSERT INTO game (uuid, path, name, scan, "
                "search) VALUES (?, ?, ?, ?, ?)", (uuid, path, name, scan, search))

    def update_file_scan(self, id, scan):
        self.cursor.execute("UPDATE file SET scan = ? WHERE id = ?",
                (scan, id))

    def remove_unscanned_files(self, scan):
        self.cursor.execute("DELETE FROM file WHERE scan != ?", (scan,))

    def remove_unscanned_configurations(self, scan):
        self.cursor.execute("DELETE FROM configuration WHERE scan != ?",
                (scan,))

    def remove_unscanned_games(self, scan):
        self.cursor.execute("DELETE FROM game WHERE scan != ?",
                (scan,))

    def rollback(self):
        print("Database.rollback")
        self.init()
        self.connection.rollback()

    def commit(self):
        print("Database.commit")
        self.init()
        self.connection.commit()

    def clear(self):
        self.init()
        self.cursor.execute("DELETE FROM file")
