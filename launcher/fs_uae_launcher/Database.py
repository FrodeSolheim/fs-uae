from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
from fsgs.BaseDatabase import BaseDatabase
from .FSUAEDirectories import FSUAEDirectories

import threading
thread_local = threading.local()

VERSION = 17
RESET_VERSION = 17


class Database(BaseDatabase):

    @classmethod
    def get_path(cls):
        launcher_dir = FSUAEDirectories.get_launcher_dir()
        for i in range(8):
            if i:
                path = os.path.join(
                    launcher_dir, "Database{0}.sqlite".format(i))
            else:
                path = os.path.join(launcher_dir, "Database.sqlite")
            if os.path.exists(path):
                try:
                    os.remove(path)
                except Exception:
                    pass
        path = os.path.join(launcher_dir, "Launcher.sqlite")
        return path

    def __init__(self, SENTINEL):
        BaseDatabase.__init__(self, SENTINEL)

    def get_version(self):
        return VERSION

    def get_reset_version(self):
        return RESET_VERSION

    @classmethod
    def instance(cls, new=False):
        if new or not hasattr(cls.thread_local, "database"):
            cls.thread_local.database = cls(cls.SENTINEL)
        return cls.thread_local.database

    @classmethod
    def get_instance(cls):
        if not hasattr(cls.thread_local, "database"):
            cls.thread_local.database = cls(cls.SENTINEL)
        return cls.thread_local.database

    def __del__(self):
        print("Database.__del__")

    # def init(self):
    #     if self.connection:
    #         return
    #     self.connection = sqlite3.connect(self.get_database_path())
    #     self._cursor = self.connection.cursor()
    #     self._cursor = self._cursor
    #     self.ensure_updated_database()
    #
    # def cursor(self):
    #     self.init()
    #     return self.connection.cursor()

    def get_files(self, ext=None):
        self.init()
        query = "SELECT path, name FROM file WHERE 1 = 1 "
        args = []
        if ext is not None:
            query += " AND path like ?"
            args.append("%" + ext)
        cursor = self.internal_cursor()
        cursor.execute(query, args)
        results = []
        for row in cursor:
            data = {
                "path": self.decode_path(row[0]),
                "name": row[1]
            }
            results.append(data)
        return results

    def get_configuration_path(self, id):
        self.init()
        query = "SELECT path FROM configuration WHERE id = ?"
        cursor = self.internal_cursor()
        cursor.execute(query, (id,))
        path = self.decode_path(cursor.fetchone()[0])
        return path

    # def get_config(self, id):
    #     self.init()
    #     query = "SELECT name, uuid, path, data, game_rating.work_rating, " \
    #             "game_rating.like_rating FROM configuration LEFT JOIN " \
    #             "game_rating ON game_rating.game = uuid WHERE id = ?"
    #     self._cursor.execute(query, (id,))
    #     row = self._cursor.fetchone()
    #     return {
    #         "name": row[0],
    #         "uuid": row[1],
    #         "path": self.decode_path(row[2]),
    #         "data": row[3],
    #         "work_rating": row[4],
    #         "like_rating": row[5],
    #     }

    # def get_game_info(self, id):
    #     self.init()
    #     query = "SELECT name, uuid, path FROM game WHERE id = ?"
    #     self._cursor.execute(query, (id,))
    #     row = self._cursor.fetchone()
    #     return {
    #         "name": row[0],
    #         "uuid": row[1],
    #         "path": self.decode_path(row[2]),
    #     }

    def encode_path(self, path):
        # this only works if both path and FSUAEDirectories.base_dir
        # (etc) have been normalized with get_real_case.
        path = path.replace("\\", "/")
        base_dir = FSUAEDirectories.get_base_dir()
        if path.startswith(base_dir):
            path = path[len(base_dir):]
            if path.startswith("/"):
                path = path[1:]
            path = "$/" + path
        return path

    def decode_path(self, path):
        if not path or path[0] != "$":
            return path
        base_dir = FSUAEDirectories.get_base_dir() + "/"
        if path.startswith("$/"):
            path = base_dir + path[2:]
        return path

    def find_local_configurations(self):
        cursor = self.internal_cursor()
        #query = "SELECT id, path FROM configuration WHERE path like ?"
        #args = ["$BASE/Configurations/%"]
        #self._cursor.execute(query, args)
        a = "$/Configurations/"
        b = "$/Configurations" + "\u0030"  # one more than forward slash
        query = "SELECT id, path FROM game WHERE " \
                "path >= ? AND path < ?"
        cursor.execute(query, (a, b))
        result = {}
        for row in cursor.fetchall():
            result[self.decode_path(row[1])] = row[0]
        return result

    # def delete_configuration(self, id=-1, path=None):
    #     self.init()
    #     if path is not None:
    #         query = "DELETE FROM configuration WHERE path = ?"
    #         path = self.encode_path(path)
    #         args = [path]
    #     else:
    #         query = "DELETE FROM configuration WHERE id = ?"
    #         args = [id]
    #     self._cursor.execute(query, args)

    # def find_local_roms(self):
    #     self.init()
    #
    #     a = "$BASE/Kickstarts/"
    #     b = "$BASE/Kickstarts" + "\u0030" # one more than forward slash
    #     query = "SELECT id, path FROM file WHERE path >= ? AND path < ?"
    #     #args = ["$BASE/Kickstarts/%"]
    #     self._cursor.execute(query, (a, b))
    #     result = {}
    #     for row in self._cursor.fetchall():
    #         result[self.decode_path(row[1])] = row[0]
    #     return result
    #
    # def delete_file(self, id=-1, path=None):
    #     self.init()
    #     if path is not None:
    #         query = "DELETE FROM file WHERE path = ?"
    #         path = self.encode_path(path)
    #         args = [path]
    #     else:
    #         query = "DELETE FROM file WHERE id = ?"
    #         args = [id]
    #     self._cursor.execute(query, args)

    # def search_configurations(self, search):
    #     self.init()
    #     query = "SELECT id, name, type, sort_key FROM configuration WHERE " \
    #             "type < 5"
    #     args = []
    #     for word in search.split(" "):
    #         word = word.strip().lower()
    #         if word:
    #             #if len(args) == 0:
    #             #    query = query + " WHERE search like ?"
    #             #else:
    #             query = query + " AND search like ?"
    #             args.append("%{0}%".format(word))
    #     query = query + " ORDER BY sort_key"
    #     cursor = self.internal_cursor()
    #     cursor.execute(query, args)
    #     return cursor.fetchall()

    def find_game_variants(self, game_uuid):
        self.init()
        query = "SELECT id, name, uuid, configuration.like_rating, " \
                "configuration.work_rating, game_rating.like_rating, " \
                "have FROM configuration LEFT JOIN " \
                "game_rating ON game_rating.game = uuid WHERE " \
                "reference = ?"
        query += " ORDER BY name"
        print(query, game_uuid)
        cursor = self.internal_cursor()
        cursor.execute(query, (game_uuid,))
        return cursor.fetchall()

    def find_game_variants_new(self, game_uuid=""):
        cursor = self.internal_cursor()
        print("FIXME: not looking up ratings yet!")
        cursor.execute(
            "SELECT uuid, name, game_uuid, 0 as like_rating, "
            "0 as work_rating, have FROM game_variant WHERE "
            "game_uuid = ? ORDER BY like_rating DESC, work_rating DESC, "
            "name", (game_uuid,))

        result = []
        for row in cursor:
            result.append(dict(row))
        return result

    def get_last_game_variant(self, game_uuid):
        query = "SELECT variant_uuid FROM last_variant WHERE game_uuid = ?"
        cursor = self.internal_cursor()
        cursor.execute(query, (game_uuid,))
        row = cursor.fetchone()
        if row:
            return row[0]
        return ""

    def set_last_game_variant(self, game_uuid, variant_uuid):
        cursor = self.internal_cursor()
        cursor.execute("DELETE FROM last_variant WHERE game_uuid = ?",
                       (game_uuid,))
        cursor.execute(
            "INSERT INTO last_variant (game_uuid, variant_uuid) "
            "VALUES (?, ?)", (game_uuid, variant_uuid))

    def search_games(self, search):
        self.init()
        query = "SELECT id, name FROM game"
        args = []
        for word in search.split(" "):
            word = word.strip().lower()
            if word:
                if len(args) == 0:
                    query += " WHERE search like ?"
                else:
                    query += " AND search like ?"
                args.append("%{0}%".format(word))
        query += " ORDER BY name"
        cursor = self.internal_cursor()
        cursor.execute(query, args)
        return cursor.fetchall()

    def find_game(self, uuid="", result=None):
        self.init()
        cursor = self.internal_cursor()
        if uuid:
            cursor.execute(
                "SELECT path FROM game WHERE uuid = ? LIMIT 1", (uuid,))
        row = cursor.fetchone()
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
        cursor = self.internal_cursor()
        if sha1:
            #print("xxx", repr(sha1))
            #import traceback
            #traceback.print_stack()
            #print("check sha1")
            cursor.execute(
                "SELECT id, path, sha1, mtime, size FROM file "
                "WHERE sha1 = ? LIMIT 1", (sha1,))
        elif name:
            #print("check name")
            cursor.execute(
                "SELECT id, path, sha1, mtime, size FROM file "
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

            cursor.execute(
                "SELECT id, path, sha1, mtime, size FROM file "
                "WHERE path = ? LIMIT 1", (path,))
        row = cursor.fetchone()
        #print("---------", row)
        if row:
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
        cursor = self.internal_cursor()
        if not name:
            name = os.path.basename(path)
        path = self.encode_path(path)

        #print("adding path", path)
        #p, name = os.path.split(path)
        cursor.execute(
            "INSERT INTO file (path, sha1, mtime, size, "
            "md5, crc32, name, scan) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
            (path, sha1, mtime, size, md5, crc32, name, scan))

    def add_configuration(
            self, path="", uuid="", data="", name="", search="", scan=0,
            type=0, reference=None, like_rating=0, work_rating=0, sort_key=""):
        cursor = self.internal_cursor()
        if not sort_key:
            sort_key = name.lower()
        path = self.encode_path(path)
        cursor.execute(
            "INSERT INTO configuration (path, name, scan, "
            "search, uuid, data, type, reference, like_rating, "
            "work_rating, sort_key) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, "
            "?, ?)",
            (path, name, scan, search, uuid, data, type, reference,
            like_rating, work_rating, sort_key))

    # def ensure_game_configuration(self, uuid, name, sort_key, scan=0, type=1):
    #     cursor = self.internal_cursor()
    #     cursor.execute(
    #         "SELECT * FROM configuration WHERE uuid = ? "
    #         "AND name = ? AND sort_key = ? AND scan = ? AND type < ?",
    #         (uuid, name, sort_key, scan, type))
    #     row = cursor.fetchone()
    #     if row is None:
    #         cursor.execute("DELETE from configuration WHERE uuid = ?",
    #             (uuid,))
    #         search = name.lower()
    #         self.add_configuration(
    #             uuid=uuid, name=name, search=search, scan=scan, type=type,
    #             sort_key=sort_key)

    def add_game(self, path="", name="", sort_key=""):
        cursor = self.internal_cursor()
        path = self.encode_path(path)
        if not sort_key:
            sort_key = name.lower()
        cursor.execute(
            "SELECT id, have, name, sort_key FROM game WHERE path = ?",
            (path,))
        row = cursor.fetchone()
        if row:
            game_id = row[0]
            if row[1] == 5 and row[2] == name and row[3] == sort_key:
                # already up to date
                return game_id
        else:
            cursor.execute("INSERT INTO game (path) VALUES (?)", (path,))
            game_id = cursor.lastrowid
        cursor.execute("UPDATE game SET have = 5, name = ?, sort_key = ? "
                       "WHERE id = ?", (name, sort_key, game_id))
        return game_id

    def delete_game(self, id):
        cursor = self.internal_cursor()
        cursor.execute("DELETE FROM search_term WHERE game = ?", (id,))
        cursor.execute("DELETE FROM game WHERE id = ?", (id,))

    def update_game_search_terms(self, game_id, search_terms):
        cursor = self.internal_cursor()
        search_terms = sorted(search_terms)
        cursor.execute("SELECT term FROM search_term WHERE game = ?",
                       (game_id,))
        existing_terms = sorted(x[0] for x in cursor)
        if search_terms != existing_terms:
            cursor.execute("DELETE FROM search_term WHERE game = ?",
                           (game_id,))
            for term in search_terms:
                cursor.execute("INSERT INTO search_term (game, "
                               "term) VALUES (?, ?)", (game_id, term))

    # def remove_unscanned_configurations(self, scan):
    #     cursor = self.internal_cursor()
    #     cursor.execute("DELETE FROM configuration WHERE scan != ?", (scan,))

    #def remove_unscanned_games(self, scan):
    #    self.init()
    #    self._cursor.execute("DELETE FROM game WHERE scan != ?",
    #            (scan,))

    def find_games_new(self, search=""):
        cursor = self.internal_cursor()
        query = "SELECT uuid, name, platform, year, publisher, front_image, " \
                "title_image, screen1_image, screen2_image, screen3_image, " \
                "screen4_image, screen5_image, have, path, sort_key FROM game"
        args = []
        have_false = False
        for i, term in enumerate(search.split(" ")):
            term = term.strip().lower()
            if term == "have:false":
                have_false = True
                continue
            if term:
                #if len(args) == 0:
                #    query = query + " WHERE search like ?"
                #else:
                #query += " AND search like ?"
                #query += " AND name like ?"
                #args.append("%{0}%".format(word))
                query += " INNER JOIN search_term as st{0} ON " \
                         "game.id = st{0}.game AND st{0}.term = ?".format(i)
                args.append(term)
        if have_false:
            query += " WHERE have = 0"
        else:
            query += " WHERE have > 0"
        query += " ORDER BY sort_key, platform"
        cursor.execute(query, args)
        return cursor.fetchall()

    def add_game_new(self, uuid, name, platform, year, publisher, front_image,
                     title_image, screen1_image, screen2_image, screen3_image,
                     screen4_image, screen5_image, sort_key, scanned=0):
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT * FROM game WHERE uuid = ? "
            "AND name = ? AND platform = ? AND year = ? "
            "AND publisher = ? AND front_image = ? AND title_image = ? "
            "AND screen1_image = ? AND screen2_image = ? "
            "AND screen3_image = ? AND screen4_image = ? "
            "AND screen5_image = ? AND sort_key = ? AND scanned = ?",
            (uuid, name, platform, year, publisher, front_image,
            title_image, screen1_image, screen2_image, screen3_image,
            screen4_image, screen5_image, sort_key, scanned))
        row = cursor.fetchone()
        if row is None:
            cursor.execute("DELETE from game WHERE uuid = ?", (uuid,))
            cursor.execute(
                "INSERT INTO game(uuid, name, "
                "platform, year, publisher, front_image, title_image, "
                "screen1_image, screen2_image, screen3_image, "
                "screen4_image, screen5_image, sort_key, scanned) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                (uuid, name, platform, year, publisher, front_image,
                title_image, screen1_image, screen2_image, screen3_image,
                screen4_image, screen5_image, sort_key, scanned))

    def add_game_variant_new(
            self, uuid="", name="", game_uuid="", like_rating=0,
            work_rating=0, scanned=0):
        cursor = self.internal_cursor()
        query = "INSERT INTO game_variant(uuid, name, game_uuid, " \
                "like_rating, work_rating, scanned) VALUES " \
                "(?, ?, ?, ?, ?, ?)"
        args = (uuid, name, game_uuid, like_rating, work_rating, scanned)
        cursor.execute(query, args)
        return cursor.fetchall()

    def get_ratings_for_game(self, game_uuid):
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT like_rating, work_rating FROM rating WHERE "
            "game_uuid = ?", (game_uuid,))
        row = cursor.fetchone()
        if row is not None:
            return row[0], row[1]
        else:
            return 0, 0

    def get_last_file_event_stamps(self):
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT last_file_insert, last_file_delete FROM metadata")
        row = cursor.fetchone()
        result = {
            "last_file_insert": row[0],
            "last_file_delete": row[0],
        }
        return result

    def update_last_file_event_stamps(self, stamps):
        cursor = self.internal_cursor()
        last_stamps = self.get_last_file_event_stamps()
        if stamps["last_file_insert"] != last_stamps["last_file_insert"]:
            cursor.execute(
                "UPDATE metadata set last_file_insert = ?",
                (stamps["last_file_insert"],))
        if stamps["last_file_delete"] != last_stamps["last_file_delete"]:
            cursor.execute(
                "UPDATE metadata set last_file_delete = ?",
                (stamps["last_file_delete"],))

    def update_database_to_version_17(self):
        cursor = self.internal_cursor()

        cursor.execute("""CREATE TABLE game (
                id INTEGER PRIMARY KEY,
                uuid TEXT,
                name TEXT,
                platform TEXT,
                year INTEGER,
                publisher TEXT,
                front_image TEXT,
                title_image TEXT,
                screen1_image TEXT,
                screen2_image TEXT,
                screen3_image TEXT,
                screen4_image TEXT,
                screen5_image TEXT,
                sort_key TEXT,
                have INTEGER,
                path TEXT,
                update_stamp INTEGER
        )""")
        cursor.execute("CREATE INDEX game_uuid ON game(uuid)")
        cursor.execute("CREATE INDEX game_sort_key ON game(sort_key)")

        cursor.execute("""CREATE TABLE game_variant (
                id INTEGER PRIMARY KEY,
                uuid TEXT,
                name TEXT,
                game_uuid TEXT,
                work_rating INTEGER,
                like_rating INTEGER,
                have INTEGER,
                update_stamp INTEGER
        )""")
        cursor.execute("CREATE INDEX game_variant_uuid "
                       "ON game_variant(uuid)")
        cursor.execute("CREATE INDEX game_variant_game_uuid "
                       "ON game_variant(game_uuid)")
        cursor.execute("CREATE INDEX game_path ON game(path)")

        cursor.execute(
            "ALTER TABLE metadata ADD COLUMN last_file_insert INTEGER")
        cursor.execute(
            "ALTER TABLE metadata ADD COLUMN last_file_delete INTEGER")

        cursor.execute("""CREATE TABLE search_term (
                game INTEGER,
                term TEXT
        )""")

        cursor.execute("""
            CREATE TABLE rating (
                game_uuid VARCHAR(36) PRIMARY KEY NOT NULL,
                work_rating INT NOT NULL,
                like_rating INT NOT NULL,
                updated TIMESTAMP NULL
            );
        """)

        cursor.execute("""CREATE TABLE last_variant (
game_uuid CHAR(36) PRIMARY KEY,
variant_uuid CHAR(36) NOT NULL
)""")
