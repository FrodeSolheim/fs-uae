from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ..Settings import Settings
from .IRC import IRC
from .Netplay import Netplay
from .IRCBroadcaster import IRCBroadcaster

class Player:

    def __init__(self, nick):
        self.nick = nick
        self.values = {}

    def get(self, key):
        try:
            return self.values[key]
        except KeyError:
            return ""

    def set(self, key, value):
        self.values[key] = value

    def __str__(self):
        return "<Player {0}>".format(self.nick)
