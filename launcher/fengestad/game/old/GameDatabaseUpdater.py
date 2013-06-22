from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .GameDatabase import GameDatabase
from .GameDatabaseClient import GameDatabaseClient
from .GameDatabaseSynchronizer import GameDatabaseSynchronizer

class GameDatabaseUpdater:

    def scan(self):
        game_database = GameDatabase()
        game_database_client = GameDatabaseClient(game_database)
        synchronizer = GameDatabaseSynchronizer(game_database_client)
        synchronizer.username = "FSGameCenter"
        synchronizer.password = "FSGameCenter"
        synchronizer.synchronize()
