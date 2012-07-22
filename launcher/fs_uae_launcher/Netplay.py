from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from .Config import Config
from .IRC import IRC

class Player:
    def __init__(self):
        self.nick = ""
        self.config_version = ""
        self.config = Config.default_config.copy()

class Netplay:
    #my_nick = ""
    lobby = "#lobby"
    game = ""
    players = {}
    config_version = ""

    notice_function = None
    message_function = None

    @classmethod
    def set_config(cls, player, key, value):
        print("Netplay set config", player, key, value)
        cls.players[player].config[key] = value

    @classmethod
    def clear_player_list(cls):
        cls.players = {}
        Config.set("x_ready", "0")

    @classmethod
    def add_player(cls, player):
        player = IRC.nick(player)
        if IRC.me(player):
            return
        p = Player()
        cls.players[player] = p
        p.nick = player
        Config.set("x_ready", "0")

    @classmethod
    def remove_player(cls, player):
        player = IRC.nick(player)
        del cls.players[player]
        Config.set("x_ready", "0")

    @classmethod
    def notice(cls, message):
        if cls.notice_function:
            cls.notice_function(message)

    @classmethod
    def message(cls, message):
        if cls.message_function:
            cls.message_function(message)

    @classmethod
    def check_config(cls, key, players=None):
        host_value = Config.get(key)
        for nick, player in cls.players.iteritems():
            if player.config.get(key) != host_value:
                if players is not None:
                    players.append(nick)
                else:
                    return False
        if players and len(players) > 0:
            return False
        return True
