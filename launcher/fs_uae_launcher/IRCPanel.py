from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from .IRC import IRC
from .Netplay import Netplay

class IRCPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.heading = fsui.HeadingLabel(self, "Channel")
        self.layout.add(self.heading, fill=True)
        self.layout.add_spacer(6)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, expand=True, fill=True)

        self.output = fsui.TextArea(self, read_only=True)
        hor_layout.add(self.output, expand=4, fill=True)

        hor_layout.add_spacer(6)
        #self.users = fsui.TextArea(self)
        self.nick_list_view = fsui.ListView(self)
        hor_layout.add(self.nick_list_view, expand=True, fill=True)

        self.layout.add_spacer(6)
        self.input = fsui.TextField(self)
        self.input.on_activate = self.on_input
        self.layout.add(self.input, fill=True)

        #self.browse_button = fsui.Button(self, "Browse")
        #self.layout.add(self.browse_button)
        #self.browse_button = fsui.Button(self, "Browse")
        #self.layout.add(self.browse_button)
        #Config.add_listener(self)
        IRC.add_listener(self)

        self.channel = ""
        self.end_of_names = True
        self.nicks = []
        self.nicks_lower = []
        self.synchronize_player_list = False

    def append_text(self, message):
        self.output.append_text(message + "\n")

    def append_message(self, sender, message):
        nick = IRC.nick(sender)
        self.append_text(u"<{0}> {1}".format(nick, message))

    def on_input(self):
        message = self.input.get_text()
        #print("message", message)
        self.input.set_text(u"")
        if message.startswith(u"/"):
            if not IRC.irc.handle_command_string(message):
                self.append_text(u"Unknown command: " + message)
        else:
            self.append_message(IRC.my_nick, message)
            IRC.irc.privmsg(self.channel, message)

    def clear_nick_list(self):
        self.nicks = []
        self.nicks_lower = []
        self.nick_list_view.remove_items()
        if self.synchronize_player_list:
            Netplay.clear_player_list()

    def add_nick(self, nick):
        print("add nick", nick)
        if self.synchronize_player_list:
            Netplay.add_player(nick)
        def insert(index):
            self.nicks.insert(index, nick)
            self.nicks_lower.insert(index, lnick)
            self.nick_list_view.insert_item(index, nick)
        lnick = IRC.nick(nick).lower()
        for i, n in enumerate(self.nicks_lower):
            if lnick < n:
                insert(i)
                return
        insert(len(self.nicks))

    def remove_nick(self, nick):
        if self.synchronize_player_list:
            Netplay.remove_player(nick)
        lnick = IRC.nick(nick).lower()
        for i, n in enumerate(self.nicks_lower):
            if lnick == n:
                self.nicks.pop(i)
                self.nicks_lower.pop(i)
                self.nick_list_view.remove_item(i)
                return

    def on_irc_namreply(self, args):
        channel = args[3]
        #print("----------------", self.end_of_names)
        if channel == self.channel:
            if self.end_of_names:
                self.end_of_names = False
                self.clear_nick_list()
            nicks = args[4].split(" ")
            #print(nicks)
            for nick in nicks:
                self.add_nick(nick)

    def on_irc_endofnames(self, args):
        channel = args[2]
        if channel == self.channel:
            self.end_of_names = True

    def on_irc_join(self, args):
        who = args[0]
        nick = IRC.nick(who)
        channel = args[1]
        if not IRC.me(who):
            if channel != self.channel:
                return
            self.add_nick(nick)
            self.append_text("*** {0} joined {1}".format(nick, channel))

    def on_irc_part(self, args):
        who = args[0]
        nick = IRC.nick(who)
        channel = args[1]
        if not IRC.me(who):
            if channel != self.channel:
                return
            self.remove_nick(nick)
            self.append_text("*** {0} left {1}".format(nick, channel))

