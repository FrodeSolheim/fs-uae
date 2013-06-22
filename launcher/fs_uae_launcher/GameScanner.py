from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import time
import json
import traceback
from xml.etree.cElementTree import ElementTree
from fsgs import fsgs
from .Settings import Settings
from .I18N import _
from .Util import get_real_case
from fsgs.FileDatabase import FileDatabase
from fsgs.GameDatabaseClient import GameDatabaseClient
from fsgs.GameDatabaseSynchronizer import GameDatabaseSynchronizer
from fsgs.util.GameNameUtil import GameNameUtil


class GameScanner:

    def __init__(self, paths, on_status=None, stop_check=None):
        self.paths = paths
        self.on_status = on_status
        self._stop_check = stop_check
        self.scan_count = 0
        self.scan_version = int(time.time() * 100)

    def stop_check(self):
        if self._stop_check:
            return self._stop_check()

    def set_status(self, title, status):
        if self.on_status:
            self.on_status((title, status))

    # def scan_fs_uae_files(self, database):
    #     configurations = database.get_files(ext=".fs-uae")
    #     for c in configurations:
    #         if self.stop_check():
    #             break
    #         #name = os.path.basename(c["path"])
    #         #name = c["name"]
    #         #name = name[:-7]
    #         #search = name.lower()
    #         path = c["path"]
    #         name, ext = os.path.splitext(c["name"])
    #         search = self.create_configuration_search(name)
    #         name = self.create_configuration_name(name)
    #         database.add_configuration(
    #             path=path, uuid="", name=name, scan=self.scan_version,
    #             search=search)

    def scan(self, database):
        if not Settings.get("database_feature") == "1":
            return

        self.set_status(_("Scanning games"),
                _("Please wait..."))

        self.set_status(_("Scanning configurations"),
                _("Scanning game database entries..."))

        with fsgs.get_game_database() as game_database:
            self.update_game_database(game_database)
            if self.stop_check():
                return

            self.scan_game_database(database, game_database)
            if self.stop_check():
                return

        #database.remove_unscanned_configurations(self.scan_version)
        #print("remove unscanned games")
        #self.set_status(_("Scanning configurations"), _("Purging old entries
        # ..."))
        #database.remove_unscanned_games_new(self.scan_version)
        #print("remove unscanned configurations")
        #database.remove_unscanned_configurations(self.scan_version)

        #self.set_status(_("Scanning configurations"), _("Committing data..."))
        #database.commit()

    def update_game_database(self, game_database):
        game_database_client = GameDatabaseClient(game_database)
        synchronizer = GameDatabaseSynchronizer(
            game_database_client, on_status=self.on_status,
            stop_check=self.stop_check)
        synchronizer.username = Settings.get("database_username")
        synchronizer.password = Settings.get("database_password")
        synchronizer.synchronize()

    def scan_game_database(self, database, game_database):
        """

        :type game_database: fsgs.GameDatabase.GameDatabase
        :type database: fs_uae_launcher.Database.Database
        """
        game_database_client = GameDatabaseClient(game_database)

        existing_games = {}
        existing_variants = {}
        cursor = database.cursor()
        cursor.execute("SELECT uuid, update_stamp, have, id FROM game "
                       "WHERE uuid IS NOT NULL")
        for row in cursor:
            existing_games[row[0]] = row[1], row[2], row[3]
        cursor.execute("SELECT uuid, update_stamp, have, id FROM game_variant")
        for row in cursor:
            existing_variants[row[0]] = row[1], row[2], row[3]

        # this holds a list of game entry UUIDs which must exist / be checked
        # after variants have been processed
        ensure_updated_games = set()

        file_stamps = FileDatabase.get_instance().get_last_event_stamps()
        cached_file_stamps = database.get_last_file_event_stamps()
        added_files = file_stamps["last_file_insert"] != \
            cached_file_stamps["last_file_insert"]
        deleted_files = file_stamps["last_file_delete"] != \
            cached_file_stamps["last_file_delete"]

        game_cursor = game_database.cursor()
        game_cursor.execute(
            "SELECT a.uuid, a.game, a.variant, a.name, a.platform, "
            "a.downloadable, a.update_stamp, value, b.uuid, b.game, "
            "b.sort_key, "
            "b.year, b.publisher, b.front_sha1, b.title_sha1, "
            "b.screen1_sha1, b.screen2_sha1, b.screen3_sha1, "
            "b.screen4_sha1, b.screen5_sha1 "
            "FROM game a LEFT JOIN game b ON a.parent = b.id, value "
            "WHERE a.id = value.game AND value.status = 1 AND "
            "value.name = 'file_list' AND a.status > -30")
        for row in game_cursor:
            if self.stop_check():
                return

            (uuid, game_name, variant, alt_name, platform, downloadable,
             update_stamp,
             file_list_json, parent_uuid, parent_game, parent_sort_key, year,
             publisher, front_sha1, title_sha1, screen1_sha1, screen2_sha1,
             screen3_sha1, screen4_sha1, screen5_sha1) = row

            if not file_list_json:
                # not a game variant (with files)
                continue

            existing_variant = existing_variants.get(uuid, (None, None, None))

            # after the loop has run its course, variants to be removed
            # are left in existing_variants
            try:
                del existing_variants[uuid]
            except KeyError:
                pass

            if update_stamp == existing_variant[0]:
                # entry was already updated and has not changed
                if existing_variant[1] and not deleted_files:
                    # have this entry already and no files have been deleted
                    # since the last time

                    # print("skipping variant (no deleted files)")
                    continue
                if not existing_variant[1] and not added_files:
                    # do not have this entry, but no files have been added
                    # since the last time

                    # print("skipping variant (no added files)")
                    continue
            else:
                # when the game entry has changed, we always check it
                # regardless of file database status, since file_list may
                # have been changed, or download status... (or other info
                # needs to be corrected)
                pass

            #print("\nscanning", alt_name, update_stamp, existing_variant)

            self.scan_count += 1
            self.set_status(
                _("Scanning game variants ({count} scanned)").format(
                    count=self.scan_count), uuid)

            all_files_found = True
            try:
                file_list = json.loads(file_list_json)
            except Exception:
                # invalid JSON string
                # FIXME: log error
                all_files_found = False
                file_list = []

            for file_item in file_list:
                if file_item["name"].endswith("/"):
                    # skip directory entries:
                    continue
                #location = fsgs.file.find_by_sha1(file_item["sha1"])
                #location = fsgs.file.check_sha1(file_item["sha1"])
                result = fsgs.file.check_sha1(file_item["sha1"])
                if not result:
                    all_files_found = False
                    break

            if all_files_found:
                have_variant = 4
            elif downloadable:
                if downloadable.startswith("http"):
                    have_variant = 1
                else:
                    have_variant = 2
            else:
                have_variant = 0
                #continue

            if not game_name:
                game_name = alt_name.split("(", 1)[0]
            name = "{0} ({1}, {2})".format(game_name, platform, variant)
            search = self.create_configuration_search(name)
            config_name = self.create_configuration_name(name)

            if not parent_uuid:
                have_variant = 0
            #    reference = parent_uuid
            #    type = game_entry_type + 4
            # else:
            #    reference = uuid
            #    type = game_entry_type

            # cursor = game_database.cursor()
            # cursor.execute("SELECT like_rating, work_rating FROM game_rating "
            #               "WHERE game = ?", (uuid,))
            # row = cursor.fetchone()
            # if row is None:
            #    like_rating, work_rating = 0, 0
            # else:
            #    like_rating, work_rating = row

            # the following is used by FS-UAE Launcher for the combined
            # game / configurations list

            # database.add_configuration(
            #    path="", uuid=uuid, name=config_name, scan=self.scan_version,
            #    search=search, type=type, reference=reference,
            #    like_rating=like_rating, work_rating=work_rating)
            #
            # if parent_uuid:
            #    parent_name = "{0}\n{1}".format(parent_game, platform)
            #    database.ensure_game_configuration(
            #        parent_uuid, parent_name, parent_sort_key,
            #        scan=self.scan_version, type=game_entry_type)

            # the following is used by the FS Game Center frontend

            game_variant_id = existing_variant[2]
            if not game_variant_id:
                # variant is not in database
                cursor.execute("INSERT INTO game_variant (uuid) "
                               "VALUES (?)", (uuid,))
                game_variant_id = cursor.lastrowid

            cursor.execute(
                "UPDATE game_variant SET name = ?, game_uuid = ?, have = ?, "
                "update_stamp = ? WHERE id = ?",
                (variant, parent_uuid, have_variant, update_stamp,
                 game_variant_id))

            ensure_updated_games.add(parent_uuid)

            # database.add_game_variant_new(
            #    uuid=uuid, name=name, game_uuid=parent_uuid,
            #    like_rating=like_rating, work_rating=work_rating,
            #    scanned=self.scan_version)

            # if parent_uuid:
            #     # parent_name = "{0}\n{1}".format(parent_game, platform)
            #     # database.ensure_game_configuration(parent_uuid, parent_name,
            #     #        parent_sort_key, scan=self.scan_version)
            #     year = year or 0
            #     publisher = publisher or ""
            #     front_sha1 = "sha1:" + front_sha1 if front_sha1 else ""
            #     title_sha1 = "sha1:" + title_sha1 if title_sha1 else ""
            #     screen1_sha1 = "sha1:" + screen1_sha1 if screen1_sha1 else ""
            #     screen2_sha1 = "sha1:" + screen2_sha1 if screen2_sha1 else ""
            #     screen3_sha1 = "sha1:" + screen3_sha1 if screen3_sha1 else ""
            #     screen4_sha1 = "sha1:" + screen4_sha1 if screen4_sha1 else ""
            #     screen5_sha1 = "sha1:" + screen5_sha1 if screen5_sha1 else ""
            #
            #     database.add_game_new(
            #         parent_uuid, parent_game, platform, year, publisher,
            #         front_sha1, title_sha1, screen1_sha1, screen2_sha1,
            #         screen3_sha1, screen4_sha1, screen5_sha1,
            #         parent_sort_key, scanned=self.scan_version)

        game_cursor = game_database.cursor()
        game_cursor.execute(
            "SELECT b.uuid, b.game, b.update_stamp, "
            "b.sort_key, b.platform, "
            "b.year, b.publisher, b.front_sha1, b.title_sha1, "
            "b.screen1_sha1, b.screen2_sha1, b.screen3_sha1, "
            "b.screen4_sha1, b.screen5_sha1, b.id "
            "FROM game b WHERE files = 0")
        for row in game_cursor:
            if self.stop_check():
                return

            (game_uuid, game_name, update_stamp, sort_key, platform, year,
             publisher, front_sha1, title_sha1, screen1_sha1, screen2_sha1,
             screen3_sha1, screen4_sha1, screen5_sha1, game_id) = row

            existing_game = existing_games.get(game_uuid,
                                               (None, None, None))

            # after the loop has run its course, games to be removed
            # are left in existing_games
            # try:
            #     del existing_games[uuid]
            # except KeyError:
            #     pass

            if update_stamp == existing_game[0]:
                continue

            print("\nscanning game", game_name, update_stamp, existing_game)

            self.scan_count += 1
            self.set_status(_("Scanning games ({count} scanned)").format(
                count=self.scan_count), game_uuid)

            values = game_database_client.get_final_game_values(game_id)
            front_sha1 = values.get("front_sha1", "")
            title_sha1 = values.get("title_sha1", "")
            screen1_sha1 = values.get("screen1_sha1", "")
            screen2_sha1 = values.get("screen2_sha1", "")
            screen3_sha1 = values.get("screen3_sha1", "")
            screen4_sha1 = values.get("screen4_sha1", "")
            screen5_sha1 = values.get("screen5_sha1", "")

            #if not game_name:
            #    game_name = alt_name.split("(", 1)[0]
            #name = "{0} ({1}, {2})".format(game_name, platform, variant)
            name = game_name
            search = self.create_configuration_search(name)
            config_name = self.create_configuration_name(name)

            tags = values.get("tags", "")
            year = values.get("year", "")

            game_id = existing_game[2]
            if not game_id:
                # game is not in database
                cursor.execute("INSERT INTO game (uuid) VALUES (?)",
                               (game_uuid,))
                game_id = cursor.lastrowid

            cursor.execute(
                "UPDATE game SET name = ?, update_stamp = ?, sort_key = ?, "
                "platform = ?, "
                "publisher = ?, year = ?, front_image = ?, title_image = ?, "
                "screen1_image = ?, screen2_image = ?, screen3_image = ?, "
                "screen4_image = ?, screen5_image = ? "
                "WHERE id = ?",
                (name, update_stamp, sort_key, platform,
                 publisher or "", year or 0,
                 "sha1:" + front_sha1 if front_sha1 else "",
                 "sha1:" + title_sha1 if title_sha1 else "",
                 "sha1:" + screen1_sha1 if screen1_sha1 else "",
                 "sha1:" + screen2_sha1 if screen2_sha1 else "",
                 "sha1:" + screen3_sha1 if screen3_sha1 else "",
                 "sha1:" + screen4_sha1 if screen4_sha1 else "",
                 "sha1:" + screen5_sha1 if screen5_sha1 else "",
                 game_id))

            search_terms = set()
            for key in ["game_name", "full_name", "game_name_alt",
                        "search_terms"]:
                value = values.get(key, "")
                if value:
                    search_terms.update(GameNameUtil.extract_index_terms(
                        value))

            for tag in tags.split(","):
                tag = tag.strip().lower()
                search_terms.add("tag:" + tag)
            if year:
                search_terms.add("year:" + str(year))
            if platform:
                search_terms.add("platform:" + platform.lower())
            min_players = 0
            max_players = 0
            sim_players = 0
            players = values.get("players", "")
            if players == "1":
                min_players = max_players = 1
                search_terms.add("players:1")
            elif players:
                try:
                    parts1 = players.split("-")
                    parts2 = parts1[1].split("(")
                    min_players = int(parts1[0].strip())
                    max_players = int(parts2[0].strip())
                    sim_players = int(parts2[1].strip(" )"))
                except Exception as e:
                    print("error parsing players")
                    print(repr(e))
            if min_players > 0:
                if max_players > 0:
                    # we ignore players = 1 here, that is reserved for games
                    # with max 1 players
                    for i in range(min_players + 1, max_players + 1):
                        search_terms.add("players:{0}".format(i))
                if sim_players > 0:
                    # we ignore players = 1 here, that is reserved for games
                    # with max 1 players
                    for i in range(min_players + 1, sim_players + 1):
                        search_terms.add("players:{0}s".format(i))

            # search_terms.difference_update(["the", "a", "for", "in"])
            database.update_game_search_terms(game_id, search_terms)

        # print("a")
        # cursor.execute("SELECT game.id FROM game LEFT JOIN game_variant ON "
        #                "game.uuid = game_variant.game_uuid WHERE "
        #                "game_variant.id IS NULL")
        # print("b")
        # for row in cursor.fetchall():
        #     cursor.execute("DELETE FROM game WHERE id = ?", (row[0],))

        #print("c")
        cursor.execute("SELECT count(*) FROM game WHERE uuid IS NOT NULL "
                       "AND (have IS NULL OR have "
                       "!= (SELECT coalesce(max(have), 0) FROM game_variant "
                       "WHERE game_uuid = game.uuid))")
        #if cursor.rowcount > 0:
        update_rows = cursor.fetchone()[0]
        print(update_rows, "game entries need update for have field")
        if update_rows > 0:
            #print("c1")
            cursor.execute(
                "UPDATE game SET have = (SELECT coalesce(max(have), 0) FROM "
                "game_variant WHERE game_uuid = game.uuid) WHERE uuid IS NOT "
                "NULL AND (have IS NULL OR have != (SELECT coalesce(max("
                "have), 0) FROM game_variant WHERE game_uuid = game.uuid))")
        #print("d")
        FileDatabase.get_instance().get_last_event_stamps()
        database.update_last_file_event_stamps(file_stamps)

    # def scan_configurations(self, database):
    #     for dir in self.paths:
    #         if self.stop_check():
    #             return
    #         self.scan_dir(database, dir)

    # def scan_dir(self, database, dir):
    #     if not isinstance(dir, unicode):
    #         dir = dir.decode(sys.getfilesystemencoding())
    #     if not os.path.exists(dir):
    #         print("does not exist")
    #         return
    #     dir = get_real_case(dir)
    #
    #     for name in os.listdir(dir):
    #         if self.stop_check():
    #             return
    #         if name in [".git"]:
    #             continue
    #         path = os.path.join(dir, name)
    #         if os.path.isdir(path):
    #             self.scan_dir(database, path)
    #             continue
    #         dummy, ext = os.path.splitext(path)
    #         ext = ext.lower()
    #         #if ext not in self.extensions:
    #         #    continue
    #         if ext != ".xml":
    #             continue
    #
    #         self.scan_count += 1
    #         self.set_status(
    #             _("Scanning configurations ({count} scanned)").format(
    #                 count=self.scan_count), name)
    #
    #         print("scan", name)
    #         result = None
    #         try:
    #             tree = ElementTree()
    #             tree.parse(path)
    #             root = tree.getroot()
    #             if root.tag == "config":
    #                 result = self.scan_configuration(database, tree)
    #             elif root.tag == "game":
    #                 self.scan_game(database, tree, path)
    #         except Exception:
    #             traceback.print_exc()
    #         if result is not None:
    #             if "name" in result:
    #                 name = result["name"]
    #             else:
    #                 name, ext = os.path.splitext(name)
    #             print("found", name)
    #             search = self.create_configuration_search(name)
    #             #name = self.create_configuration_name_from_path(path)
    #             name = self.create_configuration_name(name)
    #             database.add_configuration(
    #                 path=path, uuid=result["uuid"], name=name,
    #                 scan=self.scan_version, search=search)

    # def check_if_file_exists(self, database, file_node):
    #     sha1 = file_node["sha1"]
    #     return fsgs.file.find_by_sha1(sha1)
    #
    # def scan_configuration(self, database, tree):
    #     root = tree.getroot()
    #     file_nodes = root.findall("file")
    #     if len(file_nodes) == 0:
    #         print("no files in configuration")
    #         return
    #     for file_node in file_nodes:
    #         if not self.check_if_file_exists(database, file_node):
    #             return
    #
    #     result = {}
    #
    #     game_name = ""
    #     platform_name = ""
    #     #source_name = ""
    #     variant_name = ""
    #
    #     name_node = root.find("name")
    #     if name_node is not None:
    #         variant_name = name_node.text.strip()
    #     #source_node = root.find("source")
    #     #if source_node is not None:
    #     #    source_name = source_node.text.strip()
    #     game_node = root.find("game")
    #     if game_node is not None:
    #         game_name_node = game_node.find("name")
    #         if game_name_node is not None:
    #             game_name = game_name_node.text.strip()
    #         game_platform_node = game_node.find("platform")
    #         if game_platform_node is not None:
    #             platform_name = game_platform_node.text.strip()
    #
    #     parts = []
    #     if platform_name:
    #         parts.append(platform_name)
    #     #if source_name:
    #     #    parts.append(source_name)
    #     if variant_name:
    #         parts.append(variant_name)
    #     if game_name and variant_name:
    #         result["name"] = "{0} ({1})".format(game_name, ", ".join(parts))
    #     result["uuid"] = root.get("uuid", "")
    #     return result

    # def scan_game(self, database, tree, path):
    #     #print("scan_game")
    #     root = tree.getroot()
    #     uuid = root.get("uuid")
    #     name = root.find("name").text.strip()
    #     search = self.create_configuration_search(name)
    #     database.add_game(uuid=uuid, path=path, name=name,
    #                       scan=self.scan_version, search=search)

    @classmethod
    def create_configuration_search(cls, name):
        return name.lower()

    @classmethod
    def create_configuration_name(cls, name):
        if "(" in name:
            primary, secondary = name.split("(", 1)
            secondary = secondary.replace(", ", " \u00b7 ")
            #name = primary.rstrip() + " \u2013 " + secondary.lstrip()
            name = primary.rstrip() + "\n" + secondary.lstrip()
            if name[-1] == ")":
                name = name[:-1]
            name = name.replace(") (", " \u00b7 ")
            name = name.replace(")(", " \u00b7 ")
        return name
