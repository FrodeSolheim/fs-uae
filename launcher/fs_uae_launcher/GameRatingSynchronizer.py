from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import json
import time
import urllib
import urllib2
from .I18N import _, ngettext
from .fsgs.GameDatabaseSynchronizer import GameDatabaseSynchronizer

class GameRatingSynchronizer(GameDatabaseSynchronizer):

    username = ""
    password = ""

    def __init__(self, database, on_status=None, stop_check=None):
        GameDatabaseSynchronizer.__init__(self, None, on_status, stop_check)
        self.database = database
        #self.on_status = on_status
        #self._stop_check = stop_check

    #def stop_check(self):
    #    if self._stop_check:
    #        return self._stop_check()
    #
    #def set_status(self, title, status):
    #    if self.on_status:
    #        self.on_status((title, status))

    #def synchronize(self):
    #    self._synchronize()
    #    if self.stop_check():
    #        self.database.rollback()
    #    else:
    #        print("commiting data")
    #        self.set_status(_("Updating database"), _("Committing data..."))
    #        self.database.commit()
    #        print("done")

    def _synchronize(self):
        self.set_status(_("Updating database"),
                _("Synchronizing personal ratings..."))

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
                cursor = self.database.cursor()
                cursor.execute("DELETE FROM game_rating WHERE game = ?",
                    (update["game"],))
                cursor.execute("INSERT INTO game_rating (game, work_rating, "
                        "like_rating, updated) VALUES (?, ?, ?, ?)",
                        (update["game"], update["work"], update["like"],
                        update["updated"]))
            t2 = time.time()
            print("  {0:0.2f} seconds".format(t2 - t1))

    #def get_server(self):
    #    try:
    #        server = os.environ["FS_GAME_DATABASE_SERVER"]
    #    except KeyError:
    #        server = "fengestad.no"
    #    auth_handler = urllib2.HTTPBasicAuthHandler()
    #    auth_handler.add_password(realm="FS Game Database",
    #            uri="http://{0}".format(server), user=self.username,
    #            passwd=self.password)
    #    opener = urllib2.build_opener(auth_handler)
    #    return server, opener

    def fetch_rating_entries(self):
        cursor = self.database.cursor()
        cursor.execute("SELECT max(updated) FROM game_rating")
        row = cursor.fetchone()
        last_time = row[0]
        if not last_time:
            last_time = "2012-01-01 00:00:00"            
        self.set_status(_("Updating database"),
                _("Fetching user game ratings ({0})").format(last_time))
        server = self.get_server()[0]
        url = "http://{0}/games/api/1/user_ratings?from={1}".format(server,
                urllib.quote_plus(last_time))
        print(url)
        data, json_data = self.fetch_json(url)
        #self.downloaded_size += len(data)

        return json_data
