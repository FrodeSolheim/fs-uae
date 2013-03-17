from __future__ import unicode_literals

class GameDatabaseClient:

    def __init__(self, database):
        self.database = database

    def get_or_create_game_id(self, game_uuid):
        cursor = self.database.cursor()
        cursor.execute(self.database.query(
                "SELECT id FROM game WHERE uuid = %s"), (game_uuid,))
        row = cursor.fetchone()
        if row is None:
#            cursor.execute("INSERT INTO game (uuid, updated) "
#                    "VALUES (%s, utc_timestamp())",
#                    (game_uuid,))
            cursor.execute(self.database.query("INSERT INTO game (uuid) "
                    "VALUES (%s)"),
                    (game_uuid,))
            cursor.execute(self.database.query("SELECT id FROM game "
                    "WHERE uuid = %s"), (game_uuid,))
            row = cursor.fetchone()
        return row[0]

    def get_game_id(self, game_uuid):
        cursor = self.database.cursor()
        cursor.execute(self.database.query(
                "SELECT id FROM game WHERE uuid = %s"), (game_uuid,))
        row = cursor.fetchone()
        if row is None:
            raise Exception("game {0} not found".format(game_uuid))
        return row[0]

    def get_ancestor_game_id(self, game_uuid):
        cursor = self.database.cursor()
        cursor.execute(self.database.query(
                "SELECT id, parent FROM game WHERE uuid = %s"), (game_uuid,))
        row = cursor.fetchone()
        if row is None:
            raise Exception("game {0} not found".format(game_uuid))
        return row[1] or row[0]

    def get_or_create_user_id(self, username):
        cursor = self.database.cursor()
        cursor.execute(self.database.query(
                "SELECT id FROM user WHERE username = %s"), (username,))
        row = cursor.fetchone()
        if row is None:
            cursor.execute(self.database.query("INSERT INTO user (username) "
                    "VALUES (%s)"), (username,))
            cursor.execute(self.database.query("SELECT id FROM user "
                    "WHERE username = %s"), (username,))
            row = cursor.fetchone()
        return row[0]

    def insert_game_value(self, game_id, key, value, status, user=1,
            submitted=None, update_games=True, value_id=None, reviewer=None):
        key = key.strip().lower().replace("-", "_")
        value = value.strip()

        cursor = self.database.cursor()
        #print((game_id, key, value))
        cursor.execute(self.database.query("SELECT value FROM value "
                "WHERE game = %s AND name = %s and status = 1"),
                (game_id, key))
        row = cursor.fetchone()
        #if row is not None and row[0].decode("UTF-8") == value:
        if row is not None and row[0] == value:
            # key - value already active
            if value_id:
                # we must insert this anyway, since a specific value id is
                # wanted in the database
                pass
            else:
                return
        if status == 1:
            cursor.execute(self.database.query("UPDATE value SET status = 0 "
                    "WHERE game = %s and name = %s and status = %s"),
                    (game_id, key, status))
        if submitted and value_id:
            cursor.execute(self.database.query("INSERT INTO value (id, game, "
                    "name, value, submitted, submitter, status) "
                    "VALUES (%s, %s, %s, %s, %s, %s, %s)"),
                    (value_id, game_id, key, value, submitted,
                    user, status))
            cursor.execute(self.database.query(
                    "SELECT max(id) from value"))
        elif reviewer is not None:
            cursor.execute(self.database.query("INSERT INTO value (game, "
                    "name, value, submitted, submitter, status, "
                    "reviewer, reviewed) "
                    "VALUES (%s, %s, %s, %s, %s, %s, %s, utc_timestamp())"),
                    (game_id, key, value, submitted, user, status, reviewer))
        else:
            cursor.execute(self.database.query("INSERT INTO value (game, "
                    "name, value, submitted, submitter, status) "
                    "VALUES (%s, %s, %s, utc_timestamp(), %s, %s)"),
                    (game_id, key, value, user, status))

        if update_games and status == 1 and key in ["game_name", "sort_key",
                "variant_name", "x_name", "parent_uuid", "file_list"]:
            self.update_game(game_id)

    def get_final_game_values(self, game_id, recursive=True):
        cursor = self.database.cursor()
        values = {}
        original_game_id = game_id
        while game_id:
            cursor.execute(self.database.query("SELECT name, value FROM "
                    "value WHERE game = %s and status = 1"), (game_id,))
            parent_uuid = ""
            for row in cursor:
                name, value = row
                if not value:
                    continue
                if name == "parent_uuid":
                    parent_uuid = value
                if name == "file_list" and game_id != original_game_id:
                    continue
                if not name in values:
                    values[name] = value
            if not recursive:
                return values
            if parent_uuid:
                print("cannot find parent for", game_id, parent_uuid)
                cursor.execute(self.database.query("SELECT id FROM game "
                        "WHERE uuid = %s"), (parent_uuid,))
                game_id = cursor.fetchone()[0]
            else:
                game_id = None
        return values

    def get_game_values(self, game_id, status=1):
        return self.get_final_game_values(game_id, False)

    def get_pending_game_values(self, game_id):
        cursor = self.database.cursor()
        values = []
        cursor.execute(self.database.query("SELECT name, value FROM "
                "value WHERE game = %s and status = -1"), (game_id,))
        for row in cursor:
            name, value = row
            values.append((name, value))
        return values

    def get_game_value_data(self, game_id):
        cursor = self.database.cursor()
        values = []
        cursor.execute(self.database.query("SELECT value.id, status, name, "
                "value, username FROM value, user "
                "WHERE value.submitter = user.id "
                "AND game = %s AND (status = 1 OR status = -1)"
                "ORDER BY status DESC, name"),
                (game_id,))
        for row in cursor:
            id, status, name, value, username = row
            values.append((id, status, name, value, username))
        return values

    def update_game(self, game_id):
        cursor = self.database.cursor()
        values = self.get_final_game_values(game_id)
        platform = values.get("platform", "")
        game = values.get("game_name", "")
        sort_key = values.get("sort_key", "")
        variant = values.get("variant_name", "")
        x_name = values.get("x_name", "")
        parent_uuid = values.get("parent_uuid", "")
        parent = None
        if parent_uuid:
            cursor.execute(self.database.query("SELECT id FROM game "
                    "WHERE uuid = %s"), (parent_uuid,))
            row = cursor.fetchone()
            if row is not None:
                parent = row[0]

        files = 1 if values.get("file_list", "") else 0
        if game and variant:
            name = "{0} ({1})".format(game, variant)
        else:
            name = x_name or game or variant

        if not sort_key:
            sort_key = game
        if sort_key and variant:
            sort_key = "{0} ({1})".format(sort_key, variant)
        else:
            sort_key = x_name or sort_key or variant
        sort_key = sort_key.lower()

        search = name.lower()
        cursor.execute(self.database.query("UPDATE game SET game = %s, "
                "variant = %s, name = %s, search = %s, platform = %s, "
                "files = %s, parent = %s, sort_key = %s WHERE id = %s"),
                (game, variant, name, search, platform, files, parent,
                sort_key, game_id))

        cursor.execute(self.database.query("SELECT id FROM game WHERE "
                "parent = %s"), (game_id,))
        game_ids = []
        for row in cursor:
            game_ids.append(row[0])
        for gid in game_ids:
            self.update_game(gid)

    def get_changes(self, start, limit):
        cursor = self.database.cursor()
        cursor.execute(self.database.query("SELECT value.id, value.name, "
                "value, user.username, submitted, game.uuid "
                "FROM value, user, game WHERE value.submitter = user.id AND "
                "value.game = game.id AND status = 1 AND value.id >= %s "
                "ORDER BY value.id LIMIT %s"),
                (start, limit))
        for row in cursor:
            yield {
                "id": row[0],
                "key": row[1],
                #"value": row[2].decode("UTF-8"),
                "value": row[2],
                "submitter": row[3],
                "submitted": str(row[4]),
                "game": row[5],
            }

    def get_ratings(self, start, limit, user_id=None):
        cursor = self.database.cursor()
        if user_id is not None:
            cursor.execute(self.database.query("SELECT game.uuid, "
                    "game_rating.work_rating, game_rating.like_rating, "
                    "game_rating.updated FROM game, game_rating WHERE "
                    "game_rating.game = game.id AND game_rating.updated >= %s "
                    "AND game_rating.user = %s LIMIT %s"),
                    (start, user_id, limit))
        else:
            cursor.execute(self.database.query("SELECT uuid, work_rating, "
                    "like_rating, updated FROM game WHERE updated >= %s "
                    "LIMIT %s"), (start, limit))
        for row in cursor:
            yield {
                "game": row[0],
                "work": row[1],
                "like": row[2],
                "updated": str(row[3]),
            }

    def get_user_id(self, username, password):
        cursor = self.database.cursor()
        cursor.execute(self.database.query("SELECT id FROM user WHERE "
                "username = %s and password = %s"),
                (username, password))
        row = cursor.fetchone()
        if row is None:
            return None
        return row[0]

    def get_last_change_id(self):
        cursor = self.database.cursor()
        result = cursor.execute(self.database.query(
                "SELECT max(id) from value"))
        last_id = result.fetchone()[0]
        if not last_id:
            return 0
        return last_id

    def get_game_database_version(self):
        cursor = self.database.cursor()
        result = cursor.execute(self.database.query(
                "SELECT games_version from metadata"))
        return result.fetchone()[0]

    def set_game_database_version(self, version):
        cursor = self.database.cursor()
        result = cursor.execute(self.database.query(
                "UPDATE metadata SET games_version = %s"), (version,))

    def delete_old_values(self):
        cursor = self.database.cursor()
        cursor.execute(self.database.query("DELETE FROM value "
                "WHERE status = 0"))

    def clear_database(self):
        cursor = self.database.cursor()
        cursor.execute(self.database.query("DELETE FROM value"))
        cursor.execute(self.database.query("DELETE FROM game"))
        cursor.execute(self.database.query("DELETE FROM user"))
