from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import uuid
import urllib2
import urlparse
import random
from .IRC import IRC
from .Channel import Channel
from .IRCColor import IRCColor
from .IRCBroadcaster import IRCBroadcaster
from .ConnectionTester import ConnectionTester
from ..Amiga import Amiga
from ..Config import Config
from ..Database import Database
from ..builtin_configs import find_downloadable_file
from ..server.Server import Server
from ..server.ServerWindow import ServerWindow

class Netplay:

    host_ports = []

    enabled = False
    game_channel = ""
    connection_tester = None
    start_sequence = ""
    players = {}

    @classmethod
    def new_host_port(cls):
        if not cls.host_ports is None:
            cls.host_ports = list(range(25102, 25500))
            random.shuffle(cls.host_ports)
        return cls.host_ports.pop()

    @classmethod
    def player(cls, nick):
        try:
            return cls.players[nick]
        except Exception:
            from .Player import Player
            cls.players[nick] = Player(nick)
            print("players are now", cls.players)
            return cls.players[nick]

    @classmethod
    def remove_player(cls, nick):
        try:
            del cls.players[nick]
        except Exception:
            print("warning, tried to remove player (but wasn't in list):",
                    nick)
        else:
            print("players are now", cls.players)

    @classmethod
    def start_netplay_game(cls):
        if Netplay.set_ready():
            if Netplay.is_op():
                cls.game_info("you are marked as ready, initiating game start")
                Netplay.initiate_start_sequence()
            else:
                cls.game_info("you are marked as ready "
                        "(only ops can start game)")
        else:
            cls.game_info("you are not ready")

    @classmethod
    def is_op(cls):
        return IRC.channel(cls.game_channel).is_op(IRC.my_nick)

    @classmethod
    def initiate_start_sequence(cls):
        print("initiate start sequence")
        if not cls.game_channel:
            cls.game_info("cannot start - no game channel")
        cls.start_sequence = str(uuid.uuid4())
        message = u"__prestart {0} {1}".format(cls.start_sequence,
                cls.get_config_hash())
        IRC.channel(cls.game_channel).privmsg(message)
        # in case one wants to try a single-player game for testing
        cls.on_ackstart()

    @classmethod
    def on_ackstart(cls):
        config_hash = cls.get_config_hash()
        start_sequence = cls.start_sequence
        player_count = 0
        for player in cls.players.values():
            player_count += 1
            if player.nick == IRC.my_nick:
                # implicitly ok!
                continue
            if player.get("config_hash") != config_hash:
                return
            if player.get("start_sequence") != start_sequence:
                return
        want_players = int(Config.get("__netplay_players") or "0")
        if player_count != want_players:
            cls.game_warning(u"cannot start game, wanted {0} players, "
                    "has {1}".format(want_players, player_count))
            return
        cls.game_info("sending game start command to all clients!")
        message = u"__start {0} {1}".format(cls.start_sequence,
                cls.get_config_hash())
        IRC.channel(cls.game_channel).privmsg(message)
        cls.do_start_game()

    @classmethod
    def do_start_game(cls):
        cls.game_info("starting game!")
        print("do start game")

        # we now reset __netplay_addresses so that the ConnectionTester
        # does not connect to the game in progress
        Config.set("__netplay_addresses", "")
        from ..LaunchHandler import LaunchHandler
        LaunchHandler.start_local_game()

        # game done, resetting config as the same game server cannot be
        # used again
        cls.reset_netplay_config()

    @classmethod
    def get_config_hash(cls):
        return Config.checksum()

    @classmethod
    def set_ready(cls):
        if not cls.enabled:
            return False
        if not cls.game_channel:
            IRC.warning("cannot set ready - not in a game")
            return False
        if not Config.get("__netplay_addresses"):
            cls.game_warning("cannot set ready - no server configured")
            cls.game_info("an operator needs to use /startgame or "
                    "/hostgame (or /customgame) first")
            return False
        if not Config.get("__netplay_host"):
            cls.game_warning("cannot set ready - no connection to game "
                    "server")
            return False
        Config.set("__netplay_ready", "1")
        # check in case an component has force __netplay_ready back to 0
        return Config.get("__netplay_ready") == "1"

    @classmethod
    def connect(cls):
        cls.enabled = True
        IRC.connect()
        if cls.connection_tester is None:
            cls.connection_tester = ConnectionTester()

    @classmethod
    def is_connected(cls):
        return cls.enabled

    @classmethod
    def on_config(cls, key, value):
        if key == "__netplay_host_last_error":
            cls.game_warning(u"problem connecting to game server: " + value)
        elif key == "__netplay_host":
            if value:
                cls.game_info(u"successfully connected to net play host: "
                        + value)
        if not IRC.running:
            return
        if not cls.game_channel:
            return
        if key in Config.sync_keys_set:
            message = u"__config {0} {1}".format(key, value)
            IRC.channel(cls.game_channel).privmsg(message)

    @classmethod
    def game_message(cls, message, color=None):
        if not cls.game_channel:
            IRC.warning(u"game message received (but not "
                    u"currently in a game channel): " + message)
            return
        IRC.channel(cls.game_channel).message(message, color)
        if cls.game_channel != IRC.active_channel:
            # also paste message to active console
            Channel.active().message(message, color)

    @classmethod
    def game_warning(cls, message):
        cls.game_message(message, IRCColor.WARNING)

    @classmethod
    def game_info(cls, message):
        cls.game_message(message, IRCColor.INFO)

    @classmethod
    def is_game_channel(cls, name):
        if not IRC.is_channel(name):
            return False
        if name.startswith("&"):
            return True
        if name.endswith("-game"):
            return True
        return False

    @classmethod
    def on_irc(cls, key, args):
        if key == "joined":
            if cls.is_game_channel(args["channel"]):
                if cls.game_channel:
                    IRC.part(cls.game_channel)
                cls.game_channel = ""
                cls.reset_netplay_config()
                cls.on_join_game_channel(args["channel"])
        elif key == "parted":
            if cls.game_channel == args["channel"]:
                cls.game_channel = ""
                cls.reset_netplay_config()
        elif key == "privmsg":
            if args["message"].startswith("__"):
                if args["channel"] == cls.game_channel:
                    cls.handle_game_instruction(args["nick"], args["message"])
        elif key == "join":
            if args["channel"] == cls.game_channel:
                # initialize new player
                cls.player(args["nick"])
                if args["nick"] != IRC.my_nick:
                    if cls.is_op():
                        # operator - send config to new player(s)
                        IRC.channel(cls.game_channel).action(
                                "sends config on arrival of new player")
                        cls.send_config()
        elif key == "part":
            if args["channel"] == cls.game_channel:
                Netplay.remove_player(args["nick"])

    @classmethod
    def on_join_game_channel(cls, channel):
        # first we erase config which is not part of the default configuration
        # key set, to avoid some problems
        keys = Config.config.keys()
        for key in keys:
            if key not in Config.default_config:
                Config.set(key, "")
        cls.game_channel = channel
        # reset list of players
        cls.players = {}

    @classmethod
    def reset_config(cls):
        Config.load_default_config()

    @classmethod
    def reset_netplay_config(cls):
        Config.set_multiple([("__netplay_id", ""),
                ("__netplay_password", ""),
                ("__netplay_players", ""),
                ("__netplay_port", ""),
                ("__netplay_addresses", ""),
                ("__netplay_host", "")])

    @classmethod
    def handle_command(cls, command):
        if not command.startswith("/"):
            return False
        args = command[1:].split(" ")
        print(args)
        command = args[0]
        function_name = "command_" + args[0]
        try:
            function = getattr(cls, function_name)
        except AttributeError:
            return False
        else:
            try:
                function(args[1:])
            except Exception, e:
                cls.game_warning(repr(e))
            return True

    @classmethod
    def command_ready(cls, args):
        cls.set_ready()
        if Config.get("__netplay_ready") ==  "1":
            cls.game_info("status set to ready")

    @classmethod
    def command_notready(cls, args):
        Config.set("__netplay_ready", "0")
        cls.game_info("status set to not ready")

    @classmethod
    def command_check(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        if len(args) != 1:
            IRC.warning("syntax: /check <key>")
            return
        if not channel.is_op():
            IRC.warning("check: you need to be an operator")
            return
        for arg in args:
            cls.print_check_request(IRC.my_nick, arg, Config.get(arg))
            message = u"__check {0} {1}".format(arg, Config.get(arg))
            IRC.channel(cls.game_channel).privmsg(message)

    @classmethod
    def command_verify(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        if len(args) != 0:
            IRC.warning("syntax: /verify")
            return
        if not channel.is_op():
            IRC.warning("verify: you need to be an operator")
            return
        #for arg in args:
        #    cls.game_info("requesting verification of {0} = {1}".format(arg,
        #            Config.get(arg)))
        cls.print_verify_request(IRC.my_nick)
        IRC.channel(cls.game_channel).privmsg(u"__beginverify")
        for key in Config.checksum_keys:
            value = Config.get(key)
            message = u"__verify {0} {1}".format(key, value)
            IRC.channel(cls.game_channel).privmsg(message)
        IRC.channel(cls.game_channel).privmsg(u"__endverify")

    @classmethod
    def command_sendconfig(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        if len(args) != 0:
            IRC.warning("syntax: /sendconfig")
            return
        cls.game_info("sending config")
        cls.send_config()

    @classmethod
    def send_config(cls):
        if not cls.game_channel:
            return
        channel = IRC.channel(cls.game_channel)
        # sending keys in preferred key order
        for key in Config.sync_keys_list:
            value = Config.get(key)
            message = u"__config {0} {1}".format(key, value)
            channel.privmsg(message)

    @classmethod
    def require_game_channel(cls, channel):
        if not IRC.running:
            return False
        result = cls.game_channel and channel.name == cls.game_channel
        if not result:
            channel.warning("not in active game channel")
        return result

    @classmethod
    def command_resetconfig(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return

        channel.info("config reset by request")
        # send resetconfig command to other players first
        if channel.is_op():
            channel.privmsg("__resetconfig")
        # then reset own config, so config changes are applied after
        # other players have reset their configs
        Netplay.reset_config()

    @classmethod
    def parse_server_args(cls, args, default_port):
        if len(args) == 0:
            return None
        host = args.pop(0)
        if ":" in host:
            host, port = host.split(":", 1)
            port = int(port)
        else:
            port = default_port
        if len(args) > 0:
            players = args.pop(0)
            players = int(players)
        else:
            players = len(cls.players)
        if len(args) > 0:
            password = args.pop(0)
        else:
            password = ""
        if len(args) > 0:
            return None
        return host, port, players, password

    @classmethod
    def command_startserver(cls, args):
        return cls.command_startgame(args)

    @classmethod
    def command_startgame(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        result = cls.parse_server_args(args, 25101)
        if not result:
            IRC.warning("usage: /startgame <host>[:<port>] "
                    "[<players>] [<password>]")
            return
        host, port, players, password = result

        if not channel.is_op():
            IRC.warning("startgame: you need to be an operator")
            return

        url = u"http://{0}:{1}/game/create?players={2}&password={3}".format(
                host, port, players, password)
        try:
            f = urllib2.urlopen(url)
            result = f.read()
        except urllib2.URLError, e:
            channel.warning(u"problem starting game server: {0}".format(
                    repr(e)))
        else:
            print(result)
            result_dict = urlparse.parse_qs(result)
            game_id = result_dict["id"][0]
            game_password = result_dict["password"][0]
            game_port = result_dict["port"][0]
            game_addresses = result_dict["addresses"][0]
            Config.set_multiple([("__netplay_game", game_id),
                    ("__netplay_password", game_password),
                    ("__netplay_players", str(players)),
                    ("__netplay_port", game_port),
                    ("__netplay_host", u""),
                    ("__netplay_addresses", game_addresses)])
            channel.info(u"started game id: {0} password: {1} "
                    "server: {2} port: {3}".format(
                    game_id, game_password, game_addresses, game_port))

    @classmethod
    def command_hostgame(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        result = cls.parse_server_args(args, cls.new_host_port())
        if not result:
            IRC.warning("usage: /hostgame <host>[:<port>] "
                    "[<players>] [<password>]")
            return
        host, port, players, password = result

        if not channel.is_op():
            IRC.warning("hostgame: you need to be an operator")
            return

        server = Server(port, players, password)
        server.start()
        window = ServerWindow(None, server)
        window.Show()

        game_id = str(uuid.uuid4())
        Config.set_multiple([("__netplay_game", game_id),
                ("__netplay_password", password),
                ("__netplay_players", str(players)),
                ("__netplay_port", str(port)),
                ("__netplay_host", u""),
                ("__netplay_addresses", host)])
        channel.info(u"started game id: {0} password: {1} "
                "server: {2} port: {3}".format(
                game_id, password, host, port))

    @classmethod
    def command_setserver(cls, args):
        return cls.command_customgame(args)

    @classmethod
    def command_customgame(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        result = cls.parse_server_args(args, 25100)
        if not result:
            IRC.warning("usage: /customgame <addresses>[:<port>] "
                    "[<players>] [<password>]")
            return
        addresses, port, players, password = result

        if not channel.is_op():
            IRC.warning("customgame: you need to be an operator")
            return

        game_id = str(uuid.uuid4())
        Config.set_multiple([("__netplay_game", game_id),
                ("__netplay_password", password),
                ("__netplay_players", str(players)),
                ("__netplay_port", str(port)),
                ("__netplay_host", u""),
                ("__netplay_addresses", addresses)])
        channel.info(u"started game id: {0} password: {1} "
                "server: {2} port: {3}".format(
                game_id, password, addresses, port))

    @classmethod
    def command_set(cls, args):
        channel = Channel.active()
        if not cls.require_game_channel(channel):
            return
        if len(args) < 1:
            IRC.warning("usage: /set <key> [<value>]")
            return
        if not channel.is_op():
            IRC.warning("hostgame: you need to be an operator")
            return

        key = args[0]
        value = " ".join(args[1:])
        print("config, setting", key, "to", value)
        channel.action("set option {0} to \"{1}\"".format(key, value))
        Config.set(key, value)

    @classmethod
    def handle_game_instruction(cls, nick, message):
        channel = IRC.channel(cls.game_channel)

        #if Netplay.game_channel != self.name:
        #    self.warning(self, u"ignored command in "
        #            "non-active game: {0}".format(command), IRCColor.WARNING)
        #    return
        words = message.split(u" ")
        command = words[0]
        arg = u" ".join(words[1:])
        if command == "__config":
            if channel.is_op(nick):
                args = arg.split(u" ", 1)
                key = args[0]
                value = ""
                if len(args) > 1:
                    value = args[1]
                cls.set_config(key.strip(), value.strip())
        elif command == "__prestart":
            if channel.is_op(nick):
                seq, config_hash = arg.split(u" ")
                my_config_hash = cls.get_config_hash()
                if my_config_hash != config_hash:
                    channel.privmsg("not the same config hash")
                    return
                if Config.get("__netplay_ready") != "1":
                    channel.privmsg("(op tried to start game, "
                            "but I am not ready)")
                    return
                channel.privmsg("__ackstart {0} {1}".format(seq,
                    my_config_hash))
        elif command == "__ackstart":
            start_sequence, config_hash = arg.split(u" ")
            cls.player(nick).set("start_sequence", start_sequence)
            cls.player(nick).set("config_hash", config_hash)
            cls.on_ackstart()
            # FIXME: check start sequence?
        elif command == "__start":
            if channel.is_op(nick):
                start_sequence, config_hash = arg.split(u" ")
                my_config_hash = Netplay.get_config_hash()
                if my_config_hash != config_hash:
                    channel.action("could not start game "
                            "(mismatching config hash)")
                    return
                Netplay.do_start_game()
        elif command == "__resetconfig":
            if channel.is_op(nick):
                channel.info("config reset by op")
                Netplay.reset_config()
        elif command == "__check":
            if channel.is_op(nick):
                args = arg.split(u" ")
                key = args[0]
                value = u" ".join(args[1:])
                my_value = Config.get(key)
                cls.print_check_request(nick, key, value)
                cls.print_check_response(IRC.my_nick, key, my_value, value)
                channel.privmsg(u"__ackcheck {0} {1}".format(key, my_value))
        elif command == "__beginverify":
            if channel.is_op(nick):
                cls.print_verify_request(nick)
        elif command == "__verify":
            if channel.is_op(nick):
                args = arg.split(u" ")
                key = args[0]
                value = u" ".join(args[1:])
                if Config.get(key) != value:
                    cls.print_verify_response(IRC.my_nick, key,
                            Config.get(key))
                    channel.privmsg(u"__ackverify {0} {1}".format(key,
                        Config.get(key)))
        elif command == "__endverify":
            pass
        elif command == "__ackcheck":
            args = arg.split(u" ")
            key = args[0]
            value = u" ".join(args[1:])
            my_value = Config.get(key)
            cls.print_check_response(nick, key, value, my_value)
        elif command == "__ackverify":
            args = arg.split(u" ", 1)
            cls.print_verify_response(nick, args[0], u" ".join(args[1:]))
        else:
            channel.warning(u"unknown command received: {0}".format(
                    command))

    @classmethod
    def print_check_request(cls, nick, key, value):
        IRC.channel(cls.game_channel).info(
                u"* {0} requested check of {1} ({2})".format(
                nick, key, value))

    @classmethod
    def print_check_response(cls, nick, key, value, check_value):
        if value == check_value:
            color = IRCColor.INFO
        else:
            color = IRCColor.WARNING
        IRC.channel(cls.game_channel).message(
                u"{0} = {1} ({2})".format(key, value, nick), color)

    @classmethod
    def print_verify_request(cls, nick):
        IRC.channel(cls.game_channel).info(
                u"* {0} requested automatic config verification".format(nick))

    @classmethod
    def print_verify_response(cls, nick, key, value):
        IRC.channel(cls.game_channel).warning(
                u"{0} = {1} ({2})".format(key, value, nick))

    @classmethod
    def set_config(cls, key, value):
        # this config was received from a game channel operator
        print("received config", key, value)
        channel = IRC.channel(cls.game_channel)
        if key not in Config.sync_keys_set:
            print("not processing this key")
            return
        elif key in ["x_kickstart_file_sha1", "x_kickstart_ext_file_sha1"]:
            cls.set_kickstart_config(key, value)
        elif key in cls.file_config:
            cls.set_file_config(key, value)
        else:
            Config.set(key, value)

    @classmethod
    def set_kickstart_config(cls, key, value):
        channel = IRC.channel(cls.game_channel)
        if key == "x_kickstart_file_sha1":
            if value == Amiga.INTERNAL_ROM_SHA1:
                    Config.set_multiple([
                            ("kickstart_file", "internal"),
                            ("x_kickstart_file", "internal"),
                            (key, value)])
            else:
                path = Database.get_instance().find_file(sha1=value)
                if path:
                    Config.set_multiple([
                            ("kickstart_file", path),
                            ("x_kickstart_file", path),
                            (key, value)])
                else:
                    channel.action(u"could not find kickstart for {0}".format(
                            repr(value)))
        elif key == "x_kickstart_ext_file_sha1":
            if not value:
                Config.set_multiple([
                        ("kickstart_ext_file", ""),
                        ("x_kickstart_ext_file", ""),
                        (key, "")])
                return
            path = Database.get_instance().find_file(sha1=value)
            if path:
                Config.set_multiple([
                        ("kickstart_ext_file", path),
                        ("x_kickstart_ext_file", path),
                        (key, value)])
            else:
                channel.action(u"could not find (ext) kickstart "
                        "for {0}".format(repr(value)))

    @classmethod
    def set_file_config(cls, key, value):
        channel = IRC.channel(cls.game_channel)
        set_key = cls.file_config[key]
        if not value:
            Config.set_multiple([(set_key, ""), (key, "")])
            return
        if value.startswith("http://") or value.startswith("https://"):
            path = value
        else:
            path = Database.get_instance().find_file(sha1=value)
        if not path:
            path = find_downloadable_file(value)
        if path:
            Config.set_multiple([(set_key, path), (key, value)])
        else:
            Config.set_multiple([(set_key, ""), (key, "")])
            channel.action(u"could not find {1} for "
                    "for {0}".format(value, set_key))

    file_config = {
    }
    for i in range(Amiga.MAX_FLOPPY_DRIVES):
        file_config["x_floppy_drive_{0}_sha1".format(i)] = \
                "floppy_drive_{0}".format(i)
    for i in range(Amiga.MAX_FLOPPY_IMAGES):
        file_config["x_floppy_image_{0}_sha1".format(i)] = \
                "floppy_image_{0}".format(i)
    for i in range(Amiga.MAX_CDROM_DRIVES):
        file_config["x_cdrom_drive_{0}_sha1".format(i)] = \
                "cdrom_drive_{0}".format(i)
    for i in range(Amiga.MAX_CDROM_IMAGES):
        file_config["x_cdrom_image_{0}_sha1".format(i)] = \
                "cdrom_image_{0}".format(i)
    for i in range(Amiga.MAX_HARD_DRIVES):
        file_config["x_hard_drive_{0}_sha1".format(i)] = \
                "hard_drive_{0}".format(i)

Config.add_listener(Netplay)
IRCBroadcaster.add_listener(Netplay)
