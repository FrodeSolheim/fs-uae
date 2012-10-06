from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from .IRCPanel import IRCPanel
from .Netplay import Netplay
from .Config import Config
from .IRC import IRC

class GamePanel(IRCPanel):

    def __init__(self, parent):
        IRCPanel.__init__(self, parent)
        Config.add_listener(self)
        self.synchronize_player_list = True

        # FIXME: function should be removed when shutting down
        Netplay.notice_function = self.netplay_notice
        Netplay.message_function = self.netplay_message

    def netplay_notice(self, message):
        self.append_text(message)

    def netplay_message(self, message):
        self.append_message(IRC.my_nick, message)
        IRC.irc.privmsg(self.channel, message)

    def on_irc_join(self, args):
        who = args[0]
        channel = args[1]
        if IRC.me(who):
            if channel.startswith("&"):
                text = "Game ({0})".format(channel)
                self.heading.set_text(text)
                self.channel = channel
                self.append_text("*** you joined {0}".format(channel))
        if channel == self.channel:
            for key, value in Config.sync_items():
                message = "__config {0}={1}".format(key, value)
                IRC.irc.privmsg(self.channel, message)
        return IRCPanel.on_irc_join(self, args)

    def on_irc_privmsg(self, args):
        sender = args[0]
        destination = args[1]
        message = args[2]
        if destination == self.channel:
            if message.startswith("__config "):
                self.handle_config(sender, message[9:])
                return True
            if message.startswith("__check "):
                self.handle_check(sender, message[8:])
                return True
            if message.startswith("__abort "):
                self.handle_abort(sender, message[8:])
                return True
        return IRCPanel.on_irc_privmsg(self, args)

    def handle_config(self, who, params):
        key, value = params.split("=", 1)
        #Config.set(key, value)

        # FIXME: SET SOME CONFIG VALUES...
        if True:
            pass

        print("received config", key, value)
        nick = IRC.nick(who)
        Netplay.set_config(nick, key, value)

    def handle_abort(self, who, params):
        message = "*** launch aborted by {0}: {1}".format(IRC.nick(who),
                params)
        self.append_text(message)
        Netplay.config_version = ""

    def abort_launch(self, message):
        Netplay.message("*** aborted launch: {0}".format(message))
        Netplay.message("__abort {0}".format(message))

    def is_op(self, who):
        nick = IRC.nick(who)
        check = "@" + nick
        for n in self.nicks:
            if n == nick:
                return True
        return False

    def handle_check(self, who, params):
        args = params.split(" ")
        #key, value = params.split("=", 1)
        config_version = args[0]
        checksum = args[1]
        #Config.set(key, value)
        #print("received config", key, value)
        #nick = IRC.nick(who)
        #Netplay.set_config(nick, key, value)
        if not self.is_op(who):
            return self.abort_launch("game was not started by an op")
        if Config.get("__netplay_ready") != "1":
            return self.abort_launch("not ready")
        if Config.checksum() == checksum:
            message = "__verified {0}".format(config_version)
            Netplay.message(message)
        else:
            self.abort_launch("config checksum mismatch")

    def on_config(self, key, value):
        if self.channel:
            if key in Config.sync_keys_set:
                message = u"__config {0}={1}".format(key, value)
                IRC.irc.privmsg(self.channel, message)

    def handle_special_message(self, sender, message):
        """
        message = message[2:]
        print("game special message:", message)
        parts = message.split(" ", 1)
        command = parts[0]
        if len(parts) == 2:
            param_s = parts[1]
        else:
            param_s = ""
        #param_l = param_s.split(" ")

        name = "handle_" + command
        try:
            method = getattr(self, name)
        except AttributeError:
            print("no handler for game command", command)
        else:
            method(param_s)
        """
