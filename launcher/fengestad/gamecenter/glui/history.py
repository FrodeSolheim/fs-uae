from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division

#import pyapp
from fengestad.gamecenter.database import Database

class GameHistory:

    @classmethod
    def get_recent_ids(cls):
        ids = []
        k = 0
        while True:
            full_id = fs.config_string("RecentlyPlayed/%d" % k)
            if not full_id:
                break
            ids.append(full_id.strip())
            k += 1
        return ids

    @classmethod
    def get_recently_played(cls, count):
        game_list = []
        for full_id in cls.get_recent_ids():
            try:
                name, platform = full_id.rsplit("(")
                platform = platform[:-1].strip()
                name = name.strip()
                Database.cursor.execute("SELECT id, name, config, platform, "
                        "imageratio, screenratio, year, publisher, developer, subtitle FROM Game "
                        "WHERE name = ? AND platform = ?",
                        (name, platform))
                last_item = None
                for row in Database.cursor:
                    if last_item:
                        if last_item["name"] == row[1] and \
                                last_item["platform"] == row[3]:
                            last_item["configurations"].append(row[2])
                            continue
                    item = {"id": row[0], "name": row[1], "configurations": [row[2]],
                            "platform": row[3], "ratio": row[4], "screenratio": row[5],
                            "year": row[6], "publisher": row[7], "developer": row[8],
                            "subtitlepos": row[9] }
                    game_list.append(item)
                    last_item = item
                
                #game_info = GameList.get().get_game(full_id)
            except Exception:
                pass
        return game_list

    def add_recently_played(cls, game_info):
        print("STUB: add_recently_played")
        return
        full_id = u"{name} ({platform})".format(**game_info)
        ids = cls.get_recent_ids()
        try:
            ids.remove(full_id)
        except ValueError:
            pass
        ids.insert(0, full_id)
        ids = ids[:15]
        pyapp.user.ini.remove_section("RecentlyPlayed")
        for i, full_id in enumerate(ids):
            pyapp.user.ini.set("RecentlyPlayed/%d" % i, full_id)

