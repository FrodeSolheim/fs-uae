from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import threading
from collections import deque
import fs_uae_launcher.fsui as fsui
from .Settings import Settings
from .Netplay import Netplay
from .oyoyo.client import IRCClient
from .oyoyo.cmdhandler import DefaultCommandHandler
from .oyoyo import helpers

from .IRC import IRC

_handler = None

class CommandHandler(DefaultCommandHandler):

    #def __init__(self, client, handler):
    #    DefaultCommandHandler.__init__(self, client)

    #def privmsg(self, nick, chan, msg):
    #    print("%s in %s said: %s" % (nick, chan, msg))
    #
    def nicknameinuse(self, nick, *message):
        print("Nickname in use", nick, message)
        helpers.nick(self.client, self.client.handler.generate_nick())

    #def endofmotd(self, *args):
    #    print("end of motd", args)
    #    helpers.join(self.client, "#FS-UAE")

    #def __getattr__(self, name):
    #    print("__getattr__", name)
    #    def handler(*args):
    #        print(name, args)
    #    return handler
    def __unhandled__(self, command, *args):
    #    print("__unhandled__", command, args)
        handler = self.client.handler
        #handler.queue_message((command, args))
        handler.post_message(command, args)

class IRCHandler:

    def __init__(self, lobby):
        #self.queue = deque()
        #self.queue_lock = threading.Lock()
        self.nick_number = 0
        #self.on_message = on_message
        self.stopping = False
        global _handler
        _handler = self
        #self.lobby = lobby
        #self.game = None

    def post_message(self, command, args):
        self.on_message(command, args)

    #def queue_message(self, message):
    #    with self.queue_lock:
    #        self.queue.append(message)

    #def get_message(self):
    #    with self.queue_lock:
    #        return self.queue.popleft()

    def join(self, channel):
        helpers.join(self.client, channel)

    def part(self, channel):
        helpers.part(self.client, channel)

    def privmsg(self, destination, message):
        helpers.msg(self.client, destination, message)

    def generate_nick(self, reset=False):
        if reset:
            self.nick_number = 0
        self.nick_number += 1
        if self.nick_number == 1:
            return Settings.get_nick()
        else:
            return Settings.get_nick() + str(self.nick_number)

    @classmethod
    def get(cls):
        #global _handler
        #if _handler is None:
        #    _handler = IRCHandler()
        return _handler

    def start(self):
        threading.Thread(target=self.irc_thread).start()

    def stop(self):
        self.stopping = True
        helpers.quit(self.client, "Exited")
        #self.client.quit()

    def irc_thread(self):
        try:
            self.irc_main()
        except Exception:
            import traceback
            traceback.print_exc()

    def irc_main(self):
        global lobbyc
        self.client = IRCClient(CommandHandler,
                host=Settings.irc_lobby_server,
                port=6667, nick=self.generate_nick(True),
                blocking=True,
                connect_cb=self.connect_callback)
        self.client.handler = self
        self.connection = self.client.connect()

        while not self.stopping:
            #print("...")
            self.connection.next()
            #conn_2.next()
        print("irc_main done")

    def connect_callback(self, client):
        print("connected")

    def on_message(self, command, args):
        print(command, args)
        args = list(args)
        for i, arg in enumerate(args):
            if arg is not None:
                args[i] = arg.decode("UTF-8", errors="replace")
        def func():
            if IRC.broadcast(command, args):
                return

            name = "irc_" + command
            try:
                method = getattr(self, name)
            except AttributeError:
                if self.server_message(command, args):
                    pass
                else:
                    message = "*** {0} ({1}".format(command,
                            repr(args))
                    self.append_text(message)
            else:
                method(*args)
        fsui.call_after(func)

    def append_text(self, message):
        self.lobby.append_text(message)

    def command_join(self, args):
        if len(args) == 1:
            self.join(args[0])
        else:
            self.append_text("invalid arguments")

    def command_leave(self, args):
        if len(args) == 1:
            self.part(args[0])
        else:
            self.append_text("invalid arguments")

    def command_part(self, args):
        if len(args) == 1:
            self.part(args[0])
        else:
            self.append_text("invalid arguments")

    def handle_command_string(self, message):
        message = message[1:]
        parts = message.split(" ")
        command = parts[0].lower()
        args = parts[1:]
        name = "command_" + command
        try:
            method = getattr(self, name)
        except AttributeError:
            return False
        else:
            method(args)
            return True

    def server_message(self, command, args):
        #return 0
        if command in ["notice", "yourhost", "created",
                "luserclient", "luserchannels", "luserme",
                "n_local", "n_global", "luserconns", "motdstart",
                "motd", "welcome"]:
            self.append_text(args[2])
            return 1
        return 0

    def irc_endofmotd(self, *args):
        self.append_text(args[2])
        IRC.my_nick = args[1]
        self.join(Netplay.lobby)
        self.join("&game")

    def filter_nick(self, full):
        nick = full.split("!")[0]
        return nick

    def irc_privmsg(self, sender, destination, message):
        nick = self.filter_nick(sender)
        if destination == Netplay.lobby:
            message = u"<{0}> {1}".format(nick, message)
            self.lobby.append_text(message)
        elif destination == Netplay.game:
           # if message.startswith("__"):
           #     self.game.handle_special_message(sender, message)
           #     return
            message = u"<{0}> {1}".format(nick, message)
            self.game.append_text(message)
        else:
            message = u"<{0} -> {1}> {2}".format(nick, destination,
                    message)
            self.append_text(message)

    def irc_kick(self, kicker, channel, who, why):
        self.append_text(u"* {0} kicked {1} from {2}: {3}".format(
                self.filter_nick(kicker), who, channel, why))

    def irc_part(self, who, channel, *args):
        if IRC.me(who):
            self.append_text("* you left " + channel)
        else:
            self.append_text("* {0} left ({1}) ".format(who, channel))

    def irc_join(self, who, channel):
        if IRC.me(who):
            self.append_text("* you joined " + channel)
        else:
            self.append_text("* {0} joined ({1}) ".format(who, channel))
        if channel.startswith("&"):
            # game channel
            if IRC.me(who):
                self.join_game_channel(channel)

    def irc_quit(self, who, why):
        self.append_text("* {0} quit ({1}) ".format(who, why))

    def leave_game(self, leave):
        self.append_text("* leaving game " + Netplay.game)
        if leave:
            self.part(Netplay.game)
        Netplay.game = ""

    def join_game_channel(self, channel):
        print("join game channel:", channel)
        if Netplay.game:
            self.leave_game(True)
        Netplay.game = channel
        self.append_text("* joined game " + Netplay.game)
