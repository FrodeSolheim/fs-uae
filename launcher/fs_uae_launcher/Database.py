from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sqlite3
from .Settings import Settings

class Database:

    VERSION = 5

    instance = None

    @classmethod
    def get_database_path(self):
        launcher_dir = Settings.get_launcher_dir()
        for i in range(8):
            if i:
                path = os.path.join(launcher_dir,
                        "Database{0}.sqlite".format(i))
            else:
                path = os.path.join(launcher_dir, "Database.sqlite")
            if os.path.exists(path):
                try:
                    os.remove(path)
                except Exception:
                    pass
        path = os.path.join(launcher_dir, "Launcher.sqlite")
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
        self._cursor = self.cursor
        self.ensure_updated_database()

    def get_files(self, ext=None):
        self.init()
        query = "SELECT path, name FROM file WHERE 1 = 1 "
        args = []
        if ext is not None:
            query = query + " AND path like ?"
            args.append("%" + ext)
        self.cursor.execute(query, args)
        results = []
        for row in self.cursor:
            data = {
                "path": self.decode_path(row[0]),
                "name": row[1]
            }
            results.append(data)
        return results

    def get_configuration_path(self, id):
        self.init()
        query = "SELECT path FROM configuration WHERE id = ?"
        self.cursor.execute(query, (id,))
        path = self.decode_path(self.cursor.fetchone()[0])
        return path

    def get_config(self, id):
        self.init()
        query = "SELECT name, uuid, path, data FROM configuration WHERE id = ?"
        self.cursor.execute(query, (id,))
        row = self.cursor.fetchone()
        return {
            "name": row[0],
            "uuid": row[1],
            "path": self.decode_path(row[2]),
            "data": row[3],
        }

    def get_game_info(self, id):
        self.init()
        query = "SELECT name, uuid, path FROM game WHERE id = ?"
        self.cursor.execute(query, (id,))
        row = self.cursor.fetchone()
        return {
            "name": row[0],
            "uuid": row[1],
            "path": self.decode_path(row[2]),
        }

    def encode_path(self, path):
        # this only works if both path and Settings.base_dir (etc) have been
        # normalized with get_real_case.
        path = path.replace("\\", "/")
        base_dir = Settings.get_base_dir()
        if path.startswith(base_dir):
            path = path[len(base_dir):]
            if path.startswith("/"):
                path = path[1:]
            path = "$BASE/" + path
        return path

    def decode_path(self, path):
        if not path or path[0] != "$":
            return path
        base_dir = Settings.get_base_dir() + "/"
        # FIXME: $base_dir is deprecated
        path = path.replace("$base_dir/", base_dir)
        path = path.replace("$BASE/", base_dir)
        return path

    def find_local_configurations(self):
        self.init()
        #query = "SELECT id, path FROM configuration WHERE path like ?"
        #args = ["$BASE/Configurations/%"]
        #self.cursor.execute(query, args)
        a = "$BASE/Configurations/"
        b = "$BASE/Configurations" + "\u0030" # one more than forward slash
        query = "SELECT id, path FROM configuration WHERE " \
                "path >= ? AND path < ?"
        self.cursor.execute(query, (a, b))
        result = {}
        for row in self.cursor.fetchall():
            result[self.decode_path(row[1])] = row[0]
        return result

    def delete_configuration(self, id=-1, path=None):
        self.init()
        if path is not None:
            query = "DELETE FROM configuration WHERE path = ?"
            path = self.encode_path(path)
            args = [path]
        else:
            query = "DELETE FROM configuration WHERE id = ?"
            args = [id]
        self.cursor.execute(query, args)

    def find_local_roms(self):
        self.init()

        a = "$BASE/Kickstarts/"
        b = "$BASE/Kickstarts" + "\u0030" # one more than forward slash
        query = "SELECT id, path FROM file WHERE path >= ? AND path < ?"
        #args = ["$BASE/Kickstarts/%"]
        self.cursor.execute(query, (a, b))
        result = {}
        for row in self.cursor.fetchall():
            result[self.decode_path(row[1])] = row[0]
        return result

    def delete_file(self, id=-1, path=None):
        self.init()
        if path is not None:
            query = "DELETE FROM file WHERE path = ?"
            path = self.encode_path(path)
            args = [path]
        else:
            query = "DELETE FROM file WHERE id = ?"
            args = [id]
        self.cursor.execute(query, args)

    def search_configurations(self, search):
        self.init()
        query = "SELECT id, name, type FROM configuration WHERE type < 2"
        args = []
        for word in search.split(" "):
            word = word.strip().lower()
            if word:
                #if len(args) == 0:
                #    query = query + " WHERE search like ?"
                #else:
                query = query + " AND search like ?"
                args.append("%{0}%".format(word))
        query = query + " ORDER BY name"
        self.cursor.execute(query, args)
        return self.cursor.fetchall()

    def find_game_variants(self, game_uuid):
        self.init()
        query = "SELECT id, name, uuid, like_rating, work_rating " \
                "FROM configuration WHERE reference = ?"
        query = query + " ORDER BY name"
        print(query, game_uuid)
        self.cursor.execute(query, (game_uuid,))
        return self.cursor.fetchall()

    def get_last_game_variant(self, game_uuid):
        query = "SELECT variant_uuid FROM last_variant WHERE game_uuid = ?"
        self.cursor.execute(query, (game_uuid,))
        row = self.cursor.fetchone()
        if row:
            return row[0]
        return ""

    def set_last_game_variant(self, game_uuid, variant_uuid):
        self.cursor.execute("DELETE FROM last_variant WHERE game_uuid = ?",
                (game_uuid,))
        self.cursor.execute("INSERT INTO last_variant (game_uuid, "
                "variant_uuid) VALUES (?, ?)", (game_uuid, variant_uuid))
        #self.cursor.execute("UPDATE last_variant SET variant_uuid = ? "
        #        "WHERE game_uuid = ?", (variant_uuid, game_uuid))

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
                args.append("%{0}%".format(word))
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
            path = self.decode_path(row[0])
            if result is not None:
                result["path"] = path
            return path
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
            #print("check sha1")
            self.cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE sha1 = ? LIMIT 1", (sha1,))
        elif name:
            #print("check name")
            self.cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE name = ? COLLATE NOCASE LIMIT 1", (name.lower(),))
        else:
            path = self.encode_path(path)
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
            path = self.decode_path(row[1])
            if result is not None:
                result["id"] = row[0]
                result["path"] = path
                result["sha1"] = row[2]
                result["mtime"] = row[3]
                result["size"] = row[4]
            return path
        else:
            if result is not None:
                result["id"] = None
                result["path"] = None
                result["sha1"] = None
                result["mtime"] = None
                result["size"] = None
            return None

    def add_file(self, path="", sha1=None, md5=None, crc32=None, mtime=0,
            size=0, scan=0, name=""):
        self.init()
        path = self.encode_path(path)

        #print("adding path", path)
        #p, name = os.path.split(path)
        self.cursor.execute("INSERT INTO file (path, sha1, mtime, size, "
                "md5, crc32, name, scan) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                (path, sha1, mtime, size, md5, crc32, name, scan))

    def add_configuration(self, path="", uuid="", data="", name="",
                search="", scan=0, type=0, reference=None,
                like_rating=0, work_rating=0):
        self.init()
        path = self.encode_path(path)
        self.cursor.execute("INSERT INTO configuration (path, name, scan, "
                "search, uuid, data, type, reference, like_rating, "
                "work_rating) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                (path, name, scan, search, uuid, data, type, reference,
                like_rating, work_rating))

    def ensure_game_configuration(self, uuid, name, scan=0, type=1):
        self.init()
        self.cursor.execute("SELECT * FROM configuration WHERE uuid = ? "
                "AND name = ? AND scan = ? AND type = ?",
                (uuid, name, scan, type))
        row = self.cursor.fetchone()
        if row is None:
            self.cursor.execute("DELETE from configuration WHERE uuid = ?",
                    (uuid,))
            #self.cursor.execute("INSERT into configuration WHERE uuid = ?",
            #        (uuid,))
            self.add_configuration(uuid=uuid, name=name, scan=scan, type=type)

    def add_game(self, uuid="", path="", name="", search="", scan=0):
        self.init()
        path = self.encode_path(path)
        self.cursor.execute("INSERT INTO game (uuid, path, name, scan, "
                "search) VALUES (?, ?, ?, ?, ?)", (uuid, path, name, scan, search))

    def update_file_scan(self, id, scan):
        self.init()
        self.cursor.execute("UPDATE file SET scan = ? WHERE id = ?",
                (scan, id))

    def update_archive_scan(self, path, scan):
        self.init()
        path = self.encode_path(path)
        #self.cursor.execute("UPDATE file SET scan = ? WHERE path like ?",
        #        (scan, path + u"{0}%".format(unicode(os.sep))))

        a = path + "\u002f" # forward slash
        b = path + "\u0030" # one more than forward slash
        self.cursor.execute("UPDATE file SET scan = ? WHERE "
                "path >= ? AND path < ?", (scan, a, b))


    def remove_unscanned_files(self, scan):
        self.init()
        self.cursor.execute("DELETE FROM file WHERE scan != ?", (scan,))

    def remove_unscanned_configurations(self, scan):
        self.init()
        self.cursor.execute("DELETE FROM configuration WHERE scan != ?",
                (scan,))

    def remove_unscanned_games(self, scan):
        self.init()
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
uuid text,
path text,
data text,
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

    def update_database_to_version_2(self):
        self.cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN type INTEGER NOT NULL DEFAULT 0")
        self.cursor.execute("UPDATE configuration SET type = 0")
        self.cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN reference TEXT")
        self.cursor.execute("CREATE INDEX configuration_reference "
                "ON configuration(reference)")
        self.cursor.execute("CREATE INDEX configuration_type_search "
                "ON configuration(type, search)")

    def update_database_to_version_3(self):
            self.cursor.execute("""CREATE TABLE last_variant (
game_uuid CHAR(36) PRIMARY KEY,
variant_uuid CHAR(36) NOT NULL
)""")

    def update_database_to_version_4(self):
        self.cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN work_rating INTEGER NOT NULL DEFAULT 0")
        self.cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN like_rating INTEGER NOT NULL DEFAULT 0")
        self.cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN keywords TEXT NOT NULL DEFAULT ''")

    def update_database_to_version_5(self):
        # Delete "old" Amiga Forever indexed ROMs
        self.cursor.execute("DELETE FROM file WHERE sha1 = ?",
                ("c39bd9094d4e5f4e28c1411f3086950406062e87",))
        self.cursor.execute("DELETE FROM file WHERE sha1 = ?",
                ("c3c481160866e60d085e436a24db3617ff60b5f9",))
