from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .BaseDatabase import BaseDatabase

VERSION = 13
RESET_VERSION = 13


class GameDatabase(BaseDatabase):

    def __init__(self, path):
        BaseDatabase.__init__(self, BaseDatabase.SENTINEL)
        self._path = path
        #self._connection = None
        #self._cursor = None

    def get_path(self):
        return self._path

    def get_version(self):
        return VERSION

    def get_reset_version(self):
        return RESET_VERSION

    def query(self, q):
        return q.replace("%s", "?")

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

    def get_license_code_for_url(self, url):
        cursor = self.internal_cursor()
        cursor.execute(
            "SELECT license_code FROM file "
            "WHERE external = ? LIMIT 1", (url,))
        row = cursor.fetchone()
        if not row:
            return None
        return row[0]

    def update_database_to_version_13(self):
        cursor = self.internal_cursor()
        cursor.execute("""
            ALTER TABLE metadata ADD COLUMN games_version
            INTEGER NOT NULL DEFAULT 0;
        """)

        cursor.execute("""
            CREATE TABLE user (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL,
                password TEXT NOT NULL DEFAULT '',
                email TEXT NOT NULL DEFAULT ''
            );
        """)

        cursor.execute("""
            INSERT INTO user (username) VALUES ("System");
        """)

        cursor.execute("""
            CREATE TABLE game (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                uuid CHAR(36) NOT NULL,
                parent INT,
                game TEXT NOT NULL DEFAULT '',
                variant TEXT NOT NULL DEFAULT '',
                platform TEXT NOT NULL DEFAULT '',
                name TEXT NOT NULL DEFAULT '',
                search TEXT NOT NULL DEFAULT '',
                files INTEGER NOT NULL DEFAULT 0,
                sort_key TEXT NOT NULL DEFAULT '',
                status INTEGER NOT NULL DEFAULT 0,
                downloadable TEXT,
                year TEXT NOT NULL DEFAULT '',
                publisher TEXT NOT NULL DEFAULT '',
                front_sha1 TEXT NOT NULL DEFAULT '',
                title_sha1 TEXT NOT NULL DEFAULT '',
                screen1_sha1 TEXT NOT NULL DEFAULT '',
                screen2_sha1 TEXT NOT NULL DEFAULT '',
                screen3_sha1 TEXT NOT NULL DEFAULT '',
                screen4_sha1 TEXT NOT NULL DEFAULT '',
                screen5_sha1 TEXT NOT NULL DEFAULT '',
                update_stamp INTEGER NOT NULL DEFAULT 0,
                FOREIGN KEY (parent) REFERENCES game (id)
                ON UPDATE CASCADE ON DELETE SET NULL
            );
        """)

        cursor.execute("""
            CREATE INDEX game_uuid ON game (uuid);
        """)

        cursor.execute("""
            CREATE TABLE value (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                game INT NOT NULL,
                name VARCHAR(64) NOT NULL,
                value TEXT NOT NULL,
                status INT NOT NULL,
                submitted TIMESTAMP NOT NULL,
                submitter INT NOT NULL,
                reviewed TIMESTAMP NULL,
                reviewer INT,
                FOREIGN KEY (game) REFERENCES game (id) ON UPDATE CASCADE,
                FOREIGN KEY (submitter) REFERENCES user (id) ON UPDATE CASCADE,
                FOREIGN KEY (reviewer) REFERENCES user (id) ON UPDATE CASCADE
            );
        """)

        cursor.execute("""
            CREATE INDEX value_game_status ON value (game, status);
        """)

        cursor.execute("""
            CREATE TABLE rating (
                game_uuid VARCHAR(36) PRIMARY KEY NOT NULL,
                work_rating INT NOT NULL,
                like_rating INT NOT NULL,
                updated TIMESTAMP NULL
            );
        """)
