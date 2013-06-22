from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.six as six
import os
import json
import time
#import zlib
from gzip import GzipFile
from fs_uae_launcher.six.moves import cStringIO as StringIO

try:
    from urllib.request import HTTPBasicAuthHandler, build_opener, Request
except ImportError:
    from urllib2 import HTTPBasicAuthHandler, build_opener, Request
try:
    from urllib.parse import quote_plus
except ImportError:
    from urllib import quote_plus

# leXME: remove dependency
from fs_uae_launcher.I18N import _, ngettext


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

            # FIXME: move to a common method
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
            print("  processing {0} change entries".format(num_changes))
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

        # while True:
        #     if self.stop_check():
        #         return
        #     json_data = self.fetch_file_entries()
        #     num_files = len(json_data["files"])
        #
        #     # FIXME: move to a common method
        #     have_version = self.client.get_game_database_version()
        #     if have_version != json_data["version"]:
        #         self.set_status(_("Updating database"),
        #                 _("Resetting database..."))
        #         print("have version {0}, need version {1}".format(
        #                 repr(have_version), repr(json_data["version"])))
        #         print("clearing database")
        #         self.client.clear_database()
        #         print("setting database version")
        #         self.client.set_game_database_version(json_data["version"])
        #         continue
        #
        #     if num_files == 0:
        #         print("no more files")
        #         break
        #     print("  processing {0} file entries".format(num_files))
        #     t1 = time.time()
        #     cursor = self.client.database.cursor()
        #     for file in json_data["files"]:
        #         file_id = file["id"]
        #         file_sha1 = file["sha1"]
        #         file_external = file["external"]
        #         file_license_code = file["license_code"]
        #         query = "DELETE FROM file WHERE sha1 = ?"
        #         cursor.execute(query, (file_sha1,))
        #         if file_license_code:
        #             query = "INSERT INTO file (id, sha1, external, " \
        #                     "license_code) VALUES (?, ?, ?, ?)"
        #             cursor.execute(query, (file_id, file_sha1, file_external,
        #                     file_license_code))
        #         else:
        #             # files without license code are not inserted / are
        #             # deleted from the database
        #             pass
        #     t2 = time.time()
        #     print("  {0:0.2f} seconds".format(t2 - t1))

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
                print("\n\n\n\n....\n\n\n\n")
                cursor = self.client.database.cursor()
                cursor.execute("SELECT count(*) FROM rating WHERE game_uuid = "
                               "? AND work_rating = ? AND like_rating = ? "
                               "AND updated = ?", (update["game"],
                               update["work"], update["like"],
                               update["updated"]))
                if cursor.fetchone()[0] == 1:
                    # we want to avoid needlessly creating update transactions
                    continue
                cursor.execute("DELETE FROM rating WHERE game_uuid = ?",
                               (update["game"],))
                cursor.execute(
                    "INSERT INTO rating (game_uuid, work_rating, "
                    "like_rating, updated) VALUES (?, ?, ?, ?)",
                    (update["game"], update["work"], update["like"],
                    update["updated"]))
            t2 = time.time()
            print("  {0:0.2f} seconds".format(t2 - t1))

        print("downloaded size: {0:0.2f} MiB".format(
            self.downloaded_size / (1024 * 1024)))
        count = 0
        for game_uuid, game_id in six.iteritems(games):
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
            server = "oagd.net"
        auth_handler = HTTPBasicAuthHandler()
        auth_handler.add_password(realm="Open Amiga Game Database",
                uri="http://{0}".format(server), user=self.username,
                passwd=self.password)
        opener = build_opener(auth_handler)
        return server, opener

    def fetch_change_entries(self):
        last_id = self.client.get_last_change_id()
        self.set_status(_("Updating database"),
                _("Fetching database entries ({0})").format(last_id + 1))
        server = self.get_server()[0]
        url = "http://{0}/api/1/changes?from={1}".format(server,
                last_id + 1)
        print(url)
        data, json_data = self.fetch_json(url)
        self.downloaded_size += len(data)

        #print(json_data)
        return json_data

    # def fetch_file_entries(self):
    #     cursor = self.client.database.cursor()
    #     result = cursor.execute(self.database.query(
    #         "SELECT max(id) from file"))
    #     last_id = result.fetchone()[0]
    #     if not last_id:
    #         last_id = 0
    #
    #     self.set_status(_("Updating database"),
    #             _("Fetching file entries ({0})").format(last_id + 1))
    #     server = self.get_server()[0]
    #     url = "http://{0}/api/1/files?from={1}".format(server,
    #             last_id + 1)
    #     print(url)
    #     data, json_data = self.fetch_json(url)
    #     self.downloaded_size += len(data)
    #     return json_data

    def fetch_rating_entries(self):
        cursor = self.client.database.cursor()
        cursor.execute("SELECT max(updated) FROM rating")
        row = cursor.fetchone()
        last_time = row[0]
        if not last_time:
            last_time = "2012-01-01 00:00:00"            
        self.set_status(_("Updating database"),
                _("Fetching game ratings ({0})").format(last_time))
        server = self.get_server()[0]
        url = "http://{0}/api/1/ratings?from={1}".format(server,
                quote_plus(last_time))
        print(url)
        data, json_data = self.fetch_json(url)
        self.downloaded_size += len(data)

        #print(json_data)
        return json_data

    def fetch_json_attempt(self, url):
        server, opener = self.get_server()
        request = Request(url)
        request.add_header("Accept-Encoding", "gzip")
        response = opener.open(request)
        # print(response.headers)
        data = response.read()

        #print(dir(response.headers))
        content_encoding = response.headers.getheader(
                "content-encoding", "").lower()
        if content_encoding == "gzip":
            #data = zlib.decompress(data)
            fake_stream = StringIO(data)
            data = GzipFile(fileobj=fake_stream).read()

        #else:
        #    data = response.read()
        #if int(time.time()) % 2 == 0:
        #    raise Exception("fail horribly")
        return data, json.loads(data)

    def fetch_json(self, url):
        for i in range(20):
            try:
                return self.fetch_json_attempt(url)
            except Exception as e:
                print(e)
                sleep_time = 2.0 + i * 0.3
                # FIXME: change second {0} to {1}
                self.set_status(_("Updating database"),
                        _("Download failed (attempt {0}) - "
                                "retrying in {0} seconds").format(
                                i + 1, int(sleep_time)))
                time.sleep(sleep_time)
                self.set_status(_("Updating database"),
                        _("Retrying last operation (attempt {0})").format(
                                i + 1))

        return self.fetch_json_attempt(url)
