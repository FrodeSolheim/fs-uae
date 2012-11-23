from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import json
import time
import urllib2
#import threading
from ..I18N import _, ngettext

class GameDatabaseSynchronizer:

    def __init__(self, client, on_status=None, stop_check=None):
        self.client = client
        self.downloaded_size = 0
        self.on_status = on_status
        self._stop_check = stop_check

    def stop_check(self):
        if self._stop_check:
            return self._stop_check()

    def set_status(self, title, status):
        if self.on_status:
            self.on_status((title, status))

    def synchronize(self):
        self._synchronize()
        if self.stop_check():
            database.rollback()
        else:
            print("commiting data")
            self.set_status(_("Updating database"), _("Committing data..."))
            self.client.database.commit()
            print("done")

    def _synchronize(self):
        self.set_status(_("Updating database"),
                _("Synchronizing database..."))
        games = {}
        users = {}
        while True:
            if self.stop_check():
                return
            json_data = self.fetch_entries()
            num_changes = len(json_data["changes"])

            have_version = self.client.get_game_database_version()
            if have_version != json_data["version"]:
                self.set_status(_("Updating database"),
                        _("Resetting database..."))
                print("have version {0}, need version {1}".format(
                        repr(have_version), repr(json_data["version"])))
                print("clearing database")
                self.client.clear_database()
                print("setting database version")
                self.client.set_game_database_version(json_data["version"])
                continue

            if num_changes == 0:
                print("no more changes")
                break
            print("  processing {0} entries".format(num_changes))
            t1 = time.time()
            for change in json_data["changes"]:
                try:
                    game_id = games[change["game"]]
                except KeyError:
                    game_id = self.client.get_or_create_game_id(change["game"])
                    games[change["game"]] = game_id
                try:
                    user_id = users[change["submitter"]]
                except KeyError:
                    user_id = self.client.get_or_create_user_id(
                            change["submitter"])
                    users[change["submitter"]] = user_id
                self.client.insert_game_value(game_id, change["key"],
                        change["value"], status=1, user=user_id,
                        submitted=change["submitted"],
                        update_games=False, value_id=change["id"])
            t2 = time.time()
            print("  {0:0.2f} seconds".format(t2 - t1))
            #self.client.database.commit()
        print("downloaded size: {0:0.2f} MiB".format(
                self.downloaded_size / (1024 * 1024)))
        count = 0
        for game_uuid, game_id in games.iteritems():
            if self.stop_check():
                return
            if count % 50 == 0:
                self.set_status(_("Updating database"),
                        _("Updating game entries:") +  " {0} / {1}".format(
                        count, len(games)))
            if count % 250 == 0:
                print("updating game entries: {0} / {1}".format(count,
                        len(games)))
            #print(game_id)
            self.client.update_game(game_id)
            count += 1
        print("purging old values")
        self.set_status(_("Updating database"), _("Purging old entries..."))
        self.client.delete_old_values()

    def fetch_entries(self):
        last_id = self.client.get_last_update_id()
        self.set_status(_("Updating database"),
                _("Fetching database entries ({0})").format(last_id + 1))
        try:
            server = os.environ["FS_GAME_DATABASE_SERVER"]
        except KeyError:
            server = "fengestad.no"
        url = "http://{0}/games/api/1/changes?from={1}".format(server,
                last_id + 1)
        print(url)
        data = urllib2.urlopen(url).read()
        json_data = json.loads(data)
        self.downloaded_size += len(data)

        #print(json_data)
        return json_data
