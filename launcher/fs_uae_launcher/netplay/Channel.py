from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from ..Config import Config
from .IRC import IRC
from .IRCColor import IRCColor
#from .Netplay import Netplay
from .IRCBroadcaster import IRCBroadcaster

class Channel:

    @classmethod
    def active(cls):
        return IRC.channel(IRC.active_channel)

    def __init__(self, name):
        self.name = name
        self.lines = []
        self.colors = []
        self.nicks = set()
        self.ops = set()
        self.half_ops = set()
        self.voices = set()

    def get_text(self):
        return u"\n".join(self.lines) + u"\n"

    def is_op(self, nick=None):
        if nick is None:
            nick = IRC.my_nick
        return nick in self.ops

    def message(self, message, color=None):
        if color is None:
            # default color does not work properly on Mac
            color = IRCColor.MESSAGE
        # remove special character (action)
        message = message.replace(u"\u0001", "")
        # remove special character (bold font)
        message = message.replace(u"\u0002", "")
        line = message + u"\n"
        self.lines.append(line)
        self.colors.append(color)
        IRCBroadcaster.broadcast("message", {"channel": self.name,
            "message": line, "color": color})

    def info(self, message):
        return self.message(message, color=IRCColor.INFO)

    def warning(self, message):
        return self.message(message, color=IRCColor.WARNING)

    def get_nick_list(self):
        nicks = []
        for nick in self.nicks:
            if nick in self.ops:
                status = "@"
            elif nick in self.half_ops:
                status = "%"
            elif nick in self.voices:
                status = "+"
            else:
                status = ""
            nicks.append(status + nick)
        return sorted(nicks)

    def action(self, message):
        message = u"\u0001ACTION {0}\u0001".format(message)
        self.privmsg(message)

    def privmsg(self, message, notice=False, echo=True):
        if echo and not message.startswith(u"__"):
            if notice:
                text = u"-{0}- {1}".format(IRC.my_nick, message)
            elif message.startswith(u"[*] "):
                text = u"* {0} {1}".format(IRC.my_nick, message[4:])
            elif message.startswith(u"\u0001ACTION"):
                text = u"* {0} {1}".format(IRC.my_nick, message[8:-1])
            else:
                text = u"<{0}> {1}".format(IRC.my_nick, message)
            self.message(text, IRCColor.MY_MESSAGE)
        if notice:
            IRC.notice(self.name, message)
        else:
            IRC.privmsg(self.name, message)

    def notice(self, message):
        return self.privmsg(message, notice=True)

    def add_nick(self, nick):
        self.nicks.add(nick)
        IRCBroadcaster.broadcast("join", {"channel": self.name,
                "nick": nick})

    def remove_nick(self, nick):
        IRCBroadcaster.broadcast("part", {"channel": self.name,
                "nick": nick})
        if nick not in self.nicks:
            #print("Channel.parted - warning, nick not in list", nick)
            pass
        else:
            self.nicks.remove(nick)
            IRCBroadcaster.broadcast("nick_list", {"channel": self.name})
        if nick in self.ops:
            self.ops.remove(nick)
        if nick in self.half_ops:
            self.half_ops.remove(nick)
        if nick in self.voices:
            self.voices.remove(nick)

    def handle_leave_channel(self):
        self.nicks.clear()
        self.ops.clear()
        self.voices.clear()
        IRCBroadcaster.broadcast("parted", {"channel": self.name})

    def on_privmsg(self, nick, message, notice=False):
        if message.startswith(u"__"):
            pass
        else:
            color = IRCColor.MESSAGE
            if notice:
                text = u"-{0}- {1}".format(nick, message)
                color = IRCColor.NOTICE
            elif message.startswith(u"[*] "):
                text = u"* {0} {1}".format(nick, message[4:])
            elif message.startswith(u"\u0001ACTION"):
                text = u"* {0} {1}".format(nick, message[8:-1])
            else:
                text = u"<{0}> {1}".format(nick, message)
            self.message(text, color)

    def on_notice(self, nick, message):
        return self.on_privmsg(nick, message, notice=True)

    def on_currenttopic(self, topic):
        self.message(u"{0}".format(topic), IRCColor.TOPIC)

    def on_join(self, nick):
        if IRC.me(nick):
            self.nicks.clear()
            self.ops.clear()
            self.voices.clear()
            self.message("* you joined {0} ".format(self.name), IRCColor.JOIN)
            IRC.set_active_channel(self.name)
            IRCBroadcaster.broadcast("joined", {"channel": self.name})
        else:
            self.message("* {0} joined ({1}) ".format(
                    nick, self.name), IRCColor.JOIN)
            if nick in self.nicks:
                print("Channel.joined - warning, nick already in list", nick)
            else:
                #self.nicks.add(nick)
                self.add_nick(nick)
        IRCBroadcaster.broadcast("nick_list", {"channel": self.name})

    def on_kick(self, kicker, kickee, reason):
        self.message(u"* {0} kicked {1} ({2})".format(
                kicker, kickee, reason), IRCColor.KICK)
        if IRC.me(kickee):
            self.handle_leave_channel()

    def on_topic(self, who, topic):
        self.message(u"* {0} changed topic to:\n{1} ".format(who, topic),
                IRCColor.TOPIC)

    def on_mode(self, who, args):
        if args[0] == "+o":
            for nick in args[1:]:
                self.ops.add(nick)
        elif args[0] == "-o":
            for nick in args[1:]:
                if nick in self.ops:
                    self.ops.remove(nick)
        elif args[0] == "+h":
            for nick in args[1:]:
                self.half_ops.add(nick)
        elif args[0] == "-h":
            for nick in args[1:]:
                if nick in self.half_ops:
                    self.half_ops.remove(nick)
        elif args[0] == "+v":
            for nick in args[1:]:
                self.voices.add(nick)
        elif args[0] == "-v":
            for nick in args[1:]:
                if nick in self.voices:
                    self.voices.remove(nick)
        if args[0].startswith("+"):
            color = IRCColor.POS_MODE
        else:
            color = IRCColor.NEG_MODE
        self.message(u"* {0} sets mode {1}".format(who, u" ".join(args)),
                color)
        IRCBroadcaster.broadcast("nick_list", {"channel": self.name})

    def on_namreply(self, nicks):
        for nick in nicks:
            if nick.startswith('@'):
                #self.nicks.add(nick[1:])
                self.ops.add(nick[1:])
                self.add_nick(nick[1:])
            elif nick.startswith('+'):
                #self.nicks.add(nick[1:])
                self.voices.add(nick[1:])
                self.add_nick(nick[1:])
            else:
                #self.nicks.add(nick)
                self.add_nick(nick)
        IRCBroadcaster.broadcast("nick_list", {"channel": self.name})

    def on_part(self, nick):
        if IRC.me(nick):
            self.message("* you left " + self.name, IRCColor.PART)
            self.handle_leave_channel()
        else:
            self.message("* {0} left {1}".format(
                    nick, self.name), IRCColor.PART)
            self.remove_nick(nick)
        IRCBroadcaster.broadcast("nick_list", {"channel": self.name})

    def on_quit(self, nick, reason):
        if nick in self.nicks or self.name == nick or not self.name:
            self.message("* {0} quit ({1}) ".format(nick, reason),
                    IRCColor.PART)
        self.remove_nick(nick)
