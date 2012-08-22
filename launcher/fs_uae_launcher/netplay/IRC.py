from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import sys
import threading
import fs_uae_launcher.fsui as fsui
from ..Settings import Settings
from .oyoyo.client import IRCClient
from .oyoyo import helpers
from .CommandHandler import CommandHandler
from .IRCBroadcaster import IRCBroadcaster
from .IRCColor import IRCColor

class IRC:

    running = False
    stopping = False
    nick_number = 0
    channels = {}
    my_nick = ""

    # FIXME: should not be hardcoded here
    default_channel = "#lobby"

    active_channel = default_channel

    @classmethod
    def message(cls, message, color=None):
        cls.channel(cls.active_channel).message(message, color)

    @classmethod
    def warning(cls, message):
        cls.message(message, IRCColor.WARNING)

    @classmethod
    def info(cls, message):
        cls.message(message, IRCColor.INFO)

    @classmethod
    def is_channel(cls, name):
        return len(name) > 0 and name[0] in "&#+!"

    @classmethod
    def channel(cls, name):
        # FIXME: name should be checked case independently
        if not name:
            name = cls.default_channel

        from .Channel import Channel
        try:
            return cls.channels[name]
        except KeyError:
            print("new channel", repr(name))
            cls.channels[name] = Channel(name)
            print("channels are now", repr(cls.channels))
            #cls.set_active_channel(name)
            IRCBroadcaster.broadcast("channel_list", {"added": name})
            return cls.channels[name]

    @classmethod
    def set_active_channel(cls, name):
        cls.active_channel = name
        IRCBroadcaster.broadcast("active_channel", {"channel": name})

    @classmethod
    def connect(cls):
        cls.start()

    @classmethod
    def start(cls):
        cls.stopping = False
        if cls.running:
            print("IRC.start - already running")
            return
        threading.Thread(target=cls.irc_thread).start()
        cls.running = True

    @classmethod
    def stop(cls):
        if not cls.running:
            return
        cls.stopping = True
        helpers.quit(cls.client, "Exited")
        #cls.client.quit()

    @classmethod
    def irc_thread(cls):
        try:
            cls.irc_main()
        except Exception, e:
            def func():
                cls.warning(repr(e))
            fsui.call_after(func)
            import traceback
            traceback.print_exc()
        cls.running = False

    @classmethod
    def nick(cls, spec):
        nick = spec.split("!")[0]
        nick = nick.strip("@+")
        return nick

    @classmethod
    def me(cls, spec):
        nick = spec.split("!")[0]
        return nick == cls.my_nick

    @classmethod
    def get_irc_server_host(cls):
        return Settings.get_irc_server()

    @classmethod
    def irc_main(cls):
        def func():
            cls.message(u"connecting to {0}...".format(
                    cls.get_irc_server_host()))
        fsui.call_after(func)

        cls.client = IRCClient(CommandHandler,
                host=cls.get_irc_server_host(),
                port=6667, nick=cls.generate_nick(True),
                blocking=True,
                connect_cb=cls.connect_callback)
        cls.client.handler = cls
        cls.connection = cls.client.connect()

        while not cls.stopping:
            cls.connection.next()
        print("irc_main done")
        cls.running = False

    @classmethod
    def connect_callback(cls, client):
        def func():
            cls.message(u"connected to {0}".format(cls.get_irc_server_host()))
        fsui.call_after(func)

    @classmethod
    def post_message(cls, command, args):
        if cls.stopping:
            print("ignoring message", command, "because we're stopping")
            return
        #print("post_message", command, args)
        #cls.on_message(command, args)
        #print(command, args)
        args = list(args)
        for i, arg in enumerate(args):
            if arg is not None:
                args[i] = arg.decode("UTF-8", errors="replace")
        def func():
            #if IRC.broadcast(command, args):
            #    return
            if cls.irc_server_message(command, args):
                return
            name = "irc_" + command
            try:
                method = getattr(cls, name)
            except AttributeError:
                cls.info(u" ".join([command] + args))
                pass
            else:
                method(*args)
        fsui.call_after(func)

    @classmethod
    def privmsg(cls, destination, message):
        helpers.msg(cls.client, destination, message)

    @classmethod
    def notice(cls, destination, message):
        cls.client.send(u"notice {0} :{1}".format(destination, message))

    @classmethod
    def generate_nick(cls, reset=False):
        if reset:
            cls.nick_number = 0
        cls.nick_number += 1
        if cls.nick_number == 1:
            nick = Settings.get_irc_nick()
        else:
            nick = Settings.get_irc_nick() + str(cls.nick_number)
        cls.my_nick = nick
        return nick

    @classmethod
    def append_text(cls, message):
        cls.message(message)

    @classmethod
    def handle_command(cls, message):
        if message.startswith("/"):
            return cls.handle_command_string(message)
        else:
            if not cls.active_channel:
                cls.warning("no active channel")
            else:
                cls.channel(cls.active_channel).privmsg(message)

    @classmethod
    def handle_command_string(cls, message):
        message = message[1:]
        parts = message.split(u" ")
        command = parts[0].lower()
        args = parts[1:]
        name = "command_" + command
        try:
            method = getattr(cls, name)
        except AttributeError:
            cls.warning(command + u": unknown command")
            return False
        else:
            method(args)
            return True

    @classmethod
    def command_raw(cls, args):
        if len(args) >= 1:
            cls.client.send(u" ".join(args))
        else:
            cls.warning("usage: /raw <raw irc message>")

    @classmethod
    def command_whois(cls, args):
        if len(args) >= 1:
            cls.client.send(u"whois {0}".format(u" ".join(args)))
        else:
            cls.warning("usage: /whois <nick>")

    @classmethod
    def command_away(cls, args):
        if len(args) == 0:
            cls.client.send(u"away")
        else:
            cls.client.send(u"away {0}".format((u" ").join(args)))

    @classmethod
    def command_back(cls, args):
        if len(args) == 0:
            cls.client.send(u"away")
        else:
            cls.warning("usage: /back")

    @classmethod
    def command_msg(cls, args):
        if len(args) >= 2:
            channel = args[0]
            message = u" ".join(args[1:])
            #cls.channel(channel).privmsg(message)
            #cls.channel(channel).message("<{0}> {1}".format(cls.my_nick,
            #        message), IRCColor.MY_MESSAGE)
            cls.client.send(u"privmsg {0} :{1}".format(channel,
                    message))
        else:
            cls.warning("usage: /msg <nick|channel> <message>")

    @classmethod
    def command_notice(cls, args):
        if len(args) >= 2:
            channel = args[0]
            message = u" ".join(args[1:])
            #cls.channel(channel).notice(message)
            cls.client.send(u"notice {0} :{1}".format(channel,
                    message))
        else:
            cls.warning("usage: /notice <nick|channel> <message>")

    @classmethod
    def command_oper(cls, args):
        if len(args) == 2:
            cls.client.send(u"oper {0} {1}".format(args[0], args[1]))
        else:
            cls.warning("usage: /oper <user> <password>")

    @classmethod
    def command_slap(cls, args):
        if len(args) == 1:
            message = u"slaps {0} around a bit with a large trout".format(
                    args[0])
            cls.channel(cls.active_channel).action(message)
        else:
            cls.warning("usage: /slap <nick>")

    @classmethod
    def command_me(cls, args):
        if len(args) > 0:
            message = u" ".join(args)
            cls.channel(cls.active_channel).action(message)
        else:
            cls.warning("usage: /me <message>")



    @classmethod
    def command_mode(cls, args):
        if len(args) >= 2:
            cls.client.send(u"mode {0}".format(u" ".join(args)))
        else:
            cls.warning("usage: /mode <channel|nick> <parameters...>")

    @classmethod
    def command_kick(cls, args):
        if not cls.is_channel(cls.active_channel):
            cls.warning("cannot kick - not in a channel")
            return
        if len(args) >= 1:
            cls.kick(cls.active_channel, args[0], u" ".join(args[1:]))
        else:
            cls.warning("usage: /kick <nick> [<message>]")

    @classmethod
    def kick(cls, channel, nick, message="kicked"):
        cls.client.send("KICK", channel, nick, u":{0}".format(message))

    @classmethod
    def command_join(cls, args):
        if len(args) == 1:
            cls.join(args[0])
        else:
            cls.warning("join: invalid arguments")

    @classmethod
    def join(cls, channel):
        helpers.join(cls.client, channel)

    @classmethod
    def command_leave(cls, args):
        return cls.command_part(args)

    @classmethod
    def command_part(cls, args):
        if len(args) == 1:
            cls.part(args[0])
        else:
            cls.warning("part: invalid arguments")

    @classmethod
    def part(cls, channel):
        helpers.part(cls.client, channel)

    @classmethod
    def irc_server_message(cls, command, args):
        if command in ["yourhost", "created",
                "luserclient", "luserchannels", "luserme",
                "n_local", "n_global", "luserconns", "motdstart",
                "motd", "nomotd", "endofmotd"]:
            cls.message(args[2])
            return 1
        return 0

    @classmethod
    def irc_featurelist(cls, server, nick, *features):
        cls.message(" ".join(features))

    @classmethod
    def irc_welcome(cls, server, nick, message):
        cls.message(message)

        password = Settings.get_irc_nickserv_pass()
        if password:
            cls.privmsg("nickserv", u"identify {0}".format(password))
        
        #cls.join("#support")
        cls.join("#lobby")

        # convenience for development...
        for arg in sys.argv:
            if arg.startswith("--netplay-game="):
                cls.join("&" + arg[15:])
                break

    @classmethod
    def filter_nick(cls, full):
        nick = full.split("!")[0]
        return nick

    @classmethod
    def irc_privmsg(cls, sender, destination, message):
        nick = cls.filter_nick(sender)
        if cls.is_channel(destination):
            channel = destination
        else:
            channel = IRC.filter_nick(sender)
        cls.channel(channel).on_privmsg(nick, message)
        IRCBroadcaster.broadcast("privmsg", {"channel": channel,
            "message": message, "nick": nick})

    @classmethod
    def irc_notice(cls, sender, destination, message):
        if sender:
            nick = cls.filter_nick(sender)
        else:
            nick = "(server)"
        if cls.is_channel(destination):
            channel = destination
        else:
            channel = cls.active_channel
        cls.channel(channel).on_notice(nick, message)
        IRCBroadcaster.broadcast("notice", {"channel": channel,
            "message": message, "nick": nick})

    @classmethod
    def irc_kick(cls, kicker, channel, who, why):
        cls.channel(channel).on_kick(IRC.filter_nick(kicker),
                who, why)

    @classmethod
    def irc_part(cls, who, channel, *args):
        cls.channel(channel).on_part(IRC.filter_nick(who))

    @classmethod
    def irc_join(cls, who, channel):
        cls.channel(channel).on_join(IRC.filter_nick(who))

    @classmethod
    def irc_endofnames(cls, sender, recipient, channel, message):
        pass

    @classmethod
    def irc_namreply(cls, server, who, equals, channel, nicks):
        cls.channel(channel).on_namreply(nicks.split(" "))

    @classmethod
    def irc_currenttopic(cls, sender, recipient, channel, topic):
        cls.channel(channel).on_currenttopic(topic)

    @classmethod
    def irc_topicinfo(cls, sender, recipient, channel, who, when):
        pass

    @classmethod
    def irc_cannotsendtochan(cls, server, who, channel, message):
        cls.channel(channel).warning(u"cannot send to channel: " + message)

    @classmethod
    def irc_topic(cls, who, channel, topic):
        cls.channel(channel).on_topic(who, topic)

    @classmethod
    def irc_mode(cls, who, destination, *args):
        if cls.is_channel(destination):
            cls.channel(destination).on_mode(IRC.filter_nick(who), args)
        else:
            cls.message("{0} set mode {1} {2}".format(
                    who, destination, u" ".join(args)))

    @classmethod
    def irc_quit(cls, who, reason):
        for name in cls.channels:
            cls.channel(name).on_quit(IRC.filter_nick(who), reason)
