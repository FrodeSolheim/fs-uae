from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sqlite3
from .Settings import Settings

class Database:

    VERSION = 7

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
        self._cursor = None

    def init(self):
        if self.connection:
            return
        self.connection = sqlite3.connect(self.get_database_path())
        self._cursor = self.connection.cursor()
        self._cursor = self._cursor
        self.ensure_updated_database()

    def cursor(self):
        self.init()
        return self.connection.cursor()

    def get_files(self, ext=None):
        self.init()
        query = "SELECT path, name FROM file WHERE 1 = 1 "
        args = []
        if ext is not None:
            query = query + " AND path like ?"
            args.append("%" + ext)
        self._cursor.execute(query, args)
        results = []
        for row in self._cursor:
            data = {
                "path": self.decode_path(row[0]),
                "name": row[1]
            }
            results.append(data)
        return results

    def get_configuration_path(self, id):
        self.init()
        query = "SELECT path FROM configuration WHERE id = ?"
        self._cursor.execute(query, (id,))
        path = self.decode_path(self._cursor.fetchone()[0])
        return path

    def get_config(self, id):
        self.init()
        query = "SELECT name, uuid, path, data, game_rating.work_rating, " \
                "game_rating.like_rating FROM configuration LEFT JOIN " \
                "game_rating ON game_rating.game = uuid WHERE id = ?"
        self._cursor.execute(query, (id,))
        row = self._cursor.fetchone()
        return {
            "name": row[0],
            "uuid": row[1],
            "path": self.decode_path(row[2]),
            "data": row[3],
            "work_rating": row[4],
            "like_rating": row[5],
        }

    def get_game_info(self, id):
        self.init()
        query = "SELECT name, uuid, path FROM game WHERE id = ?"
        self._cursor.execute(query, (id,))
        row = self._cursor.fetchone()
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
        #self._cursor.execute(query, args)
        a = "$BASE/Configurations/"
        b = "$BASE/Configurations" + "\u0030" # one more than forward slash
        query = "SELECT id, path FROM configuration WHERE " \
                "path >= ? AND path < ?"
        self._cursor.execute(query, (a, b))
        result = {}
        for row in self._cursor.fetchall():
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
        self._cursor.execute(query, args)

    def find_local_roms(self):
        self.init()

        a = "$BASE/Kickstarts/"
        b = "$BASE/Kickstarts" + "\u0030" # one more than forward slash
        query = "SELECT id, path FROM file WHERE path >= ? AND path < ?"
        #args = ["$BASE/Kickstarts/%"]
        self._cursor.execute(query, (a, b))
        result = {}
        for row in self._cursor.fetchall():
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
        self._cursor.execute(query, args)

    def search_configurations(self, search):
        self.init()
        query = "SELECT id, name, type, sort_key FROM configuration WHERE " \
                "type < 2"
        args = []
        for word in search.split(" "):
            word = word.strip().lower()
            if word:
                #if len(args) == 0:
                #    query = query + " WHERE search like ?"
                #else:
                query = query + " AND search like ?"
                args.append("%{0}%".format(word))
        query = query + " ORDER BY sort_key"
        self._cursor.execute(query, args)
        return self._cursor.fetchall()

    def find_game_variants(self, game_uuid):
        self.init()
        query = "SELECT id, name, uuid, like_rating, work_rating " \
                "FROM configuration WHERE reference = ?"
        query = query + " ORDER BY name"
        print(query, game_uuid)
        self._cursor.execute(query, (game_uuid,))
        return self._cursor.fetchall()

    def get_last_game_variant(self, game_uuid):
        query = "SELECT variant_uuid FROM last_variant WHERE game_uuid = ?"
        self._cursor.execute(query, (game_uuid,))
        row = self._cursor.fetchone()
        if row:
            return row[0]
        return ""

    def set_last_game_variant(self, game_uuid, variant_uuid):
        self._cursor.execute("DELETE FROM last_variant WHERE game_uuid = ?",
                (game_uuid,))
        self._cursor.execute("INSERT INTO last_variant (game_uuid, "
                "variant_uuid) VALUES (?, ?)", (game_uuid, variant_uuid))
        #self._cursor.execute("UPDATE last_variant SET variant_uuid = ? "
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
        self._cursor.execute(query, args)
        return self._cursor.fetchall()

    def find_game(self, uuid="", result=None):
        self.init()
        if uuid:
            self._cursor.execute("SELECT path FROM game "
                    "WHERE uuid = ? LIMIT 1", (uuid,))
        row = self._cursor.fetchone()
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
            self._cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE sha1 = ? LIMIT 1", (sha1,))
        elif name:
            #print("check name")
            self._cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE name = ? COLLATE NOCASE LIMIT 1", (name.lower(),))
        else:
            path = self.encode_path(path)
            #print(path)
            #path = unicode(path)
            #print("SELECT path, sha1, mtime, size FROM file "
            #        "WHERE path = '{0}' LIMIT 1".format(path))
            #self._cursor.execute("SELECT count(*) FROM file "
            #        "WHERE lower(path) = ?", (path.lower(),))

            #self._cursor.execute("SELECT * FROM file LIMIT 1 OFFSET 100")
            #print(self._cursor.fetchall())

            self._cursor.execute("SELECT id, path, sha1, mtime, size FROM file "
                    "WHERE path = ? LIMIT 1", (path,))
        row = self._cursor.fetchone()
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
        self._cursor.execute("INSERT INTO file (path, sha1, mtime, size, "
                "md5, crc32, name, scan) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                (path, sha1, mtime, size, md5, crc32, name, scan))

    def add_configuration(self, path="", uuid="", data="", name="",
                search="", scan=0, type=0, reference=None,
                like_rating=0, work_rating=0, sort_key=""):
        self.init()
        if not sort_key:
            sort_key = name.lower()
        path = self.encode_path(path)
        self._cursor.execute("INSERT INTO configuration (path, name, scan, "
                "search, uuid, data, type, reference, like_rating, "
                "work_rating, sort_key) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, "
                "?, ?)",
                (path, name, scan, search, uuid, data, type, reference,
                like_rating, work_rating, sort_key))

    def ensure_game_configuration(self, uuid, name, sort_key, scan=0, type=1):
        self.init()
        self._cursor.execute("SELECT * FROM configuration WHERE uuid = ? "
                "AND name = ? AND sort_key = ? AND scan = ? AND type = ?",
                (uuid, name, sort_key, scan, type))
        row = self._cursor.fetchone()
        if row is None:
            self._cursor.execute("DELETE from configuration WHERE uuid = ?",
                    (uuid,))
            search = name.lower()
            self.add_configuration(uuid=uuid, name=name, search=search,
                    scan=scan, type=type, sort_key=sort_key)

    def add_game(self, uuid="", path="", name="", search="", scan=0):
        self.init()
        path = self.encode_path(path)
        self._cursor.execute("INSERT INTO game (uuid, path, name, scan, "
                "search) VALUES (?, ?, ?, ?, ?)", (uuid, path, name, scan, search))

    def update_file_scan(self, id, scan):
        self.init()
        self._cursor.execute("UPDATE file SET scan = ? WHERE id = ?",
                (scan, id))

    def update_archive_scan(self, path, scan):
        self.init()
        path = self.encode_path(path)
        #self._cursor.execute("UPDATE file SET scan = ? WHERE path like ?",
        #        (scan, path + u"{0}%".format(unicode(os.sep))))

        a = path + "\u002f" # forward slash
        b = path + "\u0030" # one more than forward slash
        self._cursor.execute("UPDATE file SET scan = ? WHERE "
                "path >= ? AND path < ?", (scan, a, b))


    def remove_unscanned_files(self, scan):
        self.init()
        self._cursor.execute("DELETE FROM file WHERE scan != ?", (scan,))

    def remove_unscanned_configurations(self, scan):
        self.init()
        self._cursor.execute("DELETE FROM configuration WHERE scan != ?",
                (scan,))

    def remove_unscanned_games(self, scan):
        self.init()
        self._cursor.execute("DELETE FROM game WHERE scan != ?",
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
        try:
            self._cursor.execute("SELECT count(*) FROM file")
        except sqlite3.OperationalError:
            self._cursor.execute("""CREATE TABLE file (
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
            self._cursor.execute("CREATE INDEX file_sha1 ON file(sha1)")
            self._cursor.execute("CREATE INDEX file_path ON file(path)")
            self._cursor.execute("CREATE INDEX file_name ON file(name collate nocase)")

        try:
            self._cursor.execute("SELECT count(*) FROM configuration")
        except sqlite3.OperationalError:
            self._cursor.execute("""CREATE TABLE configuration (
id integer primary key,
uuid text,
path text,
data text,
name text,
search text,
scan int
)""")
            self._cursor.execute("CREATE INDEX configuration_name "
                    "ON configuration(name)")
            self._cursor.execute("CREATE INDEX configuration_search "
                    "ON configuration(search)")
            self._cursor.execute("CREATE INDEX configuration_path "
                    "ON configuration(path)")

        try:
            self._cursor.execute("SELECT count(*) FROM game")
        except sqlite3.OperationalError:
            self._cursor.execute("""CREATE TABLE game (
id integer primary key,
uuid text,
path text,
name text,
search text,
scan int
)""")
            self._cursor.execute("CREATE INDEX game_uuid "
                    "ON game(uuid)")

    def update_database_to_version_2(self):
        self._cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN type INTEGER NOT NULL DEFAULT 0")
        self._cursor.execute("UPDATE configuration SET type = 0")
        self._cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN reference TEXT")
        self._cursor.execute("CREATE INDEX configuration_reference "
                "ON configuration(reference)")
        self._cursor.execute("CREATE INDEX configuration_type_search "
                "ON configuration(type, search)")

    def update_database_to_version_3(self):
            self._cursor.execute("""CREATE TABLE last_variant (
game_uuid CHAR(36) PRIMARY KEY,
variant_uuid CHAR(36) NOT NULL
)""")

    def update_database_to_version_4(self):
        self._cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN work_rating INTEGER NOT NULL DEFAULT 0")
        self._cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN like_rating INTEGER NOT NULL DEFAULT 0")
        self._cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN keywords TEXT NOT NULL DEFAULT ''")

    def update_database_to_version_5(self):
        # Delete "old" Amiga Forever indexed ROMs
        self._cursor.execute("DELETE FROM file WHERE sha1 = ?",
                ("c39bd9094d4e5f4e28c1411f3086950406062e87",))
        self._cursor.execute("DELETE FROM file WHERE sha1 = ?",
                ("c3c481160866e60d085e436a24db3617ff60b5f9",))

    def update_database_to_version_6(self):
        self._cursor.execute("ALTER TABLE configuration "
                "ADD COLUMN sort_key TEXT NOT NULL DEFAULT ''")
        self._cursor.execute("UPDATE configuration "
                "SET sort_key = lower(name)")

    def update_database_to_version_7(self):
        self._cursor.execute("""
            CREATE TABLE game_rating (
                game VARCHAR(36) PRIMARY KEY NOT NULL,
                work_rating INT NOT NULL,
                like_rating INT NOT NULL,
                updated TIMESTAMP NULL
            );
        """)
