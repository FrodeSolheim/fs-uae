from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

class GameDatabase(object):

    @classmethod
    def get_instance(cls):
        from fs_uae_launcher.Database import Database
        return Database.get_instance()

#import os
#import sqlite3
#from fengestad.base.Application import Application
#from fengestad.base.Paths import Paths
#
#class GameDatabase:
#
#    VERSION = 4
#    instance = None
#
#    database_path = None
#
#    @classmethod
#    def get_database_path(cls):
#        # FIXME:
#        path = Application.get().get_data_path("game-database-beta1.sqlite")
#        return path
#
#        #if not cls.database_path:
#        #    raise Exception("GameDatabase: Database path not set")
#        #return cls.database_path
#
#    @classmethod
#    def set_database_path(cls, path):
#        cls.database_path = path
#
#    @classmethod
#    def get_instance(cls):
#        if not hasattr(cls.thread_local, "instance"):
#            cls.thread_local.instance = cls()
#        return cls.thread_local.instance
#
#    @classmethod
#    def get(cls):
#        return cls()
#
#    def __init__(self, path=""):
#        self._path = path or self.get_database_path()
#        self._connection = None
#        self._cursor = None
#
#    def init(self):
#        if self._connection:
#            return
#        self._connection = sqlite3.connect(self._path)
#        self._cursor = self._connection.cursor()
#        self.ensure_updated_database()
#
#    def query(self, q):
#        return q.replace("%s", "?")
#
#    def cursor(self):
#        self.init()
#        return self._connection.cursor()
#
#    def rollback(self):
#        print("GameDatabase.rollback")
#        self.init()
#        self._connection.rollback()
#
#    def commit(self):
#        print("GameDatabase.commit")
#        self.init()
#        self._connection.commit()
#
#    def ensure_updated_database(self):
#        try:
#            self._cursor.execute("SELECT version FROM metadata")
#            version = self._cursor.fetchone()[0]
#        except sqlite3.OperationalError:
#            self._cursor.execute("CREATE TABLE metadata (version INTEGER)")
#            self._cursor.execute("INSERT INTO metadata (version) VALUES (?)", (0,))
#            version = 0
#        if self.VERSION < version:
#            raise Exception("GameDatabase: Installed database schema is "
#                    "newer. Have you started an old version?")
#        if self.VERSION > version:
#            self.update_database(version, self.VERSION)
#
#    def update_database(self, old, new):
#        for i in range(old + 1, new + 1):
#            print("upgrading game database to version", i)
#            getattr(self, "update_database_to_version_{0}".format(i))()
#            self._cursor.execute("UPDATE metadata SET version = ?", (i,))
#            self.commit()
#
#    def update_database_to_version_1(self):
#        self._cursor.execute("""
#            ALTER TABLE metadata ADD COLUMN games_version
#            INTEGER NOT NULL DEFAULT 0;
#        """)
#
#        self._cursor.execute("""
#            CREATE TABLE user (
#                id INTEGER PRIMARY KEY AUTOINCREMENT,
#                username TEXT NOT NULL,
#                password TEXT NOT NULL DEFAULT '',
#                email TEXT NOT NULL DEFAULT ''
#            );
#        """)
#
#        self._cursor.execute("""
#            INSERT INTO user (username) VALUES ("System");
#        """)
#
#        self._cursor.execute("""
#            CREATE TABLE game (
#                id INTEGER PRIMARY KEY AUTOINCREMENT,
#                uuid CHAR(36) NOT NULL,
#                parent INT,
#                game TEXT NOT NULL DEFAULT '',
#                variant TEXT NOT NULL DEFAULT '',
#                platform TEXT NOT NULL DEFAULT '',
#                name TEXT NOT NULL DEFAULT '',
#                search TEXT NOT NULL DEFAULT '',
#                files INTEGER NOT NULL DEFAULT 0,
#                FOREIGN KEY (parent) REFERENCES game (id)
#                ON UPDATE CASCADE ON DELETE SET NULL
#            );
#        """)
#
#        self._cursor.execute("""
#            CREATE INDEX game_uuid ON game (uuid);
#        """)
#
#        self._cursor.execute("""
#            CREATE TABLE value (
#                id INTEGER PRIMARY KEY AUTOINCREMENT,
#                game INT NOT NULL,
#                name VARCHAR(64) NOT NULL,
#                value TEXT NOT NULL,
#                status INT NOT NULL,
#                submitted TIMESTAMP NOT NULL,
#                submitter INT NOT NULL,
#                reviewed TIMESTAMP NULL,
#                reviewer INT,
#                FOREIGN KEY (game) REFERENCES game (id) ON UPDATE CASCADE,
#                FOREIGN KEY (submitter) REFERENCES user (id) ON UPDATE CASCADE,
#                FOREIGN KEY (reviewer) REFERENCES user (id) ON UPDATE CASCADE
#            );
#        """)
#
#        self._cursor.execute("""
#            CREATE INDEX value_game_status ON value (game, status);
#        """)
#
#        #self._cursor.execute("""
#        #    CREATE INDEX value_game_name_status ON value (game, name, status);
#        #""")
#
#    def update_database_to_version_2(self):
#        self._cursor.execute("""
#            CREATE TABLE game_rating (
#                game VARCHAR(36) PRIMARY KEY NOT NULL,
#                work_rating INT NOT NULL,
#                like_rating INT NOT NULL,
#                updated TIMESTAMP NULL
#            );
#        """)
#
#    def update_database_to_version_3(self):
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN sort_key TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            UPDATE game SET sort_key = lower(name);
#        """)
#
#    def update_database_to_version_4(self):
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN year TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN publisher TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN front_sha1 TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN title_sha1 TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN screen1_sha1 TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN screen2_sha1 TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN screen3_sha1 TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN screen4_sha1 TEXT NOT NULL DEFAULT '';
#        """)
#        self._cursor.execute("""
#            ALTER TABLE game ADD COLUMN screen5_sha1 TEXT NOT NULL DEFAULT '';
#        """)
