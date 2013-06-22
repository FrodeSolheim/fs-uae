from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import time
import zlib
import json
import hashlib
import traceback
from .Archive import Archive
#from .Database import Database

#from .ROMManager import ROMManager
#from .I18N import _, ngettext
#from .Util import get_real_case

from .LocalFileDatabase import LocalFileDatabase
from .LocalGameDatabase import LocalGameDatabase
from .GameDatabase import GameDatabase

def _(msg):
    return msg

class GameScanner:

    def __init__(self):
        self.on_status = None
        #self._stop_check = None
        self.scan_count = 0
        self.scan_version = int(time.time() * 100)

    def stop_check(self):
        pass

    def set_status(self, title, status):
        if self.on_status:
            self.on_status((title, status))

    def scan(self):
        self.set_status(_("Scanning configurations"),
                _("Please wait..."))

        self.set_status(_("Scanning configurations"),
                _("Scanning game database entries..."))

        self.local_game_database = LocalGameDatabase()
        self.local_file_database = LocalFileDatabase()
        self.game_database = GameDatabase()
        self.scan_game_database()

        if self.stop_check():
            # aborted
            #database.rollback()
            return

        #database.remove_unscanned_configurations(self.scan_version)
        print("remove unscanned games")
        self.set_status(_("Scanning configurations"), _("Purging old entries..."))
        self.local_game_database.remove_unscanned_games(self.scan_version)

        self.set_status(_("Scanning configurations"), _("Committing data..."))
        self.local_game_database.commit()

    def scan_game_database(self):
        game_cursor = self.game_database.cursor()
        game_cursor.execute("SELECT a.uuid, a.game, a.variant, a.name, "
                "a.platform, value, b.uuid, b.game, b.sort_key, "
                "b.year, b.publisher, b.front_sha1, b.title_sha1, "
                "b.screen1_sha1, b.screen2_sha1, b.screen3_sha1, "
                "b.screen4_sha1, b.screen5_sha1 "
                "FROM game a LEFT JOIN game b ON a.parent = b.id, value "
                "WHERE a.id = value.game AND status = 1 AND "
                "value.name = 'file_list'")
        for row in game_cursor:
            if self.stop_check():
                return

            uuid, game, variant, alt_name, platform, file_list_json, \
                    parent_uuid, parent_game, parent_sort_key, \
                    year, publisher, front_sha1, title_sha1, screen1_sha1, \
                    screen2_sha1, screen3_sha1, screen4_sha1, \
                    screen5_sha1 = row
            if not file_list_json:
                # not a game variant (with files)
                continue

            self.scan_count += 1
            self.set_status(
                    _("Scanning configurations ({count} scanned)").format(
                    count=self.scan_count), uuid)

            try:
                file_list = json.loads(file_list_json)
            except ValueError:
                # invalid JSON string
                continue
            all_found = True
            for file_item in file_list:
                if file_item["name"].endswith("/"):
                    # skip directory entries:
                    continue
                if not self.local_file_database.find_file(
                        sha1=file_item["sha1"]):
                    all_found = False
                    break
            if not all_found:
                print("not found", uuid)
                continue
            print("found", uuid)
            if not game:
                game = alt_name.split("(", 1)[0]
            name = "{0} ({1}, {2})".format(game, platform, variant)
            #search = self.create_configuration_search(name)
            #name = self.create_configuration_name(name)
            
            if not parent_uuid:
                parent_uuid = ""
            
            cursor = self.game_database.cursor()
            cursor.execute("SELECT like_rating, work_rating FROM game_rating "
                    "WHERE game = ?", (uuid,))
            row = cursor.fetchone()
            if row is None:
                like_rating, work_rating = 0, 0
            else:
                like_rating, work_rating = row

            self.local_game_database.add_game_variant(uuid=uuid, name=name,
                    game_uuid=parent_uuid, like_rating=like_rating,
                    work_rating=work_rating,
                    scanned=self.scan_version)

            if parent_uuid:
                #parent_name = "{0}\n{1}".format(parent_game, platform)
                #database.ensure_game_configuration(parent_uuid, parent_name,
                #        parent_sort_key, scan=self.scan_version)
                year = year or 0
                publisher = publisher or ""
                front_sha1 = "sha1:" + front_sha1 if front_sha1 else ""
                title_sha1 = "sha1:" + title_sha1 if title_sha1 else ""
                screen1_sha1 = "sha1:" + screen1_sha1 if screen1_sha1 else ""
                screen2_sha1 = "sha1:" + screen2_sha1 if screen2_sha1 else ""
                screen3_sha1 = "sha1:" + screen3_sha1 if screen3_sha1 else ""
                screen4_sha1 = "sha1:" + screen4_sha1 if screen4_sha1 else ""
                screen5_sha1 = "sha1:" + screen5_sha1 if screen5_sha1 else ""

                self.local_game_database.add_game(parent_uuid, parent_game,
                        platform, year, publisher, front_sha1, title_sha1,
                        screen1_sha1, screen2_sha1, screen3_sha1,
                        screen4_sha1, screen5_sha1, parent_sort_key,
                        scanned=self.scan_version)
