from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import json
import time
import urllib
import urllib2
#import threading
from ..I18N import _, ngettext

class GameDatabaseSynchronizer:

    username = ""
    password = ""

    def __init__(self, client, on_status=None, stop_check=None):
        if client:
            self.client = client
            self.database = client.database
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
            self.client.database.rollback()
        else:
            print("commiting data")
            self.set_status(_("Updating database"), _("Committing data..."))
            self.database.commit()
            print("done")

    def _synchronize(self):
        self.set_status(_("Updating database"),
                _("Synchronizing database..."))
        games = {}
        users = {}
        while True:
            if self.stop_check():
                return
            json_data = self.fetch_change_entries()
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

        last_json_data = ""
        while True:
            if self.stop_check():
                return
            json_data = self.fetch_rating_entries()
            if json_data == last_json_data:
                print("no more changes")
                break
            last_json_data = json_data
            num_changes = len(json_data["ratings"])
            print("  processing {0} entries".format(num_changes))
            t1 = time.time()
            for update in json_data["ratings"]:
                cursor = self.client.database.cursor()
                cursor.execute("DELETE FROM game_rating WHERE game = ?",
                    (update["game"],))
                cursor.execute("INSERT INTO game_rating (game, work_rating, "
                        "like_rating, updated) VALUES (?, ?, ?, ?)",
                        (update["game"], update["work"], update["like"],
                        update["updated"]))
            t2 = time.time()
            print("  {0:0.2f} seconds".format(t2 - t1))

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

    def get_server(self):
        try:
            server = os.environ["FS_GAME_DATABASE_SERVER"]
        except KeyError:
            server = "fengestad.no"
        auth_handler = urllib2.HTTPBasicAuthHandler()
        auth_handler.add_password(realm="FS Game Database",
                uri="http://{0}".format(server), user=self.username,
                passwd=self.password)
        opener = urllib2.build_opener(auth_handler)
        return server, opener

    def fetch_change_entries(self):
        last_id = self.client.get_last_change_id()
        self.set_status(_("Updating database"),
                _("Fetching database entries ({0})").format(last_id + 1))
        server = self.get_server()[0]
        url = "http://{0}/games/api/1/changes?from={1}".format(server,
                last_id + 1)
        print(url)
        data, json_data = self.fetch_json(url)
        self.downloaded_size += len(data)

        #print(json_data)
        return json_data

    def fetch_rating_entries(self):
        cursor = self.client.database.cursor()
        cursor.execute("SELECT max(updated) FROM game_rating")
        row = cursor.fetchone()
        last_time = row[0]
        if not last_time:
            last_time = "2012-01-01 00:00:00"            
        self.set_status(_("Updating database"),
                _("Fetching game ratings ({0})").format(last_time))
        server = self.get_server()[0]
        url = "http://{0}/games/api/1/ratings?from={1}".format(server,
                urllib.quote_plus(last_time))
        print(url)
        data, json_data = self.fetch_json(url)
        self.downloaded_size += len(data)

        #print(json_data)
        return json_data

    def fetch_json_attempt(self, url):
        server, opener = self.get_server()
        data = opener.open(url).read()
        #if int(time.time()) % 2 == 0:
        #    raise Exception("fail horribly")
        return data, json.loads(data)

    def fetch_json(self, url):
        for i in range(20):
            try:
                return self.fetch_json_attempt(url)
            except Exception, e:
                sleep_time = 2.0 + i * 0.3
                self.set_status(_("Updating database"),
                        _("Download failed (attempt {0}) - "
                                "retrying in {0} seconds").format(
                                i + 1, int(sleep_time)))
                time.sleep(sleep_time)
                self.set_status(_("Updating database"),
                        _("Retrying last operation (attempt {0})").format(
                                i + 1))

        return self.fetch_json_attempt(url)
